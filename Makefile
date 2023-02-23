# SPDX-License-Identifier: GPL-2.0
# Copyright 2023 NXP

include ../scripts/Makefile.include

bindir ?= /usr/bin

ifeq ($(srctree),)
srctree := $(patsubst %/,%,$(dir $(CURDIR)))
srctree := $(patsubst %/,%,$(dir $(srctree)))
endif

# Do not use make's built-in rules
# (this improves performance and avoids hard-to-debug behaviour);
MAKEFLAGS += -r

CFLAGS += -O2 -Wall -g -D_GNU_SOURCE -I$(OUTPUT)include

ALL_TARGETS := obx_imx_s32k_spi_ipc_d obx_spi_ipc_cli
ALL_PROGRAMS := $(patsubst %,$(OUTPUT)%,$(ALL_TARGETS))

all: $(ALL_PROGRAMS)

export srctree OUTPUT CC LD CFLAGS
include $(srctree)/tools/build/Makefile.include

#
# We need the following to be outside of kernel tree
#
$(OUTPUT)include/linux/spi: ../../include/uapi/linux/spi
	mkdir -p $(OUTPUT)include/linux/spi 2>&1 || true
	ln -sf $(CURDIR)/../../include/uapi/linux/spi/spidev.h $@
	ln -sf $(CURDIR)/../../include/uapi/linux/spi/spi.h $@

prepare: $(OUTPUT)include/linux/spi

OBX_IMX_S32K_IPC_IN := $(OUTPUT)obx_imx_s32k_spi_ipc_d-in.o
$(OBX_IMX_S32K_IPC_IN): prepare FORCE
	$(Q)$(MAKE) $(build)=obx_imx_s32k_spi_ipc_d
$(OUTPUT)obx_imx_s32k_spi_ipc_d: $(OBX_IMX_S32K_IPC_IN)
	$(QUIET_LINK)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

OBX_IMX_S32K_IPC_IN := $(OUTPUT)obx_spi_ipc_cli-in.o
$(OBX_IMX_S32K_IPC_IN): prepare FORCE
	$(Q)$(MAKE) $(build)=obx_spi_ipc_cli
$(OUTPUT)obx_spi_ipc_cli: $(OBX_IMX_S32K_IPC_IN)
	$(QUIET_LINK)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(ALL_PROGRAMS)
	rm -rf $(OUTPUT)include/
	find $(or $(OUTPUT),.) -name '*.o' -delete
	find $(or $(OUTPUT),.) -name '\.*.o.d' -delete
	find $(or $(OUTPUT),.) -name '\.*.o.cmd' -delete

install: $(ALL_PROGRAMS)
	install -d -m 755 $(DESTDIR)$(bindir);		\
	for program in $(ALL_PROGRAMS); do		\
		install $$program $(DESTDIR)$(bindir);	\
	done

FORCE:

.PHONY: all install clean FORCE prepare
