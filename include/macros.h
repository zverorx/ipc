
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

#ifndef MACROS_H_SENTRY
#define MACROS_H_SENTRY

/**
 * @def CHECK_CMD_LN_PARAM(ERROR)
 * @brief Exits with ERROR if argc != QT_CMD_LN_PARAM.
 */

#define CHECK_CMD_LN_PARAM(ERROR) \
    do { \
        if(argc != QT_CMD_LN_PARAM) { \
			fprintf(stderr, "Usage: %s <IP/CIDR>\n", argv[0]); \
            return ERROR; \
        } \
    } while(0)

/**
 * @def CHECK_POINTER_IP_V4(POINTER, ERROR)
 * @brief Exits with ERROR if POINTER is NULL, else initializes its flags.
 */
#define CHECK_POINTER_IP_V4(POINTER, ERROR) \
	do { \
		if(!POINTER) { \
			fputs("Memory allocation failed\n", stderr); \
			return ERROR; \
		} \
		assign_flags_ip_v4(POINTER); \
	} while(0)

/**
 * @def ASSIGN_FIELD(FIELDNAME, POINTER, ERROR)
 * @brief Calls assign_FIELDNAME(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets POINTER->FIELDNAME_set = 1 on success.
 */
#define ASSIGN_FIELD(FIELDNAME, POINTER, ERROR) \
	do { \
		if(!assign_##FIELDNAME(POINTER)) { \
			fprintf(stderr, "Error: Unable to get %s\n", #FIELDNAME); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->FIELDNAME##_set = 1; \
	} while(0)

/**
 * @def ASSIGN_HOSTX(FIELDNAME, POINTER, ERROR)
 * @brief Calls assign_FIELDNAME(); on fail and not host route, prints error, frees POINTER, returns ERROR.
 */
#define ASSIGN_HOSTX(FIELDNAME, POINTER, ERROR) \
	do { \
		if(!assign_##FIELDNAME(POINTER) && !POINTER->is_host_route) { \
			fprintf(stderr, "Error: Unable to get %s\n", #FIELDNAME); \
			free(POINTER); \
			return ERROR; \
		} \
	} while(0)

/**
 * @def ASSIGN_QT_HOSTS(POINTER, ERROR)
 * @brief Calls assign_qt_hosts(); on fail, prints error, frees POINTER, returns ERROR.
 */
#define ASSIGN_QT_HOSTS(POINTER, ERROR) \
	do { \
		if(!assign_qt_hosts(POINTER)) { \
			fputs("Error: Unable to get quantity hosts\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
	} while(0)

/**
 * @def ASSIGN_BROADCAST(POINTER, ERROR)
 * @brief Calls assign_broadcast(); on fail and not p2p, prints error, frees POINTER, returns ERROR.
 * @brief Sets broadcast_set = 1 on success.
 */
#define ASSIGN_BROADCAST(POINTER, ERROR) \
	do { \
		if(!assign_broadcast(POINTER) && !POINTER->is_point_to_point) { \
			fputs("Error: Unable to get broadcast\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->broadcast_set = 1; \
	} while(0)

/**
 * @def ASSIGN_ADDR(IP, POINTER, ERROR)
 * @brief Calls assign_addr(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets addr_set = 1 on success.
 */
#define ASSIGN_ADDR(IP, POINTER, ERROR) \
	do { \
		if(!assign_addr(IP, POINTER)) { \
			fputs("Error: Incorrect IP\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		POINTER->addr_set = 1; \
	} while(0)

/**
 * @def ASSIGN_BITMASK(CIDR, POINTER, ERROR)
 * @brief Calls assign_bitmask(); on fail, prints error, frees POINTER, returns ERROR.
 * @brief Sets bitmask_set = 1 and updates is_host_route or is_point_to_point if needed.
 */
#define ASSIGN_BITMASK(CIDR, POINTER, ERROR) \
	do { \
		if(!assign_bitmask(CIDR, POINTER)) { \
			fputs("Error: Incorrect mask\n", stderr); \
			free(POINTER); \
			return ERROR; \
		} \
		else if(POINTER->bitmask == BITS_IN_IP) { \
			POINTER->is_host_route = 1; \
			POINTER->bitmask_set = 1; \
		} \
		else if(POINTER->bitmask == BITS_IN_IP - 1) { \
			POINTER->is_point_to_point = 1; \
			POINTER->bitmask_set = 1; \
		} \
		else { \
			POINTER->is_host_route = 0; \
			POINTER->is_point_to_point = 0; \
			POINTER->bitmask_set = 1; \
		} \
	} while(0)

#endif /* MACROS_H_SENTRY */