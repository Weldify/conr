# conr
conr provides simple C/C++ rendering and input functions for the Windows console.
___
# API
```c
typedef uint16_t uint16;
typedef int16_t int16;
typedef uint8_t uint8;
typedef int8_t int8;

#define KEY_NONE 0
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define CHAR_NONE 0

uint8 InitConr(uint16 width, uint16 height); // initiates conr. must be called before using any functions.
uint8 StepConr(); // updates delta time and tick
void CloseConr(); // closes conr

float GetDelta(); // returns delta time
float GetTick(); // returns elapsed time since conr startup

void Fill(char filler); // fills the entire screen with a specific character
void Swap(); // updates the screen

void SetChar(char c); // sets the character for all future draw calls
void SetColorKey(char c); // sets the colorkey for all future draw calls. this character will be ignored when drawing strings

void Point(int16 x, int16 y); // draws a point
void Line(int16 x0, int16 y0, int16 x1, int16 y1); // draws a line
void Rect(int16 x, int16 y, int16 w, int16 h); // draws a rectangle outline
void Circle(int16 x, int16 y, int16 radius); // draws a circle outline
void String(int16 x, int16 y, const char* chars); // draws a string, respects newlines

char GetKeyPress(); // returns the key that was just pressed, if any. otherwise, this function returns KEY_NONE
```
___
# Example
```c
#include "conr.h"

int main()
{
	InitConr(128, 128);
	SetColorKey(' ');

	uint8 isVisible = 1;
	while (StepConr())
	{
		char c = GetKeyPress();
		switch (c)
		{
		case KEY_W:
			isVisible = !isVisible;
			break;
		}

		Fill('.');

		if (isVisible)
		{
			String(64, 64,
				"  ###  \n"
				"  # #  \n"
				"  ###  \n"
				"   #   \n"
				"#######\n"
				"   #   \n"
				"   #   \n"
				"  # #  \n"
				"  # #  \n"
				"  # #  "
			);
		}
		
		Swap();
	}

	CloseConr();

	return 0;
}
```
