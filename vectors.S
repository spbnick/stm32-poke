.syntax unified

.section ".vectors"

/* Stack top */
.word 0x20001000
/* Handlers */
  .word  reset_handler
  .word  nmi_handler
  .word  hardfault_handler
  .word  memmanage_handler
  .word  busfault_handler
  .word  usagefault_handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  svc_handler
  .word  debugmon_handler
  .word  0
  .word  pendsv_handler
  .word  systick_handler
  .word  wwdg_irqhandler
  .word  pvd_irqhandler
  .word  tamper_irqhandler
  .word  rtc_irqhandler
  .word  flash_irqhandler
  .word  rcc_irqhandler
  .word  exti0_irqhandler
  .word  exti1_irqhandler
  .word  exti2_irqhandler
  .word  exti3_irqhandler
  .word  exti4_irqhandler
  .word  dma1_channel1_irqhandler
  .word  dma1_channel2_irqhandler
  .word  dma1_channel3_irqhandler
  .word  dma1_channel4_irqhandler
  .word  dma1_channel5_irqhandler
  .word  dma1_channel6_irqhandler
  .word  dma1_channel7_irqhandler
  .word  adc1_2_irqhandler
  .word  usb_hp_can1_tx_irqhandler
  .word  usb_lp_can1_rx0_irqhandler
  .word  can1_rx1_irqhandler
  .word  can1_sce_irqhandler
  .word  exti9_5_irqhandler
  .word  tim1_brk_irqhandler
  .word  tim1_up_irqhandler
  .word  tim1_trg_com_irqhandler
  .word  tim1_cc_irqhandler
  .word  tim2_irqhandler
  .word  tim3_irqhandler
  .word  tim4_irqhandler
  .word  i2c1_ev_irqhandler
  .word  i2c1_er_irqhandler
  .word  i2c2_ev_irqhandler
  .word  i2c2_er_irqhandler
  .word  spi1_irqhandler
  .word  spi2_irqhandler
  .word  usart1_irqhandler
  .word  usart2_irqhandler
  .word  usart3_irqhandler
  .word  exti15_10_irqhandler
  .word  rtcalarm_irqhandler
  .word  usbwakeup_irqhandler
  .word  tim8_brk_irqhandler
  .word  tim8_up_irqhandler
  .word  tim8_trg_com_irqhandler
  .word  tim8_cc_irqhandler
  .word  adc3_irqhandler
  .word  fsmc_irqhandler
  .word  sdio_irqhandler
  .word  tim5_irqhandler
  .word  spi3_irqhandler
  .word  uart4_irqhandler
  .word  uart5_irqhandler
  .word  tim6_irqhandler
  .word  tim7_irqhandler
  .word  dma2_channel1_irqhandler
  .word  dma2_channel2_irqhandler
  .word  dma2_channel3_irqhandler
  .word  dma2_channel4_5_irqhandler

default_handler:
    wfi
    b default_handler

  .weak  nmi_handler
  .thumb_set nmi_handler,default_handler
  
  .weak  hardfault_handler
  .thumb_set hardfault_handler,default_handler
  
  .weak  memmanage_handler
  .thumb_set memmanage_handler,default_handler
  
  .weak  busfault_handler
  .thumb_set busfault_handler,default_handler

  .weak  usagefault_handler
  .thumb_set usagefault_handler,default_handler

  .weak  svc_handler
  .thumb_set svc_handler,default_handler

  .weak  debugmon_handler
  .thumb_set debugmon_handler,default_handler

  .weak  pendsv_handler
  .thumb_set pendsv_handler,default_handler

  .weak  systick_handler
  .thumb_set systick_handler,default_handler

  .weak  wwdg_irqhandler
  .thumb_set wwdg_irqhandler,default_handler

  .weak  pvd_irqhandler
  .thumb_set pvd_irqhandler,default_handler

  .weak  tamper_irqhandler
  .thumb_set tamper_irqhandler,default_handler

  .weak  rtc_irqhandler
  .thumb_set rtc_irqhandler,default_handler

  .weak  flash_irqhandler
  .thumb_set flash_irqhandler,default_handler

  .weak  rcc_irqhandler
  .thumb_set rcc_irqhandler,default_handler

  .weak  exti0_irqhandler
  .thumb_set exti0_irqhandler,default_handler

  .weak  exti1_irqhandler
  .thumb_set exti1_irqhandler,default_handler

  .weak  exti2_irqhandler
  .thumb_set exti2_irqhandler,default_handler

  .weak  exti3_irqhandler
  .thumb_set exti3_irqhandler,default_handler

  .weak  exti4_irqhandler
  .thumb_set exti4_irqhandler,default_handler

  .weak  dma1_channel1_irqhandler
  .thumb_set dma1_channel1_irqhandler,default_handler

  .weak  dma1_channel2_irqhandler
  .thumb_set dma1_channel2_irqhandler,default_handler

  .weak  dma1_channel3_irqhandler
  .thumb_set dma1_channel3_irqhandler,default_handler

  .weak  dma1_channel4_irqhandler
  .thumb_set dma1_channel4_irqhandler,default_handler

  .weak  dma1_channel5_irqhandler
  .thumb_set dma1_channel5_irqhandler,default_handler

  .weak  dma1_channel6_irqhandler
  .thumb_set dma1_channel6_irqhandler,default_handler

  .weak  dma1_channel7_irqhandler
  .thumb_set dma1_channel7_irqhandler,default_handler

  .weak  adc1_2_irqhandler
  .thumb_set adc1_2_irqhandler,default_handler

  .weak  usb_hp_can1_tx_irqhandler
  .thumb_set usb_hp_can1_tx_irqhandler,default_handler

  .weak  usb_lp_can1_rx0_irqhandler
  .thumb_set usb_lp_can1_rx0_irqhandler,default_handler

  .weak  can1_rx1_irqhandler
  .thumb_set can1_rx1_irqhandler,default_handler

  .weak  can1_sce_irqhandler
  .thumb_set can1_sce_irqhandler,default_handler

  .weak  exti9_5_irqhandler
  .thumb_set exti9_5_irqhandler,default_handler

  .weak  tim1_brk_irqhandler
  .thumb_set tim1_brk_irqhandler,default_handler

  .weak  tim1_up_irqhandler
  .thumb_set tim1_up_irqhandler,default_handler

  .weak  tim1_trg_com_irqhandler
  .thumb_set tim1_trg_com_irqhandler,default_handler

  .weak  tim1_cc_irqhandler
  .thumb_set tim1_cc_irqhandler,default_handler

  .weak  tim2_irqhandler
  .thumb_set tim2_irqhandler,default_handler

  .weak  tim3_irqhandler
  .thumb_set tim3_irqhandler,default_handler

  .weak  tim4_irqhandler
  .thumb_set tim4_irqhandler,default_handler

  .weak  i2c1_ev_irqhandler
  .thumb_set i2c1_ev_irqhandler,default_handler

  .weak  i2c1_er_irqhandler
  .thumb_set i2c1_er_irqhandler,default_handler

  .weak  i2c2_ev_irqhandler
  .thumb_set i2c2_ev_irqhandler,default_handler

  .weak  i2c2_er_irqhandler
  .thumb_set i2c2_er_irqhandler,default_handler

  .weak  spi1_irqhandler
  .thumb_set spi1_irqhandler,default_handler

  .weak  spi2_irqhandler
  .thumb_set spi2_irqhandler,default_handler

  .weak  usart1_irqhandler
  .thumb_set usart1_irqhandler,default_handler

  .weak  usart2_irqhandler
  .thumb_set usart2_irqhandler,default_handler

  .weak  usart3_irqhandler
  .thumb_set usart3_irqhandler,default_handler

  .weak  exti15_10_irqhandler
  .thumb_set exti15_10_irqhandler,default_handler

  .weak  rtcalarm_irqhandler
  .thumb_set rtcalarm_irqhandler,default_handler

  .weak  usbwakeup_irqhandler
  .thumb_set usbwakeup_irqhandler,default_handler

  .weak  tim8_brk_irqhandler
  .thumb_set tim8_brk_irqhandler,default_handler

  .weak  tim8_up_irqhandler
  .thumb_set tim8_up_irqhandler,default_handler

  .weak  tim8_trg_com_irqhandler
  .thumb_set tim8_trg_com_irqhandler,default_handler

  .weak  tim8_cc_irqhandler
  .thumb_set tim8_cc_irqhandler,default_handler

  .weak  adc3_irqhandler
  .thumb_set adc3_irqhandler,default_handler

  .weak  fsmc_irqhandler
  .thumb_set fsmc_irqhandler,default_handler

  .weak  sdio_irqhandler
  .thumb_set sdio_irqhandler,default_handler

  .weak  tim5_irqhandler
  .thumb_set tim5_irqhandler,default_handler

  .weak  spi3_irqhandler
  .thumb_set spi3_irqhandler,default_handler

  .weak  uart4_irqhandler
  .thumb_set uart4_irqhandler,default_handler

  .weak  uart5_irqhandler
  .thumb_set uart5_irqhandler,default_handler

  .weak  tim6_irqhandler
  .thumb_set tim6_irqhandler,default_handler

  .weak  tim7_irqhandler
  .thumb_set tim7_irqhandler,default_handler

  .weak  dma2_channel1_irqhandler
  .thumb_set dma2_channel1_irqhandler,default_handler

  .weak  dma2_channel2_irqhandler
  .thumb_set dma2_channel2_irqhandler,default_handler

  .weak  dma2_channel3_irqhandler
  .thumb_set dma2_channel3_irqhandler,default_handler

  .weak  dma2_channel4_5_irqhandler
  .thumb_set dma2_channel4_5_irqhandler,default_handler
