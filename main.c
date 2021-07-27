
#include "mcc_generated_files/mcc.h"
uint8_t FSRVal;
volatile int pattern = 0;

void playfreq1(void) {
    EnableWG_SetLow();
    SPI1_WriteByte(0b00100001); //reset ad9833 and prepare to enter word
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b01010000); //write frequency 1 LSB first
    SPI1_WriteByte(0b11000111);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b01000000); //write frequency 2 MSB second
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b11000000); //write phase register to zero
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b00100000); //exit reset
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
}

void playfreq2(void) {
    EnableWG_SetLow();
    SPI1_WriteByte(0b00100001); //reset ad9833 and prepare to enter word
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b01010000); //write frequency 1 LSB first
    SPI1_WriteByte(0b11001011);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b01000000); //write frequency 2 MSB second
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b11000000); //write phase register to zero
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
    __delay_ms(0.05);
    EnableWG_SetLow();
    SPI1_WriteByte(0b00100000); //exit reset
    SPI1_WriteByte(0b00000000);
    __delay_ms(0.05);
    EnableWG_SetHigh();
}

float ADCRead(void) {
    FSR_SetHigh();
    EnableADC_SetLow();
    uint8_t first = SPI1_ReadByte();  //first 8 bits of data from ADC
    uint8_t second = SPI1_ReadByte();  //last 8 bits of data from ADC
    EnableADC_SetHigh();
    FSR_SetLow();
    uint8_t reorient1 = (first & 0x0F) << 4 | (first & 0xF0) >> 4;  //replace first 4 bits with last 4 bits in data
    uint8_t reorient2 = (second & 0x0F) << 4 | (second & 0xF0) >> 4;  //replace first 4 bits with last 4 bits in data
    uint8_t data = reorient1 | reorient2;  //determine the 8 bits of data sent by the ADC
    return data;
}

void main(void) {

    SYSTEM_Initialize();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    TMR2_StartTimer();
    while (1) {
        FSRVal = ADCRead(); //read ADC through SPI and store value
        if (FSRVal < 0b00001010) {
            Bridge_SetLow();
        } else if (pattern == 0 && FSRVal > 0b00001010) {
            Bridge_SetHigh();
            EUSART_Write('1');
            LED_SetHigh();
            playfreq1();
        }
        else if (pattern == 1 && FSRVal > 0b00001010) {
            Bridge_SetHigh();
            EUSART_Write('1');
            LED_SetLow();
            playfreq2();
        }
    }
}
