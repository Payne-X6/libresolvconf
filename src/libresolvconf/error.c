#include "error.h"

#include <string.h>

static const char *error_str[] = {
    [0] = "Error while parsing input"
};

const char *libresolvconf_strerror(int error)
{
    if (error < E_BEGIN) {
        return strerror(error);
    } else {
        return error_str[error - (E_BEGIN + 1)];
    }
}