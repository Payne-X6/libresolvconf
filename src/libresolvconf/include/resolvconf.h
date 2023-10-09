#pragma once

#include <stddef.h>
#include <stdint.h>

#include "error.h"
#include "vector_it.h"

typedef enum {
	LOOKUP_UNSPEC = 0,
	LOOKUP_BIND = 1,
	LOOKUP_FILE = 2
} __attribute__ ((__packed__)) libresolvconf_lookup_t;

typedef struct {
	vector_it_t nameservers;
	vector_it_t domains;
	vector_it_t sortlist;
	int family[2];
	libresolvconf_lookup_t lookup[2];
	struct {
		uint8_t attempts;
		uint8_t timeout;
		uint8_t ndots;
		bool    debug                  : 1;
		bool    edns0                  : 1;
		bool    inet6                  : 1;
		bool    insecure1              : 1;
		bool    insecure2              : 1;
		bool    ip6_bytestring         : 1;
		bool    ip6_dotint             : 1;
		bool    no_check_names         : 1;
		bool    no_reload              : 1;
		bool    no_tld_query           : 1;
		bool    rotate                 : 1;
		bool    single_request         : 1;
		bool    single_request_reopen  : 1;
		bool    tcp                    : 1;
		bool    trust_ad               : 1;
		bool    use_vc                 : 1;
	} options;
	struct {
		size_t line;
		size_t col;
	} error;
} resolv_conf_t;

int load_defaults(resolv_conf_t *conf);
int load_file(resolv_conf_t *conf, const char *path);
int load_env(resolv_conf_t *conf);