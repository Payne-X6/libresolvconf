#include <stdio.h>

#include <criterion/criterion.h>

#include <libresolvconf/resolvconf.h>

lresconf_conf_t conf = { 0 };

void conf_deinit(void)
{
	lresconf_conf_deinit(&conf);
}

Test(resconf, fixtures, .fini = conf_deinit)
{
	cr_assert(lresconf_load_defaults(&conf) == LRESCONF_EOK, "Unable to load defaults");
	cr_assert(lresconf_load_file(&conf, "/etc/resolv.conf") == LRESCONF_EOK, "Unable to load file");
	cr_assert(lresconf_load_env(&conf) == LRESCONF_EOK, "Unable to load file");
}

Test(error, test)
{
	cr_assert(lresconf_strerror(LRESCONF_EOK) != NULL, "Wrong strerror of LRESCONF_EOK");
	cr_assert(lresconf_strerror(LRESCONF_EPARSING) != NULL, "Wrong strerror of LRESCONF_EPARSING");
	cr_assert(lresconf_strerror(LRESCONF_EDOMAINLEN) != NULL, "Wrong strerror of LRESCONF_EDOMAINLEN");
}

Test(str_array, test)
{
	const char array[] = "Lorem\0ipsum\0dolor\0sit\0amet,\0consectetuer\0"
	"adipiscing\0elit.\0Neque\0porro\0quisquam\0est,\0qui\0dolorem\0ipsum\0"
	"quia\0dolor\0sit\0amet,\0consectetur,\0adipisci\0velit,\0sed\0quia\0non\0"
	"numquam\0eius\0modi\0tempora\0incidunt\0ut\0labore\0et\0dolore\0magnam\0"
	"aliquam\0quaerat\0voluptatem.\0Aliquam\0ante.\0Donec\0quis\0nibh\0at\0"
	"felis\0congue\0commodo.\0Nulla\0non\0arcu\0lacinia\0neque\0faucibus\0"
	"fringilla.\0Nullam\0at\0arcu\0a\0est\0sollicitudin\0euismod.\0"
	"Pellentesque\0arcu.\0Duis\0condimentum\0augue\0id\0magna\0semper\0"
	"rutrum.\0Duis\0aute\0irure\0dolor\0in\0reprehenderit\0in\0voluptate\0"
	"velit\0esse\0cillum\0dolore\0eu\0fugiat\0nulla\0pariatur.\0Fusce\0wisi.\0"
	"Quis\0autem\0vel\0eum\0iure\0reprehenderit\0qui\0in\0ea\0voluptate\0"
	"velit\0esse\0quam\0nihil\0molestiae\0consequatur,\0vel\0illum\0qui\0"
	"dolorem\0eum\0fugiat\0quo\0voluptas\0nulla\0pariatur?\0Proin\0mattis\0"
	"lacinia\0justo.\0Nullam\0sit\0amet\0magna\0in\0magna\0gravida\0"
	"vehicula.\0Donec\0iaculis\0gravida\0nulla.";
	for (
		lresconf_arr_cstr_it_t it = array;
		!lresconf_arr_cstr_it_end(it);
		lresconf_arr_cstr_it_next(&it)
	) {
		cr_assert(strlen(it) > 0, "Short string");
	}
}
