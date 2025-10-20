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
#include "macros.h"

#define QT_CMD_LN_PARAM		2
#define CMD_LN_PARAM_IP		argv[1]


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
