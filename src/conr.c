#include "conr.h"
#include <Windows.h>
#include <stdint.h>
#include <conio.h>
#include <time.h>
#include <string.h>

static uint16 width, height, dWidth, dHeight, bufSize;
static char curChar;
static wchar_t* buf;

static HANDLE hConsole;
static DWORD bytesWritten;
static COORD writeCoord;

static clock_t lastTime;
static float delta;
static float tick;
static char colorKey;

uint8 InitConr(uint16 w, uint16 h)
{
	width = w;
	height = h;
	dWidth = w - 1;
	dHeight = h - 1;
	bufSize = w * h;

	buf = malloc((bufSize + 1) * sizeof(wchar_t));
	if (buf == NULL) return 0;

	Fill(' ');
	buf[bufSize] = '\0';

	COORD coord;
	coord.X = width;
	coord.Y = height;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = height - 1;
	rect.Right = width - 1;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(handle, coord);
	SetConsoleWindowInfo(handle, TRUE, &rect);

	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

	bytesWritten = 0;

	writeCoord.X = 0;
	writeCoord.Y = 0;

	lastTime = clock();
	delta = .0f;
	tick = .0f;
	colorKey = ' ';

	return 1;
}

uint8 StepConr()
{
	clock_t nowTime = clock();
	delta = (float)(nowTime - lastTime) / CLOCKS_PER_SEC;
	lastTime = nowTime;
	tick += delta;

	return 1;
}

void CloseConr()
{
	free(buf);
}

float GetDelta()
{
	return delta;
}

float GetTick()
{
	return tick;
}

void Fill(char filler)
{
	for (uint16 i = 0; i < bufSize; i++)
	{
		buf[i] = filler;
	}
}

void Swap()
{
	WriteConsoleOutputCharacter(hConsole, buf, bufSize, writeCoord, &bytesWritten);
}

void SetChar(char c)
{
	curChar = c;
}

void SetColorKey(char c)
{
	colorKey = c;
}

void Point(int16 x, int16 y)
{
	if (x < 0 || x > width || y < 0 || y > width) return;

	buf[x + y * width] = curChar;
}

static void PointC(int16 x, int16 y, char c)
{
	if (x < 0 || x > width || y < 0 || y > width) return;

	buf[x + y * width] = c;
}

void Line(int16 x0, int16 y0, int16 x1, int16 y1)
{
	int16 stepx, stepy;
	int16 dx = x1 - x0, dy = y1 - y0;

	if (dy < 0) { dy = -dy; stepy = -1; }
	else stepy = 1;

	if (dx < 0) { dx = -dx; stepx = -1; }
	else stepx = 1;

	dy <<= 1;
	dx <<= 1;

	Point(x0, y0);

	if (dx > dy) 
	{
		int16 fraction = dy - (dx >> 1);
		while (x0 != x1) 
		{
			x0 += stepx;

			if (fraction >= 0) 
			{
				y0 += stepy;
				fraction -= dx;
			}

			fraction += dy;
			Point(x0, y0);
		}

	}
	else 
	{
		int16 fraction = dx - (dy >> 1);
		while (y0 != y1) 
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}

			y0 += stepy;
			fraction += dx;
			Point(x0, y0);
		}
	}
}

void Rect(int16 ox, int16 oy, int16 w, int16 h)
{
	int8 xSide = w > 0 ? 1 : -1;
	int8 ySide = h > 0 ? 1 : -1;

	for (int16 x = ox; x <= ox + abs(w); x += xSide)
	{
		Point(x, oy);
		Point(x, oy + h);
	}

	for (int16 y = ox; y <= ox + abs(h); y += ySide)
	{
		Point(ox, y);
		Point(ox + w, y);
	}
}

static void BresenhamSubCircle(int16 xc, int16 yc, int16 x, int16 y)
{
	Point(xc + x, yc + y);
	Point(xc - x, yc + y);
	Point(xc + x, yc - y);
	Point(xc - x, yc - y);
	Point(xc + y, yc + x);
	Point(xc - y, yc + x);
	Point(xc + y, yc - x);
	Point(xc - y, yc - x);
}

void Circle(int16 x, int16 y, int16 r)
{
	r = abs(r);

	int16 xc = 0, yc = r;
	int16 d = 3 - 2 * r;

	BresenhamSubCircle(x, y, xc, yc);
	while (yc >= xc)
	{
		xc++;

		if (d > 0)
		{
			yc--;
			d = d + 4 * (xc - yc) + 10;
		}
		else d = d + 4 * xc + 6;

		BresenhamSubCircle(x, y, xc, yc);
	}
}

void String(int16 x, int16 y, const char* str)
{
	size_t len = strlen(str);

	int16 curX = x;
	for (int i = 0; i < len; i++)
	{
		char c = str[i];
		if (c == '\n')
		{
			y++;
			curX = x;
			continue;
		}

		if (c != colorKey)
			PointC(curX, y, c);

		curX++;
	}
}

char GetKeyPress()
{
	if (_kbhit())
	{
		return _getch();
	}
	else return KEY_NONE;
}