// MineSweeper 1.0

#include <windows.h>
#include "mine.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				

UINT map[X_AXIS][Y_AXIS];							
TCHAR buf[10];									
UINT FCount;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,		
		   LPSTR lpCmdLine, int iCmdShow)

{
	int a = sizeof (UINT);
	static TCHAR szAppName[] = TEXT("MineSweeper 1.0");				
	HWND hwnd;								
	MSG msg;								
	WNDCLASS wndclass;							

/**************************** Window class attributes ****************************/

	wndclass.style		= CS_HREDRAW | CS_VREDRAW;			
	wndclass.lpfnWndProc	= WndProc;					
	wndclass.cbClsExtra	= 0;						
	wndclass.cbWndExtra	= 0;						
	wndclass.hInstance	= hInstance;					
	wndclass.hIcon		= LoadIcon(hInstance, IDI_APPLICATION);		
	wndclass.hCursor	= LoadCursor(hInstance, IDC_ARROW);		
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);		
	wndclass.lpszMenuName	= NULL;						
	wndclass.lpszClassName	= szAppName;					

	
	if (! RegisterClass(&wndclass) )					
	{
		MessageBox(NULL, TEXT("THIS PROGRAM REQUIES WINDOWS NT!"), 
			szAppName, MB_ICONERROR);

		return 0;
	}
	
/**************************** Create window ****************************/

	hwnd = CreateWindow( szAppName,						
			     TEXT ("MineSweeper 1.0"),					
			     WS_OVERLAPPEDWINDOW^WS_THICKFRAME,			
			     CW_USEDEFAULT,					
			     CW_USEDEFAULT,					
			     380,						
			     450,						
			     NULL,						
			     NULL,						
			     hInstance,						
			     NULL);						

	ShowWindow (hwnd, iCmdShow);						
	UpdateWindow(hwnd);							

/*************************** Message Loop ***************************/

	while (GetMessage (&msg, NULL, 0, 0))										
	{
		TranslateMessage(&msg);						
		DispatchMessage(&msg);						

	}
	return msg.wParam;

}

/**************************** Window Proc ****************************/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC		hdc ;							
	PAINTSTRUCT	ps ;							
	int x,y;
	

	switch( message)
	{

	case WM_CREATE:									
		Initial(hwnd);
		return 0;

	case WM_PAINT:								
		hdc = BeginPaint(hwnd, &ps);					
		DrawGame(hdc);
		DrawMap(hwnd, hdc);
		EndPaint (hwnd, &ps);						
		return 0;

	case WM_LBUTTONUP:										// left button up
		x = LOWORD (lParam) / BLOCK_SIZE;
		y = HIWORD (lParam) / BLOCK_SIZE;
		
		if ( x >= X_AXIS || x < 0 || y >= Y_AXIS || y < 0)
			return 1;
		switch (map[x][y] & COVER)
		{
			case FLAG:
				map[x][y] = map[x][y] ^ FLAG | UNCLICK;
				FCount++;
				break;

			case QUESTION:
				map[x][y] = map[x][y] ^ FLAG | UNCLICK;
				break;

			case UNCLICK:
				map[x][y] = map[x][y] ^ UNCLICK;
				if (map[x][y] == BLANCK)
					expand(x, y);
				break;
			
			default:
				break;
		
		}
		
		InvalidateRect(hwnd, NULL, TRUE);

		if (map[x][y] == MINE)
		{
			if (IDYES == MessageBox(hwnd, TEXT("Failure! Restart?"), TEXT("Failure!"), MB_YESNO) )
			{
				Initial(hwnd);
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);
			}
			else
				SendMessage (hwnd, WM_DESTROY, wParam, lParam);
		}	
		return 0;

	case WM_RBUTTONUP:			
		//鼠标右键抬起
		x = LOWORD (lParam) / BLOCK_SIZE;
		y = HIWORD (lParam) / BLOCK_SIZE;

		if ( x >= X_AXIS || x < 0 || y >= Y_AXIS || y < 0)
			return 1;

		switch (map[x][y] & COVER)
		{
			case UNCLICK:
				map[x][y] = map[x][y] ^ UNCLICK | FLAG;
				FCount--;
				InvalidateRect(hwnd, NULL, TRUE);
				UpdateWindow(hwnd);

				if (FCount == 0)
				{
					if (success() == true)
					{
						if (IDYES == MessageBox(hwnd, TEXT("Congratulations"), TEXT("Succeed!"), MB_YESNO))
						{
							Initial(hwnd);	
							InvalidateRect(hwnd, NULL, TRUE);
							UpdateWindow(hwnd);
						}
						else 
						{
							SendMessage(hwnd, WM_DESTROY, wParam, lParam);
						}
					}
					
					else
					{
						MessageBox(hwnd, TEXT("Wrong flag! Try again!"), TEXT("Failure!"), MB_OK);
					}
					
				}
				break;

			case FLAG:
				map[x][y] = map[x][y] ^ FLAG | UNCLICK;
				FCount++;
				break;

			case QUESTION:
				break;

			default:
				break;
		}
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		
		return 0;

	case WM_DESTROY:							
		PostQuitMessage(0);						
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);

}