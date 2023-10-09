#include "resolvconf.h"

#include <fcntl.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "parser.h"
#include "vector.h"

#define SORTLISTNAMELEN (MAXHOSTNAMELEN * 2)

int load_defaults(resolv_conf_t *conf)
{
	vector_t nameservers, domains, sortlist;
	int ret = vector_init(&nameservers, INET6_ADDRSTRLEN * 3);
	if (ret) {
		return ret;
	}

	ret = vector_init(&domains, MAXHOSTNAMELEN * 4);
	if (ret) {
		vector_deinit(&nameservers);
		return ret;
	}

	ret = vector_init(&sortlist, SORTLISTNAMELEN * 4);
	if (ret) {
		vector_deinit(&nameservers);
		vector_deinit(&domains);
		return ret;
	}

	// TODO initialize nameservers

	char hostname[MAXHOSTNAMELEN];
	ret = gethostname(hostname, MAXHOSTNAMELEN);
	char *domainname = strchr(hostname, '.');
	if (domainname != NULL) {
		domainname++;
		vector_push_back(&domains, domainname, strlen(domainname));
	}

	// TODO initialize sortlist

	conf->nameservers = vector_begin(&nameservers);
	conf->domains = vector_begin(&domains);
	conf->sortlist = vector_begin(&sortlist);
	conf->family[0] = AF_INET;
	conf->family[1] = AF_INET6;
	conf->lookup[0] = LOOKUP_BIND;
	conf->lookup[1] = LOOKUP_FILE;
	conf->options = (typeof(conf->options)){
		.attempts = RES_DFLRETRY,
		.debug = false,
		.edns0 = false,
		.inet6 = false,
		.insecure1 = false,
		.insecure2 = false,
		.ip6_bytestring = false,
		.ip6_dotint = false,
		.ndots = 1,
		.no_check_names = false,
		.no_reload = false,
		.no_tld_query = false,
		.rotate = false,
		.single_request = false,
		.single_request_reopen = false,
		.tcp = false,
		.timeout = RES_TIMEOUT,
		.trust_ad = false,
		.use_vc = false
	};
	conf->error.col = 0;
	conf->error.line = 0;

	return 0;
}

int load_file(resolv_conf_t *conf, const char *path)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		return errno;
	}

	struct stat statbuf;
	int ret = fstat(fd, &statbuf);
	if (ret < 0) {
		close(fd);
		return errno;
	}
	if (statbuf.st_size == 0) {
		close(fd);
		return E_OK;
	}

	char *ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		return errno;
	}

	ret = parse(conf, ptr, statbuf.st_size);

	ret = munmap(ptr, statbuf.st_size);
	if(ret != 0){
		return errno;
	}

	return 0;
}

int load_env(resolv_conf_t *conf)
{
	static const char *LOCALDOMAIN_ENV = "LOCALDOMAIN";
	static const char *RES_OPTIONS_ENV = "RES_OPTIONS";

	char *localdomain = getenv(LOCALDOMAIN_ENV);
	if (localdomain) {
		vector_t domains;
		int ret = vector_init(&domains, MAXHOSTNAMELEN * 4);
		if (ret) {
			return ret;
		}

		while (localdomain) {
			char *localdomain_end = strchr(localdomain, ' ');
			if (localdomain_end == NULL) {
				vector_push_back(&domains, localdomain, strlen(localdomain));
				localdomain = localdomain_end;
			} else {
				vector_push_back(&domains, localdomain, localdomain_end - localdomain);
				localdomain = localdomain_end + 1;
			}
		}

		free(conf->domains);
		conf->domains = vector_begin(&domains);
	}

	char *options = getenv(RES_OPTIONS_ENV);
	if (options) {
		//TODO dedicated options automata
	}

	return 0;
}