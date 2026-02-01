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

#ifndef SUBNET_LIST_H_SENTRY
#define SUBNET_LIST_H_SENTRY

#include "ipv4_t.h"

/**
 * @struct subnet
 * @brief IPv4 subnet list node.
 */
struct subnet {
    uint8_t minaddr[OCTET_COUNT];   /**< Minimum subnet address */
    uint8_t maxaddr[OCTET_COUNT];   /**< Maximum subnet address */
    struct subnet *next;            /**< Next node */
};

/**
 * @brief Appends a node to the end of the linked list.
 * @param head Head of the list (must not be NULL).
 * @param node Node to append (must not be NULL).
 * @return Pointer to the appended node, or NULL on error.
 */
struct subnet *add_to_list(struct subnet *head, struct subnet *node);

/**
 * @brief Frees all nodes in the linked list.
 * @param head Head of the list.
 */
void remove_list(struct subnet *head);

/**
 * @brief Initializes subnet node with IP address data.
 * @param node Node to initialize.
 * @param ip Source IP data.
 * @return 0 on success, -1 on error.
 */
int init_node(struct subnet *node, ipv4_t *ip);

/**
 * @brief Prints all nodes in the list.
 * @param head Head of the list.
 */
void print_list(const struct subnet *head);

#endif /* SUBNET_LIST_H_SENTRY */
