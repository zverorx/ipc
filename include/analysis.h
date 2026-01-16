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

#ifndef ANALYSIS_H_SENTRY
#define ANALYSIS_H_SENTRY

#include "ipv4_t.h"

/**
 * 
 */
int analysis_start(ipv4_t *ip, char *ip_str);

/**
 * @brief Prints the ip_v4 structure data to stdout.
 * @param ip A reference to a structure storing IP data.
 * @return void.
 */
void print_ip_v4(const ipv4_t *ip);

/**
 * @brief Sets all flag fields of the ip_v4 structure to 0. 
 * @param ip A reference to a structure storing IP data.
 * @return void.
 * @note The flags are set to 0: addr_set, bitmask_set, netmask_set, wildcard_set, network_set, broadcast_set.
 */
void assign_flags_ip_v4(ipv4_t *ip);

#endif /* ANALYSIS_H_SENTRY */
