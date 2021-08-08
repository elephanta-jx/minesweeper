// MineSweeper 1.0

#include <windows.h>
#include "mine.h"

void DrawGame(HDC hdc)								
{
	int x,y;
	for (x = 0; x < X_AXIS; x++)
	{
		for (y = 0; y < Y_AXIS; y++)
		{
			Rectangle(hdc, x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y+1)* BLOCK_SIZE);
		}
	}

}

/* 
	randomly populate the positions of mines
*/
void CalMine()
{
	int i = MINE_NUM;
	int iPos;
	int x,y;
	
	srand( GetTickCount() );
	while (i)
	{
		iPos = rand() % (X_AXIS * Y_AXIS);
		y = iPos / Y_AXIS;
		x = iPos % X_AXIS;

		if ( (map[x][y] & FACT) == BLANCK)
		{
			map[x][y] = MINE | UNCLICK;
			i--;
		}

		else
		{
			continue;
		}
		
	}
}


/* 
	generating the surrounding numbers
*/

void CalcNum()
{
	int x,y;

	for (x = 0; x < X_AXIS; x++)
	{
		for (y = 0; y < Y_AXIS; y++)
		{
			if ( (map[x][y] & FACT) == MINE)
			{	
				if (x > 0 && (map[x-1][y] & FACT) != MINE)					
					map[x-1][y]++;
				if (y > 0 && (map[x][y-1] & FACT) != MINE)					
					map[x][y-1]++;
				if (x < X_AXIS -1 && (map[x+1][y] & FACT) != MINE)				
					map[x+1][y]++;
				if (y < Y_AXIS -1 && (map[x][y+1] & FACT) != MINE)				
					map[x][y+1]++;

				if (x > 0 && y > 0 && (map[x-1][y-1] & FACT) != MINE)			
					map[x-1][y-1]++;
				if (x > 0 && y < Y_AXIS - 1 && (map[x-1][y+1] & FACT) != MINE)		
					map[x-1][y+1]++;
				if (x < X_AXIS - 1 && y > 0 && (map[x+1][y-1] & FACT) != MINE)		
					map[x+1][y-1]++;
				if (x < X_AXIS - 1 && y < Y_AXIS - 1 && (map[x+1][y+1] & FACT) != MINE)	
					map[x+1][y+1]++;
			}
		}
	}
}

void Initial(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	int x,y;
	
	for (x = 0; x < X_AXIS; x++)
	{
		for (y = 0; y < Y_AXIS; y++)
		{
			map[x][y] = UNCLICK | BLANCK;
		}
	}
	FCount = MINE_NUM;
	CalMine();
	CalcNum();
}


bool success()
{
	int x,y;
	
	for (x = 0; x < X_AXIS; x++)
	{
		for (y = 0; y < Y_AXIS; y++)
		{
			if ((map[x][y] & COVER) == FLAG)
			{
				if ( (map[x][y] & FACT) == MINE )
				{
					continue;
				}
				else 
					return false;
			}
		}
	}
	return true;
}

/*
	expand empty spaces 
*/
void expand(int x, int y)	
{
	if ( x+1 < X_AXIS && (map[x+1][y] & FACT) == BLANCK && (map[x+1][y] & COVER) == UNCLICK)
	{
		map[x+1][y] = BLANCK;
		expand(x+1, y);
	}
	else if ( x+1 < X_AXIS && (map[x+1][y] & FACT) >= ONE && (map[x+1][y] & FACT) <= EIGHT)
	{
		map[x+1][y] = map[x+1][y] & FACT;
	}

	if ( x-1 >= 0 && (map[x-1][y] & FACT) == BLANCK && (map[x-1][y] & COVER) == UNCLICK)
	{
		map[x-1][y] = BLANCK;
		expand(x-1, y);
	}
	else if ( x-1 >= 0 && (map[x-1][y] & FACT) >= ONE && (map[x-1][y] & FACT) <= EIGHT)
	{
		map[x-1][y] = map[x-1][y] & FACT;
	}

		
	if ( y+1 < Y_AXIS && (map[x][y+1] & FACT) == BLANCK && (map[x][y+1] & COVER) == UNCLICK)
	{
		map[x][y+1] = BLANCK;
		expand(x, y+1);
	}
	else if ( y+1 < Y_AXIS && (map[x][y+1] & FACT) >= ONE && (map[x][y+1] & FACT) <= EIGHT)
	{
		map[x][y+1] = map[x][y+1] & FACT;
	}
		
	if ( y - 1 >= 0 && (map[x][y-1] & FACT) == BLANCK && (map[x][y-1] & COVER) == UNCLICK)
	{
		map[x][y-1] = BLANCK;
		expand(x, y-1);
	}
	else if ( y - 1 >= 0 && (map[x][y-1] & FACT) >= ONE && (map[x][y-1] & FACT) <= EIGHT)
	{
		map[x][y-1] = map[x][y-1] & FACT;
	}
		
	

	if ( x + 1 < X_AXIS && y +1 < Y_AXIS && (map[x+1][y+1] & FACT) >= ONE && (map[x+1][y+1] & FACT) <= EIGHT)
	{
		map[x+1][y+1] = map[x+1][y+1] & FACT;
	}
		
	if ( x + 1 < X_AXIS && y -1 >= 0 && (map[x+1][y-1] & FACT) >= ONE && (map[x+1][y-1] & FACT) <= EIGHT)
	{
		map[x+1][y-1] = map[x+1][y-1] & FACT;
	}
		
	if ( x - 1 >= 0 && y + 1 < Y_AXIS && (map[x-1][y+1] & FACT) >= ONE && (map[x-1][y+1] & FACT) <= EIGHT)
	{
		map[x-1][y+1] = map[x-1][y+1] & FACT;
	}
		
	if ( x - 1 >= 0 && y - 1 >= 0 && (map[x-1][y-1] & FACT) >= ONE && (map[x-1][y-1] & FACT) <= EIGHT)
	{
		map[x-1][y-1] = map[x-1][y-1] & FACT;	
	}
		

}

void DrawMap(HWND hwnd, HDC hdc)
{
	int x, y;
	RECT rect;

	for (x = 0; x < X_AXIS; x++)
	{
		for (y = 0; y < Y_AXIS; y++)
		{
			switch (map[x][y] & COVER) 
			{
				case UNCLICK:
					SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
					Rectangle (hdc, x * BLOCK_SIZE, y * BLOCK_SIZE, 
						(x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
					break;

				case FLAG:
					SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
					Rectangle (hdc, x * BLOCK_SIZE + 10, y * BLOCK_SIZE + 10, 
						(x + 1) * BLOCK_SIZE - 10, (y + 1) * BLOCK_SIZE - 10);
					break;

				case QUESTION:
					break;

				case CLICKED:

					switch (map[x][y])
					{
						case MINE:
							SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
							Ellipse(hdc, x * BLOCK_SIZE, y * BLOCK_SIZE, 
								(x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
							break;

						case BLANCK:
							SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
							Rectangle (hdc, x * BLOCK_SIZE, y * BLOCK_SIZE, 
								(x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
							break;
						default:
							if (map[x][y] >= ONE && map[x][y] <= EIGHT) 
							{
								rect.top = y * BLOCK_SIZE;
								rect.bottom = (y + 1) * BLOCK_SIZE;
								rect.left = x * BLOCK_SIZE;
								rect.right = (x + 1) * BLOCK_SIZE;

								wsprintf(buf, TEXT("%d"), map[x][y]);

								DrawText (hdc, buf, -1, &rect, 
								DT_SINGLELINE | DT_CENTER | DT_VCENTER);
							}

							break;
					}
					break;

				default:
					MessageBox(hwnd, TEXT("Unknown block"), TEXT("Error"), MB_OK);
					break;
				
			}

		}
	}
}

