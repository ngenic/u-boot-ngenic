/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2020 Ngenic AB
 * Copyright (C) 2015 Technexion Ltd.
 *
 * Configuration settings for the Technexion PICO-IMX6UL-EMMC board.
 */
#ifndef __PICO_IMX6UL_CONFIG_H
#define __PICO_IMX6UL_CONFIG_H


#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/mach-imx/gpio.h>



/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(35 * SZ_1M) /* Increase due to DFU */

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART6_BASE_ADDR

/* MMC Configs */
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC1_BASE_ADDR
#define CONFIG_SUPPORT_EMMC_BOOT

#define CONFIG_USBD_HS

#define CONFIG_EXTRA_ENV_SETTINGS \
	"image=zImage\0" \
	"console=ttymxc5\0" \
	"fdt_file=set_this_in_preboot\0" \
	"fdt_addr=0x83000000\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcbootpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcrootpart=" __stringify(MMC_ROOT_PART) "\0" \
	"mmcautodetect=yes\0" \
	"findrootuuid=part uuid mmc ${mmcdev}:${mmcrootpart} uuid\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=PARTUUID=${uuid} rootwait ro " \
			"fbcon=scrollback:1024k consoleblank=0 caam\0" \
	"loadimage=load mmc ${mmcdev}:${mmcbootpart} ${loadaddr} ${image}\0" \
	"loadfdt=load mmc ${mmcdev}:${mmcbootpart} ${fdt_addr} ${fdt_file}\0" \
	"bootscr=boot.scr\0" \
	"script_addr=0x81000000\0" \
	"loadbootscr=" \
		"fatload mmc ${mmcdev}:${mmcbootpart} ${script_addr} ${bootscr};\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run findrootuuid; " \
		"run mmcargs; " \
		"if run loadfdt; then " \
			"bootz ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"echo WARN: Cannot load the DT; " \
		"fi;\0" \

#define CONFIG_PREBOOT \
	"if run loadbootscr; then " \
		"echo Found boot.scr. Executing script...; " \
		"source ${script_addr}; " \
	"fi;"

#ifndef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"if mmc rescan; then " \
		"if run loadimage; then " \
			"run mmcboot; " \
		"else " \
			"echo ERROR: Cannot load kernel image.; " \
		"fi; " \
	"else " \
		"echo WARN: mmc rescan failed; " \
	"fi;"
#endif

/* Miscellaneous configurable options */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_MEMTEST_START + SZ_128M

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		0
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1
#define MMC_ROOT_PART	2

/* I2C configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1
#define CONFIG_SYS_I2C_SPEED		100000

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR  0x08

/* environment organization */
#define CONFIG_ENV_SIZE			SZ_8K
#define CONFIG_ENV_OFFSET		(8 * SZ_64K)

/* USB Configs */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

/* Network support */
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET2_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x1
#define CONFIG_FEC_XCV_TYPE		RMII

/* 
 * From https://www.denx.de/wiki/DULG/UBootEnvVariables
 * After reset, U-Boot will wait this number of seconds before it executes the contents of the bootcmd variable. During this time a countdown is printed, which can be interrupted by pressing any key.
 * Set this variable to 0 boot without delay. Be careful: depending on the contents of your bootcmd variable, this can prevent you from entering interactive commands again forever!
 * Set this variable to -1 to disable autoboot. Set this variable to -2 to boot without delay and not check for abort.
 */
#define CONFIG_BOOTDELAY 5

/*
 * From: http://git.denx.de/?p=u-boot.git;a=blob;f=doc/README.watchdog
 * CONFIG_HW_WATCHDOG
 *     This enables hw_watchdog_reset to be called during various loops,
 *     including waiting for a character on a serial port. But it
 *     does not also call hw_watchdog_init. Boards which want this
 *     enabled must call this function in their board file. This split
 *     is useful because some rom's enable the watchdog when downloading
 *     new code, so it must be serviced, but the board would rather it
 *     was off. And, it cannot always be turned off once on.
 * 
 * CONFIG_WATCHDOG_TIMEOUT_MSECS
 *     Can be used to change the timeout for i.mx31/35/5x/6x.
 *     If not given, will default to maximum timeout. This would
 *     be 128000 msec for i.mx31/35/5x/6x.
 * CONFIG_IMX_WATCHDOG
 *     Available for i.mx31/35/5x/6x to service the watchdog. This is not
 *     automatically set because some boards (vision2) still need to define
 *     their own hw_watchdog_reset routine.
 *     TODO: vision2 is removed now, so perhaps this can be changed.
 */
#define CONFIG_HW_WATCHDOG
#define CONFIG_IMX_WATCHDOG
#define CONFIG_WATCHDOG_TIMEOUT_MSECS	60 * 1000

#define CONFIG_PWM_IMX
#define CONFIG_IMX6_PWM_PER_CLK 66000000

#endif /* __PICO_IMX6UL_CONFIG_H */
