/*
 * Continuously take measurements with ADC1 on PA0 and output them on USART1
 */
#include "init.h"
#include "rcc.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "stk.h"
#include "nvic.h"
#include <stdbool.h>

volatile struct adc *ADC;
volatile bool UPDATED;
volatile unsigned int VAL;

void adc1_2_irq_handler(void) __attribute__ ((isr));
void
adc1_2_irq_handler(void)
{
    VAL = ADC->dr & ADC_DR_DATA_MASK;
    UPDATED = true;
}

void systick_handler(void) __attribute__ ((isr));
void
systick_handler(void)
{
    /* Start another conversion */
    ADC->cr2 |= ADC_CR2_ADON_MASK;
}

int
main(void)
{
    volatile struct usart *usart = USART1;

    /* Basic init */
    init();
    
    /* Set ADC clock prescaler to produce 12MHz */
    RCC->cfgr = (RCC->cfgr & (~RCC_CFGR_ADCPRE_MASK)) |
                (RCC_CFGR_ADCPRE_VAL_PCLK2_DIV6 << RCC_CFGR_ADCPRE_LSB);

    /* Enable clock to I/O port A and ADC1 */
    RCC->apb2enr |= RCC_APB2ENR_IOPAEN_MASK | RCC_APB2ENR_ADC1EN_MASK;

    /*
     * Configure pins
     */
    /* Configure TX pin (PA9) */
    gpio_pin_conf(GPIO_A, 9,
                  GPIO_MODE_OUTPUT_50MHZ, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

    /* Configure RX pin (PA10) */
    gpio_pin_conf(GPIO_A, 10,
                  GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);

    /* Configure ADC pin */
    gpio_pin_conf(GPIO_A, 0,
                  GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG);

    /*
     * Configure USART
     */
    /* Enable clock to USART1 */
    RCC->apb2enr |= RCC_APB2ENR_USART1EN_MASK;

    /* Enable USART, leave the default mode of 8N1 */
    usart->cr1 |= USART_CR1_UE_MASK;

    /* Set baud rate of 115200 based on PCLK1 at 36MHz */
    usart->brr = usart_brr_val(72 * 1000 * 1000, 115200);

    /* Enable receiver and transmitter */
    usart->cr1 |= USART_CR1_RE_MASK | USART_CR1_TE_MASK;

    /*
     * Configure ADC1
     */
    ADC = ADC1;
    /* Turn on ADC */
    ADC->cr2 |= ADC_CR2_ADON_MASK;

    /*
     * Wait for at least one microsecond for ADC to stabilize, and for two ADC
     * cycles before starting calibration.
     */
    {
        volatile unsigned int i;
        for (i = 0; i < 36; i++);
    }

    /* Calibrate the ADC */
    ADC->cr2 |= ADC_CR2_CAL_MASK;
    while (ADC->cr2 & ADC_CR2_CAL_MASK);

    /* Enable end-of-conversion (EOC) interrupt */
    ADC->cr1 |= ADC_CR1_EOCIE_MASK;
    NVIC->iser[NVIC_INT_ADC1_2 / 32] |= 1 << (NVIC_INT_ADC1_2 % 32);

    /* Set channel zero sampling time to 71.5 ADC cycles for precision */
    ADC->smpr2 = (ADC->smpr2 & (~ADC_SMPR2_SMP0_MASK)) |
                 (ADC_SMPRX_SMPX_VAL_71_5C << ADC_SMPR2_SMP0_LSB);

    /*
     * Leave the default of single conversion of channel zero (default),
     * right-aligned data.
     */

    /* Initialize global vars to an impossible value */
    VAL = ~0;
    UPDATED = false;

    /*
     * Set SysTick timer to fire the interrupt each half-second.
     * NOTE the ST PM0056 says: "When HCLK is programmed at the maximum
     * frequency, the SysTick period is 1ms."
     */
    STK->val = STK->load =
        (((STK->calib & STK_CALIB_TENMS_MASK) >> STK_CALIB_TENMS_LSB) + 1) *
        500 - 1;
    STK->ctrl |= STK_CTRL_ENABLE_MASK | STK_CTRL_TICKINT_MASK;

    /*
     * Transfer
     */
    while (true) {
        bool updated;
        unsigned int val;
        unsigned int nibble_lsb;
        unsigned char nibble;

        /* Wait for ADC value to change */
        do {
            asm ("wfi");
            asm ("cpsid i");
            updated = UPDATED;
            if (updated) {
                val = VAL;
                UPDATED = false;
            }
            asm ("cpsie i");
        } while (!updated);

        /* Start new line */
        while (!(usart->sr & USART_SR_TXE_MASK));
        usart->dr = '\r';
        while (!(usart->sr & USART_SR_TXE_MASK));
        usart->dr = '\n';

        /* Output current value */
        nibble_lsb = 12;
        while (true) {
            /* Wait for transmit register to empty */
            while (!(usart->sr & USART_SR_TXE_MASK));
            /* Write the nibble */
            nibble = (val >> nibble_lsb) & 0xf;
            usart->dr = nibble <= 9 ? '0' + nibble : 'a' - 10 + nibble;
            /* Move to next nibble */
            if (nibble_lsb == 0) {
                break;
            }
            nibble_lsb -= 4;
        }

        /* Wait for transfer to complete */
        while (!(usart->sr & USART_SR_TC_MASK));
    }
}
