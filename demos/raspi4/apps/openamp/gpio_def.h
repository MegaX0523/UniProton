#ifndef GPIO_DEF_H
#define GPIO_DEF_H

/*my set*/
// 外设基地址（树莓派4B）
#define BCM2711_PERI_BASE 0xFE000000
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000)
#define UART0_BASE (BCM2711_PERI_BASE + 0x201000)
#define SPI0_BASE (BCM2711_PERI_BASE + 0x204000)

#define GPFSEL0 *(volatile uint32_t *)(GPIO_BASE + 0x00)    // GPIO功能选择寄存器0
#define GPFSEL1 *(volatile uint32_t *)(GPIO_BASE + 0x04)    // GPIO功能选择寄存器1
#define GPFSEL2 *(volatile uint32_t *)(GPIO_BASE + 0x08)    // GPIO功能选择寄存器2
#define GPSET0 *(volatile uint32_t *)(GPIO_BASE + 0x1C)   // GPIO设置寄存器0
#define GPCLR0 *(volatile uint32_t *)(GPIO_BASE + 0x28)   // GPIO清除寄存器0

#define SPI0_CE1 7   // GPIO7
#define SPI0_CE0 8   // GPIO8
#define SPI0_MISO 9  // GPIO9
#define SPI0_MOSI 10 // GPIO10
#define SPI0_SCLK 11 // GPIO11
#define CONVST_PIN  17    // Conversion start pin
#define RESET_PIN   18    // Reset pin
#define BUSY_PIN    27    // Busy signal pin


// GPIO pin modes
#define GPIO_INPUT  0b000
#define GPIO_OUTPUT 0b001   // Output mode, default low
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