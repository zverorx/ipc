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

#ifndef FUNCTIONS_H_SENTRY
#define FUNCTIONS_H_SENTRY

#include <stdint.h>
#include <stdlib.h>

#define INDX_FRST_OCT		0
#define INDX_SCND_OCT		1
#define INDX_THRD_OCT		2
#define INDX_FRTH_OCT		3

#define OCTET_COUNT			4
#define BITS_IN_IP			32

/**
 * @brief Structure to store IPv4 address analysis data.
 *
 * This structure holds the original IP address, the subnet mask,
 * derived network information (network address, broadcast address, usable host range),
 * and wildcard mask. It also includes flags to track which fields have been successfully calculated.
 */
struct ip_v4 {
    uint8_t addr[OCTET_COUNT];          /**< The original IPv4 address octets. */
    uint8_t bitmask;                    /**< The subnet mask length in CIDR notation (e.g., 24 for /24). */
    uint8_t netmask[OCTET_COUNT];       /**< The subnet mask in dotted-decimal notation (e.g., 255.255.255.0). */
    uint8_t wildcard[OCTET_COUNT];      /**< The wildcard (inverse) mask in dotted-decimal notation. */
    uint8_t network[OCTET_COUNT];       /**< The calculated network address. */
    uint8_t broadcast[OCTET_COUNT];     /**< The calculated broadcast address (not set for /31). */
    uint8_t hostmin[OCTET_COUNT];       /**< The first usable host address (not set for /32). */
    uint8_t hostmax[OCTET_COUNT];       /**< The last usable host address (not set for /32). */
    uint64_t qt_hosts;                  /**< The total number of host addresses in the subnet. */

    uint8_t addr_set;                   /**< Flag indicating if the 'addr' field is valid (1) or not (0). */
    uint8_t bitmask_set;                /**< Flag indicating if the 'bitmask' field is valid (1) or not (0). */
    uint8_t netmask_set;                /**< Flag indicating if the 'netmask' field is valid (1) or not (0). */
    uint8_t wildcard_set;               /**< Flag indicating if the 'wildcard' field is valid (1) or not (0). */
    uint8_t network_set;                /**< Flag indicating if the 'network' field is valid (1) or not (0). */
    uint8_t broadcast_set;              /**< Flag indicating if the 'broadcast' field is valid (1) or not (0). */

    uint8_t is_host_route;              /**< Flag indicating if the subnet is a /32 host route (no usable hosts for interfaces). */
    uint8_t is_point_to_point;          /**< Flag indicating if the subnet is a /31 point-to-point link (no broadcast address). */
};

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
struct ip_v4 *assign_addr(const char *ip_str, struct ip_v4 *ip_ptr);

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
struct ip_v4 *assign_bitmask(const char *cidr, struct ip_v4 *ip_ptr);

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
struct ip_v4 *assign_netmask(struct ip_v4 *ip_ptr);

/**
 * @brief Assigns to the wildcard field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the wildcard field can be assigned a value, otherwise NULL.
 * @pre The bitmask_set flag in ip_ptr must be set to 1.
 * @note This function calculates the wildcard mask based on the bitmask value stored in the ip_ptr structure.
 * @warning This function does not modify the wildcard_set flag. 
 *     The calling code must set ip_ptr->wildcard_set = 1 upon successful return.
 */
struct ip_v4 *assign_wildcard(struct ip_v4 *ip_ptr);

/**
 * @brief Assigns to the network field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the network field can be assigned a value, otherwise NULL.
 * @pre The addr_set and netmask_set flags in ip_ptr must be set to 1.
 * @post If successful, the network field in ip_ptr is populated.
 * @note This function calculates the network address by performing 
 *     a bitwise AND operation between the IP address and the netmask.
 * @warning This function does not modify the network_set flag. 
 *     The calling code must set ip_ptr->network_set = 1 upon successful return.
 */
struct ip_v4 *assign_network(struct ip_v4 *ip_ptr);

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
struct ip_v4 *assign_broadcast(struct ip_v4 *ip_ptr);

/**
 * @brief Assigns to the hostmin field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmin field can be assigned a value, otherwise NULL.
 * @pre The network_set flag in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmin.
 * @note For /31 networks (is_point_to_point flag), hostmin equals network address.
 */
struct ip_v4 *assign_hostmin(struct ip_v4 *ip_ptr);

/**
 * @brief Assigns to the hostmax field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hostmax field can be assigned a value, otherwise NULL.
 * @pre The network_set and broadcast_set flags in ip_ptr must be set to 1.
 * @note For /32 networks (is_host_route flag), this function will not calculate hostmax.
 * @note For /31 networks (indicated by the is_point_to_point flag), hostmax is calculated as network address + 1.
 */
struct ip_v4 *assign_hostmax(struct ip_v4 *ip_ptr);

/**
 * @brief Assigns to the hosts field of the ip_v4 structure.
 * @param ip_ptr A reference to a structure storing IP data.
 * @return Address of the ip_v4 structure, if the hosts field can be assigned a value, otherwise NULL.
 * @pre The bimask_set flag in ip_ptr must be set to 1.
 */
struct ip_v4 *assign_qt_hosts(struct ip_v4 *ip_ptr);

#endif
