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

#ifndef SUBNET_H_SENTRY
#define SUBNET_H_SENTRY

#include <stddef.h>

#include "ipv4_t.h"

/**
 * @brief Dividing the network into subnets.
 *
 * @param ip Structure with address data.
 * @param ip_str IP address in CIDR notation.
 * @param arr Stores the parts into which the network should be divided.
 *            The size of the array is the number of parts, the value of
 *            each element is the number of addresses in the part.
 *            If the array is initialized with zeros, then the division
 *            occurs into equal parts.
 * @param len Size of arr.
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int subnetting_start(ipv4_t *ip, const char *ip_str, int *arr, size_t len);

#endif /* SUBNET_H_SENTRY */
