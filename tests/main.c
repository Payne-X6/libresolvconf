#include <criterion/criterion.h>

#include <libresolvconf/resolvconf.h>

Test(simple, test)
{
    lresconf_conf_t conf = { 0 };
    cr_assert(lresconf_load_defaults(&conf) == LRESCONF_EOK, "Unable to load defaults");
    lresconf_conf_deinit(&conf);
}