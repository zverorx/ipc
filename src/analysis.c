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
 * @brief Prints the ip data to stdout.
 * @param ip A reference to a structure storing IP data.
 */
static void print_ip_v4(const ipv4_t *ip);

int analysis_start(ipv4_t *ip, char *ip_str)
{
	fill_flags(ip);

	if (!fill_addr(ip_str, ip)) { return EXIT_FAILURE; }
	ip->addr_set = 1;

	if(!fill_bitmask(ip_str, ip)) { return EXIT_FAILURE; }
	else if(ip->bitmask == BITS_IN_IP) {
		ip->is_host_route = 1;
		ip->bitmask_set = 1;
	}
	else if(ip->bitmask == BITS_IN_IP - 1) {
		ip->is_point_to_point = 1;
		ip->bitmask_set = 1;
	}
	else {
		ip->is_host_route = 0;
		ip->is_point_to_point = 0;
		ip->bitmask_set = 1;
	}

	if(!fill_netmask(ip)) { return EXIT_FAILURE; }
	ip->netmask_set = 1;

	if(!fill_wildcard(ip)) { return EXIT_FAILURE; }
	ip->wildcard_set = 1;

	if(!fill_network(ip)) { return EXIT_FAILURE; }
	ip->network_set = 1;

	if(!fill_broadcast(ip) && !ip->is_point_to_point) {
		return EXIT_FAILURE;
	}
	ip->broadcast_set = 1;

	if(!fill_hostmin(ip) && !ip->is_host_route) {
		return EXIT_FAILURE;
	}

	if(!fill_hostmax(ip) && !ip->is_host_route) {
		return EXIT_FAILURE;
	}

	if(!fill_qt_hosts(ip)) { return EXIT_FAILURE; }

	print_ip_v4(ip);

    return EXIT_SUCCESS;
}

static void print_ip_v4(const ipv4_t *ip)
{
	if(!ip) {
		return;
	}

	printf("%15s%-20s%-40s%-11s\n", "", "DEC", "BIN", "HEX");
	/* Print IP */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "IP",	
			ip->addr[INDX_FRST_OCT], 
			ip->addr[INDX_SCND_OCT], 
			ip->addr[INDX_THRD_OCT],
			ip->addr[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->addr[INDX_FRST_OCT], 
			ip->addr[INDX_SCND_OCT], 
			ip->addr[INDX_THRD_OCT],
			ip->addr[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x\n", 
			ip->addr[INDX_FRST_OCT], 
			ip->addr[INDX_SCND_OCT], 
			ip->addr[INDX_THRD_OCT],
			ip->addr[INDX_FRTH_OCT]);
	/* Print bitmask */
	printf("%-15s%d\n", "Bitmask", ip->bitmask);

	/* Prtint netmask */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Netmask",	
			ip->netmask[INDX_FRST_OCT], 
			ip->netmask[INDX_SCND_OCT], 
			ip->netmask[INDX_THRD_OCT],
			ip->netmask[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->netmask[INDX_FRST_OCT], 
			ip->netmask[INDX_SCND_OCT], 
			ip->netmask[INDX_THRD_OCT],
			ip->netmask[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->netmask[INDX_FRST_OCT], 
			ip->netmask[INDX_SCND_OCT], 
			ip->netmask[INDX_THRD_OCT],
			ip->netmask[INDX_FRTH_OCT],
			"");

	/* Print wildcard */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Wildcard",	
			ip->wildcard[INDX_FRST_OCT], 
			ip->wildcard[INDX_SCND_OCT], 
			ip->wildcard[INDX_THRD_OCT],
			ip->wildcard[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->wildcard[INDX_FRST_OCT], 
			ip->wildcard[INDX_SCND_OCT], 
			ip->wildcard[INDX_THRD_OCT],
			ip->wildcard[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->wildcard[INDX_FRST_OCT], 
			ip->wildcard[INDX_SCND_OCT], 
			ip->wildcard[INDX_THRD_OCT],
			ip->wildcard[INDX_FRTH_OCT],
			"");
	
	/* Print network */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Network",	
			ip->network[INDX_FRST_OCT], 
			ip->network[INDX_SCND_OCT], 
			ip->network[INDX_THRD_OCT],
			ip->network[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip->network[INDX_FRST_OCT], 
			ip->network[INDX_SCND_OCT], 
			ip->network[INDX_THRD_OCT],
			ip->network[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip->network[INDX_FRST_OCT], 
			ip->network[INDX_SCND_OCT], 
			ip->network[INDX_THRD_OCT],
			ip->network[INDX_FRTH_OCT],
			"");

	/* Print broadcast */
	if(ip->is_point_to_point) {
		printf("%-15s%s\n", "Broadcast", "No broadcast (point-to-point)");
	}
	else {
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Broadcast",	
				ip->broadcast[INDX_FRST_OCT], 
				ip->broadcast[INDX_SCND_OCT], 
				ip->broadcast[INDX_THRD_OCT],
				ip->broadcast[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip->broadcast[INDX_FRST_OCT], 
				ip->broadcast[INDX_SCND_OCT], 
				ip->broadcast[INDX_THRD_OCT],
				ip->broadcast[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip->broadcast[INDX_FRST_OCT], 
				ip->broadcast[INDX_SCND_OCT], 
				ip->broadcast[INDX_THRD_OCT],
				ip->broadcast[INDX_FRTH_OCT],
				"");
	}
	
	if(ip->is_host_route) {
		printf("%-15s%s\n", "Hostmin", "No usable hosts");
		printf("%-15s%s\n", "Hostmax", "No usable hosts");
	}
	else {
		/* Print Hostmin */
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Hostmin",	
				ip->hostmin[INDX_FRST_OCT], 
				ip->hostmin[INDX_SCND_OCT], 
				ip->hostmin[INDX_THRD_OCT],
				ip->hostmin[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip->hostmin[INDX_FRST_OCT], 
				ip->hostmin[INDX_SCND_OCT], 
				ip->hostmin[INDX_THRD_OCT],
				ip->hostmin[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip->hostmin[INDX_FRST_OCT], 
				ip->hostmin[INDX_SCND_OCT], 
				ip->hostmin[INDX_THRD_OCT],
				ip->hostmin[INDX_FRTH_OCT],
				"");
		
		/* Print Hostmax */
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Hostmax",	
				ip->hostmax[INDX_FRST_OCT], 
				ip->hostmax[INDX_SCND_OCT], 
				ip->hostmax[INDX_THRD_OCT],
				ip->hostmax[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip->hostmax[INDX_FRST_OCT], 
				ip->hostmax[INDX_SCND_OCT], 
				ip->hostmax[INDX_THRD_OCT],
				ip->hostmax[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip->hostmax[INDX_FRST_OCT], 
				ip->hostmax[INDX_SCND_OCT], 
				ip->hostmax[INDX_THRD_OCT],
				ip->hostmax[INDX_FRTH_OCT],
				"");
	}

	/* Print quantity hosts */
	printf("%-15s%ld\n", "Hosts", ip->qt_hosts);
}
