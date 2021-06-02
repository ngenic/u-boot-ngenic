// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2020 Linumiz
 * Author: Parthiban Nallathambi <parthiban@linumiz.com>
 */

#include <asm/arch/clock.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/global_data.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <dm/ofnode.h>
#include <env.h>
#include <i2c_eeprom.h>
#include <init.h>
#include <miiphy.h>
#include <netdev.h>


DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}

#ifdef CONFIG_FEC_MXC

static int setup_fec(void)
{
	struct iomuxc *const iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int ret;

	/*
	 * Use 50M anatop loopback REF_CLK1 for ENET1,
	 * clear gpr1[13], set gpr1[17].
	 */
	clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC1_MASK,
			IOMUX_GPR1_FEC1_CLOCK_MUX1_SEL_MASK);

	ret = enable_fec_anatop_clock(0, ENET_50MHZ);
	if (ret)
		return ret;

	enable_enet_clk(1);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	/*
	 * Defaults + Enable status LEDs (LED1: Activity, LED0: Link) & select
	 * 50 MHz RMII clock mode.
	 */
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0x8190);

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}
#endif /* CONFIG_FEC_MXC */

int board_init(void)
{
#ifdef CONFIG_FEC_MXC
	setup_fec();
#endif
	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	// Read boot pin configuration from SRC_SBMR2.
	uint32_t BMOD = ((src_base->sbmr2 & SRC_SBMR2_BMOD_MASK) >> SRC_SBMR2_BMOD_SHIFT);
	if (BMOD == SRC_SBMR2_BMOD_SERIAL) {
		printf("Serial Downloader boot detected - enabling fastboot.\n");
		env_set("bootcmd", "echo Entering fastboot; fastboot usb 0");
	}

	return 0;
}
#endif

#ifdef CONFIG_SYS_I2C_MAC_OFFSET
int mac_read_from_eeprom(void)
{
	int ret;

	// Read chosen eeprom node from device tree.
	ofnode node = ofnode_get_chosen_node("mac,eeprom");
	if (ofnode_equal(node, ofnode_null()))
		return -1;

	// Acquire udevice handle from the node.
	struct udevice *dev;
	ret = uclass_get_device_by_ofnode(UCLASS_I2C_EEPROM, node, &dev);
	if (ret)
		return ret;

	// Read 2 bytes more than prescribed because of a prototype hardware bug.
	uint8_t data[8];
	ret = i2c_eeprom_read(dev, CONFIG_SYS_I2C_MAC_OFFSET-2, data, sizeof(data));
	if (ret)
		return ret;

	// Check first 2 bytes to determine type of EEPROM.
	// This is because prototype hardware had wrong chips installed.
	uint8_t mac[6];
	if (data[0] == 0xFF && data[1] == 0xFF) {
		// This is a proper 24AA02E48, use MAC data as-is.
		memcpy(mac, data+2, sizeof(mac));
	} else {
		// This is a 24AA02E64 which should not be used like that
		// since it's an EUI-64 chip. Bodge together something that
		// looks like a MAC address but is not guaranteed unique.
		printf("WARNING: You have a 24AA02E64 chip installed. MAC address may be broken.\n");
		// The byte layout here is used in Ngenic Gateway (see preboot.scr).
		mac[0] = data[0];
		mac[1] = data[1];
		mac[2] = data[2];
		mac[3] = data[5];
		mac[4] = data[6];
		mac[5] = data[7];
	}
 
	eth_env_set_enetaddr("ethaddr", mac);

	return 0;
}
#endif // CONFIG_SYS_I2C_MAC_OFFSET
