/* 
 * This file is part of ipc.
 * ipc - IP calculator.
 *
 * Copyright (C) 2026 Egorov Konstantin
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
 * @brief Prints IPv4 network information to stdout.
 * 
 * Displays IP details in DEC/BIN/HEX formats.
 * Special handling for:
 * 	- /31 (point-to-point links with no network/broadcast addresses)
 * 	- /32 (single host addresses with no network/broadcast).
 * 
 * @param ip Pointer to IPv4 data structure.
 */
static void print_ipv4(const ipv4_t *ip);

int analysis_start(ipv4_t *ip, const char *ip_str)
{
	if (!ip) { return EXIT_FAILURE; }
	if (!ip_str) { return EXIT_FAILURE; }

	memset(ip, 0, sizeof(ipv4_t));

	if (!fill_addr(ip, ip_str)) { return EXIT_FAILURE; }
	if (!fill_bitmask(ip, ip_str)) { return EXIT_FAILURE; }
	if (!fill_netmask(ip)) { return EXIT_FAILURE; }
	if (!fill_wildcard(ip)) { return EXIT_FAILURE; }
	if (!fill_network(ip)) { return EXIT_FAILURE; }
	if (!fill_broadcast(ip)) { return EXIT_FAILURE; }
	if (!fill_hostmin(ip)) { return EXIT_FAILURE; }
	if (!fill_hostmax(ip)) { return EXIT_FAILURE; }
	if (!fill_hostcnt(ip)) { return EXIT_FAILURE; }

	print_ipv4(ip);

    return EXIT_SUCCESS;
}

static void print_ipv4(const ipv4_t *ip)
{
	if(!ip) { return; }

	/* Print title */
	printf("%15s%-20s%-40s%-11s\n", "", "DEC", "BIN", "HEX");

	/* Print addr */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Addr",	
			ip->addr[0], 
			ip->addr[1], 
			ip->addr[2],
			ip->addr[3],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->addr[0], 
			ip->addr[1], 
			ip->addr[2],
			ip->addr[3],
			"");
	printf("%02x.%02x.%02x.%02x\n", 
			ip->addr[0], 
			ip->addr[1], 
			ip->addr[2],
			ip->addr[3]);

	/* Print bitmask */
	printf("%-15s%d\n", "Bitmask", ip->bitmask);

	/* Prtint netmask */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Netmask",	
			ip->netmask[0], 
			ip->netmask[1], 
			ip->netmask[2],
			ip->netmask[3],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->netmask[0], 
			ip->netmask[1], 
			ip->netmask[2],
			ip->netmask[3],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->netmask[0], 
			ip->netmask[1], 
			ip->netmask[2],
			ip->netmask[3],
			"");

	/* Print wildcard */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Wildcard",	
			ip->wildcard[0], 
			ip->wildcard[1], 
			ip->wildcard[2],
			ip->wildcard[3],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->wildcard[0], 
			ip->wildcard[1], 
			ip->wildcard[2],
			ip->wildcard[3],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->wildcard[0], 
			ip->wildcard[1], 
			ip->wildcard[2],
			ip->wildcard[3],
			"");
	
	/* Print network */
	if(ip->is_point_to_point || ip->is_host_route) {
		printf("%-15s%s\n", "Network", "No network");
	}
	else {
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Network",	
				ip->network[0], 
				ip->network[1], 
				ip->network[2],
				ip->network[3],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip->network[0], 
				ip->network[1], 
				ip->network[2],
				ip->network[3],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip->network[0], 
				ip->network[1], 
				ip->network[2],
				ip->network[3],
				"");
	}

	/* Print broadcast */
	if(ip->is_point_to_point || ip->is_host_route) {
		printf("%-15s%s\n", "Broadcast", "No broadcast");
	}
	else {
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Broadcast",	
				ip->broadcast[0], 
				ip->broadcast[1], 
				ip->broadcast[2],
				ip->broadcast[3],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip->broadcast[0], 
				ip->broadcast[1], 
				ip->broadcast[2],
				ip->broadcast[3],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip->broadcast[0], 
				ip->broadcast[1], 
				ip->broadcast[2],
				ip->broadcast[3],
				"");
	}
	
	/* Print hostmin */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
			"Hostmin",	
			ip->hostmin[0], 
			ip->hostmin[1], 
			ip->hostmin[2],
			ip->hostmin[3],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->hostmin[0], 
			ip->hostmin[1], 
			ip->hostmin[2],
			ip->hostmin[3],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->hostmin[0], 
			ip->hostmin[1], 
			ip->hostmin[2],
			ip->hostmin[3],
			"");
	
	/* Print hostmax */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
			"Hostmax",	
			ip->hostmax[0], 
			ip->hostmax[1], 
			ip->hostmax[2],
			ip->hostmax[3],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->hostmax[0], 
			ip->hostmax[1], 
			ip->hostmax[2],
			ip->hostmax[3],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->hostmax[0], 
			ip->hostmax[1], 
			ip->hostmax[2],
			ip->hostmax[3],
			"");
	
	/* Print number of hosts */
	printf("%-15s%ld\n", "Hosts", ip->hostcnt);

	return;
}
