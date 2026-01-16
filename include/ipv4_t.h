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

#ifndef IPV4_H_SENTRY
#define IPV4_H_SENTRY

#include <stdint.h>

#define OCTET_COUNT			4

/**
 * @brief Structure to store IPv4 address analysis data.
 *
 * This structure holds the original IP address, the subnet mask,
 * derived network information (network address, broadcast address, usable host range),
 * and wildcard mask. It also includes flags to track which fields have been successfully calculated.
 */
typedef struct ipv4 {
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
} ipv4_t;

#endif /* IPV4_H_SENTRY */