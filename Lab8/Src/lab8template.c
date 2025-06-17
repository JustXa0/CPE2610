/**
 * Assignment: Lab8 - I2C Movement
 * Name:
 * Course: CPE2610
 * Dependencies: uart_driver.h, stm32f411.h, delay.h
 * Description: I2C ADXL345 is used to display the g forces to a console.
 * Instructions:
 * 	C-Level - Use ADXL345 Documentation and STM32F411 Reference Manual and Notes
 * 		to fill replace the block comments below to enable the functionality
 * 	B-Level - Include annotated waveforms showing a transmission setting
 * 		a register and a transmission receiving sensor data from the
 * 		accelerometer
 * 	A-Level Use the results in an application which makes use of the
 * 		accelerometer data.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f411.h"
#include "uart_driver.h"
#include "delay.h"


#define ADXL345_ADDRESS 0x53
#define ADXL345_SENSORS 0x32
#define ADXL345_DATAFORMAT 0x31
#define ADXL345_POWERCTL 0x2D

#define F_CPU 16000000UL


 /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
 */

/**
 * I2C1_Init
 * Sets up PortB and I2C1
 */
void I2C1_Init();

/**
 * I2C1_WriteByte
 * Writes ACCELL[reg]<--data
 * Args:
 * 	address - 8-bit address (note includes write/read bit)
 * 	reg - register in accell
 * 	data - 8-bit data to store
 */
void I2C1_WriteByte(uint8_t address, uint8_t reg, uint8_t data);

/**
 * I2C1_ReadBytes
 * Read ACCELL[A(0)..A(N-1)] > DATA[0..N-1]
 * Args:
 * 	address - 8-bit address (note includes write/read bit)
 * 	reg - register in accell
 * 	data - 8-bit data to store
 */
void I2C1_ReadBytes(uint8_t address, uint8_t reg,
		uint8_t *data, uint8_t length);

/**
 * ADXL345_Init
 * Sets up the Data Format and Start Measuring
 */
void ADXL345_Init();

/**
 * Samples G's in X,Y,and Z and prints them to the console
 */
int main(void){
	init_usart2(57600,F_CPU);
	I2C1_Init();
	ADXL345_Init();


	uint8_t data[6];
	while (1) {
		// Read 6 bytes from the ADXL345
		I2C1_ReadBytes(ADXL345_ADDRESS, ADXL345_SENSORS, data, 6);

		// Process raw data into x,y,z (e.g., convert to acceleration values)
		int16_t x = (int16_t)(data[1] << 8 | data[0]);
		int16_t y = (int16_t)(data[3] << 8 | data[2]);
		int16_t z = (int16_t)(data[5] << 8 | data[4]);

		printf("X:%d\tY:%d\tZ:%d\n",x,y,z);

		//Delay for 250ms
		SystemMilli(250);
	}
}

void I2C1_Init(){
	//1. Turn on GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//Pins to Alt Function
	GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE7_Msk);
	GPIOB->MODER |= (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);
	//No pull ups
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk | GPIO_PUPDR_PUPD7_Msk);

	//Open Drain
	GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

	//High Speed Output
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6_Msk | GPIO_OSPEEDR_OSPEED7_Msk);
	GPIOB->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED6_Pos) | (3 << GPIO_OSPEEDR_OSPEED7_Pos));

	//AF## = ?
	GPIOB->AFR[0] &= ~(0xFF << (24));
	GPIOB->AFR[0] |= (0x44 << (24));

	//Turn on I2C1
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;


	// Configure the I2C1 peripheral
	I2C1->CR1 |= I2C_CR1_SWRST; // Reset I2C1
	I2C1->CR1 &= ~I2C_CR1_SWRST; // Clear reset
	I2C1->CR2 = 0x10; // Set peripheral clock frequency to 16 MHz
	I2C1->CCR = 80; // Set clock control register for 100 kHz
	I2C1->TRISE = 17; // Set maximum rise time
	I2C1->CR1 |= I2C_CR1_PE; // Enable I2C1
}

void I2C1_WriteByte(uint8_t address, uint8_t reg, uint8_t data) {
	// Generate START condition
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)); // Wait for START condition generated

	// Send slave 8-bit address with write bit (A(0) = /****/)
	I2C1->DR = ((address << 1) | 0b0);
	while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Wait for address sent
	(void)I2C1->SR2; // Clear ADDR flag

	// Send chip register address byte
	I2C1->DR = reg;
	while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait for data register empty

	// Send data byte
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait for data register empty

	// Generate STOP condition
	I2C1->CR1 |= I2C_CR1_STOP;
}


void I2C1_ReadBytes(uint8_t address, uint8_t reg,
		uint8_t *data, uint8_t length) {
	// Generate START condition
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)); // Wait for START condition generated

	// Send slave address with write bit (A(0) = 0)
	I2C1->DR = ((address << 1) | 0b0);
	while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Wait for address sent
	(void)I2C1->SR2; // Clear ADDR flag

	// Send register byte
	I2C1->DR = reg;
	while (!(I2C1->SR1 & I2C_SR1_TXE)); // Wait for data register empty

	// Generate RESTART condition
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB)); // Wait for START condition generated


	// Send slave address with read bit (A(0) = 1)
	I2C1->DR = ((address << 1) | 0b1);
	while (!(I2C1->SR1 & I2C_SR1_ADDR)); // Wait for address sent
	(void)I2C1->SR2; // Clear ADDR flag

	//Turn on ACK response to receiving data
	I2C1->CR1 |= I2C_CR1_ACK;

	for (uint8_t i = 0; i < length; i++) {
		if (i == length - 1) {
			// Generate NACK and STOP condition for the last byte
			I2C1->CR1 &= ~I2C_CR1_ACK;
			I2C1->CR1 |= I2C_CR1_STOP;
		}

		while (!(I2C1->SR1 & I2C_SR1_RXNE)); // Wait for data received
		data[i] = I2C1->DR; // Read data byte
	}
}


void ADXL345_Init(){
	//Configure Data Format to +/-16g
	I2C1_WriteByte(ADXL345_ADDRESS, ADXL345_DATAFORMAT, 0x0B);
	//Configure Power Control to Measure
	I2C1_WriteByte(ADXL345_ADDRESS, ADXL345_POWERCTL, 0x08);
}
