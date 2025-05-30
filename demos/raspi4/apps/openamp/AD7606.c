#include <stdint.h>
#include "gpio_def.h"

// Define GPIO pins (arbitrary assignments)
#define CS_PIN      0     // Chip select pin (for SPI)

// Initialize AD7606
void AD7606_Init(void)
{
    // Initialize control pins as outputs
    set_gpio(CONVST_PIN, 1);  // Set CONVST high initially
    set_gpio(RESET_PIN, 1);   // Set RESET high initially
    
    // Perform reset sequence
    set_gpio(RESET_PIN, 0);   // Reset low
    // Small delay could be added here if needed
    set_gpio(RESET_PIN, 1);   // Reset high
}

// Start conversion
void AD7606_StartConversion(void)
{
    set_gpio(CONVST_PIN, 0);  // CONVST low to start conversion
    // Small delay could be added here if needed
    set_gpio(CONVST_PIN, 1);  // CONVST back to high
}

// Read data from AD7606 (reads one channel)
int16_t AD7606_ReadChannel(uint8_t channel)
{
    int16_t result;
    uint8_t msb, lsb;
    
    if (channel > 7) return 0;  // AD7606 has 8 channels (0-7)
    
    // Read MSB and LSB
    msb = spi0_send_byte(CS_PIN, 0xFF);  // Send dummy byte to receive MSB
    lsb = spi0_send_byte(CS_PIN, 0xFF);  // Send dummy byte to receive LSB
    
    // Combine MSB and LSB to form 16-bit result
    result = (msb << 8) | lsb;
    
    return result;
}

// Read all channels at once
void AD7606_ReadAllChannels(int16_t* data)
{
    int i;
    
    // Start conversion
    AD7606_StartConversion();
    
    // Wait for BUSY to go high then low
    // Note: In actual implementation, you might need to implement a proper way to read BUSY pin
    
    // Read all 8 channels
    for (i = 0; i < 8; i++) {
        data[i] = AD7606_ReadChannel(i);
    }
}