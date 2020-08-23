/*
 * Print a message using semihosting interface.
 * See https://wiki.segger.com/Semihosting and
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0471m/pge1358787045051.html
 */

int
main(void)
{
    static const char *message="Hello, World!\r\n";

    asm(".thumb\n\t"
        /* SYS_WRITE0 */
        "mov r0, #0x04\n\t"
        /* String address */
        "ldr r1, %0\n\t"
        "bkpt 0xab\n\t"
        /* Output operands */
        :
        /* Input operands */
        : "m" (message)
        /* Clobbers */
        : "r0", "r1"
    );

    while (1);
}
