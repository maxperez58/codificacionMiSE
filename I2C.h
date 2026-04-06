/*
 * I2C.h
 *
 *  Created on: 6 abr. 2026
 *      Author: Max
 */

#ifndef I2C_H_
#define I2C_H_


#include <msp430.h>
#include <stdint.h>

I2C_send(uint8_t addr, uint8_t *buffer, uint8_t n_dades);
I2C_receive(uint8_t addr, uint8_t *buffer, uint8_t n_dades);



#endif /* I2C_H_ */
