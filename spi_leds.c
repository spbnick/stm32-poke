/*
 * Control 8 LEDs connected via SPI and a TLC5916IN.
 * Increment the output byte every second.
 *
 * A3 - GPIO    - LE(ED1)
 * A4 - GPIO    - nOE(ED2)
 * A5 - SCK     - CLK
 * A6 - MISO    - SDO
 * A7 - MOSI    - SDI
 */
#include <rcc.h>
#include <gpio.h>
#include <init.h>
#include <tim.h>
#include <spi.h>
#include <stk.h>
#include <stdbool.h>

#define STOP \
    do {                \
        asm ("wfi");    \
    } while (1)

static volatile struct spi *SPI;
static volatile unsigned int VAL;

void systick_handler(void) __attribute__ ((isr));
void
systick_handler(void)
{
    unsigned int v;

    /* If loading stage */
    if ((VAL & 1) == 0) {
        /* Output value */
        SPI->dr = (VAL >> 1) & 0xff;
        while (!(SPI->sr & SPI_SR_TXE_MASK));
        /* Receive and discard the answer */
        while (!(SPI->sr & SPI_SR_RXNE_MASK));
        v = SPI->dr;
        (void)v;

        /* Enable loading the data to the outputs */
        gpio_pin_set(GPIO_A, 3, true);
    } else {
        /* Disable loading the data to the outputs */
        gpio_pin_set(GPIO_A, 3, false);
    }

    VAL++;
}

int
main(void)
{
    /* Basic init */
    init();

    /* Use the first SPI peripheral */
    SPI = SPI1;

    /*
     * Enable clocks
     */
    /* Enable APB2 clock to I/O port A and SPI1 */
    RCC->apb2enr |= RCC_APB2ENR_IOPAEN_MASK | RCC_APB2ENR_SPI1EN_MASK;

    /*
     * Configure pins
     */
    /* A3 - GPIO - LE(ED1), push-pull output */
    gpio_pin_set(GPIO_A, 3, false);
    gpio_pin_conf(GPIO_A, 3,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_GP_PUSH_PULL);
    /* A4 - GPIO - nOE(ED2), open-drain output */
    gpio_pin_set(GPIO_A, 4, true);
    gpio_pin_conf(GPIO_A, 4,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_GP_OPEN_DRAIN);
    /* A5 - SCK, alternate function push-pull */
    gpio_pin_conf(GPIO_A, 5,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    /* A6 - MISO, input pull-up */
    gpio_pin_conf(GPIO_A, 6,
                  GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL);
    gpio_pin_set(GPIO_A, 6, true);
    /* A7 - MOSI, alternate function push-pull */
    gpio_pin_conf(GPIO_A, 7,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

    /*
     * Configure the SPI
     * Set it to run at APB2clk/16, make it a master, enable software NSS pin
     * management, raise it, and enable SPI.
     */
    SPI->cr1 = (SPI->cr1 &
                ~(SPI_CR1_BR_MASK | SPI_CR1_MSTR_MASK | SPI_CR1_SPE_MASK |
                  SPI_CR1_SSM_MASK | SPI_CR1_SSI_MASK)) |
               (SPI_CR1_BR_VAL_FPCLK_DIV16 << SPI_CR1_BR_LSB) |
               (SPI_CR1_MSTR_VAL_MASTER << SPI_CR1_MSTR_LSB) |
               SPI_CR1_SSM_MASK | SPI_CR1_SSI_MASK | SPI_CR1_SPE_MASK;

    /* Start with zero output */
    VAL = 0;

    /* Enable the outputs */
    gpio_pin_set(GPIO_A, 4, false);

    /*
     * Set SysTick timer to fire the interrupt each half-second.
     * NOTE the ST PM0056 says: "When HCLK is programmed at the maximum
     * frequency, the SysTick period is 1ms."
     */
    STK->val = STK->load =
        (((STK->calib & STK_CALIB_TENMS_MASK) >> STK_CALIB_TENMS_LSB) + 1) *
        500 - 1;
    STK->ctrl |= STK_CTRL_ENABLE_MASK | STK_CTRL_TICKINT_MASK;

    STOP;
}
