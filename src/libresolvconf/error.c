#include "error.h"

static const char *libresolvconf_error_str[] = {
    [0] = "Error while parsing input"
};

const char *libresolvconf_strerror(int error)
{
    if (error < E_BEGIN) {
        return strerror(error);
    } else {
        return libresolvconf_error_str[error - (E_BEGIN + 1)];
    }
}