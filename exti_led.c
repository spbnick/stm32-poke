/*
 * Light the PC13 LED whenever PB15 is pulled HIGH
 */
#include <init.h>
#include <gpio.h>
#include <exti.h>
#include <afio.h>
#include <nvic.h>
#include <rcc.h>

#define STOP \
    do {                \
        asm ("wfi");    \
    } while (1)

void exti15_10_irq_handler(void) __attribute__ ((isr));
void
exti15_10_irq_handler(void)
{
    /* If the pin's interrupt is pending */
    if (EXTI->pr & (1 << 15)) {
        /* Update the LED */
        gpio_pin_set(GPIO_C, 13, !gpio_pin_get(GPIO_B, 15));
        /* Acknowledge the interrupt */
        EXTI->pr |= (1 << 15);
    }
}

int
main(void)
{
    /* Basic init */
    init();

    /* Enable APB2 clock to I/O port B and C, and to AFIO */
    RCC->apb2enr |= RCC_APB2ENR_IOPBEN_MASK | RCC_APB2ENR_IOPCEN_MASK |
                    RCC_APB2ENR_AFIOEN_MASK;

    /* Turn off the LED */
    gpio_pin_set(GPIO_C, 13, 1);
    /* Set PC13 to general purpose open-drain output, max speed 2MHz */
    gpio_pin_conf(GPIO_C, 13,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_GP_OPEN_DRAIN);

    /* Configure B15 as pull-down input */
    gpio_pin_set(GPIO_B, 15, 0);
    gpio_pin_conf(GPIO_B, 15,
                  GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL);

    /* Enable interrupt on both edges of B15 */
    afio_exti_set_port(15, AFIO_EXTI_PORT_B);
    EXTI->imr |= 1 << 15;
    EXTI->rtsr |= 1 << 15;
    EXTI->ftsr |= 1 << 15;
    nvic_int_set_enable_ext(15);

    STOP;
}
