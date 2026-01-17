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

#include "ipv4_t.h"
#include "fill_ipv4.h"
#include "analysis.h"

/**
 * @enum mode
 * @brief Command line options. 
 */
enum mode { analysis, subnetting };

/**
 * @brief Process main() command-line arguments.
 * 
 * Parses command-line arguments to determine program operation mode
 * and extract IP address string.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param[out] mode Program operation mode.
 * @param[out] ip_str Extracted IP address string. The caller must free.
 * 
 * @return 0 on success, -1 on error.
 */
static int process_args(int argc, char **argv, 
				 		enum mode *mode, char **ip_str);

int main(int argc, char **argv)
{
	int res;
	enum mode mode;
	char *ip_str = NULL;
	ipv4_t *ip = NULL;

	ip = malloc(sizeof(ipv4_t));
	if (!ip) { goto handle_error; }

	res = process_args(argc, argv, &mode, &ip_str);
	if (res == -1) { goto handle_error; }

	switch (mode) {
		case analysis:
			res = analysis_start(ip, ip_str);	
			if (res == -1) { goto handle_error; }
			break;
		
		case subnetting:
			/* TODO: subnetting */
			/* res = subnetting_start();	
			if (res == -1) { goto handle_error; } */
			break;
	}

	free(ip_str);
	free(ip);
	return EXIT_SUCCESS;

	handle_error:
		free(ip_str);
		free(ip);
		fputs("Usage: ipc <-a|-s> <ip/bitmask>\n", stderr);
		return EXIT_FAILURE;
}

static int process_args(int argc, char **argv, 
				 		enum mode *mode, char **ip_str)
{
	if (argc < 3) { return -1; }

	if (strcmp(argv[1], "-a") == 0) { *mode = analysis; }
	else if (strcmp(argv[1], "-s") == 0) { *mode = subnetting; }
	else { return -1; }

	if (argv[2]) { *ip_str = strdup(argv[2]); }
	if (!*ip_str) { return -1; }

	return 0;
}
