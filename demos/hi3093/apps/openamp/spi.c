#include <stdint.h>
#include <unistd.h>
#include "spi.h"
#include "gpio_def.h"

// SPI初始化函数
void SPI0_Init(void)
{

}

// SPI发送和接收一个字节
uint8_t SPI0_TransferByte(uint8_t CSx, uint8_t data)
{

    return 0;
}

// SPI发送多个字节
void SPI0_TransferBuffer(uint8_t CSx, uint8_t *txBuffer, uint8_t *rxBuffer, uint32_t length)
{

}

// 选择从设备
void SPI0_SelectSlave(uint8_t CSx)
{
    if (CSx == SPI0_CE0)
    {
        GPIO_SET(SPI0_CE0, GPIO_LEVEL_LOW); // 设置CE0为低电平
    }
    else if(CSx == SPI0_CE1)
    {
        SPI0->CS = (SPI0->CS & ~0x03) | SPI_CS_CS1;
        GPIO_SET(SPI0_CE1, GPIO_LEVEL_LOW); // 设置CE1为低电平
    }
}

void SPI0_DeselectSlave(uint8_t CSx)
{
    if (CSx == SPI0_CE0)
    {
        GPIO_SET(SPI0_CE0, GPIO_LEVEL_HIGH); // 设置CE0为高电平
    }
    else if(CSx == SPI0_CE1)
    {
        GPIO_SET(SPI0_CE1, GPIO_LEVEL_HIGH); // 设置CE1为高电平
    }
}

void SPI0_Set_CPHA(uint8_t cpha)
{
    if (cpha == CPHA1)
    {

    }
    else if (cpha == CPHA0)
    {

    }
}