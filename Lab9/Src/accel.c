/*
 * accel.c
 *
 *  Created on: May 7, 2025
 *      Author: prendivillej
 */

#include "accel.h"
#include "stm32f411.h"

#define ADXL345_ADDRESS 0x53
#define ADXL345_SENSORS 0x32
#define ADXL345_DATAFORMAT 0x31
#define ADXL345_POWERCTL 0x2D

/**
 * ADXL345Init - initializes the I2C component and accelerometer for reads
 * Args - N/A
 * Returns - N/A
 */
void ADXL345Init() {
	I2C1_Init();
	//Configure Data Format to +/-16g
	I2C1_WriteByte(ADXL345_ADDRESS, ADXL345_DATAFORMAT, 0x0B);
	//Configure Power Control to Measure
	I2C1_WriteByte(ADXL345_ADDRESS, ADXL345_POWERCTL, 0x08);
}

/**
 * ADXL345Read - reads from x, y, and z position registers and passes them to addresses specified
 * Args - x, y, z: variables to store read positions
 * Returns - N/A
 */
void ADXL345Read(int16_t *x, int16_t *y, int16_t *z) {
	uint8_t data[6];
	I2C1_ReadBytes(ADXL345_ADDRESS, ADXL345_SENSORS, data, 6);

	*x = (int16_t)(data[1] << 8 | data[0]);
	*y = (int16_t)(data[3] << 8 | data[2]);
	*z = (int16_t)(data[5] << 8 | data[4]);
}

static void I2C1_WriteByte(uint8_t address, uint8_t reg, uint8_t data) {
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

static void I2C1_ReadBytes(uint8_t address, uint8_t reg,
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

static void I2C1_Init(){
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
