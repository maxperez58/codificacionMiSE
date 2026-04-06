/*
 * delay.c
 *
 *  Created on: 6 abr. 2026
 *      Author: Max
 */

uint8_t seguir;
uint16_t contador;
uint16_t valor_cont;



void delay_ms(uint16_t msec)
{
    seguir =1;
    contador = 0;
    valor_cont = msec;
    TB3CTL |= MC_1 | TBCLR;
    while(seguir);
    TB3CTL &= ~MC_1;

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

