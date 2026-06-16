
#include "system.h"

#include <stdint.h>


uint32_t buffer[WIDTH * HEIGHT]; // gets copied to diplay 60 times a second
uint32_t displayBuffer[WIDTH * HEIGHT]; // DISPLAY

uint8_t systemMemory[4096]; // MEMORY, 4096 8-bit blocks
// TODO: another magic number to get rid of

struct registerStruct cpuRegisters = {0x0};

struct stackStruct callStack = {0x0};

enum tempKeyboard
{
    k1,k2,k3,k4,kq,kw,ke,kr,ka,ks,kd,kf,kz,kx,kc,kv,
};
