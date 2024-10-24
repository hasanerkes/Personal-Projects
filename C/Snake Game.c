#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

// Config

#define PlayableMapSizeX 10
#define PlayableMapSizeY 10

// Constants

#define MapSizeX PlayableMapSizeX + 2
#define MapSizeY PlayableMapSizeY + 2

#define BorderX MapSizeX - 1
#define BorderY MapSizeY - 1
#define MaxSnakeSize (PlayableMapSizeX * PlayableMapSizeY)

char up_key = 'w';
char down_key = 's';
char left_key = 'a';
char right_key = 'd';

int main() {
	struct SnakeParts {
		int PositionX;
		int PositionY;
		int Shape;
	};

	struct SnakeParts SnakeArray[MaxSnakeSize];

	int SnakeSize = 1;
	SnakeArray[0].PositionX = 1;
	SnakeArray[0].PositionY = 1;
	SnakeArray[0].Shape = 2;

	int NextDirection = 2;
	int PreviousDirection = 2;

	int Map[MapSizeY][MapSizeX]; /* -3 = [+], -2 = [-], -1 = [|], 0 = [ ], 1 = [<], 2 = [>], 3 = [^], 4 = [v], 5 = [*] */

	for (int y = 0; y < MapSizeY; y++) {
		for (int x = 0; x < MapSizeX; x++) {
			if ((y == 0 && x == 0) || (y == 0 && x == BorderX) || (y == BorderY && x == 0) || (y == BorderY && x == BorderX)) {
				Map[y][x] = -3;
			}
			else if ((y == 0) || (y == BorderY)) {
				Map[y][x] = -2;
			}
			else if ((x == 0) || (x == BorderX)) {
				Map[y][x] = -1;
			}
			else {
				Map[y][x] = 0;
			}
		}
	}

	Map[2][1] = 5; // First Food

	int run = 1;

	while (run == 1) {
		clock_t start = clock();

		char c;

		if (_kbhit()) {
			c = tolower(_getch());

			if (c == left_key) {
				if (PreviousDirection == 3 || PreviousDirection == 4) {
					NextDirection = 1;
				}
			}
			else if (c == right_key) {
				if (PreviousDirection == 3 || PreviousDirection == 4) {
					NextDirection = 2;
				}
			}
			else if (c == up_key) {
				if (PreviousDirection == 1 || PreviousDirection == 2) {
					NextDirection = 3;
				}
			}
			else if (c == down_key) {
				if (PreviousDirection == 1 || PreviousDirection == 2) {
					NextDirection = 4;
				}
			}
		}

		if (start % 1000 == 0) { // Tick
			int* HeadPositionX = &SnakeArray[0].PositionX;
			int* HeadPositionY = &SnakeArray[0].PositionY;

			int PreviousHeadPosX = *HeadPositionX;
			int PreviousHeadPosY = *HeadPositionY;
			int PreviousHeadShape = SnakeArray[0].Shape;
			int* PreviousTailPosX = &SnakeArray[SnakeSize].PositionX;
			int* PreviousTailPosY = &SnakeArray[SnakeSize].PositionY;

			// Move Head

			if (NextDirection == 1) {
				*HeadPositionX = *HeadPositionX - 1;
			}
			else if (NextDirection == 2) {
				*HeadPositionX = *HeadPositionX + 1;
			}
			else if (NextDirection == 3) {
				*HeadPositionY = *HeadPositionY - 1;
			}
			else {
				*HeadPositionY = *HeadPositionY + 1;
			}

			SnakeArray[0].Shape = NextDirection;
			Map[*HeadPositionY][*HeadPositionX] = SnakeArray[0].Shape;
			PreviousDirection = NextDirection;

			// Check State

			system("cls");

			if (*HeadPositionX < 0 || *HeadPositionX > MapSizeX - 1 || *HeadPositionY < 0 || *HeadPositionY > MapSizeY - 1) {
				printf("\nOut of Bounds");
				run = 0;
			}
			else {
				int* HeadValue = &Map[*HeadPositionY][*HeadPositionX];

				if ((*HeadValue == -2) || (*HeadValue == -1) || (*HeadValue == 1) || (*HeadValue == 2) || (*HeadValue == 3) || (*HeadValue == 4)) {
					printf("\nHit");
					run = 0; // Lose Condition
				}
				else {
					if (*HeadValue == 5) { // Food
						SnakeSize = SnakeSize + 1;

						if (SnakeSize == MaxSnakeSize) {
							printf("\nWin");
							run = 0; // Win Condition
						}
						else {
							// Create New // iterate map until random value 0
						}
					}
				}
			}

			// Move Other Parts

			for (int i = (SnakeSize - 1); i > 0; i--) {
				int* PosX = &SnakeArray[i].PositionX;
				int* PosY = &SnakeArray[i].PositionY;
				int* Shape = &SnakeArray[i].Shape;

				if (i == 1) {
					*PosX = PreviousHeadPosX;
					*PosY = PreviousHeadPosY;
					*Shape = PreviousHeadShape;
				}
				else {
					*PosX = SnakeArray[i - 1].PositionX;
					*PosY = SnakeArray[i - 1].PositionY;
					*Shape = SnakeArray[i - 1].Shape;
				}

				Map[*PosY][*PosX] = *Shape;
			}

			// Render

			printf("Snake Head Position: (%d, %d)\n", SnakeArray[0].PositionX, SnakeArray[0].PositionY);

			for (int y = 0; y < MapSizeY; y++) {
				for (int x = 0; x < MapSizeX; x++) {
					int value = Map[y][x];

					if (value == -3) {
						printf("[+]");
					}
					else if (value == -2) {
						printf("[-]");
					}
					else if (value == -1) {
						printf("[|]");
					}
					else if (value == 0) {
						printf("[ ]");
					}
					else if (value == 1) {
						printf("[<]");
					}
					else if (value == 2) {
						printf("[>]");
					}
					else if (value == 3) {
						printf("[^]");
					}
					else if (value == 4) {
						printf("[v]");
					}
					else {
						printf("[*]");
					}
				}

				printf("\n");
			}
		}
	}
}