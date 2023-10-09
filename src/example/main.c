#include <stdio.h>

#include <resolvconf.h>

#include <sys/socket.h>

#define RESOLV_CONF_PATH "/etc/resolv.conf"

#define bool_to_str(x) (x ? "true" : "false")

int main(int argc, char **argv)
{
	resolv_conf_t conf = { 0 };
	int ret = load_defaults(&conf);
	if (ret != 0) {
		return 1;
	}
	ret = load_file(&conf, RESOLV_CONF_PATH);
	if (ret != 0) {
		return 2;
	}
	ret = load_env(&conf);
	if (ret != 0) {
		return 3;
	}

	// Print
	if (!vector_it_end(conf.nameservers)) {
		printf("nameservers:");
	}
	for (vector_it_t it = conf.nameservers;
	     !vector_it_end(it);
	     vector_it_next(&it))
	{
		printf(" %s", it);
	}
	printf("\n");

	if (!vector_it_end(conf.domains)) {
		printf("domains:");
	}
	for (vector_it_t it = conf.domains;
	     !vector_it_end(it);
	     vector_it_next(&it))
	{
		printf(" %s", it);
	}
	printf("\n");

	
	switch (conf.family[0]) {
	case AF_INET:
		printf("family: inet");
		break;
	case AF_INET6:
		printf("family: inet6");
		break;
	default:
		goto family_end;
		break;
	}
	switch (conf.family[1]) {
	case AF_INET:
		printf(" inet\n");
		break;
	case AF_INET6:
		printf(" inet6\n");
		break;
	default:
		printf("\n");
		break;
	}

family_end:
	switch (conf.lookup[0]) {
	case LOOKUP_BIND:
		printf("lookup: bind");
		break;
	case LOOKUP_FILE:
		printf("lookup: file");
		break;
	default:
		goto lookup_end;
		break;
	}
	switch (conf.lookup[1]) {
	case LOOKUP_BIND:
		printf(" bind\n");
		break;
	case LOOKUP_FILE:
		printf(" file\n");
		break;
	default:
		printf("\n");
		break;
	}

lookup_end:
	printf("sortlist:\n");

	printf("options:\n");
	printf("\tattempts: %d\n", conf.options.attempts);
	printf("\tdebug: %s\n", bool_to_str(conf.options.debug));
	printf("\tedns0: %s\n", bool_to_str(conf.options.edns0));
	printf("\tinet6: %s\n", bool_to_str(conf.options.inet6));
	printf("\tinsecure1: %s\n", bool_to_str(conf.options.insecure1));
	printf("\tinsecure2: %s\n", bool_to_str(conf.options.insecure2));
	printf("\tip6-bytestring: %s\n", bool_to_str(conf.options.ip6_bytestring));
	printf("\tip6-dotint: %s\n", bool_to_str(conf.options.ip6_dotint));
	printf("\tndots: %d\n", conf.options.ndots);
	printf("\tno-check-names: %s\n", bool_to_str(conf.options.no_check_names));
	printf("\tno-reload: %s\n", bool_to_str(conf.options.no_reload));
	printf("\tno-tld-query: %s\n", bool_to_str(conf.options.no_tld_query));
	printf("\trotate: %s\n", bool_to_str(conf.options.rotate));
	printf("\tsingle-request: %s\n", bool_to_str(conf.options.single_request));
	printf("\tsingle-request-reopen: %s\n", bool_to_str(conf.options.single_request_reopen));
	printf("\ttcp: %s\n", bool_to_str(conf.options.tcp));
	printf("\ttimeout: %d\n", conf.options.timeout);
	printf("\ttrust-ad: %s\n", bool_to_str(conf.options.trust_ad));
	printf("\tuse-vc: %s\n", bool_to_str(conf.options.use_vc));

	return 0;
}