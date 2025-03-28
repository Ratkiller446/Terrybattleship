#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdint.h>

#define BOARD_SIZE 5
#define EMPTY '.'
#define HIT 'X'
#define MISS 'O'
#define SHIP '#'
#define NUM_SHIPS 3
#define MAX_INPUT_SIZE 8

typedef struct {
    char symbol;
    uint8_t length;
    const char *name;
} Ship;

char playerBoard[BOARD_SIZE][BOARD_SIZE];
char computerBoard[BOARD_SIZE][BOARD_SIZE];
char playerViewOfComputer[BOARD_SIZE][BOARD_SIZE];

// Function prototypes
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void displayBoard(const char board[BOARD_SIZE][BOARD_SIZE]);
int8_t placeShip(char board[BOARD_SIZE][BOARD_SIZE], uint8_t length, uint8_t isComputer);
int8_t makeMove(char board[BOARD_SIZE][BOARD_SIZE], char viewBoard[BOARD_SIZE][BOARD_SIZE], uint8_t x, uint8_t y);
int8_t computerMove(char board[BOARD_SIZE][BOARD_SIZE]);
uint8_t checkForWin(const char board[BOARD_SIZE][BOARD_SIZE]);
void clearScreen(void);
void displayTitle(void);
void flushInputBuffer(void);  // Prototype added to prevent implicit declaration error

int main(void) {
    srand((unsigned int)time(NULL));

    const Ship ships[NUM_SHIPS] = {
        {'A', 1, "Patrol Boat"},
        {'B', 2, "Destroyer"},
        {'C', 3, "Battleship"}
    };

    uint8_t gameOver = 0;
    char input[MAX_INPUT_SIZE];

    // Initialize all boards
    initializeBoard(playerBoard);
    initializeBoard(computerBoard);
    initializeBoard(playerViewOfComputer);

    // Game introduction
    clearScreen();
    displayTitle();
    printf("WELCOME TO TEMPLE BATTLESHIP!\n");
    printf("God guides your missiles on the HOLY 5x5 GRID!\n\n");

    // Player places ships
    printf("Place your ships on the sacred waters!\n");
    for (uint8_t i = 0; i < NUM_SHIPS; i++) {
        printf("Placing your %s (length %u):\n", ships[i].name, ships[i].length);
        displayBoard(playerBoard);
        while (!placeShip(playerBoard, ships[i].length, 0)) {
            printf("Invalid placement! Try again with divine precision!\n");
        }
    }

    // Computer places ships
    printf("Computer is placing ships...\n");
    for (uint8_t i = 0; i < NUM_SHIPS; i++) {
        while (!placeShip(computerBoard, ships[i].length, 1)) {
            // Retry until successfully placed
        }
    }

    // Main game loop
    while (!gameOver) {
        clearScreen();
        displayTitle();

        printf("COMPUTER'S WATERS (? = unknown, O = miss, X = hit):\n");
        displayBoard(playerViewOfComputer);
        printf("\nYOUR HOLY FLEET:\n");
        displayBoard(playerBoard);

        // Player's turn
        printf("\nEnter coordinates to strike (e.g., A0): ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("INPUT ERROR! THE CIA IS TAMPERING WITH YOUR TERMINAL!\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) >= 2 &&
            toupper((unsigned char)input[0]) >= 'A' &&
            toupper((unsigned char)input[0]) < 'A' + BOARD_SIZE &&
            isdigit((unsigned char)input[1]) &&
            (input[1] - '0') < BOARD_SIZE) {
            uint8_t x = (uint8_t)(toupper((unsigned char)input[0]) - 'A');
        uint8_t y = (uint8_t)(input[1] - '0');
        int8_t result = makeMove(computerBoard, playerViewOfComputer, x, y);
        if (result == 1) {
            printf("DIVINE HIT! God guides your missiles!\n");
            if (checkForWin(computerBoard)) {
                gameOver = 1;
                printf("VICTORY! GOD'S LIGHT SHINES UPON YOU!\n");
                break;
            }
        } else if (result == 0) {
            printf("MISS! The enemy has been spared... for now.\n");
        } else {
            printf("You already fired there! The divine doesn't waste ammunition!\n");
            continue;
        }

        // Computer's turn
        printf("Computer is making a move...\n");
        int8_t compResult = computerMove(playerBoard);
        if (compResult == 1) {
            printf("The computer hit your ship!\n");
            if (checkForWin(playerBoard)) {
                gameOver = 1;
                printf("DEFEAT! YOUR FLEET HAS BEEN SUNK BY THE ENEMY!\n");
                break;
            }
        } else if (compResult == 0) {
            printf("The computer missed!\n");
        } else {
            printf("The computer has no valid moves left.\n");
        }
            } else {
                printf("Invalid coordinates! Use format like A0.\n");
            }

            printf("Press Enter to continue...");
            flushInputBuffer();
    }

    // Display final boards
    printf("\nFINAL COMPUTER'S BOARD:\n");
    displayBoard(computerBoard);
    printf("\nYOUR FINAL BOARD:\n");
    displayBoard(playerBoard);

    return 0;
}

/** Initializes the game board with empty spaces */
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

/** Displays the game board with row and column labels */
void displayBoard(const char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("  ");
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        printf("%u ", i);
    }
    printf("\n");
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        printf("%c ", 'A' + i);
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

/** Places a ship on the board; returns 1 on success, 0 on failure */
int8_t placeShip(char board[BOARD_SIZE][BOARD_SIZE], uint8_t length, uint8_t isComputer) {
    uint8_t x, y;
    int8_t direction;
    char input[MAX_INPUT_SIZE];

    if (isComputer) {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        direction = rand() % 2;
    } else {
        printf("Enter starting position (e.g., A0): ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL || strlen(input) < 2) {
            return 0;
        }
        input[strcspn(input, "\n")] = 0;
        if (toupper((unsigned char)input[0]) < 'A' || toupper((unsigned char)input[0]) >= 'A' + BOARD_SIZE ||
            !isdigit((unsigned char)input[1]) || (input[1] - '0') >= BOARD_SIZE) {
            return 0;
            }
            x = (uint8_t)(toupper((unsigned char)input[0]) - 'A');
        y = (uint8_t)(input[1] - '0');
        printf("Enter 0 for horizontal or 1 for vertical placement: ");
        if (scanf("%hhd", &direction) != 1 || (direction != 0 && direction != 1)) {
            printf("Invalid direction! Enter 0 or 1.\n");
            flushInputBuffer();
            return 0;
        }
        flushInputBuffer();
    }

    if (direction == 0) { // Horizontal
        if (y + length > BOARD_SIZE) return 0;
        for (uint8_t i = 0; i < length; i++) {
            if (board[x][y + i] != EMPTY) return 0;
        }
        for (uint8_t i = 0; i < length; i++) {
            board[x][y + i] = SHIP;
        }
    } else { // Vertical
        if (x + length > BOARD_SIZE) return 0;
        for (uint8_t i = 0; i < length; i++) {
            if (board[x + i][y] != EMPTY) return 0;
        }
        for (uint8_t i = 0; i < length; i++) {
            board[x + i][y] = SHIP;
        }
    }
    return 1;
}

/** Processes a player's move; returns 1 for hit, 0 for miss, -1 if already fired upon */
int8_t makeMove(char board[BOARD_SIZE][BOARD_SIZE], char viewBoard[BOARD_SIZE][BOARD_SIZE], uint8_t x, uint8_t y) {
    if (board[x][y] == EMPTY) {
        viewBoard[x][y] = MISS;
        return 0;  // Miss
    } else if (board[x][y] == SHIP) {
        viewBoard[x][y] = HIT;
        board[x][y] = HIT;
        return 1;  // Hit
    } else {
        return -1; // Already fired upon
    }
}

/** Computer makes a random move; returns 1 for hit, 0 for miss, -1 if no moves left */
int8_t computerMove(char board[BOARD_SIZE][BOARD_SIZE]) {
    uint8_t x, y;
    uint8_t attempts = 0;
    const uint8_t max_attempts = BOARD_SIZE * BOARD_SIZE;

    do {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        attempts++;
        if (attempts > max_attempts) {
            return -1; // No valid moves left
        }
    } while (board[x][y] == HIT || board[x][y] == MISS);

    if (board[x][y] == SHIP) {
        board[x][y] = HIT;
        return 1; // Hit
    } else {
        board[x][y] = MISS;
        return 0; // Miss
    }
}

/** Checks if all ships are sunk; returns 1 if won, 0 if ships remain */
uint8_t checkForWin(const char board[BOARD_SIZE][BOARD_SIZE]) {
    for (uint8_t i = 0; i < BOARD_SIZE; i++) {
        for (uint8_t j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == SHIP) {
                return 0; // Ships remain
            }
        }
    }
    return 1; // All ships sunk
}

/** Clears the input buffer to prevent leftover characters */
void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/** Clears the console screen (platform-dependent) */
void clearScreen(void) {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

/** Displays the game title */
void displayTitle(void) {
    printf("===== BATTLESHIP: THE DIVINE CONQUEST =====\n");
    printf("     Let the holy ships guide your fate!\n");
    printf("==========================================\n");
}
