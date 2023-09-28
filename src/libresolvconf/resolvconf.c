#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include "resolvconf.h"
#include "parser.h"

int load_defaults(resolv_conf_t *conf)
{
	return 0;
}

int load_file(resolv_conf_t *conf, const char *path)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0) {
		printf("Error: could not open '%s'\n", path);
		return -1;
	}

	struct stat statbuf;
	int ret = fstat(fd, &statbuf);
	if (ret < 0) {
		printf("Error: could not obtaint stats of '%s'\n", path);
		close(fd);
		return -2;
	}
	if (statbuf.st_size == 0) {
		close(fd);
		return 0;
	}

	char *ptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (ptr == MAP_FAILED) {
		printf("Error: file memory mapping failed\n");
		return -3;
	}

	ret = parse(conf, ptr, statbuf.st_size);

	ret = munmap(ptr, statbuf.st_size);
	if(ret != 0){
		printf("Error: file memory unmapping failed\n");
		return -6;
	}

	return 0;
}

int load_env(resolv_conf_t *conf)
{
	return 0;
}