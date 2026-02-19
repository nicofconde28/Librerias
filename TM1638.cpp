#include "TM1638.h"

TM1638::TM1638(uint8_t stb, uint8_t clk, uint8_t dio)
{
    _stb = stb;
    _clk = clk;
    _dio = dio;
}

void TM1638::init()
{
    pinMode(_stb, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);

    digitalWrite(_stb, HIGH);
    digitalWrite(_clk, LOW);

    sendCommand(0x8F); // Display ON, brillo máximo
}

void TM1638::setBrightness(uint8_t brightness)
{
    if (brightness > 7) brightness = 7;
    sendCommand(0x88 | brightness);
}

void TM1638::displayDigit(uint8_t position, uint8_t data)
{
    sendData(position << 1, data);
}

void TM1638::displayNumber(uint32_t number)
{
    for (int i = 0; i < 8; i++)
    {
        digit[i] = number % 10;
        number /= 10;
    }

    for (int i = 0; i < 8; i++)
    {
        displayDigit(i, digitToSegment[digit[7 - i]]);
    }
}

void TM1638::clearDisplay()
{
    for (int i = 0; i < 16; i++)
    {
        sendData(i, 0x00);
    }
}

uint8_t TM1638::readKeys()
{
    uint8_t keys = 0;

    digitalWrite(_stb, LOW);
    sendByte(0x42);

    pinMode(_dio, INPUT);

    for (int i = 0; i < 4; i++)
    {
        uint8_t data = receiveByte();
        keys |= (data << i);
    }

    pinMode(_dio, OUTPUT);
    digitalWrite(_stb, HIGH);

    return keys;
}

void TM1638::sendCommand(uint8_t command)
{
    digitalWrite(_stb, LOW);
    sendByte(command);
    digitalWrite(_stb, HIGH);
}

void TM1638::sendData(uint8_t address, uint8_t data)
{
    sendCommand(0x44); // Modo escritura fija

    digitalWrite(_stb, LOW);
    sendByte(0xC0 | address);
    sendByte(data);
    digitalWrite(_stb, HIGH);
}

void TM1638::sendByte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_clk, LOW);
        digitalWrite(_dio, data & 0x01);
        delayMicroseconds(1);
        digitalWrite(_clk, HIGH);
        delayMicroseconds(1);
        data >>= 1;
    }
}

uint8_t TM1638::receiveByte()
{
    uint8_t data = 0;

    for (int i = 0; i < 8; i++)
    {
        digitalWrite(_clk, LOW);
        delayMicroseconds(1);

        if (digitalRead(_dio))
            data |= (1 << i);

        digitalWrite(_clk, HIGH);
        delayMicroseconds(1);
    }

    return data;
}
