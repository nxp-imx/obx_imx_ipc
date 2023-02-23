/* SPDX-License-Identifier: GPL-2.0
 * Copyright 2023 NXP
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>

#define ID_SIZE 16
#define PORT 8080
#define SA struct sockaddr

struct update_req {
	int tx_rx; /* 0 to update tx frame table; 1 to update rx table */
	int frm_idx; /* row: idx in the frame table */
	int byte_idx; /* col: byte number at the idx of frame table */
	int val;
};

int ID_TABLE_MAX_ELEMS = 20;

int ID_TX_table_size = 20;

uint8_t ID_TX_table[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

uint8_t ID_RX_table[20] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1, 2, 3, 4};

