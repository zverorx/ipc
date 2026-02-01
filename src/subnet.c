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
 * 
 */
static ipv4_t *switch_subnet(ipv4_t *ip);

/**
 * @brief Calculates needed bits to fit count_subnets.
 * @param count_subnets Required number of subnets.
 * @return Minimum bit count, or 0 for <=1 subnet.
 */
static int expand_bitmask(int count_subnets);

int subnetting_start(ipv4_t *ip, const char *ip_str, int *arr, size_t len)
{
    struct subnet *head = calloc(1, sizeof(struct subnet));
    struct subnet *new_node = NULL;

    if (!ip || !ip_str || !arr || !len || !head) { return EXIT_FAILURE; }

	if (!fill_addr(ip, ip_str)) { goto handle_error; }
	if (!fill_bitmask(ip, ip_str)) { goto handle_error; }
    ip->bitmask += expand_bitmask(len);
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
    int last_net_octet;

    if (!ip) { return NULL; }
    if (!ip->network_set) { return NULL; }

    host_bit = 32 - ip->bitmask;
    last_net_octet = ip->bitmask / 8;

    ip->network[last_net_octet] += 1 << host_bit; /* FIXME */

    return ip;
}

static int expand_bitmask(int count_subnets)
{
    int res_exp = 1;
    int pow;

    if (count_subnets <= 1) { return 0; }

    for (pow = 1; pow < count_subnets; pow++) {
        res_exp = 1;
        for (int iter = pow; iter > 0; iter--) {
            res_exp *= 2;
        }

        if (res_exp >= count_subnets) { break; }
    }

    return pow;
}
