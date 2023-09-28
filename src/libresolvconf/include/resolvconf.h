#pragma once

#include <stddef.h>

#include "vector_it.h"

typedef struct {
    vector_it_t nameservers;
    vector_it_t domains;
    struct {
        unsigned ndots                  : 7;
        unsigned timeout                : 7;
        unsigned attempts               : 7;
        unsigned debug                  : 1;
        unsigned rotate                 : 1;
        unsigned no_check_names         : 1;
        unsigned ends0                  : 1;
        unsigned inet6                  : 1;
        unsigned ip6_bytestring         : 1;
        unsigned ip6_dotint             : 1;
        unsigned single_request         : 1;
        unsigned single_request_reopen  : 1;
        unsigned no_tld_query           : 1;
        unsigned use_vc                 : 1;
        unsigned no_reload              : 1;
        unsigned ins1                   : 1;
        unsigned ins2                   : 1;
        unsigned tcp                    : 1;
        unsigned trust_ad               : 1;
    } options;
    struct {
        size_t line;
        size_t col;
        int code;
    } error;
} resolv_conf_t;

int load_defaults(resolv_conf_t *conf);
int load_file(resolv_conf_t *conf, const char *path);
int load_env(resolv_conf_t *conf);