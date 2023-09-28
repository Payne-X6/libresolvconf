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

	# ip
	ip4_octet = (
			("25" [0-5]) |
			('2' [0-4] [0-9]) |
			([0-1]?[0-9]?[0-9])
		);
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
	options_attempts =              "attempts" ':' ([0-1]?[0-9]?[0-9]);
	options_debug =                 "debug";
	options_edns =                  "edns0";
	options_inet6 =                 "inet6";
	options_inet6_bytestring =      "ip6-bytestring";
	options_insecure1 =             "insecure1";
	options_insecure2 =             "insecure2";
	options_ip6_dotint =            "ip6-dotint";
	options_ndots =                 "ndots" ':' ([0-1]?[0-9]?[0-9]);
	options_no_check_names =        "no-check-names";
	options_no_ip6_dotint =         "no-ip6-dotint";
	options_no_reload =             "no-reload";
	options_no_tld_query =          "no-tld-query";
	options_rotate =                "rotate";
	options_single_request =        "single-request";
	options_single_request_reopen = "single-request-reopen";
	options_tcp =                   "tcp";
	options_timeout =               "timeout" ':' ([0-1]?[0-9]?[0-9]);
	options_trust_ad =              "trust-ad";
	options_use_vc =                "use-vc";

	# cmd
	comment =    '#' [^\n]*;
	domain =     "domain" %domain_clean ws (search_domain) >store_value_ptr %domain_store;
	family =     "family" (ws "inet" [46]){1,2};
	lookup =     "lookup" ws (
			"bind" |
			"bind" ws "file" |
			"file" |
			"file" ws "bind"
		);
	nameserver = "nameserver" ws ip;
	options =    "options" (ws (
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
		))+;
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
	main := (line '\n' $nl)* line?;
}%%

%% write data;

int parse(resolv_conf_t *out, char *in, size_t len)
{
	(void)resolvconf_en_main;

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
	out->options.ndots = 1;

	return 0;
}