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

#include "ipv4_t.h"
#include "fill_ipv4.h"

ipv4_t *fill_addr(const char *ip_str, ipv4_t *ip_ptr)
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

ipv4_t *fill_bitmask(const char *cidr, ipv4_t *ip_ptr)
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

ipv4_t *fill_netmask(ipv4_t *ip_ptr)
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

ipv4_t *fill_wildcard(ipv4_t *ip_ptr)
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

ipv4_t *fill_network(ipv4_t *ip_ptr)
{
	if(!ip_ptr->addr_set || !ip_ptr->netmask_set) {
		return NULL;
	}
	
	for(int i = 0; i < OCTET_COUNT; i++) {
		ip_ptr->network[i] = ip_ptr->addr[i] & ip_ptr->netmask[i];
	}
	return ip_ptr;
}

ipv4_t *fill_broadcast(ipv4_t *ip_ptr)
{
	if(!ip_ptr->network_set || !ip_ptr->wildcard_set || ip_ptr->is_point_to_point) {
		return NULL;
	}
	
	for(int i = 0; i < OCTET_COUNT; i++) {
		ip_ptr->broadcast[i] = ip_ptr->network[i] | ip_ptr->wildcard[i];
	}
	return ip_ptr;
}

ipv4_t *fill_hostmin(ipv4_t *ip_ptr)
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

ipv4_t *fill_hostmax(ipv4_t *ip_ptr)
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

ipv4_t *fill_qt_hosts(ipv4_t *ip_ptr)
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

void fill_flags(ipv4_t *ip_ptr)
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
