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
#include "macros.h"
#include "analysis.h"

int analysis_start(ipv4_t *ip, char *ip_str)
{
	assign_flags_ip_v4(ip);
	ASSIGN_ADDR(ip_str, ip, EXIT_FAILURE);
	ASSIGN_BITMASK(ip_str, ip, EXIT_FAILURE);
	ASSIGN_FIELD(netmask, ip, EXIT_FAILURE);
	ASSIGN_FIELD(wildcard, ip, EXIT_FAILURE);
	ASSIGN_FIELD(network, ip, EXIT_FAILURE);
	ASSIGN_BROADCAST(ip, EXIT_FAILURE);
	ASSIGN_HOSTX(hostmin, ip, EXIT_FAILURE);
	ASSIGN_HOSTX(hostmax, ip, EXIT_FAILURE);
	ASSIGN_QT_HOSTS(ip, EXIT_FAILURE);
	print_ip_v4(ip);
    return EXIT_SUCCESS;
}

void assign_flags_ip_v4(ipv4_t *ip_ptr)
{
	if(!ip_ptr) { return; }

	ip_ptr->addr_set = 0;
	ip_ptr->bitmask_set = 0;
	ip_ptr->netmask_set = 0;
	ip_ptr->wildcard_set = 0;
	ip_ptr->network_set = 0;
	ip_ptr->broadcast_set = 0;

	return;
}

void print_ip_v4(const ipv4_t *ip_ptr)
{
	if(!ip_ptr) {
		return;
	}

	printf("%15s%-20s%-40s%-11s\n", "", "DEC", "BIN", "HEX");
	/* Print IP */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "IP",	
			ip_ptr->addr[INDX_FRST_OCT], 
			ip_ptr->addr[INDX_SCND_OCT], 
			ip_ptr->addr[INDX_THRD_OCT],
			ip_ptr->addr[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip_ptr->addr[INDX_FRST_OCT], 
			ip_ptr->addr[INDX_SCND_OCT], 
			ip_ptr->addr[INDX_THRD_OCT],
			ip_ptr->addr[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x\n", 
			ip_ptr->addr[INDX_FRST_OCT], 
			ip_ptr->addr[INDX_SCND_OCT], 
			ip_ptr->addr[INDX_THRD_OCT],
			ip_ptr->addr[INDX_FRTH_OCT]);
	/* Print bitmask */
	printf("%-15s%d\n", "Bitmask", ip_ptr->bitmask);

	/* Prtint netmask */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Netmask",	
			ip_ptr->netmask[INDX_FRST_OCT], 
			ip_ptr->netmask[INDX_SCND_OCT], 
			ip_ptr->netmask[INDX_THRD_OCT],
			ip_ptr->netmask[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip_ptr->netmask[INDX_FRST_OCT], 
			ip_ptr->netmask[INDX_SCND_OCT], 
			ip_ptr->netmask[INDX_THRD_OCT],
			ip_ptr->netmask[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip_ptr->netmask[INDX_FRST_OCT], 
			ip_ptr->netmask[INDX_SCND_OCT], 
			ip_ptr->netmask[INDX_THRD_OCT],
			ip_ptr->netmask[INDX_FRTH_OCT],
			"");

	/* Print wildcard */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Wildcard",	
			ip_ptr->wildcard[INDX_FRST_OCT], 
			ip_ptr->wildcard[INDX_SCND_OCT], 
			ip_ptr->wildcard[INDX_THRD_OCT],
			ip_ptr->wildcard[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip_ptr->wildcard[INDX_FRST_OCT], 
			ip_ptr->wildcard[INDX_SCND_OCT], 
			ip_ptr->wildcard[INDX_THRD_OCT],
			ip_ptr->wildcard[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip_ptr->wildcard[INDX_FRST_OCT], 
			ip_ptr->wildcard[INDX_SCND_OCT], 
			ip_ptr->wildcard[INDX_THRD_OCT],
			ip_ptr->wildcard[INDX_FRTH_OCT],
			"");
	
	/* Print network */
	printf("%-15s%03d.%03d.%03d.%03d%5s",
		    "Network",	
			ip_ptr->network[INDX_FRST_OCT], 
			ip_ptr->network[INDX_SCND_OCT], 
			ip_ptr->network[INDX_THRD_OCT],
			ip_ptr->network[INDX_FRTH_OCT],
			"");
	printf("%08b.%08b.%08b.%08b%5s", 
			ip_ptr->network[INDX_FRST_OCT], 
			ip_ptr->network[INDX_SCND_OCT], 
			ip_ptr->network[INDX_THRD_OCT],
			ip_ptr->network[INDX_FRTH_OCT],
			"");
	printf("%02x.%02x.%02x.%02x%5s\n", 
			ip_ptr->network[INDX_FRST_OCT], 
			ip_ptr->network[INDX_SCND_OCT], 
			ip_ptr->network[INDX_THRD_OCT],
			ip_ptr->network[INDX_FRTH_OCT],
			"");

	/* Print broadcast */
	if(ip_ptr->is_point_to_point) {
		printf("%-15s%s\n", "Broadcast", "No broadcast (point-to-point)");
	}
	else {
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Broadcast",	
				ip_ptr->broadcast[INDX_FRST_OCT], 
				ip_ptr->broadcast[INDX_SCND_OCT], 
				ip_ptr->broadcast[INDX_THRD_OCT],
				ip_ptr->broadcast[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip_ptr->broadcast[INDX_FRST_OCT], 
				ip_ptr->broadcast[INDX_SCND_OCT], 
				ip_ptr->broadcast[INDX_THRD_OCT],
				ip_ptr->broadcast[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip_ptr->broadcast[INDX_FRST_OCT], 
				ip_ptr->broadcast[INDX_SCND_OCT], 
				ip_ptr->broadcast[INDX_THRD_OCT],
				ip_ptr->broadcast[INDX_FRTH_OCT],
				"");
	}
	
	if(ip_ptr->is_host_route) {
		printf("%-15s%s\n", "Hostmin", "No usable hosts");
		printf("%-15s%s\n", "Hostmax", "No usable hosts");
	}
	else {
		/* Print Hostmin */
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Hostmin",	
				ip_ptr->hostmin[INDX_FRST_OCT], 
				ip_ptr->hostmin[INDX_SCND_OCT], 
				ip_ptr->hostmin[INDX_THRD_OCT],
				ip_ptr->hostmin[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip_ptr->hostmin[INDX_FRST_OCT], 
				ip_ptr->hostmin[INDX_SCND_OCT], 
				ip_ptr->hostmin[INDX_THRD_OCT],
				ip_ptr->hostmin[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip_ptr->hostmin[INDX_FRST_OCT], 
				ip_ptr->hostmin[INDX_SCND_OCT], 
				ip_ptr->hostmin[INDX_THRD_OCT],
				ip_ptr->hostmin[INDX_FRTH_OCT],
				"");
		
		/* Print Hostmax */
		printf("%-15s%03d.%03d.%03d.%03d%5s",
				"Hostmax",	
				ip_ptr->hostmax[INDX_FRST_OCT], 
				ip_ptr->hostmax[INDX_SCND_OCT], 
				ip_ptr->hostmax[INDX_THRD_OCT],
				ip_ptr->hostmax[INDX_FRTH_OCT],
				"");
		printf("%08b.%08b.%08b.%08b%5s", 
				ip_ptr->hostmax[INDX_FRST_OCT], 
				ip_ptr->hostmax[INDX_SCND_OCT], 
				ip_ptr->hostmax[INDX_THRD_OCT],
				ip_ptr->hostmax[INDX_FRTH_OCT],
				"");
		printf("%02x.%02x.%02x.%02x%5s\n", 
				ip_ptr->hostmax[INDX_FRST_OCT], 
				ip_ptr->hostmax[INDX_SCND_OCT], 
				ip_ptr->hostmax[INDX_THRD_OCT],
				ip_ptr->hostmax[INDX_FRTH_OCT],
				"");
	}

	/* Print quantity hosts */
	printf("%-15s%ld\n", "Hosts", ip_ptr->qt_hosts);
}