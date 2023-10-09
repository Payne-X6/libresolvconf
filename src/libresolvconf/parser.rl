#include "parser.h"

#include <assert.h>
#include <stdio.h>

#include <arpa/inet.h>

#include <sys/param.h>

#include "vector.h"

%%{
	machine resolvconf;

	action store_value_ptr {
		in_begin_p = fpc;
	}

	action ip_store {
		assert(in_begin_p);
		ptrdiff_t len = fpc - in_begin_p;
		vector_push_back(&nameservers, in_begin_p, len);
		in_begin_p = NULL;
	}

	action clear_int {
		in_int = 0;
	}

	action append_int {
		in_int = (in_int * 10) + (fc - '0');
	}

	action attempts_store {
		out->options.attempts = in_int;
	}

	action debug_store {
		out->options.debug = true;
	}

	action edns0_store {
		out->options.edns0 = true;
	}

	action inet6_store {
		out->options.inet6 = true;
	}

	action insecure1_store {
		out->options.insecure1 = true;
	}

	action insecure2_store {
		out->options.insecure2 = true;
	}

	action ip6_bytestring_store {
		out->options.ip6_bytestring = true;
	}

	action ip6_dotint_store {
		out->options.ip6_dotint = true;
	}

	action no_check_names_store {
		out->options.no_check_names = true;
	}

	action ndots_store {
		out->options.ndots = in_int;
	}

	action no_ip6_dotint_store {
		out->options.ip6_dotint = false;
	}

	action no_reload_store {
		out->options.no_reload = true;
	}

	action no_tld_query_store {
		out->options.no_tld_query = true;
	}

	action rotate_store {
		out->options.rotate = true;
	}

	action single_request_store {
		out->options.single_request = true;
	}

	action single_request_reopen_store {
		out->options.single_request_reopen = true;
	}

	action tcp_store {
		out->options.tcp = true;
	}

	action timeout_store {
		out->options.timeout = in_int;
	}

	action trust_ad_store {
		out->options.trust_ad = true;
	}

	action use_vc_store {
		out->options.use_vc = true;
	}

	action domain_clean {
		vector_clean(&domains);
	}

	action domain_store {
		assert(in_begin_p);
		ptrdiff_t len = fpc - in_begin_p;
		vector_push_back(&domains, in_begin_p, len);
		in_begin_p = NULL;
	}

	action nl {
		curline++;
	}

	# common
	ws = [\t ]+;
	unsigned_byte = (
			("25" [0-5]) |
			('2' [0-4] [0-9]) |
			([0-1]?[0-9]?[0-9])
		);

	# ip
	ip4_octet = unsigned_byte;
	ip6_word = [0-9a-fA-F]{1,4};

	ip4 = ip4_octet '.' ip4_octet '.' ip4_octet '.' ip4_octet;
	ip4in6 = (
			("::" (ip6_word (':' ip6_word) {0,4})?) |
			(ip6_word "::" (ip6_word (':' ip6_word) {0,3})?) |
			(ip6_word ':') {2} ':' (ip6_word (':' ip6_word) {0,2})? |
			(ip6_word ':') {3} ':' (ip6_word (':' ip6_word) {0,1})? |
			(ip6_word ':') {4} ':' ip6_word |
			(ip6_word ':') {5} ip6_word
		) ip4;
	ip6 = (
			"::" (ip6_word (':' ip6_word) {0,6})? |
			ip6_word "::" (ip6_word (':' ip6_word) {0,5})? |
			(ip6_word ':') {2} ':' (ip6_word (':' ip6_word) {0,4})? |
			(ip6_word ':') {3} ':' (ip6_word (':' ip6_word) {0,3})? |
			(ip6_word ':') {4} ':' (ip6_word (':' ip6_word) {0,2})? |
			(ip6_word ':') {5} ':' (ip6_word (':' ip6_word) {0,1})? |
			(ip6_word ':') {6} ':' ip6_word |
			(ip6_word ':') {7} ip6_word
		);
	ip = (ip4 | ip4in6 | ip6) >store_value_ptr %ip_store;

	# search domain
	search_domain_word = [a-zA-Z0-9]+;
	search_domain = search_domain_word ('.' search_domain_word)* '.'?;

	# cmd options
	options_attempts =              "attempts" ':' (unsigned_byte >clear_int $append_int %attempts_store);
	options_debug =                 "debug" %debug_store;
	options_edns =                  "edns0" %edns0_store;
	options_inet6 =                 "inet6" %inet6_store;
	options_insecure1 =             "insecure1" %insecure1_store;
	options_insecure2 =             "insecure2" %insecure2_store;
	options_inet6_bytestring =      "ip6-bytestring" %ip6_bytestring_store;
	options_ip6_dotint =            "ip6-dotint" %ip6_dotint_store;
	options_ndots =                 "ndots" ':' (unsigned_byte >clear_int $append_int %ndots_store);
	options_no_check_names =        "no-check-names" %no_check_names_store;
	options_no_ip6_dotint =         "no-ip6-dotint" %no_ip6_dotint_store;
	options_no_reload =             "no-reload" %no_reload_store;
	options_no_tld_query =          "no-tld-query" %no_tld_query_store;
	options_rotate =                "rotate" %rotate_store;
	options_single_request =        "single-request" %single_request_store;
	options_single_request_reopen = "single-request-reopen" %single_request_reopen_store;
	options_tcp =                   "tcp" %tcp_store;
	options_timeout =               "timeout" ':' (unsigned_byte >clear_int $append_int %timeout_store);
	options_trust_ad =              "trust-ad" %trust_ad_store;
	options_use_vc =                "use-vc" %use_vc_store;

	# cmd
	comment =    ('#'|';') [^\n]*;
	domain =     "domain" %domain_clean ws (search_domain) >store_value_ptr %domain_store;
	family =     "family" ws (
			"inet4" |
			"inet4" ws "inet6" |
			"inet6" |
			"inet6" ws "inet4"
		);
	lookup =     "lookup" ws (
			"bind" |
			"bind" ws "file" |
			"file" |
			"file" ws "bind"
		);
	nameserver = "nameserver" ws ip;
	options_values = (
			options_attempts |
			options_debug |
			options_edns |
			options_inet6 |
			options_inet6_bytestring |
			options_insecure1 |
			options_insecure2 |
			options_ip6_dotint |
			options_ndots |
			options_no_check_names |
			options_no_ip6_dotint |
			options_no_reload |
			options_no_tld_query |
			options_rotate |
			options_single_request |
			options_single_request_reopen |
			options_tcp |
			options_timeout |
			options_trust_ad |
			options_use_vc
		);
	options =    "options" (ws options_values)+;
	search =     "search" %domain_clean (ws (search_domain) >store_value_ptr %domain_store)+;
	sortlist =   "sortlist" (ws ip ('/' ip)?)+;

	# document
	line =  ws? (
			comment |
			domain |
			family |
			lookup |
			nameserver |
			options |
			search |
			sortlist
		)? ws? ;
	document := (line '\n' $nl)* line?;
}%%

%% write data;

int parse(resolv_conf_t *out, char *in, size_t len)
{
	(void)resolvconf_en_document;

	vector_t domains, nameservers;
	int ret = vector_init(&nameservers, INET6_ADDRSTRLEN * 3);
	if (ret != 0) {
		printf("Error: not enough memory");
		return -4;
	}
	
	ret = vector_init(&domains, MAXHOSTNAMELEN * 4);
	if (ret != 0) {
		printf("Error: not enough memory");
		vector_deinit(&domains);
		return -5;
	}
	// TODO init from hostname

	int curline = 1;

	char *in_begin_p = NULL;
	unsigned in_int = 0;

	int cs;
	char *p = in, *pe = in + len, *eof = pe;

	%% write init;

	%% write exec;

	if (cs < resolvconf_first_final || cs == resolvconf_error) {
		// TODO line and column error
		vector_deinit(&nameservers);
		vector_deinit(&domains);
		return -7;
	} 

	out->nameservers = nameservers.data;
	out->domains = domains.data;

	return 0;
}