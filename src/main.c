/* 
 * This file is part of ipc.
 * ipc - IP calculator, designed for IP analysis 
 *
 * Copyright (C) 2025 Egorov Konstantin
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

/**
 * @file main.c
 * @brief IP calculator, designed for IP analysis
 */

#include <stdio.h>
#include "functions.h"

#define QT_CMD_LN_PARAM		2
#define CMD_LN_PARAM_IP		argv[1]

/**
 * @def CHECK_CMD_LN_PARAM(ERROR)
 * @brief Exits with ERROR if argc != QT_CMD_LN_PARAM.
 */

#define CHECK_CMD_LN_PARAM(ERROR) \
    do { \
        if(argc != QT_CMD_LN_PARAM) { \
			fprintf(stderr, "Usage: %s <IP/CIDR>\n", argv[0]); \
            return ERROR; \
        } \
    } while(0)

/**
 * @def CHECK_POINTER_IP_V4(POINTER, ERROR)
 * @brief Exits with ERROR if POINTER is NULL, else initializes its flags.
 */
#define CHECK_POINTER_IP_V4(POINTER, ERROR) \
	do { \
		if(!POINTER) { \
			fputs("Memory allocation failed\n", stderr); \
			return ERROR; \
		} \
		assign_flags_ip_v4(POINTER); \
	} while(0)

/**
 * @def ASSIGN_FIELD(FIELDNAME, POINTER, ERROR)
 * @brief Calls assign_FIELDNAME(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets POINTER->FIELDNAME_set = 1 on success.
 */
#define ASSIGN_FIELD(FIELDNAME, POINTER, ERROR) \
	do { \
		if(!assign_##FIELDNAME(POINTER)) { \
			fprintf(stderr, "Error: Unable to get %s\n", #FIELDNAME); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->FIELDNAME##_set = 1; \
	} while(0)

/**
 * @def ASSIGN_HOSTX(FIELDNAME, POINTER, ERROR)
 * @brief Calls assign_FIELDNAME(); on fail and not host route, prints error, frees POINTER, returns ERROR.
 */
#define ASSIGN_HOSTX(FIELDNAME, POINTER, ERROR) \
	do { \
		if(!assign_##FIELDNAME(POINTER) && !POINTER->is_host_route) { \
			fprintf(stderr, "Error: Unable to get %s\n", #FIELDNAME); \
			free(POINTER); \
			return ERROR; \
		} \
	} while(0)

/**
 * @def ASSIGN_QT_HOSTS(POINTER, ERROR)
 * @brief Calls assign_qt_hosts(); on fail, prints error, frees POINTER, returns ERROR.
 */
#define ASSIGN_QT_HOSTS(POINTER, ERROR) \
	do { \
		if(!assign_qt_hosts(POINTER)) { \
			fputs("Error: Unable to get quantity hosts\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
	} while(0)

/**
 * @def ASSIGN_BROADCAST(POINTER, ERROR)
 * @brief Calls assign_broadcast(); on fail and not p2p, prints error, frees POINTER, returns ERROR.
 * @brief Sets broadcast_set = 1 on success.
 */
#define ASSIGN_BROADCAST(POINTER, ERROR) \
	do { \
		if(!assign_broadcast(POINTER) && !POINTER->is_point_to_point) { \
			fputs("Error: Unable to get broadcast\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->broadcast_set = 1; \
	} while(0)

/**
 * @def ASSIGN_ADDR(IP, POINTER, ERROR)
 * @brief Calls assign_addr(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets addr_set = 1 on success.
 */
#define ASSIGN_ADDR(IP, POINTER, ERROR) \
	do { \
		if(!assign_addr(IP, POINTER)) { \
			fputs("Error: Incorrect IP\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->addr_set = 1; \
	} while(0)

/**
 * @def ASSIGN_BITMASK(CIDR, POINTER, ERROR)
 * @brief Calls assign_bitmask(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets bitmask_set = 1 and updates is_host_route or is_point_to_point if needed.
 */
#define ASSIGN_BITMASK(CIDR, POINTER, ERROR) \
	do { \
		if(!assign_bitmask(CIDR, POINTER)) { \
			fputs("Error: Incorrect mask\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		else if(POINTER->bitmask == BITS_IN_IP) { \
			POINTER->is_host_route = 1; \
			POINTER->bitmask_set = 1; \
		} \
		else if(POINTER->bitmask == BITS_IN_IP - 1) { \
			POINTER->is_point_to_point = 1; \
			POINTER->bitmask_set = 1; \
		} \
		else { \
			POINTER->is_host_route = 0; \
			POINTER->is_point_to_point = 0; \
			POINTER->bitmask_set = 1; \
		} \
	} while(0)

/**
 * @enum errors
 * @brief Error codes returned by the program.
 */
enum errors {
	SUCCESS					= 0,	/*< No error. */
	CMD_LINE_PARAM_ERR		= 1,	/*< Wrong number of command-line args. */
	MEMORY_ALLOC_ERR		= 2,	/*< Malloc failed for ip_v4 struct. */
	INCORRECT_IP_ERR		= 3,	/*< Invalid IP address format. */
	INCORRECT_MASK_ERR		= 4,	/*< Invalid bitmask. */
	GET_NETMASK_ERR			= 5,	/*< Failed to compute netmask. */
	GET_WILDCARD_ERR		= 6,	/*< Failed to compute wildcard mask. */
	GET_NETWORK_ERR			= 7,	/*< Failed to compute network address. */
	GET_BROADCAST_ERR		= 8,	/*< Failed to compute broadcast address. */
	GET_HOSTMIN_ERR			= 9,	/*< Failed to compute first host IP. */
	GET_HOSTMAX_ERR			= 10,	/*< Failed to compute last host IP. */
	GET_QT_HOSTS_ERR		= 11	/*< Failed to compute number of hosts. */
};

/**
 * @brief Prints the ip_v4 structure data to stdout.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return void.
 */
static void print_ip_v4(const struct ip_v4 *ip_ptr);

/**
 * @brief Sets all flag fields of the ip_v4 structure to 0. 
 * @param ip_ptr A reference to a structure storing IP data.
 * @return void.
 * @note The flags are set to 0: addr_set, bitmask_set, netmask_set, wildcard_set, network_set, broadcast_set.
 * @see function.h (struct ip_v4).
 */
static void assign_flags_ip_v4(struct ip_v4 *ip_ptr);

int main(int argc, char **argv)
{
	struct ip_v4 *ip_ptr = malloc(sizeof(struct ip_v4));

	CHECK_POINTER_IP_V4(ip_ptr, MEMORY_ALLOC_ERR);
	CHECK_CMD_LN_PARAM(CMD_LINE_PARAM_ERR);

	ASSIGN_ADDR(CMD_LN_PARAM_IP, ip_ptr, INCORRECT_IP_ERR);
	ASSIGN_BITMASK(CMD_LN_PARAM_IP, ip_ptr, INCORRECT_MASK_ERR);
	ASSIGN_FIELD(netmask, ip_ptr, GET_NETMASK_ERR);
	ASSIGN_FIELD(wildcard, ip_ptr, GET_WILDCARD_ERR);
	ASSIGN_FIELD(network, ip_ptr, GET_NETWORK_ERR);
	ASSIGN_BROADCAST(ip_ptr, GET_BROADCAST_ERR);
	ASSIGN_HOSTX(hostmin, ip_ptr, GET_HOSTMIN_ERR);
	ASSIGN_HOSTX(hostmax, ip_ptr, GET_HOSTMAX_ERR);
	ASSIGN_QT_HOSTS(ip_ptr, GET_QT_HOSTS_ERR);

	print_ip_v4(ip_ptr);
	free(ip_ptr);
	return SUCCESS;
}

/**
 * @brief Sets all flag fields of the ip_v4 structure to 0. 
 * @param ip_ptr A reference to a structure storing IP data.
 * @return void.
 * @note The flags are set to 0: addr_set, bitmask_set, netmask_set, wildcard_set, network_set, broadcast_set.
 * @see function.h (struct ip_v4).
 */
static void assign_flags_ip_v4(struct ip_v4 *ip_ptr)
{
	if(!ip_ptr) {
		return;
	}
	ip_ptr->addr_set = 0;
	ip_ptr->bitmask_set = 0;
	ip_ptr->netmask_set = 0;
	ip_ptr->wildcard_set = 0;
	ip_ptr->network_set = 0;
	ip_ptr->broadcast_set = 0;
	return;
}

/**
 * @brief Prints the ip_v4 structure data to stdout.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return void.
 */
static void print_ip_v4(const struct ip_v4 *ip_ptr)
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
