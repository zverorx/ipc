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

#include <stdlib.h>
#include <stdio.h>

#include "subnet_list.h"

struct subnet *add_to_list(struct subnet *head, struct subnet *node)
{
    struct subnet *last_node = NULL;

    if (!node) { return NULL; }
    if (!head) { return NULL; }

    last_node = head;
    while (last_node->next) {
        last_node = last_node->next;
    }

    last_node->next = node;

    return node;
}

void remove_list(struct subnet *head)
{
    struct subnet *next_node = NULL;

    if (!head) { return; }

    while (head) {
        next_node = head->next;
        free(head);
        head = next_node;
    }

    return;
}

int init_node(struct subnet *node, ipv4_t *ip)
{
    if (!node || !ip) { return -1; }

    for (int i = 0; i < 4; i++) {
        node->minaddr[i] = ip->network[i];
        node->maxaddr[i] = ip->broadcast[i];
    }

    return 0;
}

void print_list(const struct subnet *head)
{
    if (!head) { return; }
    while (head) {
        printf("%d.%d.%d.%d - %d.%d.%d.%d\n", head->minaddr[0],
                                              head->minaddr[1],
                                              head->minaddr[2],
                                              head->minaddr[3],
                                              head->maxaddr[0],
                                              head->maxaddr[1],
                                              head->maxaddr[2],
                                              head->maxaddr[3]);
        head = head->next;
    }

    return;
}
