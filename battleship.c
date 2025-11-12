

/* battleship - BATTLESHIP: THE DIVINE CONQUEST */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 5
#define SHIPS 3

static char pb[SIZE][SIZE], cb[SIZE][SIZE], pv[SIZE][SIZE];
static const int lens[] = {1, 2, 3};
static const char *names[] = {"Patrol Boat", "Destroyer", "Battleship"};

/* initialize board with empty water */
static void
init(char b[SIZE][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			b[i][j] = '.';
}

/* display board with coordinate labels */
static void
show(char b[SIZE][SIZE])
{
	int i, j;
	fputs("  ", stdout);
	for (i = 0; i < SIZE; i++)
		printf("%d ", i);
	putchar('\n');
	for (i = 0; i < SIZE; i++) {
		printf("%c ", 'A' + i);
		for (j = 0; j < SIZE; j++)
			printf("%c ", b[i][j]);
		putchar('\n');
	}
}

/* place ship on board - returns 1 on success, 0 on failure */
static int
place(char b[SIZE][SIZE], int len, int cpu)
{
	int x, y, d, i;
	char in[8];

	if (cpu) {
		x = rand() % SIZE;
		y = rand() % SIZE;
		d = rand() % 2;
	} else {
		fputs("Enter starting position (e.g., A0): ", stdout);
		if (!fgets(in, sizeof in, stdin) || !in[0] || !in[1])
			return 0;
		x = toupper(in[0]) - 'A';
		y = in[1] - '0';
		if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
			return 0;
		fputs("Enter 0 for horizontal or 1 for vertical placement: ", stdout);
		if (scanf("%d", &d) != 1 || (d != 0 && d != 1)) {
			while (getchar() != '\n');
			return 0;
		}
		while (getchar() != '\n');
	}

	/* check if ship fits and doesn't overlap */
	if (d == 0) {
		if (y + len > SIZE)
			return 0;
		for (i = 0; i < len; i++)
			if (b[x][y + i] != '.')
				return 0;
		for (i = 0; i < len; i++)
			b[x][y + i] = '#';
	} else {
		if (x + len > SIZE)
			return 0;
		for (i = 0; i < len; i++)
			if (b[x + i][y] != '.')
				return 0;
		for (i = 0; i < len; i++)
			b[x + i][y] = '#';
	}
	return 1;
}

/* fire at coordinates - returns 1=hit, 0=miss, -1=invalid */
static int
fire(char b[SIZE][SIZE], char v[SIZE][SIZE], int x, int y)
{
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
		return -1;
	if (b[x][y] == '.') {
		v[x][y] = 'O';
		return 0;
	}
	if (b[x][y] == '#') {
		v[x][y] = b[x][y] = 'X';
		return 1;
	}
	return -1;
}

/* check if all ships are sunk */
static int
won(char b[SIZE][SIZE])
{
	int i, j;
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			if (b[i][j] == '#')
				return 0;
	return 1;
}

/* computer fires at random valid position */
static int
cpufire(char b[SIZE][SIZE])
{
	int x, y, n = 0, i, j;

	/* count available moves */
	for (i = 0; i < SIZE; i++)
		for (j = 0; j < SIZE; j++)
			if (b[i][j] != 'X' && b[i][j] != 'O')
				n++;
	if (!n)
		return -1;

	/* find random valid position */
	do {
		x = rand() % SIZE;
		y = rand() % SIZE;
	} while (b[x][y] == 'X' || b[x][y] == 'O');

	if (b[x][y] == '#') {
		b[x][y] = 'X';
		return 1;
	}
	b[x][y] = 'O';
	return 0;
}

int
main(void)
{
	int i, x, y, r;
	char in[8];

	srand(time(NULL));
	init(pb);
	init(cb);
	init(pv);

	puts("===== BATTLESHIP: THE DIVINE CONQUEST =====");
	puts("     Let the holy ships guide your fate!");
	puts("==========================================");
	puts("WELCOME TO TEMPLE BATTLESHIP!");
	puts("God guides your missiles on the HOLY 5x5 GRID!\n");

	/* player places ships */
	puts("Place your ships on the sacred waters!");
	for (i = 0; i < SHIPS; i++) {
		printf("Placing your %s (length %d):\n", names[i], lens[i]);
		show(pb);
		while (!place(pb, lens[i], 0))
			puts("Error: Ship placement failed. Ensure the ship fits on the board and doesn't overlap with existing ships.");
	}

	/* computer places ships */
	puts("Computer is placing ships...");
	for (i = 0; i < SHIPS; i++)
		while (!place(cb, lens[i], 1));

	/* main game loop */
	for (;;) {
		printf("\033[2J\033[H");
		puts("===== BATTLESHIP: THE DIVINE CONQUEST =====");
		puts("     Let the holy ships guide your fate!");
		puts("==========================================");
		puts("COMPUTER'S WATERS (? = unknown, O = miss, X = hit):");
		show(pv);
		puts("\nYOUR HOLY FLEET:");
		show(pb);

		/* player's turn */
		fputs("\nEnter coordinates to strike (e.g., A0): ", stdout);
		if (!fgets(in, sizeof in, stdin) || !in[0] || !in[1])
			continue;
		x = toupper(in[0]) - 'A';
		y = in[1] - '0';
		r = fire(cb, pv, x, y);
		if (r == 1) {
			puts("DIVINE HIT! God guides your missiles!");
			if (won(cb)) {
				puts("VICTORY! GOD'S LIGHT SHINES UPON YOU!");
				break;
			}
		} else if (r == 0)
			puts("MISS! The enemy has been spared... for now.");
		else {
			puts("Error: You already fired at that location. Choose a different target.");
			continue;
		}

		/* computer's turn */
		puts("Computer is making a move...");
		r = cpufire(pb);
		if (r == 1) {
			puts("The computer hit your ship!");
			if (won(pb)) {
				puts("DEFEAT! YOUR FLEET HAS BEEN SUNK BY THE ENEMY!");
				break;
			}
		} else if (r == 0)
			puts("The computer missed!");

		fputs("Press Enter to continue...", stdout);
		while (getchar() != '\n');
	}

	/* display final boards */
	puts("\nFINAL COMPUTER'S BOARD:");
	show(cb);
	puts("\nYOUR FINAL BOARD:");
	show(pb);

	return 0;
}
