/*
 * Print a message using semihosting_printf() from libstammer.
 */
#include <semihosting.h>

int
main(void)
{
    semihosting_printf("H%xllo%c %s!!%d!!%u\n", 0xe, ',', "World", 1, 1);
    while (1);
}
