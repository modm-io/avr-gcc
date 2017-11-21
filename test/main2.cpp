#include <avr/io.h>

int
main()
{
    DDRB = (1 << 1);
    PORTB |= (1 << 1);
    uint8_t read = PINB;
    (void) read;

    return 0;
}