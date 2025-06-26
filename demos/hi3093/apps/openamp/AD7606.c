#include <stdint.h>
#include "gpio_def.h"
#include "hi_spi.h"
#include "bm_gpio.h"

void AD7606_Init(void);
static void AD7606_StartConversion(void);
int8_t AD7606_ReadAllChannels(uint8_t* data);

// Initialize AD7606
void AD7606_Init(void)
{
    // Initialize GPIO pins for AD7606
    GPIO_INIT(GPIO_GROUP1, AD7606_CONVST_PIN, GPIO_OUTPUT);  // Initialize CONVST pin as output
    GPIO_INIT(GPIO_GROUP1, AD7606_RESET_PIN, GPIO_OUTPUT);   // Initialize RESET pin as output
    GPIO_INIT(GPIO_GROUP1,AD7606_BUSY_PIN, GPIO_INPUT);     // Initialize BUSY pin as input (optional, if needed)
    
    // // Perform reset sequence
    GPIO_SET_PIN(AD7606_RESET_PIN);   // Reset high
    // // Small delay could be added here if needed
    // for (volatile int i = 0; i < 100; i++);  // Simple delay loop (adjust as needed)
    GPIO_CLEAR_PIN(AD7606_RESET_PIN);   // Reset low
}

// Start conversion
static void AD7606_StartConversion(void)
{
    GPIO_CLEAR_PIN(AD7606_CONVST_PIN);  // CONVST low to start conversion
    // // Small delay could be added here if needed
    for (volatile int i = 0; i < 50; i++);  // Simple delay loop (adjust as needed)
    GPIO_SET_PIN(AD7606_CONVST_PIN);  // CONVST back to high
}

// Read all channels at once
int8_t AD7606_ReadAllChannels(uint8_t* data)
{
    int timeout = 1000;  // Timeout for BUSY signal (in case it doesn't go low)
    
    // Start conversion
    AD7606_StartConversion();
    
    // Wait for BUSY to go high then low
    // Note: In actual implementation, you might need to implement a proper way to read BUSY pin
    for (volatile int j = 0; j < 20; j++);
    // while (GPIO_GETVALUE(AD7606_BUSY_PIN) == GPIO_LEVEL_HIGH)  // Wait until BUSY goes low
    // {
    //     if (timeout-- == 0) {
    //         // Handle timeout error (e.g., log error, return, etc.)
    //         return -1;  // Indicate error
    //     }
    // }
    
    // Read 2 channels
    spi0_AD_receive(data, 4);  // Read 4 bytes (2 channels, 2 bytes each)

    return 0;  // Indicate success
}