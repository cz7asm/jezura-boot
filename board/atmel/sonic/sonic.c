/*
 * Copyright (C) 2012 Atmel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/at91sam9x5_matrix.h>
#include <asm/arch/at91sam9_smc.h>
#include <asm/arch/at91_common.h>
#include <asm/arch/at91_rstc.h>
#include <asm/arch/at91_wdt.h>
#include <asm/arch/clk.h>
#include <asm/arch/gpio.h>
#include <debug_uart.h>
#include <lcd.h>
#include <atmel_hlcdc.h>
#ifdef CONFIG_LCD_INFO
#include <nand.h>
#include <version.h>
#endif
#include <asm/mach-types.h>

DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

#ifdef CONFIG_LCD
vidinfo_t panel_info = {
	.vl_col	= 480,
	.vl_row = 272,
    .vl_rot = 2,
	.vl_clk = 24000000,
	.vl_sync = LCDC_LCDCFG5_HSPOL | LCDC_LCDCFG5_VSPOL,
	.vl_bpix = LCD_BPP,
	.vl_tft = 1,
	.vl_clk_pol = 0,
	.vl_hsync_len = 128,
	.vl_left_margin = 64,
	.vl_right_margin = 64,
	.vl_vsync_len = 2,
	.vl_upper_margin = 22,
	.vl_lower_margin = 21,
	.mmio = ATMEL_BASE_LCDC,
};

void lcd_enable(void)
{
	if (has_lcdc())
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 29, 1);	/* power up */
}

void lcd_disable(void)
{
	if (has_lcdc())
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 29, 0);	/* power down */
}

static void at91sam9x5ek_lcd_hw_init(void)
{
	if (has_lcdc()) {
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 26, 0);	/* LCDPWM */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 27, 0);	/* LCDVSYNC */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 28, 0);	/* LCDHSYNC */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 24, 0);	/* LCDDISP */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 29, 0);	/* LCDDEN */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 30, 0);	/* LCDPCK */

		at91_pio3_set_a_periph(AT91_PIO_PORTC, 0, 0);	/* LCDD0 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 1, 0);	/* LCDD1 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 2, 0);	/* LCDD2 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 3, 0);	/* LCDD3 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 4, 0);	/* LCDD4 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 5, 0);	/* LCDD5 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 6, 0);	/* LCDD6 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 7, 0);	/* LCDD7 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 8, 0);	/* LCDD8 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 9, 0);	/* LCDD9 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 10, 0);	/* LCDD10 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 11, 0);	/* LCDD11 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 12, 0);	/* LCDD12 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 13, 0);	/* LCDD13 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 14, 0);	/* LCDD14 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 15, 0);	/* LCDD15 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 16, 0);	/* LCDD16 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 17, 0);	/* LCDD17 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 18, 0);	/* LCDD18 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 19, 0);	/* LCDD19 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 20, 0);	/* LCDD20 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 21, 0);	/* LCDD21 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 22, 0);	/* LCDD22 */
		at91_pio3_set_a_periph(AT91_PIO_PORTC, 23, 0);	/* LCDD23 */

		at91_periph_clk_enable(ATMEL_ID_LCDC);
	}
}

#ifdef CONFIG_LCD_INFO
void lcd_show_board_info(void)
{
	char temp[32];

	if (has_lcdc()) {
		lcd_printf("U-Boot@Sonic, 2017 Petr Buchta\n");
		lcd_printf("%s\n", U_BOOT_VERSION);
		lcd_printf("%s CPU at %s MHz\n",
			get_cpu_name(),
			strmhz(temp, get_cpu_clk_rate()));
		lcd_printf("  %ld MB SDRAM", gd->bd->bi_dram[0].size >> 20);
	}
}
#endif /* CONFIG_LCD_INFO */
#endif /* CONFIG_LCD */

#ifdef CONFIG_DEBUG_UART_BOARD_INIT
void board_debug_uart_init(void)
{
	at91_seriald_hw_init();
}
#endif

#ifdef CONFIG_BOARD_EARLY_INIT_F
int board_early_init_f(void)
{
#ifdef CONFIG_DEBUG_UART
	debug_uart_init();
#endif
	return 0;
}
#endif

int board_init(void)
{
    // DISABLE WATCHDOG
	struct at91_wdt *wdt = (struct at91_wdt *)ATMEL_BASE_WDT;
	writel(AT91_WDT_MR_WDDIS, &wdt->mr);

	/* arch number of AT91SAM9X5EK-Board */
	gd->bd->bi_arch_number = MACH_TYPE_AT91SAM9X5EK;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

#if defined(CONFIG_USB_OHCI_NEW) || defined(CONFIG_USB_EHCI_HCD)
	at91_uhp_hw_init();
#endif
#ifdef CONFIG_LCD
	at91sam9x5ek_lcd_hw_init();
#endif
	return 0;
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *) CONFIG_SYS_SDRAM_BASE,
					CONFIG_SYS_SDRAM_SIZE);
	return 0;
}

#if defined(CONFIG_ATMEL_SPI) && !defined(CONFIG_DM_SPI)
#include <spi.h>

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
    return bus == 0 && cs == 0;
}

void spi_cs_activate(struct spi_slave *slave)
{
    at91_set_pio_output(AT91_PIO_PORTA, 14, 0);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
    at91_set_pio_output(AT91_PIO_PORTA, 14, 1);
}
#endif

#if defined(CONFIG_SPL_BUILD)
#include <spl.h>
#include <asm/arch/atmel_mpddrc.h>

void at91_spl_board_init(void)
{
#ifdef CONFIG_SYS_USE_MMC
	at91_mci_hw_init();
#endif
#ifdef CONFIG_ATMEL_SPI
    at91_spi0_hw_init(1<<4);
#endif
}

void mem_init(void)
{
	struct at91_pmc *pmc = (struct at91_pmc *)ATMEL_BASE_PMC;
	struct at91_matrix *matrix = (struct at91_matrix *)ATMEL_BASE_MATRIX;
	struct atmel_mpddrc_config conf;
	unsigned long csa;

	conf.md = ATMEL_MPDDRC_MD_SDR_SDRAM | ATMEL_MPDDRC_MD_DBW_16_BITS;
	conf.cr = 0x335;
	conf.rtr = 0x61A; /* 15.625us -> 1562 */
	conf.tpr0 = 0x3234A346;
	conf.tpr1 = 0xB;

	/* enable DDR clock */
	writel(AT91_PMC_DDR, &pmc->scer);

    /* Configure EBI Chip Select 1 */
    csa = 0;
    /* EBI CS1 assigned to DDR2SDR controller */
    csa |= AT91_MATRIX_EBI_CS1A_SDRAMC;
    /* Data bus are NOT internally pulled-up */
    csa |= AT91_MATRIX_EBI_DBPU_OFF;
	writel(csa, &matrix->ebicsa);

	/* Controller initialize */
	ddrsdrc_sdr_init(ATMEL_BASE_DDRSDRC, ATMEL_BASE_CS1, &conf);
}
#endif
