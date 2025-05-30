#ifndef GPIO_DEF_H
#define GPIO_DEF_H

/*my set*/
// �������ַ����ݮ��4B��
#define BCM2711_PERI_BASE 0xFE000000
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000)
#define UART0_BASE (BCM2711_PERI_BASE + 0x201000)
#define SPI0_BASE (BCM2711_PERI_BASE + 0x204000)

#define GPFSEL0 *(volatile uint32_t *)(GPIO_BASE + 0x00)    // GPIO����ѡ��Ĵ���0
#define GPFSEL1 *(volatile uint32_t *)(GPIO_BASE + 0x04)    // GPIO����ѡ��Ĵ���1
#define GPFSEL2 *(volatile uint32_t *)(GPIO_BASE + 0x08)    // GPIO����ѡ��Ĵ���2
#define GPSET0 *(volatile uint32_t *)(GPIO_BASE + 0x1C)   // GPIO���üĴ���0
#define GPCLR0 *(volatile uint32_t *)(GPIO_BASE + 0x28)   // GPIO����Ĵ���0

// GPIO pin modes
#define GPIO_INPUT  0b000
#define GPIO_OUTPUT 0b001
#define GPIO_ALT0   0b100
#define GPIO_ALT1   0b101
#define GPIO_ALT2   0b110
#define GPIO_ALT3   0b111
#define GPIO_ALT4   0b011
#define GPIO_ALT5   0b010

extern void GPIO_init(int pin, int mode);
extern void set_gpio(int pin, int value);
extern int get_gpio(int pin);

#endif