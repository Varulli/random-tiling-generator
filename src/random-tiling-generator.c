#include <string.h>
#include <stdint.h>
#include <time.h>
#include "init.h"

#define DIM 8
#define DIM_X 16
#define DIM_Y 8

#define BLANK 0		  // 0000 0000
#define DOT 16		  // 0001 0000
#define HORIZONTAL 5  // 0000 0101
#define VERTICAL 10	  // 0000 1010
#define top_QUAD 3	  // 0000 0011
#define mid_QUAD 6	  // 0000 0110
#define bot_QUAD 12	  // 0000 1100
#define FOURTH_QUAD 9 // 0000 1001

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

//        |
//  + -+- +
//        |
//  |  |
//  +--+ -+  +-
//        |  |

typedef struct tile
{
	int element;
	uint8_t type;
	int numChoices;
	uint8_t choices[8];
} tile;

tile array[DIM_X * DIM_Y];
tile *copy[DIM_X * DIM_Y];

void removeChoices(tile *t, uint8_t state, int dir)
{
	uint8_t arr[8] = {0};
	int l = 0;
	for (int i = 0; i < t->numChoices; i++)
	{
		if (state)
		{
			if (t->choices[i] & (1 << dir))
			{
				arr[l] = t->choices[i];
				l++;
			}
		}
		else
		{
			if (!(t->choices[i] & (1 << dir)))
			{
				arr[l] = t->choices[i];
				l++;
			}
		}
	}
	memcpy(t->choices, arr, sizeof(t->choices));
	t->numChoices = l;
}

void sort(int l, int h)
{
	if (l < h)
	{
		int i = l;
		for (int j = l; j < h; j++)
		{
			if (copy[j]->numChoices < copy[h]->numChoices)
			{
				tile *temp = copy[i];
				copy[i] = copy[j];
				copy[j] = temp;
				i++;
			}
		}
		tile *temp = copy[i];
		copy[i] = copy[h];
		copy[h] = temp;

		sort(l, i - 1);
		sort(i + 1, h);
	}
}

void printTileInfo(tile t)
{
	printf("%d, %d, { ", t.type, t.numChoices);
	for (int i = 0; i < t.numChoices - 1; i++)
	{
		printf("%d, ", t.choices[i]);
	}
	printf("%d }\n", t.choices[t.numChoices - 1]);
}

void printTiles()
{
	for (int i = 0; i < DIM_Y; i++)
	{
		char top[(DIM_X * 3) + 1] = "";
		char mid[(DIM_X * 3) + 1] = "";
		char bot[(DIM_X * 3) + 1] = "";
		for (int j = 0; j < DIM_X; j++)
		{
			switch (array[DIM_X * i + j].type)
			{
			case BLANK:
				strcat(top, "   ");
				strcat(mid, "   ");
				strcat(bot, "   ");
				break;
			case DOT:
				strcat(top, "   ");
				strcat(mid, " + ");
				strcat(bot, "   ");
				break;
			case HORIZONTAL:
				strcat(top, "   ");
				strcat(mid, "-+-");
				strcat(bot, "   ");
				break;
			case VERTICAL:
				strcat(top, " | ");
				strcat(mid, " + ");
				strcat(bot, " | ");
				break;
			case top_QUAD:
				strcat(top, " | ");
				strcat(mid, " +-");
				strcat(bot, "   ");
				break;
			case mid_QUAD:
				strcat(top, " | ");
				strcat(mid, "-+ ");
				strcat(bot, "   ");
				break;
			case bot_QUAD:
				strcat(top, "   ");
				strcat(mid, "-+ ");
				strcat(bot, " | ");
				break;
			case FOURTH_QUAD:
				strcat(top, "   ");
				strcat(mid, " +-");
				strcat(bot, " | ");
				break;
			}
		}
		printf("[%s]\n[%s]\n[%s]\n", top, mid, bot);
	}
}

int main()
{
	initialize();

	for (int i = 0; i < DIM_Y; i++)
	{
		for (int j = 0; j < DIM_X; j++)
		{
			tile newTile = {DIM_X * i + j, BLANK, 8, {BLANK, DOT, HORIZONTAL, VERTICAL, top_QUAD, mid_QUAD, bot_QUAD, FOURTH_QUAD}};
			array[DIM_X * i + j] = newTile;
			tile *pNewTile = &array[DIM_X * i + j];
			copy[DIM_X * i + j] = pNewTile;
		}
	}

	/* loop until all tiles numChoices == 0 . . .
	   section off lowest numChoices tile(s) in copy of array
		...
	   pick random tile from copy of array
		...
	   set chosen tile to random tile from choices[]
		...
	   update choices[], numChoices of all tiles
		...
	   sort copy of array by numChoices
		...
	   . . . end loop */

	// getchar();

	int timer = (int)time(NULL);
	srand(timer);

	clock_t t;

	printf("\x1b[?1049h\x1b[?25l");
	fflush(stdout);

	while (copy[DIM_X * DIM_Y - 1]->numChoices)
	{
		t = clock();

		int start = 0;
		for (start; start < DIM_X * DIM_Y; start++)
		{
			if (copy[start]->numChoices != 0)
				break;
		}
		int end = start + 1;
		for (end; end < DIM_X * DIM_Y; end++)
		{
			if (copy[start]->numChoices < copy[end]->numChoices)
				break;
		}
		tile *randTile = copy[start + (rand() % (end - start))];

		int randChoice = rand() % randTile->numChoices;
		randTile->type = randTile->choices[randChoice];
		randTile->numChoices = 0;

		if ((randTile->element + 1) % DIM_X != 0)
		{
			uint8_t state = randTile->type & (1 << RIGHT);
			removeChoices(&array[randTile->element + 1], state, LEFT);
		}
		if (randTile->element - DIM_X >= 0)
		{
			uint8_t state = randTile->type & (1 << UP);
			removeChoices(&array[randTile->element - DIM_X], state, DOWN);
		}
		if (randTile->element % DIM_X != 0)
		{
			uint8_t state = randTile->type & (1 << LEFT);
			removeChoices(&array[randTile->element - 1], state, RIGHT);
		}
		if (randTile->element + DIM_X < DIM_X * DIM_Y)
		{
			uint8_t state = randTile->type & (1 << DOWN);
			removeChoices(&array[randTile->element + DIM_X], state, UP);
		}

		sort(0, DIM_X * DIM_Y - 1);

		printf("\x1b[H");
		fflush(stdout);
		printTiles();

		while ((float)((clock() - t)) / CLOCKS_PER_SEC < 0.05)
			;
	}

	printf("Press Enter to Return");
	fflush(stdout);
	getchar();

	printf("\x1b[?25h\x1b[?1049l");
	fflush(stdout);

	return 0;
}