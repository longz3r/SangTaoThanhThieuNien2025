#include <main.h>

const unsigned char tick[] PROGMEM = {
    0b00000000,
    0b00000001,
    0b00000010,
    0b00000100,
    0b10001000,
    0b01010000,
    0b00100000,
    0b00000000
};
  
const unsigned char cross[] PROGMEM = {
    0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
};

const unsigned char degree[] PROGMEM = {
  0b00111000,
  0b00101000,
  0b00111000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};