#include "spi.h"
#include "prt_hwi.h"

void spi0_init(void)
{
    // 配置GPIO为SPI功能
    GPFSEL0 &= ~((0x7 << (3 * 7)) | // GPI07 (CE1)
                 (0x7 << (3 * 8)) | // GPI08 (CE0)
                 (0x7 << (3 * 9))); // GPI09 (MISO)

    GPFSEL0 |= (0x4 << (3 * 7)) | // GPI07 ALT0 (CE1)
               (0x4 << (3 * 8)) | // GPI08 ALT0 (CE0)
               (0x4 << (3 * 9));  // GPI09 ALT0 (MISO)

    GPFSEL1 &= ~((0x7 << (3 * 0)) | // GPI10 (MOSI)
                 (0x7 << (3 * 1))); // GPI11 (SILK)

    GPFSEL1 |= (0x4 << (3 * 0)) | // GPI10 ALT0 (MOSI)
               (0x4 << (3 * 1));  // GPI11 ALT0 (SILK)

    // 复位SPI控制器
    SPI0->CS = 0;
    SPI0->CLK = 0;

    // 设置时钟分频（APB时钟250MHz，分频为250/125=2MHz）
    SPI0->CLK = 200000000 / 2000000;

    SPI0->CS |= (SPI_CS_CPOL0 | SPI_CS_CPHA0); // 设置时钟极性和相位

    // 清除FIFO
    SPI0->CS |= SPI_CS_CLEAR;
}

void spi0_transfer(uint8_t CS, uint8_t *tx_data, uint8_t *rx_data, uint32_t len)
{
// 设置数据长度,仅DMA模式下有效
    // SPI0->DLEN = len;
    uintptr_t intSave;
    intSave = PRT_HwiLock();

    if (CS == SPI0_CE0)
    {
        SPI0->CS &= ~SPI_CS_CS1;
        SPI0->CS |= SPI_CS_CS0;
    }
    else if(CS == SPI0_CE1)
    {
        SPI0->CS &= ~SPI_CS_CS0;
        SPI0->CS |= SPI_CS_CS1;
    }

    SPI0->CS |= SPI_CS_TA;
    SPI0->FIFO = 0x66;

    for (uint32_t i = 0; i < len; i++)
    {

        // 等待发送FIFO有空间
        while ((SPI0->CS & SPI_CS_TXD) == 0)
        {
            ;
        }

        // 写入发送数据
        // SPI0->FIFO = tx_data[i];
        SPI0->FIFO = 0x55;
        
        // 等待接收FIFO有数据
        //while((SPI0->CS & SPI_CS_RXD) != 0);

        // 读取接收数据
        rx_data[i] = SPI0->FIFO;

        // 等待传输完成
        while (!(SPI0->CS & SPI_CS_DONE));

        SPI0->CS |= SPI_CS_CLEAR;
    }

    // 清除TA标志
    SPI0->CS &= ~SPI_CS_TA;
    // 清除FIFO
    SPI0->CS |= SPI_CS_CLEAR;

    PRT_HwiRestore(intSave);
}

// 将接收到的字节数据转换为浮点数
float rxdata2float(uint8_t *rx_data)
{
    float result = 0.0f;
    uint32_t temp = (rx_data[0] << 24) | (rx_data[1] << 16) | (rx_data[2] << 8) | rx_data[3];
    result = *(float *)&temp;
    return result;
}

void float2txdata(float input, uint8_t *tx_data)
{
    tx_data[0] = (*(uint32_t *)&input << 24) & 0xFF;
    tx_data[1] = (*(uint32_t *)&input << 16) & 0xFF;
    tx_data[2] = (*(uint32_t *)&input << 8) & 0xFF;
    tx_data[3] = (*(uint32_t *)&input << 0) & 0xFF;
}

// 简单单字节传输函数
uint8_t SPI0_TransferByte(uint8_t data)
{
    // 等待TX FIFO有空间
    while (!(SPI0->CS & SPI_CS_TXD))
        ;
    // 发送数据
    SPI0->FIFO = data;
    // 启动传输
    SPI0->CS |= SPI_CS_TA;
    // 等待接收数据
    // while (!(SPI0->CS & SPI_CS_RXD))
    //     ;
    // 读取接收到的数据
    for(int i = 0; i < 100; i++)
    {}
    (void)SPI0->FIFO;;
    // 等待传输完成
    while (!(SPI0->CS & SPI_CS_DONE))
        ;
    // 关闭传输
    SPI0->CS &= ~SPI_CS_TA;
    return 0;
}

// 示例使用
void spi0_test()
{
    static uint8_t sendnum = 0;
    uint8_t tx_data[6] = {sendnum, sendnum + 1, sendnum + 2, sendnum + 3, sendnum + 4, sendnum + 5};
    sendnum += 6;
    if (sendnum > 0xF0)
    {
        sendnum = 0;
    }
    uint8_t rx_data[6] = {0};

    spi0_transfer(SPI0_CE0, tx_data, rx_data, sizeof(tx_data));
}