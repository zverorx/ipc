/* 
 * This file is part of ipc.
 * ipc - IP calculator.
 *
 * Copyright (C) 2025-2026 Egorov Konstantin
 *
 * ipc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ipc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ipc. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ipv4_t.h"
#include "fill_ipv4.h"
#include "analysis.h"
#include "subnet.h"

/**
 * @enum mode
 * @brief Command line options. 
 */
enum mode { analysis, subnetting };

/**
 * @brief Process main() command-line arguments.
 * 
 * Parses command-line arguments to determine program operation mode,
 * extract IP address string, and defining parameters for subnetting.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param[out] mode Program operation mode.
 * @param[out] ip_str Extracted IP address string. The caller must free.
 * @param[out] arg_arr Array of parameters after the [--part|--equal] option.
 *		   			   The caller must free.
 * 					   --part - Initialized with parameters after --part.
 * 								The parts are an integer >= 0.
 * 					   --equal - An array initialized with zeros is created.
 * 								 The size is equal to the first parameter after --equal.
 * 								 The other parameters are ignored.
 * @param[out] arr_len Size of arg_arr.
 * 
 * @return 0 on success, -1 on error.
 */
static int process_args(int argc, char **argv, 
				 		enum mode *mode, char **ip_str,
						int **arg_arr, size_t *arr_len);

int main(int argc, char **argv)
{
	int res;
	enum mode mode;
	char *ip_str = NULL;
	ipv4_t *ip = NULL;
	int *parts = NULL;
	size_t parts_len;

	ip = malloc(sizeof(ipv4_t));
	if (!ip) { goto handle_error; }

	res = process_args(argc, argv, &mode, &ip_str, &parts, &parts_len);
	if (res == -1) { goto handle_error; }

	switch (mode) {
		case analysis:
			res = analysis_start(ip, ip_str);	
			if (res == EXIT_FAILURE) { goto handle_error; }
			break;
		
		case subnetting:
			res = subnetting_start(ip, ip_str, parts, parts_len);
			if (res == EXIT_FAILURE) { goto handle_error; }
			break;
	}

	free(ip);
	free(ip_str);
	free(parts);

	return EXIT_SUCCESS;

	handle_error:
		free(ip);
		free(ip_str);
		free(parts);
		fputs("Usage:\tipc <-a> <ip/bitmask>\n"
			  "\tipc <-s> <ip/bitmask> <--equal> <count>\n"
			  "\tipc <-s> <ip/bitmask> <--part> <uint, ...>\n\n"
			  "-a\tanalysis\n"
			  "-s\tsubnetting\n\n"
			  "\t--equal\tsplitting into equal parts\n"
			  "\t--part\tsplit into pieces of different sizes\n", stderr);
		return EXIT_FAILURE;
}

static int process_args(int argc, char **argv, 
				 		enum mode *mode, char **ip_str,
						int **arg_arr, size_t *arr_len)
{
	int long count_part;
	int long part;
	char *endptr = NULL;

	if (!argv) { return -1; }
	if (!mode) { return -1; }
	if (!ip_str) { return -1; }
	if (!arr_len) { return -1; }

	/* Checking the first parameter */
	if (strcmp(argv[1], "-a") == 0) { *mode = analysis; }
	else if (strcmp(argv[1], "-s") == 0) { *mode = subnetting; }
	else { return -1; }

	if (*mode == analysis && argc < 3) { return -1; }
	if (*mode == subnetting && argc < 5) { return -1; }

	/* Checking the second parameter */
	if (argv[2]) { *ip_str = strdup(argv[2]); }
	if (!*ip_str) { return -1; }

	/* Checking the third and other parameters */
	if (*mode == analysis) { return 0; }
	if (strcmp("--equal", argv[3]) == 0) {
		errno = 0;
		if (!argv[4]) { goto handle_error; }
		count_part = strtol(argv[4], &endptr, 10);
		if (errno == ERANGE || *endptr != '\0') { goto handle_error; }
		if (count_part <= 0) { goto handle_error; }
		*arg_arr = calloc((size_t) count_part, sizeof(int));
		*arr_len = (size_t) count_part;
	}
	else if (strcmp("--part", argv[3]) == 0) {
		if (!argv[4]) { goto handle_error; }
		count_part = argc - 4; /* minus the first four parmeters */
		if (count_part <= 0) { goto handle_error; }
		*arg_arr = calloc((size_t) count_part, sizeof(int));
		*arr_len = (size_t) count_part;
		for (int i = 4, j = 0; i < argc; i++, j++) {
			errno = 0;
			part = strtol(argv[i], &endptr, 10);
			if (errno == ERANGE || *endptr != '\0' || part <= 0) 
				{ goto handle_error; }
			(*arg_arr)[j] = (int) part;
		}
	}
	else { goto handle_error; }

	return 0;

	handle_error:
		free(*ip_str);
		*ip_str = NULL;
		free(*arg_arr);
		*arg_arr = NULL;
		return -1;
}
