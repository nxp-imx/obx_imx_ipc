// SPDX-License-Identifier: GPL-2.0
// Copyright 2023 NXP

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
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
#include <linux/spi/spidev.h>

#include "obx_imx_s32k_spi_ipc.h"

int sockfd, len, n;
struct sockaddr_in servaddr;

/* Update request for frame data table sent to
 * server socket in obx_imx_s32k_spi_ipc
 */
void tx_rx_frame_update_req(struct update_req req)
{
	int ret = -1;

	ret = sendto(sockfd, &req, sizeof(req), 0, (SA *)&servaddr, len);
	printf("cli %s status %d\n", __func__, ret);
}

int main(int argc, char *argv[])
{
	int tx_rx = -1;
	int frm_idx = -1;
	int byte_idx = -1;
	int val = -1;
	struct update_req req = {0};

	/* cli input format to update frame table data byte */
	if (argc != 5) {
		printf("Enter update req as per format\n");
		printf("Example: spi_ipc_cli <tx/rx> <idx> <byte> <val>\n");
		return 0;
	}

	tx_rx = atoi(argv[1]);
	if (tx_rx != 0 && tx_rx != 1) {
		printf("tx_rx should be [0, 1]\n");
		return 0;
	}


	frm_idx = atoi(argv[2]);
	if (frm_idx < 0 || frm_idx >= ID_TABLE_MAX_ELEMS) {
		printf("idx should fall in the range: [0, 19]\n");
		return 0;
	}

	byte_idx = atoi(argv[3]);
	if (byte_idx < 0 || byte_idx >= ID_TABLE_MAX_ELEMS) {
		printf("idx should fall in the range: [0, 19]\n");
		return 0;
	}

	val = atoi(argv[4]);
	if (val < 0) {
		printf("Enter a positive value\n");
		return 0;
	}

	req.tx_rx = tx_rx;
	req.frm_idx = frm_idx;
	req.byte_idx = byte_idx;
	req.val = val;

	/* Setup cli based packet to communicate with
	 * obx_imx_s32k_spi_ipc tool
	 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		close(sockfd);
		exit(0);
	}

	bzero(&servaddr, sizeof(len));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	len = sizeof(servaddr);

	/* Update request for frame data table formed
	 * based on cli optiona and sent to
	 * server socket in obx_imx_s32k_spi_ipc
	 */
	tx_rx_frame_update_req(req);
	close(sockfd);
}
