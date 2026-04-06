#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include "I2C.h"
#include "clocks_timers.h"
#include "delay.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    uint8_t led_on[3]  = {0x0B, 7, 7};
    uint8_t led_off[3] = {0x0B, 0, 0};
    uint8_t engn_on[5] = {0x00, 1, 255, 1, 255};
    uint8_t engn_off[5] = {0x00, 1, 0, 1, 0};

    init_clocks();
    init_timers();
    P2SEL1 &= ~BIT0;
    P2SEL0 &= ~BIT0;
    P2DIR |=  BIT0;

    __enable_interrupt();

    i2c_init();

	while(1)
	{
	   //P2OUT ^= BIT0;
	   I2C_send(0x10, led_on, 3);
	   I2C_send(0x10, engn_on, 5);
	   delay_ms(5000);
	   I2C_send(0x10, led_off, 3);
	   I2C_send(0x10, engn_off, 5);
	   delay_ms(5000);
	}
	return 0;
}



