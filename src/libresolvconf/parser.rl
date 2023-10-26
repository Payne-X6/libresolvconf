/*
* 'libresolvconf' is a shared library for parsing resolv.conf files,
* alongside associated utilities.
*
* Copyright (C) 2023 libresolvconf
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <sys/param.h>

#include "defines.h"
#include "dynarray.h"
#include "error.h"

%%{
	machine common;

	# options
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

	unsigned_byte = (
			("25" [0-5]) |
			('2' [0-4] digit) |
			([0-1]? digit{1,2})
		);
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

	# domains
	search_domain_word = alnum((alnum | '-'){0,61}alnum)?;
	search_domain = search_domain_word ('.' search_domain_word)* '.'?;
}%%

%%{
	machine resolvconf;

	include common;

	action store_value_ptr {
		in_begin_p = fpc;
	}

	action ip_store {
		assert(in_begin_p);
		ptrdiff_t len = fpc - in_begin_p;
		vector_push_back(&nameservers, in_begin_p, len);
		in_begin_p = NULL;
	}

	action domain_clear {
		vector_clear(&domains);
	}

	action domain_store {
		assert(in_begin_p);
		ptrdiff_t len = fpc - in_begin_p;
		if (len > 254) {
			out->error.line = curline;
			out->error.col = in_begin_p - curline_begin;
			return LRESCONF_EPARSING;
		}
		vector_push_back(&domains, in_begin_p, len);
		in_begin_p = NULL;
	}

	action family_store_0 {
		out->family[0] = AF_INET;
		out->family[1] = AF_UNSPEC;
	}

	action family_store_1 {
		out->family[0] = AF_INET;
		out->family[1] = AF_INET6;
	}

	action family_store_2 {
		out->family[0] = AF_INET6;
		out->family[1] = AF_UNSPEC;
	}

	action family_store_3 {
		out->family[0] = AF_INET6;
		out->family[1] = AF_INET;
	}

	action lookup_store_0 {
		out->lookup[0] = IRESCONF_LOOKUP_BIND;
		out->lookup[1] = IRESCONF_LOOKUP_UNSPEC;
	}

	action lookup_store_1 {
		out->lookup[0] = IRESCONF_LOOKUP_BIND;
		out->lookup[1] = IRESCONF_LOOKUP_FILE;
	}

	action lookup_store_2 {
		out->lookup[0] = IRESCONF_LOOKUP_FILE;
		out->lookup[1] = IRESCONF_LOOKUP_UNSPEC;
	}

	action lookup_store_3 {
		out->lookup[0] = IRESCONF_LOOKUP_FILE;
		out->lookup[1] = IRESCONF_LOOKUP_BIND;
	}

	action nl {
		curline++;
		curline_begin = fpc + 1;
	}

	# common
	ws = [\t ]+;

	# ip
	ip4_octet = unsigned_byte;
	ip6_word = xdigit{1,4};

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
	ip6_scope = ('%' [a-zA-Z0-9]+);
	ip = (ip4 | ip4in6 | ip6);
	ip_scoped = (ip4 | ip4in6 ip6_scope? | ip6 ip6_scope?);

	# cmd
	comment =    ('#'|';') [^\n]*;
	domain =     "domain" %domain_clear ws (search_domain) >store_value_ptr %domain_store;
	family =     "family" ws (
			"inet4" %family_store_0 |
			"inet4" ws "inet6" %family_store_1 |
			"inet6" %family_store_2 |
			"inet6" ws "inet4" %family_store_3
		);
	lookup =     "lookup" ws (
			"bind" %lookup_store_0 |
			"bind" ws "file" %lookup_store_1 |
			"file" %lookup_store_2 |
			"file" ws "bind" %lookup_store_3
		);
	nameserver = "nameserver" ws (ip_scoped >store_value_ptr %ip_store);
	options =    "options" (ws options_values)+;
	search =     "search" %domain_clear (ws (search_domain) >store_value_ptr %domain_store)+;
	sortlist =   "sortlist" (ws (ip ('/' ip)?) >store_value_ptr )+;

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

int parse(lresconf_conf_t *out, char *in, size_t len)
{
	(void)resolvconf_en_document;

	dynarray_t nameservers, domains, sortlist;
	int ret = vector_init(&nameservers, INET6_ADDRSTRLEN * 3);
	if (ret != 0) {
		return ret;
	}
	
	ret = vector_init(&domains, DOMAIN_NAME_LEN * 4);
	if (ret != 0) {
		vector_deinit(&nameservers);
		return ret;
	}

	ret = vector_init(&sortlist, SORTLIST_LEN * 4);
	if (ret != 0) {
		vector_deinit(&nameservers);
		vector_deinit(&domains);
		return ret;
	}

	int cs;
	char *p = in, *pe = in + len, *eof = pe;

	%% write init;

	int curline = 1;
	char *curline_begin = in;

	char *in_begin_p = NULL;
	unsigned in_int = 0;

	%% write exec;

	if (cs < resolvconf_first_final || cs == resolvconf_error) {
		vector_deinit(&nameservers);
		vector_deinit(&domains);
		vector_deinit(&sortlist);

		out->error.line = curline;
		out->error.col = p - curline_begin;
		return LRESCONF_EPARSING;
	} 

	if (vector_is_empty(&nameservers)) {
		vector_deinit(&nameservers);
	} else {
		lresconf_arr_cstr_destroy(&out->nameservers);
		out->nameservers = nameservers.data;
	}

	if (vector_is_empty(&domains)) {
		vector_deinit(&domains);
	} else {
		lresconf_arr_cstr_destroy(&out->domains);
		out->domains = domains.data;
	}

	if (vector_is_empty(&sortlist)) {
		vector_deinit(&sortlist);
	} else {
		lresconf_arr_cstr_destroy(&out->sortlist);
		out->sortlist = sortlist.data;
	}

	return 0;
}

%%{
	machine env_options;

	include common;

	action end {
		pe = fpc + 1;
	}

	str := (options_values (' ' options_values)*)? ('\0' $end);
}%%

%% write data;

int parse_env_opts(lresconf_conf_t *out, char *in)
{
    (void)env_options_en_str;
	int cs;
	char *p = in, *pe = NULL;

	%% write init;

	unsigned in_int = 0;

	%% write exec;

	if (cs < env_options_first_final || cs == env_options_error) {
		return LRESCONF_EPARSING;
	} 

	return 0;
}

%%{
	machine env_domains;

	include common;

	action store_value_ptr {
		in_begin_p = fpc;
	}

	action domain_store {
		assert(in_begin_p);
		ptrdiff_t len = fpc - in_begin_p;
		if (len > 254) {
			out->error.line = curline;
			out->error.col = in_begin_p - curline_begin;
			return LRESCONF_EPARSING;
		}
		vector_push_back(&domains, in_begin_p, len);
		in_begin_p = NULL;
	}

	action end {
		pe = fpc + 1;
	}

    str := ((search_domain >store_value_ptr %domain_store) (' ' (search_domain >store_value_ptr %domain_store))*)? ('\0' $end);
}%%

%% write data;

int parse_env_domains(lresconf_conf_t *out, char *in)
{
    (void)env_domains_en_str;

	dynarray_t domains;
	int ret = vector_init(&domains, DOMAIN_NAME_LEN * 4);
	if (ret != 0) {
		return ret;
	}

	int cs, curline = 1;
	char *p = in, *pe = NULL, *curline_begin = NULL, *in_begin_p = NULL;

	%% write init;

	%% write exec;

	if (cs < env_domains_first_final || cs == env_domains_error) {
		return LRESCONF_EPARSING;
	} 

	if (out->domains) {
		// TODO free
	}
	out->domains = domains.data;

	return 0;
}