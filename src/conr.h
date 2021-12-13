#ifndef CONR_H
#define CONR_H

#include <stdint.h>

typedef uint16_t uint16;
typedef int16_t int16;
typedef uint8_t uint8;
typedef int8_t int8;

#define KEY_NONE 0
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#ifdef __cplusplus
extern "C" 
{
#endif

uint8 InitConr(uint16 width, uint16 height);
uint8 StepConr();
void CloseConr();

float GetDelta();
float GetTick();

void Fill(char filler);
void Swap();

void SetChar(char c);

void Point(int16 x, int16 y);
void Line(int16 x0, int16 y0, int16 x1, int16 y1);
void Circle(int16 x, int16 y, int16 radius);

char GetKeyPress();

#ifdef __cplusplus
}
#endif

#endif