#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>

void init_clocks(void)
{ // Configur   e one FRAM waitstate as required by the device datasheet for MCLK operation beyond 8MHz before configuring the clock system
    FRCTL0 = FRCTLPW | NWAITS_1;
    P2SEL1 |= BIT6 | BIT7; // P2.6~P2.7: crystal pins
do
{
    CSCTL7 &= ~(XT1OFFG | DCOFFG); // Clear XT1 and DCO fault flag
    SFRIFG1 &= ~OFIFG;
} while (SFRIFG1 & OFIFG); // Test oscillator fault flag
    __bis_SR_register(SCG0); // disable FLL
    CSCTL3 |= SELREF__XT1CLK; // Set XT1 as FLL reference source
    //CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_5; // DCOFTRIM=5, DCO Range = 16MHz**
    CSCTL1 = DCORSEL_5; // DCOFTRIM=5, DCO Range = 16MHz
    CSCTL2 = FLLD_0 + 487; // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0); // enable FLL
    //Software_Trim(); // Software Trim to get the best DCOFTRIM value**
    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK; // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
    // default DCOCLKDIV as MCLK and SMCLK source
    //P1DIR |= BIT0 | BIT1; // set SMCLK, ACLK pin as output
    //P1SEL1 |= BIT0 | BIT1; // set SMCLK and ACLK pin as second function
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
}

uint8_t *PTxData; // Pointer to TX data
uint8_t TXByteCtr;
uint8_t *PRxData; // Pointer to RX data
uint8_t RXByteCtr;

void i2c_init()
{
    //P4SEL0 |= BIT7 + BIT6; * // P4.6 SDA i P4.7 SCL com a USCI si fem server USCI B1
    //P1SEL0 |= BIT3 + BIT2; * // P1.2 SDA i P1.3 SCL com a USCI si fem server USCI B0
    UCB0CTLW0 |= UCSWRST; // Aturem el mòdul
    //El configurem com a master, síncron i mode i2c, per defecte, està en single-master mode
    UCB0CTLW0 |= UCMST + UCMODE_3 + UCSSEL_2; // Use SMCLK,
    UCB0BR0 = 160; // fSCL = SMCLK(16MHz)/160 = ~100kHz
    UCB0BR1 = 0;
    UCB0CTLW0 &= ~UCSWRST; // Clear SW reset, resume operation
    UCB0IE |= UCTXIE0 | UCRXIE0; // Habilita les interrupcions a TX i RX
}

uint8_t seguir;
uint16_t contador;
uint16_t valor_cont;
void init_timers (void)
{
    TB3CCR0 = 16000;                      //indicamos hasta cuando queremos contar
    TB3CTL |= MC_0 | TBSSEL_2 | TBCLR;   //configuramos SMCLK, que empieze en stop mode y un reset inicial
    TB3CCTL0 |= CCIE;                     //habilitamos interrupcion

}

void delay_ms(uint16_t msec)
{
    seguir =1;
    contador = 0;
    valor_cont = msec;
    TB3CTL |= MC_1 | TBCLR;
    while(seguir);
    TB3CTL &= ~MC_1;

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    init_clocks();
    init_timers();
    P2SEL1 &= ~BIT0;
    P2SEL0 &= ~BIT0;
    P2DIR |=  BIT0;

    __enable_interrupt();

	while(1)
	{
	   P2OUT ^= BIT0;
	   delay_ms(100);

	}
	return 0;
}



#pragma vector=TIMER3_B0_VECTOR //Aquest és el nom important
__interrupt void NOM_QUE_VOLEM_DONAR_A_LA_ISR (void)
{
/* El que volem fer a la rutina d’atenció d’Interrupció */

    if(contador<valor_cont){
        contador=contador+1;
    }
    else {
        seguir = 0;
    }

}
