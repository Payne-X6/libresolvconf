#include <stdio.h>

#include <resolvconf.h>

#define RESOLV_CONF_PATH "/etc/resolv.conf"

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

	for (vector_it_t it = conf.nameservers;
	     !vector_it_end(it);
	     it = vector_it_next(it))
	{
		printf("%s\n", it);
	}
	for (vector_it_t it = conf.domains;
	     !vector_it_end(it);
	     it = vector_it_next(it))
	{
		printf("%s\n", it);
	}

	return 0;
}