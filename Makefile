CC=arm-none-eabi-

TARGET_CFLAGS = -mcpu=cortex-m3 -mthumb
COMMON_CFLAGS = $(TARGET_CFLAGS) -Wall -Wextra -Werror -g3
LIBS = -lstammer

PROGRAMS = \
    adc_stream          \
    blink               \
    exti_led            \
    pwm_blink           \
    semihosting         \
    semihosting_printf  \
    spi_leds            \
    tim_int_blink       \
    usart_hello

.PHONY: clean

all: $(PROGRAMS:=.bin)

%.o: %.c
	$(CC)gcc $(COMMON_CFLAGS) $(CFLAGS) -c -o $@ $<
	$(CC)gcc $(COMMON_CFLAGS) $(CFLAGS) -MM $< > $*.d

%.o: %.S
	$(CC)gcc $(COMMON_CFLAGS) $(CFLAGS) -D__ASSEMBLY__ -c -o $@ $<
	$(CC)gcc $(COMMON_CFLAGS) $(CFLAGS) -D__ASSEMBLY__ -MM $< > $*.d

define ELF_RULE
$(strip $(1))_OBJS = $$(addsuffix .o, $(1) $$($(strip $(1))_MODULES))
$(1).elf: $$($(strip $(1))_OBJS)
	$(CC)gcc -nostartfiles $(LDFLAGS) -T libstammer.ld -o $$@ \
		$$($(strip $(1))_OBJS) $(LIBS)
OBJS += $$($(strip $(1))_OBJS)
endef
$(foreach p, $(PROGRAMS), $(eval $(call ELF_RULE, $(p))))
DEPS = $(OBJS:.o=.d)
-include $(DEPS)

%.bin: %.elf
	$(CC)objcopy -O binary $< $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f $(PROGRAMS:=.elf)
	rm -f $(PROGRAMS:=.bin)
