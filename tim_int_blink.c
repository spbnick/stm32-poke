/*
 * Blink the PC13 LED using one-pulse mode timer interrupt
 */
#include <init.h>
#include <tim.h>
#include <gpio.h>
#include <nvic.h>
#include <rcc.h>

#define STOP \
    do {                \
        asm ("wfi");    \
    } while (1)

void tim2_irq_handler(void) __attribute__ ((isr));
void
tim2_irq_handler(void)
{
    if (TIM2->sr & TIM_SR_CC1IF_MASK) {
        /* Toggle the LED */
        GPIO_C->odr ^= GPIO_ODR_ODR13_MASK;
        /* If period is greater than 0.1 seconds */
        if (TIM2->arr > 200) {
            /* Lower period by 10% */
            TIM2->arr = TIM2->arr * 9 / 10;
        }
        /* Restart timer */
        TIM2->egr |= TIM_EGR_UG_MASK;
        TIM2->cr1 |= TIM_CR1_CEN_MASK | TIM_CR1_OPM_MASK;
    }
    /* Clear interrupt flags */
    TIM2->sr = 0;
}

int
main(void)
{
    volatile struct tim *tim = TIM2;

    /* Basic init */
    init();

    /*
     * Enable clocks
     */
    /* Enable APB2 clock to I/O port C */
    RCC->apb2enr |= RCC_APB2ENR_IOPCEN_MASK;
    /* Enable APB1 clock to the timer */
    RCC->apb1enr |= RCC_APB1ENR_TIM2EN_MASK;

    /*
     * Configure the LED
     */
    /* Turn off the LED */
    GPIO_C->odr |= GPIO_ODR_ODR13_MASK;
    /* Set PC13 to general purpose open-drain output, max speed 2MHz */
    gpio_pin_conf(GPIO_C, 13,
                  GPIO_MODE_OUTPUT_2MHZ, GPIO_CNF_OUTPUT_GP_OPEN_DRAIN);

    /*
     * Set up the timer
     */
    /* Enable timer interrupt */
    NVIC->iser[NVIC_INT_TIM2 / 32] |= 1 << (NVIC_INT_TIM2 % 32);
    /* Enable auto-reload preload, select downcounting */
    tim->cr1 = (tim->cr1 & ~TIM_CR1_DIR_MASK) |
               (TIM_CR1_DIR_VAL_DOWN << TIM_CR1_DIR_LSB) |
               TIM_CR1_ARPE_MASK;
    /* Set prescaler to get CK_CNT = 2KHz = 72MHz(APB1*2) / 36000 */
    tim->psc = 36000;
    /* Set auto-reload register to have period of 1 second */
    tim->arr = 2000;
    /* Generate an update event to transfer data to shadow registers */
    tim->egr |= TIM_EGR_UG_MASK;
    /* Enable Capture/Compare 1 interrupt */
    tim->dier |= TIM_DIER_CC1IE_MASK;
    /* Enable counter */
    tim->cr1 |= TIM_CR1_CEN_MASK | TIM_CR1_OPM_MASK;

    STOP;
}
