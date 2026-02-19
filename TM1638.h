#ifndef TM1638_H
#define TM1638_H

#include <Arduino.h>

class TM1638 {
public:
    TM1638(uint8_t stb, uint8_t clk, uint8_t dio);

    void init();
    void setBrightness(uint8_t brightness);
    void displayDigit(uint8_t position, uint8_t data);
    void displayNumber(uint32_t number);
    void clearDisplay();
    uint8_t readKeys();

private:
    void sendCommand(uint8_t command);
    void sendData(uint8_t address, uint8_t data);
    void sendByte(uint8_t data);
    uint8_t receiveByte();

    uint8_t _stb;
    uint8_t _clk;
    uint8_t _dio;

    uint8_t digit[8];

    const uint8_t digitToSegment[10] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F  // 9
    };
};

#endif
