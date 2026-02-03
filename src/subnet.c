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
 * @brief Dividing the network into equal subnets.
 * 
 * @param ip Structure with address data.
 * @param ip_str IP address in CIDR notation.
 * @param num_of_subnets Number of subnets required.
 * @param list_res List with calculation results.
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
static int equal_opt_handler(ipv4_t *ip, const char *ip_str, 
                             size_t num_of_subnets, struct subnet *list_res);
/**
 * @brief Dividing the network into different subnets. 
 * @param ip Structure with address data.
 * @param ip_str IP address in CIDR notation.
 * @param arr Stores the parts into which the network should be divided.
 *            The size of the array is the number of parts, the value of
 *            each element is the number of addresses in the part.
 * @param len Size of arr.
 * @param list_res List with calculation results.
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
static int part_opt_handler(ipv4_t *ip, const char *ip_str, int *arr, 
                            size_t len, struct subnet *list_res);

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

/**
 * @brief Verify if requested subnet partitions fit in the IPv4 network.
 * 
 * @param arr Stores the parts into which the network should be divided.
 *            The size of the array is the number of parts, the value of
 *            each element is the number of addresses in the part.
 * @param len Size of arr.
 * @param ip Structure with address data.
 * 
 * @pre The bitmask_set flag in the ipv4_t structure must be set to 1.
 */
static int parts_will_fit(int *arr, size_t len, ipv4_t *ip);

int subnetting_start(ipv4_t *ip, const char *ip_str, int *arr, size_t len)
{
    struct subnet *head = NULL;
    int res_opt;

    if (!ip || !ip_str || !arr || !len) { return EXIT_FAILURE; }

    head = calloc(1, sizeof(struct subnet));
    if (!head) { return EXIT_FAILURE; }

    if (arr[0] == '\0') { 
        res_opt = equal_opt_handler(ip, ip_str, len, head);
    }
    else { 
        res_opt = part_opt_handler(ip, ip_str, arr, len, head);
    }

    if (res_opt == EXIT_SUCCESS) { print_list(head); }
    remove_list(head);

    return res_opt;
}

static int equal_opt_handler(ipv4_t *ip, const char *ip_str, 
                             size_t num_of_subnets, struct subnet *list_res)
{
    struct subnet *new_node = NULL;

    if (!ip || !ip_str || !num_of_subnets || !list_res) { return EXIT_FAILURE; }

	if (!fill_addr(ip, ip_str)) { return EXIT_FAILURE; }
	if (!fill_bitmask(ip, ip_str)) { return EXIT_FAILURE; }
    ip->bitmask += get_min_power_of_two(num_of_subnets);
    if (32 - ip->bitmask < 1) { return EXIT_FAILURE; }
    if (!fill_netmask(ip)) { return EXIT_FAILURE; }
	if (!fill_wildcard(ip)) { return EXIT_FAILURE; }
	if (!fill_network(ip)) { return EXIT_FAILURE; }
	if (!fill_broadcast(ip)) { return EXIT_FAILURE; }

    init_node(list_res, ip);

    for (int i = 0; i < num_of_subnets - 1; i++) {
        if (!switch_subnet(ip)){ return EXIT_FAILURE; }
	    if (!fill_broadcast(ip)){ return EXIT_FAILURE; }

        new_node = calloc(1, sizeof(struct subnet));
        if (!new_node){ return EXIT_FAILURE; }

        init_node(new_node, ip);
        add_to_list(list_res, new_node);
    }

    return EXIT_SUCCESS;
}

/* 
 * For qsort in part_opt_handler.
 * Sorts in descending order.
 */
static int compare(const void *p1, const void *p2) 
{ return *(const int *) p2 - *(const int *) p1; }

static int part_opt_handler(ipv4_t *ip, const char *ip_str, int *arr, 
                            size_t len, struct subnet *list_res)
{
    struct subnet *new_node = NULL;
    uint32_t net_num, brc_num;
    uint8_t new_bitmask;

    if (!ip || !ip_str || !arr || !len || !list_res) { return EXIT_FAILURE; }

    qsort(arr, len, sizeof(int), compare);

	if (!fill_addr(ip, ip_str)) { return EXIT_FAILURE; }
	if (!fill_bitmask(ip, ip_str)) { return EXIT_FAILURE; }

    if (!parts_will_fit(arr, len, ip)) { return EXIT_FAILURE; }

    new_bitmask = 32 - get_min_power_of_two(arr[0]);
    if (new_bitmask <= 0 || new_bitmask < ip->bitmask) { return EXIT_FAILURE; }
    ip->bitmask = new_bitmask;

    if (!fill_netmask(ip)) { return EXIT_FAILURE; }
    if (!fill_wildcard(ip)) { return EXIT_FAILURE; }
    if (!fill_network(ip)) { return EXIT_FAILURE; }
    if (!fill_broadcast(ip)) { return EXIT_FAILURE; }
    init_node(list_res, ip);

    for (int i = 1; i < len; i++) {
        brc_num = (ip->broadcast[0] << 24) | (ip->broadcast[1] << 16) |
                  (ip->broadcast[2] << 8) | (ip->broadcast[3]);

        net_num = brc_num + 1;

        ip->network[0] = net_num >> 24;
        ip->network[1] = net_num >> 16 & 0xFF;
        ip->network[2] = net_num >> 8 & 0xFF;
        ip->network[3] = net_num & 0xFF;

        new_bitmask = 32 - get_min_power_of_two(arr[i]);
        if (new_bitmask <= 0) { return EXIT_FAILURE; }
        ip->bitmask = new_bitmask;

        if (!fill_wildcard(ip)) { return EXIT_FAILURE; }
        if (!fill_broadcast(ip)) { return EXIT_FAILURE; }

        new_node = calloc(1, sizeof(struct subnet));
        if (!new_node){ return EXIT_FAILURE; }

        init_node(new_node, ip);
        add_to_list(list_res, new_node);
    }

    return EXIT_SUCCESS;
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

static int parts_will_fit(int *arr, size_t len, ipv4_t *ip)
{
    int pow, host_bit, res_exp;
    unsigned int demand = 0, available;

    if (!arr || !len || !ip ) { return 0; }
    if (!ip->bitmask_set) { return 0; }

    for (int i = 0; i < len; i++) {
        pow = get_min_power_of_two(arr[i]);
        res_exp = 1 << pow;
        demand += res_exp;
    }

    host_bit = 32 - ip->bitmask;
    available = 1 << host_bit;

    if (demand > available) { return 0; }

    return 1;
}
