#pragma once

#include <sys/param.h>

#define DOMAIN_NAME_LEN (MAXHOSTNAMELEN + 1)
#define SORTLIST_LEN (DOMAIN_NAME_LEN * 2)

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)