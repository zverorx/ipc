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
 * @brief Analyze IPv4 address.
 * 
 * @param ip ipv4 structure to fill.
 * @param ip_str IP address in CIDR notation.
 * 
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int analysis_start(ipv4_t *ip, char *ip_str);

#endif /* ANALYSIS_H_SENTRY */
