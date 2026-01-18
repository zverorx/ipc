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
 * @struct ipv4
 * 
 * @brief IPv4 address analysis structure.
 * 
 * @warning Initialize the structure with zeros before using. 
 */
typedef struct ipv4 {
    uint8_t addr[OCTET_COUNT];      /**< IPv4 octets */
    uint8_t bitmask;                /**< Mask length (e.g. 24) */
    uint8_t netmask[OCTET_COUNT];   /**< Subnet mask (e.g. [255][255][255][0]) */
    uint8_t wildcard[OCTET_COUNT];  /**< Inverse mask */
    uint8_t network[OCTET_COUNT];   /**< Network address */
    uint8_t broadcast[OCTET_COUNT]; /**< Broadcast address */
    uint8_t hostmin[OCTET_COUNT];   /**< First usable host */
    uint8_t hostmax[OCTET_COUNT];   /**< Last usable host */
    uint64_t hostcnt;               /**< Total host addresses */

    uint8_t addr_set;               /**< Address valid */
    uint8_t bitmask_set;            /**< Bitmask valid */
    uint8_t netmask_set;            /**< Netmask calculated */
    uint8_t wildcard_set;           /**< Wildcard calculated */
    uint8_t network_set;            /**< Network calculated */
    uint8_t broadcast_set;          /**< Broadcast calculated */

    uint8_t is_host_route;          /**< /32 host route flag */
    uint8_t is_point_to_point;      /**< /31 P2P link flag */
} ipv4_t;

#endif /* IPV4_H_SENTRY */
