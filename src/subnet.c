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

int subnetting_start(ipv4_t *ip, const char *ip_str, int *arr, size_t len)
{
    if (!ip || !ip_str || !arr || !len) { goto handle_error; }

    fputs("subnetting start\n", stderr);
    for (int i = 0; i < len; i++) {
        fprintf(stderr, "arr[%d] = %d\n", i, arr[i]);
    }

    return EXIT_SUCCESS;

    handle_error:
        return EXIT_FAILURE;
}
