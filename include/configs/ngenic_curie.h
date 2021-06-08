/* SPDX-License-Identifier: GPL-2.0+
 *
 * Copyright (C) 2021 Ngenic Poland
 * Author: Kosma Moczek <kosma.moczek@ngenic.biz>
 */

#ifndef __NGENIC_CURIE_H
#define __NGENIC_CURIE_H

#include <linux/sizes.h>
#include "mx6_common.h"

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE			SZ_512M

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* USB Configs */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1

/* MAC address EEPROM */
#define CONFIG_SYS_I2C_MAC_OFFSET 0xFA

#define BOOTENV \
	"bootcmd_normal=" \
	"mmc partconf 1;" \
	"sqfsload mmc 1:${mmc_boot_partition_enable} $kernel_addr_r /boot/zImage; " \
	"sqfsload mmc 1:${mmc_boot_partition_enable} $fdt_addr_r /boot/ngenic-curie.dtb; " \
	"setenv bootargs root=PARTLABEL=root${mmc_boot_partition_enable}; " \
	"bootz $kernel_addr_r - $fdt_addr_r; " \
	"\0" \
	"bootcmd_recovery=" \
	"echo Entering Fastboot.; " \
	"fastboot usb 0; " \
	"\0" \
	"bootcmd=" \
	"run bootcmd_normal; " \
	"\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"console=ttymxc0,115200n8\0" \
	"fdt_addr_r=0x82000000\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"kernel_addr_r=0x81000000\0" \
	"pxefile_addr_r=0x87100000\0" \
	"ramdisk_addr_r=0x82100000\0" \
	"scriptaddr=0x87000000\0" \
	"partitions=" \
		"name=root1,type=linux,size=1GB;" \
		"name=root2,type=linux,size=1GB;" \
		"name=data,type=linux,size=0" \
	"\0" \
	BOOTENV

#endif /* __NGENIC_CURIE_H */
