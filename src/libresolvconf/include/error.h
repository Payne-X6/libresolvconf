#pragma once

#include <errno.h>

typedef enum {
    E_OK = 0,
    E_BEGIN = 255,
    E_PARSING
} libresolvconf_error_t;

const char *libresolvconf_strerror(int error);