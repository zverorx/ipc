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

#include <stdio.h>
#include "ipc.h"

/**
 * @brief Assigns a value to the addr field of the ip_v4 structure.
 * @param ip_str String representation of IP (e.g., "192.168.1.1").
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the addr field can be assigned a value, otherwise NULL.
 * @post If successful, the addr field in ip_ptr is populated, and the caller is responsible for setting the addr_set flag to 1.
 * @note This function parses the IP address part of the string up to the '/' character (if present) or the end of the string.
 * @note The function validates that each octet is a number between 0 and 255 and that there are exactly 4 octets separated by dots.
 * @warning This function does not modify the addr_set flag. The calling code must set ip_ptr->addr_set = 1 upon successful return.
 */
struct ip_v4 *assign_addr(const char *ip_str, struct ip_v4 *ip_ptr)
{
	uint32_t octet = 0;
	uint32_t dot_count = 0;
	uint8_t exists_num_in_oct = 0;

	for(int i = 0; ip_str[i] != '\0' && ip_str[i] != '/'; i++) {
		if(ip_str[i] >= '0' && ip_str[i] <= '9') {
			exists_num_in_oct = 1;
			octet = octet * 10 + (ip_str[i] - '0');
		}
		else if((ip_str[i] == '.') && (octet <= 255) && exists_num_in_oct) {
			dot_count++;
			switch(dot_count) {
				case 1:
					ip_ptr->addr[INDX_FRST_OCT] = octet;
					break;
				case 2:
					ip_ptr->addr[INDX_SCND_OCT] = octet;
					break;
				case 3:
					ip_ptr->addr[INDX_THRD_OCT] = octet;
					break;
				default:
					return NULL;
			}
			octet = 0;
			exists_num_in_oct = 0;
		}
		else {
			return NULL;
		}
	}
	if((dot_count == 3) && (octet <= 255) && exists_num_in_oct) {
		ip_ptr->addr[INDX_FRTH_OCT] = octet;
	}
	else {
		return NULL;
	}
	return ip_ptr;
}

/**
 * @brief Assigns to the bitmask field of the ip_v4 structure.
 * @param cidr IP/MASK format string (e.g., "192.168.1.1/24").
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the bitmask field can be assigned a value, otherwise NULL.
 * @post If successful, the bitmask field in ip_ptr is populated.
 * @note This function parses the bitmask part of the string following the '/' character.
 * @note The function validates that the bitmask is a number between 0 and 32.
 * @warning This function does not modify the bitmask_set, is_host_route, or is_point_to_point flags. 
 *     The calling code must set these flags based on the parsed bitmask value upon successful return.
 * @see BITS_IN_IP
 */
struct ip_v4 *assign_bitmask(const char *cidr, struct ip_v4 *ip_ptr)
{
	uint8_t bitmask = 0;
	uint8_t exist_mask = 0;

	for(int i = 0; cidr[i] != '\0'; i++) {
		if(cidr[i] == '/') {
			exist_mask = 1;
			i++;
		}
		if(exist_mask && cidr[i] >= '0' && cidr[i] <= '9') {
			bitmask = bitmask * 10 + (cidr[i] - '0');
		}
		else if(exist_mask){
			return NULL;
		}
	}
	if(bitmask > 32 || !exist_mask) {
		return NULL;
	}
	ip_ptr->bitmask = bitmask;
	return ip_ptr;
}

/**
 * @brief Assigns to the netmask field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the netmask field can be assigned a value, otherwise NULL.
 * @pre The bitmask_set flag in ip_ptr must be set to 1.
 * @post If successful, the netmask field in ip_ptr is populated, 
 *     and the caller is responsible for setting the netmask_set flag to 1.
 * @note This function calculates the netmask based on the bitmask value stored in the ip_ptr structure.
 * @warning This function does not modify the netmask_set flag. 
 *     The calling code must set ip_ptr->netmask_set = 1 upon successful return.
 */
struct ip_v4 *assign_netmask(struct ip_v4 *ip_ptr)
{
	if(!ip_ptr->bitmask_set) {
		return NULL;
	}
	uint8_t octet_cnt = OCTET_COUNT;
	uint8_t octet = 1;
	uint8_t offset = 0;
	uint8_t netmask[33] = {[0 ... 32] = 0};
	for(int i = 0; i < ip_ptr->bitmask; i++) {
		netmask[i] = 1;
	}

	while(octet_cnt) {
		for(int i = 0 + 8 * offset; i <= 7 + 8 * offset; i++) {
			octet = (octet * 2) + netmask[i];
		}
		ip_ptr->netmask[offset] = octet;
		offset++;
		octet_cnt--;
		octet = 1;
	}
	return ip_ptr;
}

/**
 * @brief Assigns to the wildcard field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the wildcard field can be assigned a value, otherwise NULL.
 * @pre The bitmask_set flag in ip_ptr must be set to 1.
 * @note This function calculates the wildcard mask based on the bitmask value stored in the ip_ptr structure.
 * @warning This function does not modify the wildcard_set flag. 
 *     The calling code must set ip_ptr->wildcard_set = 1 upon successful return.
 */
struct ip_v4 *assign_wildcard(struct ip_v4 *ip_ptr)
{
	if(!ip_ptr->bitmask_set) {
		return NULL;
	}
	uint8_t octet_cnt = OCTET_COUNT;
	uint8_t octet = 1;
	uint8_t offset = 0;
	uint8_t wildcard[32] = {[0 ... 31] = 1};
	for(int i = 0; i < ip_ptr->bitmask; i++) {
		wildcard[i] = 0;
	}

	while(octet_cnt) {
		for(int i = 0 + 8 * offset; i <= 7 + 8 * offset; i++) {
			octet = (octet * 2) + wildcard[i];
		}
		ip_ptr->wildcard[offset] = octet;
		offset++;
		octet_cnt--;
		octet = 1;
	}
	return ip_ptr;
}

/**
 * @brief Assigns to the network field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the network field can be assigned a value, otherwise NULL.
 * @pre The addr_set and netmask_set flags in ip_ptr must be set to 1.
 * @post If successful, the network field in ip_ptr is populated.
 * @note This function calculates the network address by performing 
 *     a bitwise AND operation between the IP address and the netmask.
 * @warning This function does not modify the network_set flag. 
 *    The calling code must set ip_ptr->network_set = 1 upon successful return.
 */
struct ip_v4 *assign_network(struct ip_v4 *ip_ptr)
{
	if(!ip_ptr->addr_set || !ip_ptr->netmask_set) {
		return NULL;
	}
	
	for(int i = 0; i < OCTET_COUNT; i++) {
		ip_ptr->network[i] = ip_ptr->addr[i] & ip_ptr->netmask[i];
	}
	return ip_ptr;
}

/**
 * @brief Assigns to the broadcast field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the broadcast field can be assigned a value, otherwise NULL.
 * @pre The network_set and wildcard_set flags in ip_ptr must be set to 1.
 * @pre The bitmask must not be 31 (/31 networks do not have a broadcast address).
 * @post If successful, the broadcast field in ip_ptr is populated.
 * @note This function calculates the broadcast address by performing 
 *     a bitwise OR operation between the network address and the wildcard mask.
 * @warning This function does not modify the broadcast_set flag. 
 *     The calling code must set ip_ptr->broadcast_set = 1 upon successful return.
 * @warning This function will return NULL for /31 networks (bitmask == 31) as they do not have a broadcast address.
 */
struct ip_v4 *assign_broadcast(struct ip_v4 *ip_ptr)
{
	if(!ip_ptr->network_set || !ip_ptr->wildcard_set || ip_ptr->is_point_to_point) {
		return NULL;
	}
	
	for(int i = 0; i < OCTET_COUNT; i++) {
		ip_ptr->broadcast[i] = ip_ptr->network[i] | ip_ptr->wildcard[i];
	}
	return ip_ptr;
}

/**
 * @brief Assigns to the hostmin field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmin field can be assigned a value, otherwise NULL.
 * @pre The network_set flag in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmin.
 * @note For /31 networks (is_point_to_point flag), hostmin equals network address.
 */
struct ip_v4 *assign_hostmin(struct ip_v4 *ip_ptr)
{
	if(ip_ptr->network_set && !ip_ptr->is_host_route) {
		for(int i = 0; i < OCTET_COUNT - 1; i++) {
			ip_ptr->hostmin[i] = ip_ptr->network[i];
		}
		if(ip_ptr->is_point_to_point) {
			ip_ptr->hostmin[INDX_FRTH_OCT] = ip_ptr->network[INDX_FRTH_OCT];
		}
		else {
			ip_ptr->hostmin[INDX_FRTH_OCT] = ip_ptr->network[INDX_FRTH_OCT] + 1;
		}
	}
	else {
		return NULL;
	}

	return ip_ptr;
}

/**
 * @brief Assigns to the hostmax field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmax field can be assigned a value, otherwise NULL.
 * @pre The network_set and broadcast_set flags in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmax.
 * @note For /31 networks (indicated by the is_point_to_point flag), hostmax is calculated as network address + 1.
 */
struct ip_v4 *assign_hostmax(struct ip_v4 *ip_ptr)
{
	if(ip_ptr->network_set && ip_ptr->is_point_to_point) {
		for(int i = 0; i < OCTET_COUNT - 1; i++) {
			ip_ptr->hostmax[i] = ip_ptr->network[i];
		}

		ip_ptr->hostmax[INDX_FRTH_OCT] = ip_ptr->network[INDX_FRTH_OCT] + 1;
	}
	else if(ip_ptr->broadcast_set && !ip_ptr->is_host_route) {
		for(int i = 0; i < OCTET_COUNT - 1; i++) {
			ip_ptr->hostmax[i] = ip_ptr->broadcast[i];
		}

		ip_ptr->hostmax[INDX_FRTH_OCT] = ip_ptr->broadcast[INDX_FRTH_OCT] - 1;

	}
	else {
		return NULL;
	}

	return ip_ptr;
}

/**
 * @brief Assigns to the hosts field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hosts field can be assigned a value, otherwise NULL.
 * @pre The bimask_set flag in ip_ptr must be set to 1.
 */
struct ip_v4 *assign_qt_hosts(struct ip_v4 *ip_ptr)
{
	uint8_t bits_for_host;
	uint64_t qt_hosts = 1;

	if(!ip_ptr->bitmask_set) {
		return NULL;
	}
	bits_for_host = BITS_IN_IP - ip_ptr->bitmask;
	for(int i = 0; i < bits_for_host; i++) {
		qt_hosts *= 2;
	}

	if(!bits_for_host) {
		ip_ptr->qt_hosts = 0;
	}
	else if(bits_for_host == 1) {
		/*point-to-point*/
		ip_ptr->qt_hosts = 2;
	}
	else {
		ip_ptr->qt_hosts = qt_hosts - 2;
	}

	return ip_ptr;
}

/**
 * @brief Sets all flag fields of the ip_v4 structure to 0. 
 * @param ip_ptr A reference to a structure storing IP data.
 * @return void.
 * @note The flags are set to 0: addr_set, bitmask_set, netmask_set, wildcard_set, network_set, broadcast_set.
 */
void assign_flags_ip_v4(struct ip_v4 *ip_ptr)
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
void print_ip_v4(const struct ip_v4 *ip_ptr)
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