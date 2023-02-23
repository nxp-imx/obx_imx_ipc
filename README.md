/* SPDX-License-Identifier: GPL-2.0
 * Copyright 2023 NXP
 */

obx_imx_s32k_spi_ipc_d
-----------------------

/*
 * SPI IPC utility (using spidev driver)
 * This file is based on spidev_test.c file.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

Description
--------------

The obx_imx_ipc tool is developed for the orangebox platform.
It is a SPI based Inter-Process Communication protocol implementation
for communication between an i.MX SPI master and an S32K144 SPI slave.

The i.MX SPI master sends 16 byte packets and the S32K144 SPI slave
responds with 16 byte packets.

These packets include:
One IDX byte: This byte indicates the type of packet.
One Checksum byte: To indicate checksum errors for received packets.
12 byte payload: 12 bytes for payload
2 extra bytes: Unused currently.

A TX_ID_table holds IDX values in a predefined order.
The master continuously loops through these IDX values in this
table, forms the TX packets based on the IDX value and transfers
the packet.

It receives the packet from the S32K144 slave and validates the
checksum byte.

C Files:
obx_imx_s32k_spi_ipc_d.c    :  generates obx_imx_s32k_spi_ipc_d tool
			   which transmits TX_ID_table stored
			   IDX based 16 byte packets in a loop

obx_spi_ipc_cli.c           :  generates obx_spi_ipc_cli tool which
			   updates frame data tables based on
			   user input

Header Files:               : obx_imx_s32k_spi_ipc.h


Build and Execute Instructions
---------------------------------
The obx_imx_ipc containing the Makefile,
Build file, .c and .h files must be placed in the
tools folder of linux repository.
Build for both 5.15 and 6.1.x kernel work.

Run obx_imx_s32k_spi_ipc_d in background with cli
options as needed.
Now, obx_spi_ipc_cli can be run to send a req to
obx_imx_s32k_spi_ipc_d to update tx/rx frame data tables.


Version History
------------------
Version 1.0
This version is compatible with i.MX Matter 2023Q3 release (base on i.MX Yocto 6.1.36-2.1.0)
