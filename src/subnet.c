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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "ipv4_t.h"
#include "fill_ipv4.h"
#include "subnet.h"
#include "subnet_list.h"

/**
 * @brief Moves the network address to the start of the next subnet. 
 * @param ip Structure with address data.
 * @return Pointer to modified structure, or NULL otherwise.
 * @pre The network_set flag in the ipv4_t structure must be set to 1.
 */
static ipv4_t *switch_subnet(ipv4_t *ip);

/**
 * @brief Calculates the minimal power-of-two exponent to accommodate a given number.
 * @param target The required minimum capacity.
 * @return The smallest integer k such that 2^k >= target, or 0 if target <= 1.
 */
static int get_min_power_of_two(int target);

int subnetting_start(ipv4_t *ip, const char *ip_str, int *arr, size_t len)
{
    struct subnet *head = NULL;
    struct subnet *new_node = NULL;

    if (!ip || !ip_str || !arr || !len) { return EXIT_FAILURE; }

    head = calloc(1, sizeof(struct subnet));
    if (!head) { goto handle_error; }

	if (!fill_addr(ip, ip_str)) { goto handle_error; }
	if (!fill_bitmask(ip, ip_str)) { goto handle_error; }
    ip->bitmask += get_min_power_of_two(len);
    if (32 - ip->bitmask < 1) { goto handle_error; } 
	if (!fill_netmask(ip)) { goto handle_error; }
	if (!fill_wildcard(ip)) { goto handle_error; }
	if (!fill_network(ip)) { goto handle_error; }
	if (!fill_broadcast(ip)) { goto handle_error; }

    init_node(head, ip);

    for (int i = 0; i < len - 1; i++) {
        if (!switch_subnet(ip)) { goto handle_error; }
	    if (!fill_broadcast(ip)) { goto handle_error; }

        new_node = calloc(1, sizeof(struct subnet));
        if (!new_node) { goto handle_error; }

        init_node(new_node, ip);
        add_to_list(head, new_node);
    }

    print_list(head);
    remove_list(head);

    return EXIT_SUCCESS;

    handle_error:
        remove_list(head);
        return EXIT_FAILURE;
}

static ipv4_t *switch_subnet(ipv4_t *ip)
{
    int host_bit; 
    uint32_t net_num;

    if (!ip) { return NULL; }
    if (!ip->network_set) { return NULL; }

    net_num = (ip->network[0] << 24) | (ip->network[1] << 16) |
              (ip->network[2] << 8) | (ip->network[3]);

    host_bit = 32 - ip->bitmask;

    net_num += 1 << host_bit;

    ip->network[0] = net_num >> 24;
    ip->network[1] = net_num >> 16 & 0xFF;
    ip->network[2] = net_num >> 8 & 0xFF;
    ip->network[3] = net_num & 0xFF;

    return ip;
}

static int get_min_power_of_two(int target)
{
    int res_exp = 1;
    int pow = 1;

    if (target <= 1) { return 0; }

    for (; pow < target; pow++) {
        res_exp = 1;
        for (int iter = pow; iter > 0; iter--) {
            res_exp *= 2;
        }

        if (res_exp >= target) { break; }
    }

    return pow;
}
