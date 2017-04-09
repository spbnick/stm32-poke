#include "rcc.h"
#include "gpio.h"
#include "init.h"
#include "stk.h"

#define STOP \
    do {                \
        asm ("wfi");    \
    } while (1)

void systick_handler(void) __attribute__ ((isr));
void
systick_handler(void)
{
    /* Toggle the LED */
    GPIO_C->odr ^= GPIO_ODR_ODR13_MASK;
}

int
main(void)
{
    /* Basic init */
    init();

    /*
     * Enable I/O ports
     */
    /* Enable APB2 clock to I/O port C */
    RCC->apb2enr |= RCC_APB2ENR_IOPCEN_MASK;

    /*
     * Enable LED output
     */
    /* Set PC13 to general purpose open-drain output, max speed 2MHz */
    gpio_pin_conf(GPIO_C, 13,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_GP_OPEN_DRAIN);

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
