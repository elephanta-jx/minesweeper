// MineSweeper 1.0

#include <windows.h>


#ifndef BLOCK
#define BLOCK

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

#define BLANCK		0x0000
#define ONE		    0x0001
#define TWO		    0x0002
#define THREE		0x0003
#define FOUR		0x0004
#define FIVE		0x0005
#define SIX		    0x0006
#define SEVEN		0x0007
#define EIGHT		0x0008
#define MINE		0x0009
#define FACT		0x000F

#define CLICKED		0x0000
#define UNCLICK		0x0010
#define FLAG		0x0020
#define QUESTION	0x0030
#define COVER		0x00F0



#define X_AXIS		9
#define Y_AXIS		9
#define MINE_NUM	10
#define BLOCK_SIZE	40


extern UINT map[X_AXIS][Y_AXIS];							
extern TCHAR buf[10];									
extern UINT FCount;


void DrawGame(HDC hdc);								
void CalMine();
void CalcNum();
void DrawMap(HWND hwnd, HDC hdc);
void Initial(HWND hwnd);
bool success();
void expand(int ,int );

#endif