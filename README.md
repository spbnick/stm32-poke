stm32-poke
==========

This is an attempt to write C programs from scratch for a minimal
[STM32 development board][board], which uses STM32F103C8T6 chip.

Building
--------
First you'll need the compiler. If you're using a Debian-based Linux you can
get it by installing `gcc-arm-none-eabi` package. Then you'll need the
[libstammer][libstammer] library. Build it first, then export environment
variables pointing to it:

    export CFLAGS=-ILIBSTAMMER_DIR LDFLAGS=-LLIBSTAMMER_DIR

`LIBSTAMMER_DIR` above is the directory where the built libstammer is located.

After that you can build the program using `make`.

[board]: http://item.taobao.com/item.htm?spm=a1z10.1-c.w4004-9679183684.4.26lLDG&id=22097803050
[libstammer]: https://github.com/spbnick/libstammer
