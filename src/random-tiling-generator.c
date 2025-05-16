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
#define FIRST_QUAD 3  // 0000 0011
#define SECOND_QUAD 6 // 0000 0110
#define THIRD_QUAD 12 // 0000 1100
#define FOURTH_QUAD 9 // 0000 1001

#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3

typedef struct tile
{
	int element;
	uint8_t type;
	int numChoices;
	uint8_t choices[8];
} tile;

tile array[DIM_X * DIM_Y];
tile *copy[DIM_X * DIM_Y];

char verticalSymbol;
char horizontalSymbol;
char centerSymbol;
char blankSymbol;

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
				sprintf(top + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				break;
			case DOT:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", blankSymbol, centerSymbol, blankSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				break;
			case HORIZONTAL:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", horizontalSymbol, centerSymbol, horizontalSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				break;
			case VERTICAL:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", blankSymbol, centerSymbol, blankSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				break;
			case FIRST_QUAD:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", blankSymbol, centerSymbol, horizontalSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				break;
			case SECOND_QUAD:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", horizontalSymbol, centerSymbol, blankSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				break;
			case THIRD_QUAD:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", horizontalSymbol, centerSymbol, blankSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				break;
			case FOURTH_QUAD:
				sprintf(top + j * 3, "%c%c%c", blankSymbol, blankSymbol, blankSymbol);
				sprintf(mid + j * 3, "%c%c%c", blankSymbol, centerSymbol, horizontalSymbol);
				sprintf(bot + j * 3, "%c%c%c", blankSymbol, verticalSymbol, blankSymbol);
				break;
			}
		}
		printf("[%s]\n[%s]\n[%s]\n", top, mid, bot);
	}
}

void clean_fgets(char *buffer, int max)
{
	fgets(buffer, max, stdin);
	if (!strchr(buffer, '\n'))
	{
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
			;
	}
}

int main()
{
	initialize();

	for (int i = 0; i < DIM_Y; i++)
	{
		for (int j = 0; j < DIM_X; j++)
		{
			tile newTile = {DIM_X * i + j, BLANK, 8, {BLANK, DOT, HORIZONTAL, VERTICAL, FIRST_QUAD, SECOND_QUAD, THIRD_QUAD, FOURTH_QUAD}};
			array[DIM_X * i + j] = newTile;
			tile *pNewTile = &array[DIM_X * i + j];
			copy[DIM_X * i + j] = pNewTile;
		}
	}

	char buffer[10];

	printf("\x1b[?1049h");
	fflush(stdout);

	printf("  Vertical symbol? (default: '|') -> ");
	clean_fgets(buffer, 10);
	verticalSymbol = buffer[0] == '\n' ? '|' : buffer[0];
	printf("Horizontal symbol? (default: '-') -> ");
	clean_fgets(buffer, 10);
	horizontalSymbol = buffer[0] == '\n' ? '-' : buffer[0];
	printf("    Center symbol? (default: '+') -> ");
	clean_fgets(buffer, 10);
	centerSymbol = buffer[0] == '\n' ? '+' : buffer[0];
	printf("     Blank symbol? (default: ' ') -> ");
	clean_fgets(buffer, 10);
	blankSymbol = buffer[0] == '\n' ? ' ' : buffer[0];

	printf("\x1b[?25l\x1b[H");
	fflush(stdout);

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

	printf("\nPress Enter to Return");
	fflush(stdout);
	getchar();

	printf("\x1b[?25h\x1b[?1049l");
	fflush(stdout);

	return 0;
}