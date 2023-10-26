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

#pragma once

#include <stdbool.h>

typedef char *lresconf_arr_str_t;
typedef const char *lresconf_arr_cstr_t;
typedef const char *lresconf_arr_cstr_it_t;

/**
 * @brief Returns const iterator pointing to first element in array of string.
 *
 * @param[in] arr array of string.
 *
 * @return Iterator pointing to first element in array of string
 */
lresconf_arr_cstr_it_t lresconf_arr_cstr_begin(lresconf_arr_cstr_t arr);
/**
 * @brief Destroy array of string.
 *
 * @param[in] arr array of string.
 */
void lresconf_arr_cstr_destroy(lresconf_arr_cstr_t *arr);

/**
 * @brief Move iterator to next element in array.
 *
 * @param[in, out] it iterator that will be moved on next element in array.
 */
void lresconf_arr_cstr_it_next(lresconf_arr_cstr_it_t *it);
/**
 * @brief Test iterator if pointing behind the last element in array.
 *
 * @param[in]  str  iterator that has to be tested.
 *
 * @return true when at the end, else false.
 */
bool lresconf_arr_cstr_it_end(lresconf_arr_cstr_it_t it);
