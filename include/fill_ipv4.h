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

#ifndef FILL_IPV4_H_SENTRY
#define FILL_IPV4_H_SENTRY

#include <stdint.h>
#include <stdlib.h>

#include "ipv4_t.h"

#define INDX_FRST_OCT		0
#define INDX_SCND_OCT		1
#define INDX_THRD_OCT		2
#define INDX_FRTH_OCT		3

#define BITS_IN_IP			32

/**
 * @brief Fill IPv4 address from string.
 * 
 * Parses the IPv4 address string (e.g. "192.168.1.1")
 * and stores the octets in the provided ipv4_t structure.
 * Stops parsing at '/' character or null terminator.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * @param ip_str String representation of IPv4.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @note On success, the addr_set field is set to 1.
 */
ipv4_t *fill_addr(ipv4_t *ip, const char *ip_str);

/**
 * @brief Fill bitmask from string.
 * 
 * Parses a string with CIDR notation (e.g "192.168.1.1/24" or "/24")
 * and stores it in the provided ipv4_t structure.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * @param cidr String containing CIDR notation.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @note On success, the bimask_set field is set to 1.
 * @note On success, if the mask is 32, the is_host_route flag is set to 1.
 * @note On success, if the mask is 31, the is_point_to_point flag is set to 1.
 */
ipv4_t *fill_bitmask(ipv4_t *ip, const char *cidr);

/**
 * @brief Fill netmask based on the bitmask.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @pre The bitmask_set flag in the ipv4_t structure must be set to 1.
 * 
 * @note On success, the netmask_set field is set to 1.
 * 
 * @see fill_bitmask
 */
ipv4_t *fill_netmask(ipv4_t *ip);

/**
 * @brief Fill wildcard based on the bitmask.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @pre The bitmask_set flag in the ipv4_t structure must be set to 1.
 * 
 * @note On success, the wildcard_set field is set to 1.
 * 
 * @see fill_bitmask
 */
ipv4_t *fill_wildcard(ipv4_t *ip);

/**
 * @brief Fill network based on the addr and netmask.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @pre The addr_set and netmask_set flags in the ipv4_t structure must be set to 1.
 * 
 * @note On success, the network_set field is set to 1.
 * 
 * @see fill_addr
 * @see fill_netmask
 */
ipv4_t *fill_network(ipv4_t *ip);

/**
 * @brief Fill broadcast based on the network and wildcard.
 * 
 * @param ip Pointer to the ipv4_t structure to fill.
 * 
 * @return Pointer to the ipv4_t structure, otherwise NULL.
 * 
 * @pre The network_set and wildcard_set flags in the ipv4_t structure must be set to 1.
 * 
 * @note On success, the broadcast_set field is set to 1.
 * 
 * @see fill_network
 * @see fill_wildcard
 */
ipv4_t *fill_broadcast(ipv4_t *ip);

/**
 * @brief Assigns to the hostmin field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmin field can be assigned a value, otherwise NULL.
 * @pre The network_set flag in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmin.
 * @note For /31 networks (is_point_to_point flag), hostmin equals network address.
 */
ipv4_t *fill_hostmin(ipv4_t *ip_ptr);

/**
 * @brief Assigns to the hostmax field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmax field can be assigned a value, otherwise NULL.
 * @pre The network_set and broadcast_set flags in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmax.
 * @note For /31 networks (indicated by the is_point_to_point flag), hostmax is calculated as network address + 1.
 */
ipv4_t *fill_hostmax(ipv4_t *ip_ptr);

/**
 * @brief Assigns to the hosts field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hosts field can be assigned a value, otherwise NULL.
 * @pre The bimask_set flag in ip_ptr must be set to 1.
 */
ipv4_t *fill_qt_hosts(ipv4_t *ip_ptr);

#endif /* FILL_IPV4_H_SENTRY */
