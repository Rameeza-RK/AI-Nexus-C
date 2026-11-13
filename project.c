#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* =========================================================
   ANSI COLORS
   ========================================================= */

#define RESET       "\033[0m"
#define WHITE       "\033[97m"
#define RED         "\033[91m"
#define GREEN       "\033[92m"
#define YELLOW      "\033[93m"
#define BLUE        "\033[94m"
#define MAGENTA     "\033[95m"
#define CYAN        "\033[96m"

#define BG_BLUE     "\033[44m"
#define BG_GREEN    "\033[42m"
#define BG_MAGENTA  "\033[45m"

/* =========================================================
   WINDOWS ANSI SUPPORT
   ========================================================= */

#ifdef _WIN32
void enableANSI(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    if (hOut == INVALID_HANDLE_VALUE)
        return;

    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= 0x0004; /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */
    SetConsoleMode(hOut, dwMode);
}
#endif

/* =========================================================
   GENERAL FUNCTIONS
   ========================================================= */

void clearInputBuffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
        /* clear input */
    }
}

void pauseScreen(void)
{
    printf("\n");
    printf(WHITE "Press ENTER to continue..." RESET);
    getchar();
}

void printLine(int length)
{
    int i;

    printf(WHITE);
    for (i = 0; i < length; i++)
        printf("=");
    printf(RESET "\n");
}

/* =========================================================
   MAIN TOOLBOX
   ========================================================= */

void showToolbox(void)
{
    printf("\n");
    printf(WHITE);
    printLine(70);

    printf("                         C TOOLBOX\n");

    printLine(70);
    printf(RESET);

    printf(RED    "   [1]  Game Zone\n" RESET);
    printf(BLUE   "   [2]  Problem Solving Lab\n" RESET);
    printf(GREEN  "   [3]  Creative Studio\n" RESET);
    printf(MAGENTA"   [4]  Security Hub\n" RESET);
    printf(CYAN   "   [5]  Smart Systems\n" RESET);
    printf(YELLOW "   [6]  Daily Hub\n" RESET);

    printf(WHITE);
    printLine(70);
    printf(RESET);
}

/* =========================================================
   GAME ZONE MENU
   ========================================================= */

void showGameZone(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                         GAME ZONE\n");
        printLine(60);
        printf(RESET);

        printf(RED
               "   [1] Memory Match Game\n"
               RESET);

        printf(BLUE
               "   [2] Tic-Tac-Toe\n"
               RESET);

        printf(GREEN
               "   [3] C Language Quiz\n"
               RESET);

        printf(WHITE
               "   [0] Back to Toolbox\n"
               RESET);

        printf("\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                /* Memory Match */
                {
                    extern void memoryMatchMenu(void);
                    memoryMatchMenu();
                }
                break;

            case 2:
                /* Tic-Tac-Toe */
                {
                    extern void ticTacToeMenu(void);
                    ticTacToeMenu();
                }
                break;

            case 3:
                /* C Quiz */
                {
                    extern void quizMenu(void);
                    quizMenu();
                }
                break;

            case 0:
                return;

            default:
                printf(RED "Please choose a valid option.\n" RESET);
        }
    }
}

/* =========================================================
   MEMORY MATCH
   ========================================================= */

#define MAX_MEMORY_SIZE 4

void shuffleArray(int arr[], int size)
{
    int i;

    for (i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void displayMemoryBoard(
    char board[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE],
    int revealed[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE],
    int size)
{
    int i, j;

    printf("\n");

    printf(WHITE "      ");
    for (j = 0; j < size; j++)
        printf("  %d   ", j + 1);
    printf("\n" RESET);

    for (i = 0; i < size; i++)
    {
        printf(WHITE "   %d  " RESET, i + 1);

        for (j = 0; j < size; j++)
        {
            if (board[i][j] == '#')
            {
                printf(WHITE "[ # ] " RESET);
            }
            else if (revealed[i][j])
            {
                printf(CYAN "[ %c ] " RESET, board[i][j]);
            }
            else
            {
                printf(WHITE "[ ? ] " RESET);
            }
        }

        printf("\n");
    }

    printf("\n");
}

int validMemoryPosition(
    int row,
    int col,
    int size,
    int revealed[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE],
    char board[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE])
{
    if (row < 0 || row >= size ||
        col < 0 || col >= size)
        return 0;

    if (board[row][col] == '#')
        return 0;

    if (revealed[row][col])
        return 0;

    return 1;
}

void computerMemoryMove(
    char board[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE],
    int revealed[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE],
    int size,
    int *r1,
    int *c1,
    int *r2,
    int *c2)
{
    int i, j;

    /* -----------------------------------------------------
       First priority:
       If computer has already seen a pair, match it.
       ----------------------------------------------------- */

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (board[i][j] == '#' || revealed[i][j])
                continue;

            int x, y;

            for (x = i; x < size; x++)
            {
                int startY = (x == i) ? j + 1 : 0;

                for (y = startY; y < size; y++)
                {
                    if (board[x][y] == '#' || revealed[x][y])
                        continue;

                    if (board[i][j] == board[x][y])
                    {
                        *r1 = i;
                        *c1 = j;
                        *r2 = x;
                        *c2 = y;
                        return;
                    }
                }
            }
        }
    }

    /* -----------------------------------------------------
       Otherwise choose two random hidden cards.
       ----------------------------------------------------- */

    do
    {
        *r1 = rand() % size;
        *c1 = rand() % size;

    } while (!validMemoryPosition(
                 *r1,
                 *c1,
                 size,
                 revealed,
                 board));

    do
    {
        *r2 = rand() % size;
        *c2 = rand() % size;

    } while (
        (*r2 == *r1 && *c2 == *c1) ||
        !validMemoryPosition(
            *r2,
            *c2,
            size,
            revealed,
            board));
}

void playMemoryMatch(int size)
{
    char board[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE];
    int revealed[MAX_MEMORY_SIZE][MAX_MEMORY_SIZE] = {0};

    int totalCells = size * size;
    int playableCells = totalCells;

    int values[16];

    int i, j;
    int index = 0;

    int pairs;
    int blockedCell = -1;

    int playerScore = 0;
    int computerScore = 0;

    int currentPlayer = 0;
    int totalMatches = 0;

    /* -----------------------------------------------------
       3x3 has 9 cells.
       We reserve one cell as blocked.
       Remaining 8 cells = 4 pairs.
       ----------------------------------------------------- */

    if (size == 3)
    {
        blockedCell = 8;
        playableCells = 8;
    }

    pairs = playableCells / 2;

    /* Create pair values:
       A A, B B, C C...
    */

    for (i = 0; i < pairs; i++)
    {
        values[index++] = 'A' + i;
        values[index++] = 'A' + i;
    }

    shuffleArray(values, index);

    index = 0;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            int position = i * size + j;

            if (position == blockedCell)
            {
                board[i][j] = '#';
            }
            else
            {
                board[i][j] = (char)values[index++];
            }
        }
    }

    printf("\n");
    printf(CYAN "================ MEMORY MATCH ================\n" RESET);

    printf("\n");
    printf("You will play against the computer.\n");
    printf("Find matching pairs to score points.\n");

    while (totalMatches < pairs)
    {
        int r1, c1, r2, c2;

        displayMemoryBoard(board, revealed, size);

        if (currentPlayer == 0)
        {
            printf(BLUE "\nYour turn\n" RESET);

            printf("Enter first row and column: ");

            if (scanf("%d %d", &r1, &c1) != 2)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            printf("Enter second row and column: ");

            if (scanf("%d %d", &r2, &c2) != 2)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            clearInputBuffer();

            r1--;
            c1--;
            r2--;
            c2--;

            if (!validMemoryPosition(
                    r1, c1, size, revealed, board) ||
                !validMemoryPosition(
                    r2, c2, size, revealed, board) ||
                (r1 == r2 && c1 == c2))
            {
                printf(RED "Invalid card selection.\n" RESET);
                continue;
            }
        }
        else
        {
            printf(MAGENTA "\nComputer's turn...\n" RESET);

            computerMemoryMove(
                board,
                revealed,
                size,
                &r1,
                &c1,
                &r2,
                &c2);

            printf(
                "Computer selected: (%d,%d) and (%d,%d)\n",
                r1 + 1,
                c1 + 1,
                r2 + 1,
                c2 + 1
            );
        }

        revealed[r1][c1] = 1;
        revealed[r2][c2] = 1;

        displayMemoryBoard(board, revealed, size);

        if (board[r1][c1] == board[r2][c2])
        {
            printf(GREEN "MATCH! +1 point\n" RESET);

            if (currentPlayer == 0)
                playerScore++;
            else
                computerScore++;

            totalMatches++;
        }
        else
        {
            printf(RED "Not a match.\n" RESET);

            revealed[r1][c1] = 0;
            revealed[r2][c2] = 0;

            currentPlayer = !currentPlayer;
        }
    }

    printf("\n");
    printf(WHITE);
    printLine(45);
    printf(RESET);

    printf("Final Score\n\n");

    printf(BLUE "You      : %d\n" RESET, playerScore);
    printf(MAGENTA "Computer : %d\n" RESET, computerScore);

    if (playerScore > computerScore)
        printf(GREEN "\nYou Win! 🎉\n" RESET);
    else if (computerScore > playerScore)
        printf(RED "\nComputer Wins!\n" RESET);
    else
        printf(YELLOW "\nIt's a Draw!\n" RESET);

    pauseScreen();
}

void memoryMatchMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(55);
        printf("                     MEMORY MATCH\n");
        printLine(55);
        printf(RESET);

        printf(GREEN   "   [1] Easy       - 2 x 2\n" RESET);
        printf(YELLOW  "   [2] Medium     - 3 x 3\n" RESET);
        printf(RED     "   [3] Difficult  - 4 x 4\n" RESET);
        printf(WHITE   "   [0] Back\n" RESET);

        printf("\nSelect difficulty: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                playMemoryMatch(2);
                break;

            case 2:
                playMemoryMatch(3);
                break;

            case 3:
                playMemoryMatch(4);
                break;

            case 0:
                return;

            default:
                printf(RED "Invalid choice.\n" RESET);
        }
    }
}

/* =========================================================
   TIC-TAC-TOE
   ========================================================= */

void printTicTacToe(char board[3][3])
{
    int i;

    printf("\n");

    for (i = 0; i < 3; i++)
    {
        printf("       ");

        printf("[%c] [%c] [%c]\n",
               board[i][0],
               board[i][1],
               board[i][2]);

        if (i < 2)
            printf("       -----------\n");
    }

    printf("\n");
}

int checkWinner(char board[3][3])
{
    int i;

    for (i = 0; i < 3; i++)
    {
        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
        {
            if (board[i][0] == 'X')
                return 1;

            if (board[i][0] == 'O')
                return 2;
        }

        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
        {
            if (board[0][i] == 'X')
                return 1;

            if (board[0][i] == 'O')
                return 2;
        }
    }

    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
    {
        if (board[1][1] == 'X')
            return 1;

        if (board[1][1] == 'O')
            return 2;
    }

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
    {
        if (board[1][1] == 'X')
            return 1;

        if (board[1][1] == 'O')
            return 2;
    }

    return 0;
}

int boardFull(char board[3][3])
{
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
            {
                return 0;
            }
        }
    }

    return 1;
}

int minimax(char board[3][3], int maximizing)
{
    int winner = checkWinner(board);
    int bestScore;
    int i, j;

    if (winner == 2)
        return 10;

    if (winner == 1)
        return -10;

    if (boardFull(board))
        return 0;

    if (maximizing)
    {
        bestScore = -1000;

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (board[i][j] >= '1' &&
                    board[i][j] <= '9')
                {
                    char old = board[i][j];

                    board[i][j] = 'O';

                    {
                        int score = minimax(board, 0);

                        if (score > bestScore)
                            bestScore = score;
                    }

                    board[i][j] = old;
                }
            }
        }

        return bestScore;
    }
    else
    {
        bestScore = 1000;

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (board[i][j] >= '1' &&
                    board[i][j] <= '9')
                {
                    char old = board[i][j];

                    board[i][j] = 'X';

                    {
                        int score = minimax(board, 1);

                        if (score < bestScore)
                            bestScore = score;
                    }

                    board[i][j] = old;
                }
            }
        }

        return bestScore;
    }
}

void computerMoveEasy(char board[3][3])
{
    int empty[9];
    int count = 0;
    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
            {
                empty[count++] = i * 3 + j;
            }
        }
    }

    if (count > 0)
    {
        int position = empty[rand() % count];

        board[position / 3][position % 3] = 'O';
    }
}

void computerMoveMedium(char board[3][3])
{
    int i, j;

    /* Try to win */

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
            {
                char old = board[i][j];

                board[i][j] = 'O';

                if (checkWinner(board) == 2)
                    return;

                board[i][j] = old;
            }
        }
    }

    /* Try to block player */

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
            {
                char old = board[i][j];

                board[i][j] = 'X';

                if (checkWinner(board) == 1)
                {
                    board[i][j] = 'O';
                    return;
                }

                board[i][j] = old;
            }
        }
    }

    /* Otherwise random move */

    computerMoveEasy(board);
}

void computerMoveHard(char board[3][3])
{
    int bestScore = -1000;
    int bestRow = -1;
    int bestCol = -1;

    int i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] >= '1' &&
                board[i][j] <= '9')
            {
                char old = board[i][j];

                board[i][j] = 'O';

                {
                    int score = minimax(board, 0);

                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }

                board[i][j] = old;
            }
        }
    }

    if (bestRow != -1)
        board[bestRow][bestCol] = 'O';
}

void playTicTacToe(int difficulty)
{
    char board[3][3] =
    {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'}
    };

    int moves = 0;

    while (1)
    {
        int choice;
        int row, col;
        int winner;

        printTicTacToe(board);

        printf(BLUE "Your turn (X)\n" RESET);
        printf("Choose position 1-9: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        if (choice < 1 || choice > 9)
        {
            printf(RED "Choose a number from 1 to 9.\n" RESET);
            continue;
        }

        row = (choice - 1) / 3;
        col = (choice - 1) % 3;

        if (board[row][col] == 'X' ||
            board[row][col] == 'O')
        {
            printf(RED "That position is already taken.\n" RESET);
            continue;
        }

        board[row][col] = 'X';
        moves++;

        winner = checkWinner(board);

        if (winner == 1)
        {
            printTicTacToe(board);
            printf(GREEN "You Win! 🎉\n" RESET);
            break;
        }

        if (moves == 9)
        {
            printTicTacToe(board);
            printf(YELLOW "It's a Draw!\n" RESET);
            break;
        }

        printf(MAGENTA "\nComputer's turn...\n" RESET);

        if (difficulty == 1)
            computerMoveEasy(board);
        else if (difficulty == 2)
            computerMoveMedium(board);
        else
            computerMoveHard(board);

        winner = checkWinner(board);
        if (winner == 2)
        {
            printTicTacToe(board);
            printf(RED "Computer Wins!\n" RESET);
            break;
        }

        moves++;
    }

    pauseScreen();
}

void ticTacToeMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(55);
        printf("                     TIC-TAC-TOE\n");
        printLine(55);
        printf(RESET);

        printf(GREEN  "   [1] Easy       - Random Computer\n" RESET);
        printf(YELLOW "   [2] Medium     - Smart Computer\n" RESET);
        printf(RED    "   [3] Difficult  - AI / Minimax\n" RESET);
        printf(WHITE  "   [0] Back\n" RESET);

        printf("\nSelect difficulty: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
            case 2:
            case 3:
                playTicTacToe(choice);
                break;

            case 0:
                return;

            default:
                printf(RED "Invalid choice.\n" RESET);
        }
    }
}

/* =========================================================
   C LANGUAGE QUIZ
   ========================================================= */

typedef struct
{
    const char *question;
    const char *optionA;
    const char *optionB;
    const char *optionC;
    const char *optionD;
    char correct;
} QuizQuestion;

/* ---------------- EASY ---------------- */

QuizQuestion easyQuestions[] =
{
    {
        "Which data type is used to store an integer?",
        "float",
        "int",
        "char",
        "double",
        'B'
    },

    {
        "Which function is commonly used to display output in C?",
        "scanf()",
        "printf()",
        "input()",
        "display()",
        'B'
    },

    {
        "Which function is commonly used to take formatted input?",
        "printf()",
        "scanf()",
        "get()",
        "read()",
        'B'
    },

    {
        "Which statement is used for decision making?",
        "if",
        "loop",
        "include",
        "return",
        'A'
    },

    {
        "Which operator is used for addition?",
        "+",
        "-",
        "*",
        "/",
        'A'
    },

    {
        "Which loop is commonly used when the number of iterations is known?",
        "for",
        "switch",
        "if",
        "struct",
        'A'
    },

    {
        "Which symbol is used to end most C statements?",
        ":",
        ";",
        ".",
        ",",
        'B'
    },

    {
        "Which data type stores a single character?",
        "int",
        "float",
        "char",
        "double",
        'C'
    }
};

/* ---------------- MEDIUM ---------------- */

QuizQuestion mediumQuestions[] =
{
    {
        "What is the output?\nint x = 5; printf(\"%d\", x + 2);",
        "5",
        "7",
        "2",
        "0",
        'B'
    },

    {
        "What is the output?\nint x = 10; if(x > 5) printf(\"Yes\");",
        "No",
        "10",
        "Yes",
        "Error",
        'C'
    },

    {
        "How many times does this loop run?\nfor(int i=0;i<5;i++)",
        "4",
        "5",
        "6",
        "Infinite",
        'B'
    },

    {
        "What is the output?\nint x=10; printf(\"%d\", x%3);",
        "1",
        "2",
        "3",
        "0",
        'A'
    },

    {
        "What does this function return?\nint add(int a,int b){ return a+b; }",
        "The difference",
        "The product",
        "The sum",
        "Nothing",
        'C'
    },

    {
        "Which index accesses the first element of an array?",
        "0",
        "1",
        "-1",
        "10",
        'A'
    },

    {
        "What is the output?\nint x=3; printf(\"%d\", x*x);",
        "6",
        "9",
        "3",
        "12",
        'B'
    },

    {
        "Which operator checks equality?",
        "=",
        "==",
        "!=",
        ">=",
        'B'
    }
};

/* ---------------- DIFFICULT ---------------- */
QuizQuestion hardQuestions[] =
{
    {
        "Which symbol is used to access the value stored at a pointer?",
        "&",
        "*",
        "#",
        "%",
        'B'
    },

    {
        "Which function is used for dynamic memory allocation?",
        "printf()",
        "malloc()",
        "scanf()",
        "strlen()",
        'B'
    },

    {
        "Which keyword is used to define a structure?",
        "structure",
        "record",
        "struct",
        "class",
        'C'
    },

    {
        "Which function releases dynamically allocated memory?",
        "delete()",
        "remove()",
        "free()",
        "clear()",
        'C'
    },

    {
        "Which mode opens a file for writing?",
        "\"r\"",
        "\"w\"",
        "\"a\"",
        "\"read\"",
        'B'
    },

    {
        "Which concept occurs when a function calls itself?",
        "Iteration",
        "Recursion",
        "Compilation",
        "Casting",
        'B'
    },

    {
        "What does sizeof return?",
        "The value of a variable",
        "The memory size in bytes",
        "The address",
        "The data type name",
        'B'
    },

    {
        "Which operator is used to access a structure member through a pointer?",
        ".",
        "::",
        "->",
        "#",
        'C'
    }
};

void showQuizQuestion(
    QuizQuestion q,
    int number,
    int total)
{
    printf("\n");
    printf(CYAN "Question %d/%d\n" RESET, number, total);

    printf("%s\n\n", q.question);

    printf("A) %s\n", q.optionA);
    printf("B) %s\n", q.optionB);
    printf("C) %s\n", q.optionC);
    printf("D) %s\n", q.optionD);
}

char getQuizAnswer(void)
{
    char answer;

    printf("\nYour answer: ");
    scanf(" %c", &answer);
    clearInputBuffer();

    return (char)toupper((unsigned char)answer);
}

char computerQuizAnswer(
    QuizQuestion q,
    int difficulty)
{
    int randomValue = rand() % 100;

    /*
       This is a simulated computer opponent,
       not a machine-learning AI.
    */

    if (randomValue < (30 + difficulty * 20))
    {
        return q.correct;
    }

    {
        char options[] = {'A','B','C','D'};

        return options[rand() % 4];
    }
}

void runQuiz(
    QuizQuestion questions[],
    int total,
    int difficulty)
{
    int playerScore = 0;
    int computerScore = 0;
    int i;

    printf("\n");
    printf(CYAN "================ C LANGUAGE QUIZ ================\n" RESET);

    printf("\nYou are playing against the computer.\n");

    for (i = 0; i < total; i++)
    {
        char userAnswer;
        char computerAnswer;

        showQuizQuestion(
            questions[i],
            i + 1,
            total);

        userAnswer = getQuizAnswer();

        computerAnswer =
            computerQuizAnswer(
                questions[i],
                difficulty);

        if (userAnswer == questions[i].correct)
        {
            printf(GREEN "Correct! +1\n" RESET);
            playerScore++;
        }
        else
        {
            printf(
                RED "Wrong! Correct answer: %c\n" RESET,
                questions[i].correct);
        }

        if (computerAnswer == questions[i].correct)
            computerScore++;
    }

    printf("\n");
    printf(WHITE);
    printLine(45);
    printf(RESET);

    printf("QUIZ RESULT\n\n");

    printf(BLUE "Your Score      : %d/%d\n" RESET,
           playerScore,
           total);

    printf(MAGENTA "Computer Score  : %d/%d\n" RESET,
           computerScore,
           total);

    if (playerScore > computerScore)
        printf(GREEN "\nYou defeated the computer! 🎉\n" RESET);
    else if (computerScore > playerScore)
        printf(RED "\nComputer wins this round!\n" RESET);
    else
        printf(YELLOW "\nIt's a Draw!\n" RESET);

    pauseScreen();
}

void quizMenu(void)
{
    int choice;

    int easyCount =
        sizeof(easyQuestions) /
        sizeof(easyQuestions[0]);

    int mediumCount =
        sizeof(mediumQuestions) /
        sizeof(mediumQuestions[0]);

    int hardCount =
        sizeof(hardQuestions) /
        sizeof(hardQuestions[0]);

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(55);
        printf("                    C LANGUAGE QUIZ\n");
        printLine(55);
        printf(RESET);

        printf(GREEN
               "   [1] Easy       - C Basics\n"
               RESET);

        printf(YELLOW
               "   [2] Medium     - Logic & Code Output\n"
               RESET);

        printf(RED
               "   [3] Difficult  - Advanced C Concepts\n"
               RESET);

        printf(WHITE
               "   [0] Back\n"
               RESET);

        printf("\nSelect difficulty: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                runQuiz(
                    easyQuestions,
                    easyCount,
                    1);
                break;

            case 2:
                runQuiz(
                    mediumQuestions,
                    mediumCount,
                    2);
                break;

            case 3:
                runQuiz(
                    hardQuestions,
                    hardCount,
                    3);
                break;

            case 0:
                return;

            default:
                printf(RED "Invalid choice.\n" RESET);
        }
    }
}


void problemSolvingLab(void);
void calculatorMenu(void);
void logicLab(void);
void creativeStudioMenu(void);
void fibonacciTool(void);
void armstrongTool(void);
void palindromeTool(void);
void conversionTool(void);
void matrixTool(void);
void securityHubMenu(void);
void smartSystemsMenu(void);
void dailyHubMenu(void);
/* =========================================================
   MAIN
   ========================================================= */
   int main(void)
{
    int choice;

#ifdef _WIN32
    enableANSI();
#endif

    srand((unsigned int)time(NULL));

    while (1)
    {
        showToolbox();
        printf("\nSelect a tool: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Please enter a number from 0 to 6.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                showGameZone();
                break;

            case 2:
                problemSolvingLab();
                 break;

            case 3:
                 creativeStudioMenu();
                 break;

            case 4:
                 securityHubMenu();
                 break;

            case 5:
                 smartSystemsMenu();
                 break;

            case 6:
                 dailyHubMenu();
                 break;
            case 0:
                printf("\nExiting Toolbox...\n");
                return 0;

            default:
                printf(
                    RED
                    "Invalid choice. Please select 0-6.\n"
                    RESET);
        }
    }
}






/* =========================================================
   PROBLEM SOLVING LAB
   ========================================================= */

/* Forward declarations */
void problemSolvingLab(void);
void calculatorMenu(void);
void logicLab(void);

void fibonacciTool(void);
void armstrongTool(void);
void palindromeTool(void);
void conversionTool(void);
void matrixTool(void);


/* =========================================================
   CALCULATOR
   ========================================================= */

void calculatorMenu(void)
{
    int choice;

    while (1)
    {
        double a, b, result;
        int intA, intB;

        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                    CALCULATOR\n");
        printLine(60);
        printf(RESET);

        printf(CYAN   "   [1]  Addition\n" RESET);
        printf(CYAN   "   [2]  Subtraction\n" RESET);
        printf(CYAN   "   [3]  Multiplication\n" RESET);
        printf(CYAN   "   [4]  Division\n" RESET);
        printf(CYAN   "   [5]  Power\n" RESET);
        printf(CYAN   "   [6]  Square Root\n" RESET);
        printf(CYAN   "   [7]  Square\n" RESET);
        printf(CYAN   "   [8]  Modulus\n" RESET);
        printf(CYAN   "   [9]  Average\n" RESET);
        printf(CYAN   "   [10] Percentage\n" RESET);
        printf(WHITE  "   [0]  Back\n" RESET);

        printf("\nSelect operation: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                printf("\nEnter first number: ");
                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter second number: ");
                if (scanf("%lf", &b) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = a + b;

                printf(GREEN
                       "\nResult = %.2f\n"
                       RESET,
                       result);
                break;


            case 2:
                printf("\nEnter first number: ");
                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter second number: ");
                if (scanf("%lf", &b) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = a - b;

                printf(GREEN
                       "\nResult = %.2f\n"
                       RESET,
                       result);
                break;


            case 3:
                printf("\nEnter first number: ");
                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter second number: ");
                if (scanf("%lf", &b) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = a * b;

                printf(GREEN
                       "\nResult = %.2f\n"
                       RESET,
                       result);
                break;


            case 4:
                printf("\nEnter dividend: ");

                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter divisor: ");

                if (scanf("%lf", &b) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                if (b == 0)
                {
                    printf(RED
                           "\nError: Division by zero is not allowed.\n"
                           RESET);
                    break;
                }

                result = a / b;

                printf(GREEN
                       "\nResult = %.2f\n"
                       RESET,
                       result);
                break;


            case 5:
                printf("\nEnter base: ");

                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter exponent: ");

                if (scanf("%lf", &b) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = pow(a, b);

                printf(GREEN
                       "\nResult = %.2f\n"
                       RESET,
                       result);
                break;


            case 6:
                printf("\nEnter number: ");

                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                if (a < 0)
                {
                    printf(RED
                           "\nSquare root of a negative number is not "
                           "supported here.\n"
                           RESET);
                    break;
                }

                result = sqrt(a);

                printf(GREEN
                       "\nSquare Root = %.2f\n"
                       RESET,
                       result);
                break;


            case 7:
                printf("\nEnter number: ");

                if (scanf("%lf", &a) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = a * a;

                printf(GREEN
                       "\nSquare = %.2f\n"
                       RESET,
                       result);
                break;


            case 8:
                printf("\nEnter first integer: ");

                if (scanf("%d", &intA) != 1)
                {
                    clearInputBuffer();
                    printf(RED
                           "Modulus requires integers.\n"
                           RESET);
                    break;
                }

                printf("Enter second integer: ");

                if (scanf("%d", &intB) != 1)
                {
                    clearInputBuffer();
                    printf(RED
                           "Modulus requires integers.\n"
                           RESET);
                    break;
                }
                clearInputBuffer();

                if (intB == 0)
                {
                    printf(RED
                           "\nError: Modulus by zero is not allowed.\n"
                           RESET);
                    break;
                }

                printf(GREEN
                       "\nRemainder = %d\n"
                       RESET,
                       intA % intB);
                break;


            case 9:
            {
                int count;
                int i;
                double sum = 0;
                double value;

                printf("\nHow many numbers? ");

                if (scanf("%d", &count) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid input.\n" RESET);
                    break;
                }

                if (count <= 0)
                {
                    clearInputBuffer();
                    printf(RED
                           "Number of values must be greater than zero.\n"
                           RESET);
                    break;
                }

                for (i = 1; i <= count; i++)
                {
                    printf("Enter number %d: ", i);

                    if (scanf("%lf", &value) != 1)
                    {
                        clearInputBuffer();
                        printf(RED "Invalid number.\n" RESET);
                        sum = 0;
                        count = 0;
                        break;
                    }

                    sum += value;
                }

                clearInputBuffer();

                if (count > 0)
                {
                    result = sum / count;

                    printf(GREEN
                           "\nAverage = %.2f\n"
                           RESET,
                           result);
                }

                break;
            }


            case 10:
            {
                double value;
                double percent;

                printf("\nEnter value: ");

                if (scanf("%lf", &value) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid number.\n" RESET);
                    break;
                }

                printf("Enter percentage: ");

                if (scanf("%lf", &percent) != 1)
                {
                    clearInputBuffer();
                    printf(RED "Invalid percentage.\n" RESET);
                    break;
                }

                clearInputBuffer();

                result = (value * percent) / 100.0;

                printf(GREEN
                       "\n%.2f%% of %.2f = %.2f\n"
                       RESET,
                       percent,
                       value,
                       result);
                break;
            }


            case 0:
                return;


            default:
                printf(RED
                       "Invalid choice. Please select 0-10.\n"
                       RESET);
        }
    }
}
/* =========================================================
   LOGIC LAB
   ========================================================= */

void logicLab(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                     LOGIC LAB\n");
        printLine(60);
        printf(RESET);

        printf(BLUE    "   [1] Fibonacci Series\n" RESET);
        printf(GREEN   "   [2] Armstrong Number\n" RESET);
        printf(YELLOW  "   [3] Palindrome Checker\n" RESET);
        printf(CYAN    "   [4] Number System Converter\n" RESET);
        printf(MAGENTA "   [5] Matrix Operations\n" RESET);
        printf(WHITE   "   [0] Back\n" RESET);

        printf("\nSelect option: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                fibonacciTool();
                break;

            case 2:
                armstrongTool();
                break;

            case 3:
                palindromeTool();
                break;

            case 4:
                conversionTool();
                break;

            case 5:
                matrixTool();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0-5.\n"
                       RESET);
        }
    }
}


/* =========================================================
   FIBONACCI
   ========================================================= */

void fibonacciTool(void)
{
    int n;
    int i;

    unsigned long long first = 0;
    unsigned long long second = 1;
    unsigned long long next;

    printf("\n");
    printf(BLUE "========== FIBONACCI SERIES ==========\n" RESET);

    printf("\nHow many terms do you want? ");

    if (scanf("%d", &n) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (n <= 0)
    {
        printf(RED
               "Number of terms must be greater than zero.\n"
               RESET);
        return;
    }

    printf("\nSeries: ");

    for (i = 1; i <= n; i++)
    {
        printf("%llu", first);

        if (i < n)
            printf(", ");

        next = first + second;
        first = second;
        second = next;
    }

    printf("\n");

    pauseScreen();
}
/* =========================================================
   ARMSTRONG NUMBER
   ========================================================= */

int countDigits(long long number)
{
    int count = 0;

    if (number == 0)
        return 1;

    while (number != 0)
    {
        number /= 10;
        count++;
    }

    return count;
}

long long integerPower(
    int base,
    int exponent)
{
    long long result = 1;
    int i;

    for (i = 0; i < exponent; i++)
        result *= base;

    return result;
}

int isArmstrong(long long number)
{
    long long original;
    long long temp;
    long long sum = 0;

    int digits;

    if (number < 0)
        return 0;

    original = number;
    temp = number;

    digits = countDigits(number);

    while (temp > 0)
    {
        int digit = (int)(temp % 10);

        sum += integerPower(
            digit,
            digits);

        temp /= 10;
    }

    return sum == original;
}

void armstrongTool(void)
{
    long long number;

    printf("\n");
    printf(GREEN "========== ARMSTRONG CHECKER ==========\n" RESET);

    printf("\nEnter a number: ");

    if (scanf("%lld", &number) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid number.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (isArmstrong(number))
    {
        printf(GREEN
               "\n%lld is an Armstrong number.\n"
               RESET,
               number);
    }
    else
    {
        printf(YELLOW
               "\n%lld is NOT an Armstrong number.\n"
               RESET,
               number);
    }

    pauseScreen();
}


/* =========================================================
   PALINDROME
   ========================================================= */

int isPalindrome(long long number)
{
    long long original;
    long long reversed = 0;
    long long digit;

    if (number < 0)
        return 0;

    original = number;

    while (number > 0)
    {
        digit = number % 10;

        reversed =
            reversed * 10 + digit;

        number /= 10;
    }

    return original == reversed;
}

void palindromeTool(void)
{
    long long number;

    printf("\n");
    printf(YELLOW "========== PALINDROME CHECKER ==========\n" RESET);

    printf("\nEnter a number: ");

    if (scanf("%lld", &number) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid number.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (isPalindrome(number))
    {
        printf(GREEN
               "\n%lld is a Palindrome.\n"
               RESET,
               number);
    }
    else
    {
        printf(YELLOW
               "\n%lld is NOT a Palindrome.\n"
               RESET,
               number);
    }

    pauseScreen();
}


/* =========================================================
   NUMBER SYSTEM CONVERTER
   ========================================================= */

void printBinary(unsigned int number)
{
    int bits[32];
    int index = 0;

    if (number == 0)
    {
        printf("0");
        return;
    }

    while (number > 0)
    {
        bits[index++] = number % 2;
        number /= 2;
    }

    while (index > 0)
    {
        printf("%d", bits[--index]);
    }
}

void conversionTool(void)
{
    unsigned int number;

    printf("\n");
    printf(CYAN "========== NUMBER CONVERTER ==========\n" RESET);

    printf("\nEnter a non-negative decimal integer: ");

    if (scanf("%u", &number) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid number.\n" RESET);
        return;
    }

    clearInputBuffer();

    printf("\nDecimal : %u", number);

    printf("\nBinary  : ");
    printBinary(number);

    printf("\nOctal   : %o", number);

    printf("\nHex     : %X\n", number);

    pauseScreen();
}


/* =========================================================
   MATRIX
   ========================================================= */

#define MAX_MATRIX 5

void inputMatrix(
    int matrix[MAX_MATRIX][MAX_MATRIX],
    int rows,
    int cols)
{
    int i, j;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf(
                "Enter element [%d][%d]: ",
                i + 1,
                j + 1);

            while (scanf("%d",
                         &matrix[i][j]) != 1)
            {
                clearInputBuffer();

                printf(
                    RED
                    "Please enter an integer: "
                    RESET);
            }
        }
    }

    clearInputBuffer();
}

void displayMatrix(
    int matrix[MAX_MATRIX][MAX_MATRIX],
    int rows,
    int cols)
{
    int i, j;

    for (i = 0; i < rows; i++)
    {
        printf("   ");

        for (j = 0; j < cols; j++)
        {
            printf("%6d", matrix[i][j]);
        }

        printf("\n");
    }
}

void matrixTool(void)
{
    int choice;

    int A[MAX_MATRIX][MAX_MATRIX];
    int B[MAX_MATRIX][MAX_MATRIX];
    int result[MAX_MATRIX][MAX_MATRIX];

    int rows, cols;
    int rowsB, colsB;

    int i, j, k;

    while (1)
    {
        printf("\n");
        printf(MAGENTA "========== MATRIX LAB ==========\n" RESET);

        printf("\n");
        printf("[1] Matrix Addition\n");
        printf("[2] Matrix Subtraction\n");
        printf("[3] Matrix Multiplication\n");
        printf("[0] Back\n");

        printf("\nSelect operation: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        if (choice == 0)
            return;

        if (choice < 1 || choice > 3)
        {
            printf(RED
                   "Invalid choice.\n"
                   RESET);
            continue;
        }
        /* -------------------------
           Addition / Subtraction
           ------------------------- */

        if (choice == 1 || choice == 2)
        {
            printf("\nEnter rows (1-%d): ",
                   MAX_MATRIX);

            if (scanf("%d", &rows) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            printf("Enter columns (1-%d): ",
                   MAX_MATRIX);

            if (scanf("%d", &cols) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (rows < 1 || rows > MAX_MATRIX ||
                cols < 1 || cols > MAX_MATRIX)
            {
                printf(RED
                       "Invalid matrix size.\n"
                       RESET);
                continue;
            }

            printf("\nEnter Matrix A:\n");

            inputMatrix(
                A,
                rows,
                cols);

            printf("\nEnter Matrix B:\n");

            inputMatrix(
                B,
                rows,
                cols);

            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < cols; j++)
                {
                    if (choice == 1)
                    {
                        result[i][j] =
                            A[i][j] + B[i][j];
                    }
                    else
                    {
                        result[i][j] =
                            A[i][j] - B[i][j];
                    }
                }
            }

            printf("\nResult:\n");

            displayMatrix(
                result,
                rows,
                cols);

            pauseScreen();
        }

        /* -------------------------
           Matrix Multiplication
           ------------------------- */

        else if (choice == 3)
        {
            printf("\nEnter rows of Matrix A: ");

            if (scanf("%d", &rows) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            printf("Enter columns of Matrix A: ");

            if (scanf("%d", &cols) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            printf("Enter rows of Matrix B: ");

            if (scanf("%d", &rowsB) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            printf("Enter columns of Matrix B: ");

            if (scanf("%d", &colsB) != 1)
            {
                clearInputBuffer();
                printf(RED "Invalid input.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (rows < 1 || rows > MAX_MATRIX ||
                cols < 1 || cols > MAX_MATRIX ||
                rowsB < 1 || rowsB > MAX_MATRIX ||
                colsB < 1 || colsB > MAX_MATRIX)
            {
                printf(RED
                       "Invalid matrix size.\n"
                       RESET);
                continue;
            }

            if (cols != rowsB)
            {
                printf(RED
                       "\nMatrix multiplication is not possible.\n"
                       "Columns of A must equal rows of B.\n"
                       RESET);
                continue;
            }

            printf("\nEnter Matrix A:\n");

            inputMatrix(
                A,
                rows,
                cols);

            printf("\nEnter Matrix B:\n");

            inputMatrix(
                B,
                rowsB,
                colsB);

            for (i = 0; i < rows; i++)
            {
                for (j = 0; j < colsB; j++)
                {
                    result[i][j] = 0;

                    for (k = 0; k < cols; k++)
                    {
                        result[i][j] +=
                            A[i][k] * B[k][j];
                    }
                }
            }

            printf("\nResult:\n");

            displayMatrix(
                result,
                rows,
                colsB);

            pauseScreen();
        }
    }
}
/* =========================================================
   PROBLEM SOLVING LAB MAIN MENU
   ========================================================= */

void problemSolvingLab(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(65);
        printf("                  PROBLEM SOLVING LAB\n");
        printLine(65);
        printf(RESET);

        printf(BLUE
               "   [1] Calculator\n"
               RESET);

        printf(GREEN
               "   [2] Logic Lab\n"
               RESET);

        printf(WHITE
               "   [0] Back to Toolbox\n"
               RESET);

        printf("\nSelect option: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                calculatorMenu();
                break;

            case 2:
                logicLab();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0-2.\n"
                       RESET);
        }
    }
}


/* =========================================================
   CREATIVE STUDIO
   ========================================================= */

/* ---------------- SYMBOL ART ---------------- */

void symbolArt(void)
{
    int size;
    char symbol;
    int i, j;

    printf("\n");
    printf(GREEN "========== SYMBOL ART ==========\n" RESET);

    printf("Enter size (3-15): ");
    if (scanf("%d", &size) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (size < 3 || size > 15)
    {
        printf(RED "Size must be between 3 and 15.\n" RESET);
        return;
    }

    printf("Enter a symbol: ");
    scanf("%c", &symbol);
    clearInputBuffer();

    printf("\n");
    printf(CYAN "Your Pattern:\n\n" RESET);

    for (i = 1; i <= size; i++)
    {
        for (j = 1; j <= i; j++)
            printf("%c ", symbol);

        printf("\n");
    }

    pauseScreen();
}


/* ---------------- CHECKERBOARD ---------------- */

void checkerboardDesigner(void)
{
    int size;
    int i, j;

    printf("\n");
    printf(YELLOW "========== CHECKERBOARD DESIGNER ==========\n" RESET);

    printf("Enter size (3-15): ");

    if (scanf("%d", &size) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (size < 3 || size > 15)
    {
        printf(RED "Size must be between 3 and 15.\n" RESET);
        return;
    }

    printf("\n");
    printf(CYAN "Your Checkerboard:\n\n" RESET);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if ((i + j) % 2 == 0)
                printf("## ");
            else
                printf(".. ");
        }

        printf("\n");
    }

    pauseScreen();
}


/* ---------------- WAVE & ZIG-ZAG ---------------- */

void waveZigZagArt(void)
{
    int width;
    int i;

    printf("\n");
    printf(BLUE "========== WAVE & ZIG-ZAG ART ==========\n" RESET);

    printf("Enter width (10-60): ");

    if (scanf("%d", &width) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (width < 10 || width > 60)
    {
        printf(RED "Width must be between 10 and 60.\n" RESET);
        return;
    }

    printf("\n");
    printf(CYAN "Wave Pattern:\n\n" RESET);

    for (i = 0; i < width; i++)
    {
        if (i % 6 == 0 || i % 6 == 1)
            printf("    *\n");
        else if (i % 6 == 2 || i % 6 == 5)
            printf("  *\n");
        else
            printf("*\n");
    }

    printf("\n");

    printf(CYAN "Zig-Zag Pattern:\n\n" RESET);

    for (i = 0; i < width; i++)
    {
        if (i % 4 == 0)
            printf("*\n");
        else if (i % 4 == 1)
            printf("  *\n");
        else if (i % 4 == 2)
            printf("    *\n");
        else
            printf("  *\n");
    }

    pauseScreen();
}


/* ---------------- SPIRAL ART ---------------- */

void spiralArt(void)
{
    int size;
    int i, j;
    int layer;

    printf("\n");
    printf(MAGENTA "========== SPIRAL ART ==========\n" RESET);

    printf("Enter size (5-15): ");

    if (scanf("%d", &size) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (size < 5 || size > 15)
    {
        printf(RED "Size must be between 5 and 15.\n" RESET);
        return;
    }

    printf("\n");
    printf(CYAN "Spiral Pattern:\n\n" RESET);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            layer = i;

            if (j < layer)
                layer = j;

            if (size - 1 - i < layer)
                layer = size - 1 - i;

            if (size - 1 - j < layer)
                layer = size - 1 - j;

            if (layer % 2 == 0)
                printf("# ");
            else
                printf(". ");
        }

        printf("\n");
    }

    pauseScreen();
}
/* ---------------- RANDOM ART ---------------- */

void randomArtGenerator(void)
{
    int size;
    int i, j;
    int randomValue;

    printf("\n");
    printf(CYAN "========== RANDOM ART GENERATOR ==========\n" RESET);

    printf("Enter size (5-15): ");

    if (scanf("%d", &size) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (size < 5 || size > 15)
    {
        printf(RED "Size must be between 5 and 15.\n" RESET);
        return;
    }

    printf("\n");
    printf(CYAN "Generated Random Art:\n\n" RESET);

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            randomValue = rand() % 4;

            if (randomValue == 0)
                printf("* ");
            else if (randomValue == 1)
                printf("# ");
            else if (randomValue == 2)
                printf("+ ");
            else
                printf(". ");
        }

        printf("\n");
    }

    pauseScreen();
}


/* ---------------- AI DESIGN CHALLENGE ---------------- */

void aiDesignChallenge(void)
{
    int choice;
    int size;
    int i, j;

    printf("\n");
    printf(RED "========== AI DESIGN CHALLENGE ==========\n" RESET);

    printf("\n");
    printf("Choose a design challenge:\n\n");

    printf(GREEN  "[1] Diamond\n" RESET);
    printf(BLUE   "[2] Pyramid\n" RESET);
    printf(YELLOW "[3] Wave\n" RESET);

    printf("\nEnter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (choice < 1 || choice > 3)
    {
        printf(RED "Invalid choice.\n" RESET);
        return;
    }

    printf("Enter size (3-10): ");

    if (scanf("%d", &size) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid input.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (size < 3 || size > 10)
    {
        printf(RED "Size must be between 3 and 10.\n" RESET);
        return;
    }

    printf("\n");
    printf(MAGENTA "Computer AI is generating your design...\n\n" RESET);

    if (choice == 1)
    {
        printf("AI Diamond:\n\n");

        for (i = 1; i <= size; i++)
        {
            for (j = i; j < size; j++)
                printf(" ");

            for (j = 1; j <= (2 * i - 1); j++)
                printf("*");

            printf("\n");
        }

        for (i = size - 1; i >= 1; i--)
        {
            for (j = i; j < size; j++)
                printf(" ");

            for (j = 1; j <= (2 * i - 1); j++)
                printf("*");

            printf("\n");
        }
    }
    else if (choice == 2)
    {
        printf("AI Pyramid:\n\n");

        for (i = 1; i <= size; i++)
        {
            for (j = i; j < size; j++)
                printf(" ");

            for (j = 1; j <= (2 * i - 1); j++)
                printf("#");

            printf("\n");
        }
    }
    else
    {
        printf("AI Wave:\n\n");

        for (i = 0; i < size * 4; i++)
        {
            if (i % 4 == 0)
                printf("*\n");
            else if (i % 4 == 1)
                printf("  *\n");
            else if (i % 4 == 2)
                printf("    *\n");
            else
                printf("  *\n");
        }
    }

    printf("\n");
    printf(GREEN "AI Design completed!\n" RESET);

    pauseScreen();
}


/* =========================================================
   CREATIVE STUDIO MENU
   ========================================================= */

void creativeStudioMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);

        printf("                    CREATIVE STUDIO\n");

        printLine(60);
        printf(RESET);

        printf(GREEN
               "   [1] Symbol Art\n"
               RESET);

        printf(YELLOW
               "   [2] Checkerboard Designer\n"
               RESET);

        printf(BLUE
               "   [3] Wave & Zig-Zag Art\n"
               RESET);

        printf(MAGENTA
               "   [4] Spiral Art\n"
               RESET);

        printf(CYAN
               "   [5] Random Art Generator\n"
               RESET);

        printf(RED
               "   [6] AI Design Challenge\n"
               RESET);

        printf(WHITE
               "   [0] Back to Toolbox\n"
               RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();

            printf(RED
                   "Invalid input. Please enter a number.\n"
                   RESET);

            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                symbolArt();
                break;

            case 2:
                checkerboardDesigner();
                break;

            case 3:
                waveZigZagArt();
                break;

            case 4:
                spiralArt();
                break;

            case 5:
                randomArtGenerator();
                break;

            case 6:
                aiDesignChallenge();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0 to 6.\n"
                       RESET);
        }
    }
}




/* =========================================================
   SECURITY HUB
   ========================================================= */

void caesarCipher(void)
{
    char text[200];
    int shift;
    int i;

    printf("\n");
    printf(CYAN);
    printLine(60);
    printf("                     CAESAR CIPHER\n");
    printLine(60);
    printf(RESET);

    printf("\nEnter your text: ");

    if (fgets(text, sizeof(text), stdin) == NULL)
    {
        printf(RED "Input error.\n" RESET);
        pauseScreen();
        return;
    }

    printf("Enter shift value: ");

    if (scanf("%d", &shift) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid shift value.\n" RESET);
        pauseScreen();
        return;
    }

    clearInputBuffer();

    shift = shift % 26;

    for (i = 0; text[i] != '\0'; i++)
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            text[i] =
                (char)('A' +
                (text[i] - 'A' + shift + 26) % 26);
        }
        else if (text[i] >= 'a' && text[i] <= 'z')
        {
            text[i] =
                (char)('a' +
                (text[i] - 'a' + shift + 26) % 26);
        }
    }

    printf("\n");
    printf(WHITE);
    printLine(45);
    printf(RESET);

    printf(GREEN "\nEncrypted Text:\n%s" RESET, text);

    printf("\n");
    printLine(45);

    pauseScreen();
}


/* =========================================================
   PASSWORD STRENGTH CHECKER
   ========================================================= */

void passwordStrength(void)
{
    char password[200];

    int length;
    int hasUpper = 0;
    int hasLower = 0;
    int hasDigit = 0;
    int hasSpecial = 0;

    int score = 0;
    int i;

    printf("\n");
    printf(CYAN);
    printLine(60);
    printf("                PASSWORD STRENGTH CHECKER\n");
    printLine(60);
    printf(RESET);

    printf("\nEnter password: ");

    if (fgets(password, sizeof(password), stdin) == NULL)
    {
        printf(RED "Input error.\n" RESET);
        pauseScreen();
        return;
    }

    /* Remove newline */
    password[strcspn(password, "\n")] = '\0';

    length = (int)strlen(password);

    /* Analyze password */
    for (i = 0; password[i] != '\0'; i++)
    {
        if (isupper((unsigned char)password[i]))
        {
            hasUpper = 1;
        }
        else if (islower((unsigned char)password[i]))
        {
            hasLower = 1;
        }
        else if (isdigit((unsigned char)password[i]))
        {
            hasDigit = 1;
        }
        else
        {
            hasSpecial = 1;
        }
    }

    /* Score calculation */

    if (length >= 8)
        score++;

    if (length >= 12)
        score++;

    if (hasUpper)
        score++;

    if (hasLower)
        score++;

    if (hasDigit)
        score++;

    if (hasSpecial)
        score++;

    /* Result */

    printf("\n");
    printLine(50);

    printf("\n");
    printf(WHITE "             PASSWORD ANALYSIS\n" RESET);
    printf("\n");

    printf("Length          : %d\n", length);

    if (hasUpper)
        printf(GREEN "Uppercase       : Yes\n" RESET);
    else
        printf(RED "Uppercase       : No\n" RESET);

    if (hasLower)
        printf(GREEN "Lowercase       : Yes\n" RESET);
    else
        printf(RED "Lowercase       : No\n" RESET);

    if (hasDigit)
        printf(GREEN "Number          : Yes\n" RESET);
    else
        printf(RED "Number          : No\n" RESET);

    if (hasSpecial)
        printf(GREEN "Special Character: Yes\n" RESET);
    else
        printf(RED "Special Character: No\n" RESET);

    printf("\n");
    printLine(50);

    printf("\n");

    if (score <= 2)
    {
        printf(RED
               "Password Strength: WEAK\n"
               RESET);

        printf("Suggestion: Add more characters, numbers,\n");
        printf("uppercase letters and special characters.\n");
    }
    else if (score <= 4)
    {
        printf(YELLOW
               "Password Strength: MEDIUM\n"
               RESET);

        printf("Suggestion: Make your password longer\n");
        printf("and add more character types.\n");
    }
    else
    {
        printf(GREEN
               "Password Strength: STRONG\n"
               RESET);

        printf("Good combination of different character types.\n");
    }

    printf("\n");
    printLine(50);

    pauseScreen();
}


/* =========================================================
   SECURITY HUB MENU
   ========================================================= */

void securityHubMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);

        printf("                     SECURITY HUB\n");

        printLine(60);
        printf(RESET);

        printf(GREEN
               "   [1] Caesar Cipher\n"
               RESET);

        printf(YELLOW
               "   [2] Password Strength Checker\n"
               RESET);

        printf(WHITE
               "   [0] Back to Toolbox\n"
               RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();

            printf(RED
                   "Invalid input. Please enter a number.\n"
                   RESET);

            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                caesarCipher();
                break;

            case 2:
                passwordStrength();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0, 1 or 2.\n"
                       RESET);
        }
    }
}




/* =========================================================
   SMART SYSTEMS
   ========================================================= */

void bankingSystem(void)
{
    int choice;
    int balance = 10000;
    int amount;
    int transactions = 0;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                    BANKING SYSTEM\n");
        printLine(60);
        printf(RESET);

        printf(GREEN
               "   [1] Check Balance\n"
               RESET);

        printf(BLUE
               "   [2] Deposit Money\n"
               RESET);

        printf(YELLOW
               "   [3] Withdraw Money\n"
               RESET);

        printf(MAGENTA
               "   [4] ATM Transaction\n"
               RESET);

        printf(CYAN
               "   [5] Transaction Summary\n"
               RESET);

        printf(WHITE
               "   [0] Back to Smart Systems\n"
               RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();

            printf(RED
                   "Invalid input. Please enter a number.\n"
                   RESET);

            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:

                printf("\n");
                printf(GREEN
                       "Current Balance: Rs. %d\n"
                       RESET,
                       balance);

                pauseScreen();
                break;


            case 2:

                printf("\nEnter amount to deposit: ");

                if (scanf("%d", &amount) != 1)
                {
                    clearInputBuffer();

                    printf(RED
                           "Invalid amount.\n"
                           RESET);

                    break;
                }

                clearInputBuffer();

                if (amount <= 0)
                {
                    printf(RED
                           "Amount must be greater than zero.\n"
                           RESET);
                }
                else
                {
                    balance += amount;
                    transactions++;

                    printf(GREEN
                           "Deposit successful!\n"
                           RESET);

                    printf("New Balance: Rs. %d\n",
                           balance);
                }

                pauseScreen();
                break;


            case 3:

                printf("\nEnter amount to withdraw: ");

                if (scanf("%d", &amount) != 1)
                {
                    clearInputBuffer();

                    printf(RED
                           "Invalid amount.\n"
                           RESET);

                    break;
                }

                clearInputBuffer();

                if (amount <= 0)
                {
                    printf(RED
                           "Amount must be greater than zero.\n"
                           RESET);
                }
                else if (amount > balance)
                {
                    printf(RED
                           "Insufficient balance.\n"
                           RESET);
                }
                else
                {
                    balance -= amount;
                    transactions++;

                    printf(GREEN
                           "Withdrawal successful!\n"
                           RESET);

                    printf("Remaining Balance: Rs. %d\n",
                           balance);
                }

                pauseScreen();
                break;


            case 4:
            {
                int atmChoice;

                printf("\n");
                printf(CYAN);
                printLine(50);
                printf("                     ATM\n");
                printLine(50);
                printf(RESET);

                printf("   [1] Withdraw Cash\n");
                printf("   [2] Check Balance\n");
                printf("   [0] Exit ATM\n");

                printf("\nEnter ATM choice: ");

                if (scanf("%d", &atmChoice) != 1)
                {
                    clearInputBuffer();

                    printf(RED
                           "Invalid input.\n"
                           RESET);

                    pauseScreen();
                    break;
                }

                clearInputBuffer();

                if (atmChoice == 1)
                {
                    printf("\nEnter withdrawal amount: ");

                    if (scanf("%d", &amount) != 1)
                    {
                        clearInputBuffer();

                        printf(RED
                               "Invalid amount.\n"
                               RESET);

                        pauseScreen();
                        break;
                    }

                    clearInputBuffer();

                    if (amount <= 0)
                    {
                        printf(RED
                               "Invalid amount.\n"
                               RESET);
                    }
                    else if (amount > balance)
                    {
                        printf(RED
                               "Insufficient balance.\n"
                               RESET);
                    }
                    else
                    {
                        balance -= amount;
                        transactions++;

                        printf(GREEN
                               "\nPlease collect your cash.\n"
                               RESET);

                        printf("Remaining Balance: Rs. %d\n",
                               balance);
                    }

                    pauseScreen();
                }
                else if (atmChoice == 2)
                {
                    printf(GREEN
                           "\nATM Balance: Rs. %d\n"
                           RESET,
                           balance);

                    pauseScreen();
                }
                else if (atmChoice == 0)
                {
                    printf("\nExiting ATM...\n");
                    pauseScreen();
                }
                else
                {
                    printf(RED
                           "Invalid ATM choice.\n"
                           RESET);

                    pauseScreen();
                }

                break;
            }


            case 5:

                printf("\n");
                printf(WHITE);
                printLine(50);
                printf("                TRANSACTION SUMMARY\n");
                printLine(50);
                printf(RESET);

                printf("Total Transactions: %d\n",
                       transactions);

                printf("Current Balance   : Rs. %d\n",
                       balance);

                pauseScreen();
                break;


            case 0:
                return;


            default:

                printf(RED
                       "Invalid choice. Please select 0 to 5.\n"
                       RESET);
        }
    }
}



  /* =========================================================
   STOCK MARKET - AI TRADING SIMULATOR
   ========================================================= */

#define STOCK_COUNT 3

void aiStockAnalysis(
    double buyValue,
    double sellValue,
    double profitLoss,
    double percentage,
    int holdingDays)
{
    printf("\n");
    printf(CYAN "========== COMPUTER AI ANALYSIS ==========\n" RESET);

    if (profitLoss > 0)
    {
        printf(GREEN "Result: PROFIT\n" RESET);

        if (percentage >= 20)
            printf("AI: Strong positive trade.\n");
        else if (percentage >= 10)
            printf("AI: Good positive trade.\n");
        else
            printf("AI: Small but positive trade.\n");

        if (holdingDays <= 7)
            printf("AI: Short-term profitable trade.\n");
        else
            printf("AI: The stock was held for %d days.\n",
                   holdingDays);
    }
    else if (profitLoss < 0)
    {
        printf(RED "Result: LOSS\n" RESET);

        if (percentage <= -20)
            printf("AI: High loss detected. Risk was significant.\n");
        else
            printf("AI: The selling price was below the buying price.\n");

        printf("AI: Review the trade before making another decision.\n");
    }
    else
    {
        printf(YELLOW "Result: NO PROFIT / NO LOSS\n" RESET);
        printf("AI: Buying and selling values were equal.\n");
    }

    printf("\n");
    printf("Investment Value : Rs. %.2f\n", buyValue);
    printf("Selling Value    : Rs. %.2f\n", sellValue);
    printf("Holding Days     : %d\n", holdingDays);

    printf("==========================================\n");
}


/* =========================================================
   STOCK MARKET MENU
   ========================================================= */

void stockMarketMenu(void)
{
    char *stockNames[STOCK_COUNT] =
    {
        "TechCorp",
        "FutureSoft",
        "GreenEnergy"
    };

    double stockPrices[STOCK_COUNT] =
    {
        500.0,
        750.0,
        1000.0
    };

    int ownedShares[STOCK_COUNT] = {0};

    double averageBuyPrice[STOCK_COUNT] = {0};

    int firstBuyDay[STOCK_COUNT] = {0};

    double cash = 10000.0;

    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);

        printf("                    STOCK MARKET\n");

        printLine(60);
        printf(RESET);

        printf(GREEN
               "Cash Available: Rs. %.2f\n"
               RESET,
               cash);

        printf("\n");

        printf(CYAN "Available Stocks\n\n" RESET);

        printf("[1] %-15s Rs. %.2f\n",
               stockNames[0],
               stockPrices[0]);

        printf("[2] %-15s Rs. %.2f\n",
               stockNames[1],
               stockPrices[1]);

        printf("[3] %-15s Rs. %.2f\n",
               stockNames[2],
               stockPrices[2]);

        printf("\n");

        printf(BLUE
               "   [4] Buy Stock\n"
               RESET);

        printf(YELLOW
               "   [5] Sell Stock\n"
               RESET);

        printf(MAGENTA
               "   [6] Trade Analysis\n"
               RESET);

        printf(CYAN
               "   [7] View Portfolio\n"
               RESET);

        printf(WHITE
               "   [0] Back to Smart Systems\n"
               RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();

            printf(RED
                   "Invalid input.\n"
                   RESET);

            continue;
        }

        clearInputBuffer();


        /* =================================================
           BUY STOCK
           ================================================= */

        if (choice == 4)
        {
            int stock;
            int shares;
            double buyPrice;
            int buyDay;
            double totalCost;

            printf("\n");
            printf(BLUE "========== BUY STOCK ==========\n" RESET);

            printf("[1] TechCorp     Rs. %.2f\n",
                   stockPrices[0]);

            printf("[2] FutureSoft   Rs. %.2f\n",
                   stockPrices[1]);

            printf("[3] GreenEnergy  Rs. %.2f\n",
                   stockPrices[2]);

            printf("\nSelect stock: ");

            if (scanf("%d", &stock) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid stock.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (stock < 1 || stock > STOCK_COUNT)
            {
                printf(RED "Invalid stock selection.\n" RESET);
                continue;
            }

            printf("Enter number of shares: ");

            if (scanf("%d", &shares) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid number of shares.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (shares <= 0)
            {
                printf(RED "Shares must be greater than zero.\n" RESET);
                continue;
            }

            printf("Enter actual Buy Price per share: Rs. ");

            if (scanf("%lf", &buyPrice) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid price.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (buyPrice <= 0)
            {
                printf(RED "Price must be greater than zero.\n" RESET);
                continue;
            }

            printf("Enter Buy Day: ");

            if (scanf("%d", &buyDay) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid day.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (buyDay < 1)
            {
                printf(RED "Day must be 1 or greater.\n" RESET);
                continue;
            }

            totalCost = buyPrice * shares;

            if (totalCost > cash)
            {
                printf(RED
                       "\nInsufficient cash.\n"
                       RESET);

                printf("Required: Rs. %.2f\n",
                       totalCost);

                printf("Available: Rs. %.2f\n",
                       cash);

                continue;
            }

            /*
               Calculate average buying price
               when the user buys the same stock again.
            */

            if (ownedShares[stock - 1] == 0)
            {
                averageBuyPrice[stock - 1] = buyPrice;
                firstBuyDay[stock - 1] = buyDay;
            }
            else
            {
                double oldValue =
                    averageBuyPrice[stock - 1] *
                    ownedShares[stock - 1];

                double newValue =
                    buyPrice * shares;

                int totalShares =
                    ownedShares[stock - 1] + shares;

                averageBuyPrice[stock - 1] =
                    (oldValue + newValue) /
                    totalShares;

                if (buyDay < firstBuyDay[stock - 1])
                    firstBuyDay[stock - 1] = buyDay;
            }

            ownedShares[stock - 1] += shares;

            cash -= totalCost;

            printf("\n");
            printf(GREEN
                   "Stock purchased successfully!\n"
                   RESET);

            printf("Stock        : %s\n",
                   stockNames[stock - 1]);

            printf("Shares       : %d\n",
                   shares);

            printf("Buy Price    : Rs. %.2f\n",
                   buyPrice);

            printf("Buy Day      : %d\n",
                   buyDay);

            printf("Total Cost   : Rs. %.2f\n",
                   totalCost);

            pauseScreen();
        }


        /* =================================================
           SELL STOCK
           ================================================= */

        else if (choice == 5)
        {
            int stock;
            int shares;
            double sellPrice;
            int sellDay;

            double buyValue;
            double sellValue;
            double profitLoss;
            double percentage;

            int holdingDays;

            printf("\n");
            printf(YELLOW "========== SELL STOCK ==========\n" RESET);

            printf("[1] TechCorp\n");
            printf("[2] FutureSoft\n");
            printf("[3] GreenEnergy\n");

            printf("\nSelect stock: ");

            if (scanf("%d", &stock) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid stock.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (stock < 1 || stock > STOCK_COUNT)
            {
                printf(RED "Invalid stock selection.\n" RESET);
                continue;
            }

            if (ownedShares[stock - 1] <= 0)
            {
                printf(RED
                       "You do not own this stock.\n"
                       RESET);

                pauseScreen();
                continue;
            }

            printf("Owned Shares: %d\n",
                   ownedShares[stock - 1]);

            printf("Enter shares to sell: ");

            if (scanf("%d", &shares) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid number of shares.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (shares <= 0 ||
                shares > ownedShares[stock - 1])
            {
                printf(RED
                       "Invalid number of shares.\n"
                       RESET);

                continue;
            }

            printf("Enter actual Sell Price per share: Rs. ");

            if (scanf("%lf", &sellPrice) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid price.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (sellPrice <= 0)
            {
                printf(RED "Price must be greater than zero.\n" RESET);
                continue;
            }

            printf("Enter Sell Day: ");

            if (scanf("%d", &sellDay) != 1)
            {
                clearInputBuffer();

                printf(RED "Invalid day.\n" RESET);
                continue;
            }

            clearInputBuffer();

            if (sellDay < firstBuyDay[stock - 1])
            {
                printf(RED
                       "Sell Day cannot be before Buy Day.\n"
                       RESET);

                continue;
            }

            /* Calculate trade */

            buyValue =
                averageBuyPrice[stock - 1] *
                shares;

            sellValue =
                sellPrice *
                shares;

            profitLoss =
                sellValue -
                buyValue;

            if (buyValue > 0)
            {
                percentage =
                    (profitLoss / buyValue) * 100.0;
            }
            else
            {
                percentage = 0;
            }

            holdingDays =
                sellDay -
                firstBuyDay[stock - 1];

            /* Add selling money to cash */

            cash += sellValue;

            ownedShares[stock - 1] -= shares;

            /*
               If all shares are sold,
               reset stored buying information.
            */

            printf("Percentage     : %.2f%%\n", percentage);
            if (ownedShares[stock - 1] == 0)
            {
              averageBuyPrice[stock - 1] = 0;
              firstBuyDay[stock - 1] = 0;
            }
            printf("\n");
            printf(GREEN
                   "Stock sold successfully!\n"
                   RESET);

            printf("\n");
            printf("Stock          : %s\n",
                   stockNames[stock - 1]);

            printf("Shares Sold    : %d\n",
                   shares);

            printf("Buy Price      : Rs. %.2f\n",
                   averageBuyPrice[stock - 1] == 0
                       ? buyValue / shares
                       : averageBuyPrice[stock - 1]);

            printf("Sell Price     : Rs. %.2f\n",
                   sellPrice);

            printf("Buy Day        : %d\n",
                   firstBuyDay[stock - 1]);

            printf("Sell Day       : %d\n",
                   sellDay);

            printf("Holding Days   : %d\n",
                   holdingDays);

            printf("Buying Value   : Rs. %.2f\n",
                   buyValue);

            printf("Selling Value  : Rs. %.2f\n",
                   sellValue);

            if (profitLoss > 0)
            {
                printf(GREEN
                       "Profit         : Rs. %.2f\n"
                       RESET,
                       profitLoss);
            }
            else if (profitLoss < 0)
            {
                printf(RED
                       "Loss           : Rs. %.2f\n"
                       RESET,
                       -profitLoss);
            }
            else
            {
                printf(YELLOW
                       "Profit/Loss    : Rs. 0.00\n"
                       RESET);
            }

            printf("Percentage     : %.2f%%\n",
                   percentage);

            /* Computer AI */

            aiStockAnalysis(
                buyValue,
                sellValue,
                profitLoss,
                percentage,
                holdingDays);

            pauseScreen();
        }


        /* =================================================
           TRADE ANALYSIS
           ================================================= */

        else if (choice == 6)
        {
            int stock;

            printf("\n");
            printf(MAGENTA
                   "========== TRADE ANALYSIS ==========\n"
                   RESET);

            for (stock = 0;
                 stock < STOCK_COUNT;
                 stock++)
            {
                printf("\n[%d] %s\n",
                       stock + 1,
                       stockNames[stock]);

                printf("Owned Shares : %d\n",
                       ownedShares[stock]);

                if (ownedShares[stock] > 0)
                {
                    printf("Average Buy Price : Rs. %.2f\n",
                           averageBuyPrice[stock]);

                    printf("First Buy Day : %d\n",
                           firstBuyDay[stock]);

                    printf("Current Market Value : Rs. %.2f\n",
                           stockPrices[stock] *
                           ownedShares[stock]);
                }
                else
                {
                    printf("No active position.\n");
                }
            }

            pauseScreen();
        }


        /* =================================================
           VIEW PORTFOLIO
           ================================================= */

        else if (choice == 7)
        {
            int i;

            double portfolioValue = 0;

            printf("\n");
            printf(CYAN
                   "========== YOUR PORTFOLIO ==========\n"
                   RESET);

            printf("\n");

            for (i = 0; i < STOCK_COUNT; i++)
            {
                printf("[%d] %s\n",
                       i + 1,
                       stockNames[i]);

                printf("Owned Shares : %d\n",
                       ownedShares[i]);

                if (ownedShares[i] > 0)
                {
                    printf("Average Buy : Rs. %.2f\n",
                           averageBuyPrice[i]);

                    printf("Current Price: Rs. %.2f\n",
                           stockPrices[i]);

                    printf("Market Value : Rs. %.2f\n",
                           stockPrices[i] *
                           ownedShares[i]);

                    portfolioValue +=
                        stockPrices[i] *
                        ownedShares[i];
                }

                printf("\n");
            }

            printf(GREEN
                   "Cash           : Rs. %.2f\n"
                   RESET,
                   cash);

            printf(CYAN
                   "Stock Value    : Rs. %.2f\n"
                   RESET,
                   portfolioValue);

            printf(WHITE
                   "Total Portfolio: Rs. %.2f\n"
                   RESET,
                   cash + portfolioValue);

            pauseScreen();
        }


        /* =================================================
           BACK
           ================================================= */

        else if (choice == 0)
        {
            return;
        }


        /* =================================================
           INVALID
           ================================================= */

        else
        {
            printf(RED
                   "Please choose a valid option.\n"
                   RESET);
        }
    }
}

/* =========================================================
   SMART SYSTEMS MENU
   ========================================================= */

void smartSystemsMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                   SMART SYSTEMS\n");
        printLine(60);
        printf(RESET);

        printf(GREEN
               "   [1] Banking System\n"
               RESET);

        printf(CYAN
               "   [2] Stock Market\n"
               RESET);

        printf(WHITE
               "   [0] Back to Toolbox\n"
               RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();

            printf(RED
                   "Invalid input. Please enter a number.\n"
                   RESET);

            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                bankingSystem();
                break;

            case 2:
                stockMarketMenu();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0, 1 or 2.\n"
                       RESET);
        }
    }
}





void showCurrentTime(void);
void showCalendar(void);
/* =========================================================
   DAILY HUB
   ========================================================= */

int isLeapYear(int year)
{
    if (year % 400 == 0)
        return 1;

    if (year % 100 == 0)
        return 0;

    if (year % 4 == 0)
        return 1;

    return 0;
}

/* ---------------------------------------------------------
   DAYS IN MONTH
   --------------------------------------------------------- */

int daysInMonth(int month, int year)
{
    switch (month)
    {
        case 1:
            return 31;

        case 2:
            if (isLeapYear(year))
                return 29;
            else
                return 28;

        case 3:
            return 31;

        case 4:
            return 30;

        case 5:
            return 31;

        case 6:
            return 30;

        case 7:
            return 31;

        case 8:
            return 31;

        case 9:
            return 30;

        case 10:
            return 31;

        case 11:
            return 30;

        case 12:
            return 31;

        default:
            return 0;
    }
}

/* ---------------------------------------------------------
   DAY OF WEEK
   0 = Sunday
   1 = Monday
   ...
   6 = Saturday
   --------------------------------------------------------- */

int getFirstDay(int month, int year)
{
    int day = 1;
    int m = month;
    int y = year;

    if (m < 3)
    {
        m += 12;
        y--;
    }

    return (day +
            (13 * (m + 1)) / 5 +
            y +
            y / 4 -
            y / 100 +
            y / 400) % 7;
}

/* ---------------------------------------------------------
   SHOW CURRENT TIME
   --------------------------------------------------------- */

void showCurrentTime(void)
{
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    printf("\n");
    printf(CYAN);
    printLine(50);
    printf("                     CURRENT TIME\n");
    printLine(50);
    printf(RESET);

    printf("\n");

    printf(WHITE "Date : %02d-%02d-%04d\n" RESET,
           localTime->tm_mday,
           localTime->tm_mon + 1,
           localTime->tm_year + 1900);

    printf(GREEN "Time : %02d:%02d:%02d\n" RESET,
           localTime->tm_hour,
           localTime->tm_min,
           localTime->tm_sec);

    printf("\n");
}

/* ---------------------------------------------------------
   SHOW CALENDAR
   --------------------------------------------------------- */

void showCalendar(void)
{
    int month;
    int year;
    int firstDay;
    int totalDays;
    int i;

    const char *months[] =
    {
        "",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    printf("\n");
    printf(MAGENTA);
    printLine(60);
    printf("                         CALENDAR\n");
    printLine(60);
    printf(RESET);

    printf("\nEnter year: ");

    if (scanf("%d", &year) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid year.\n" RESET);
        return;
    }

    printf("Enter month (1-12): ");

    if (scanf("%d", &month) != 1)
    {
        clearInputBuffer();
        printf(RED "Invalid month.\n" RESET);
        return;
    }

    clearInputBuffer();

    if (month < 1 || month > 12)
    {
        printf(RED "Please enter a month from 1 to 12.\n" RESET);
        return;
    }

    if (year < 1)
    {
        printf(RED "Please enter a valid year.\n" RESET);
        return;
    }

    firstDay = getFirstDay(month, year);

    /*
       Zeller's formula returns:
       0 = Saturday
       1 = Sunday
       2 = Monday
       ...
       6 = Friday

       Convert it to:
       0 = Sunday
       1 = Monday
       ...
       6 = Saturday
    */

    firstDay = (firstDay + 6) % 7;

    totalDays = daysInMonth(month, year);

    printf("\n");
    printf(WHITE "                 %s %d\n\n" RESET,
           months[month], year);

    printf(WHITE
           " Sun  Mon  Tue  Wed  Thu  Fri  Sat\n"
           RESET);

    for (i = 0; i < firstDay; i++)
    {
        printf("     ");
    }

    for (i = 1; i <= totalDays; i++)
    {
        printf("%4d ", i);

        if ((i + firstDay) % 7 == 0)
            printf("\n");
    }

    printf("\n");
}

/* ---------------------------------------------------------
   DAILY HUB MENU
   --------------------------------------------------------- */
   
void dailyHubMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf(WHITE);
        printLine(60);
        printf("                         DAILY HUB\n");
        printLine(60);
        printf(RESET);

        printf(CYAN   "   [1] Current Time\n" RESET);
        printf(MAGENTA"   [2] Calendar\n" RESET);
        printf(WHITE  "   [0] Back to Toolbox\n" RESET);

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            printf(RED "Invalid input. Please enter a number.\n" RESET);
            continue;
        }

        clearInputBuffer();

        switch (choice)
        {
            case 1:
                showCurrentTime();
                pauseScreen();
                break;

            case 2:
                showCalendar();
                pauseScreen();
                break;

            case 0:
                return;

            default:
                printf(RED
                       "Invalid choice. Please select 0, 1 or 2.\n"
                       RESET);
        }
    }
}