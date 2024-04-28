#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Minesweeper
{
private:
    int **grid;
    bool **revealed;
    bool **flagged;
    int width, height, mines;
    int remainingSafeSquares; // Variable to track remaining non-mine squares

    bool isValid(int x, int y)
    {
        return (x >= 0 && x < height && y >= 0 && y < width);
    }

    int countMines(int x, int y)
    {
        int count = 0;
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0)
                    continue;
                int nx = x + dx, ny = y + dy;
                if (isValid(nx, ny) && grid[nx][ny] == -1)
                {
                    count++;
                }
            }
        }
        return count;
    }

    void placeMines()
    {
        srand(time(nullptr)); // Seed the random number generator
        for (int i = 0; i < mines; i++)
        {
            int x, y;
            do
            {
                x = rand() % height;
                y = rand() % width;
            } while (grid[x][y] == -1);
            grid[x][y] = -1;
        }

        for (int x = 0; x < height; x++)
        {
            for (int y = 0; y < width; y++)
            {
                if (grid[x][y] != -1)
                {
                    grid[x][y] = countMines(x, y);
                    remainingSafeSquares++; // Count each non-mine square
                }
            }
        }
    }

public:
    Minesweeper(int w, int h, int m) : width(w), height(h), mines(m), remainingSafeSquares(0)
    {
        grid = new int *[height];
        revealed = new bool *[height];
        flagged = new bool *[height];
        for (int i = 0; i < height; i++)
        {
            grid[i] = new int[width];
            revealed[i] = new bool[width];
            flagged[i] = new bool[width];
            for (int j = 0; j < width; j++)
            {
                grid[i][j] = 0;
                revealed[i][j] = false;
                flagged[i][j] = false;
            }
        }
        placeMines();
    }

    ~Minesweeper()
    {
        for (int i = 0; i < height; i++)
        {
            delete[] grid[i];
            delete[] revealed[i];
            delete[] flagged[i];
        }
        delete[] grid;
        delete[] revealed;
        delete[] flagged;
    }

    void display(bool showMines = false)
    {
        cout << "Remaining Safe Squares: " << remainingSafeSquares << endl;

        // Print the top column labels
        cout << "    "; // Add extra space before column numbers for alignment
        for (int y = 0; y < width; y++)
        {
            if (y < 10)
            {
                cout << " " << y << "  "; // Extra space for single digit columns
            }
            else
            {
                cout << y << "  "; // Normal spacing for double digits
            }
        }
        cout << endl;

        // Print the top border
        cout << "   +"; // Align the start of the grid
        for (int y = 0; y < width; y++)
        {
            cout << "---+";
        }
        cout << endl;

        for (int x = 0; x < height; x++)
        {
            // Handle single and double-digit row numbers
            if (x < 10)
            {
                cout << " " << x << " |"; // Extra space for single digit rows
            }
            else
            {
                cout << x << " |"; // Normal for double digits
            }

            for (int y = 0; y < width; y++)
            {
                if (revealed[x][y])
                {
                    if (grid[x][y] == -1)
                    {
                        cout << " * |"; // Mine
                    }
                    else
                    {
                        cout << " " << grid[x][y] << " |"; // Number of mines around
                    }
                }
                else if (flagged[x][y])
                {
                    cout << " F |"; // Flagged
                }
                else
                {
                    cout << "   |"; // Unrevealed
                }
            }
            cout << endl;

            // Print border after each row
            cout << "   +"; // Align the start of the row border
            for (int y = 0; y < width; y++)
            {
                cout << "---+";
            }
            cout << endl;
        }
    }

    bool openCell(int x, int y)
    {
        if (!isValid(x, y) || revealed[x][y] || flagged[x][y])
            return true;
        revealed[x][y] = true;
        if (grid[x][y] == -1)
            return false;       // Mine hit
        remainingSafeSquares--; // Decrement count of unrevealed safe squares
        if (grid[x][y] == 0)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if (isValid(nx, ny) && !revealed[nx][ny])
                        openCell(nx, ny);
                }
            }
        }
        return true;
    }

    void toggleFlag(int x, int y)
    {
        if (!revealed[x][y])
            flagged[x][y] = !flagged[x][y];
    }

    bool checkWin()
    {
        for (int x = 0; x < height; x++)
        {
            for (int y = 0; y < width; y++)
            {
                if (!revealed[x][y] && grid[x][y] != -1)
                    return false;
            }
        }
        return true;
    }
};

int main()
{
    srand(time(nullptr)); // Seed the random number generator

    while (true)
    {
        cout << "Main Menu\n";
        cout << "1. Play\n";
        cout << "2. Help\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Select Difficulty:\n";
            cout << "1) Easy (grid size: 6x6, No. of mines = 6)\n";
            cout << "2) Medium (grid size: 12x12, No. of mines = 24)\n";
            cout << "3) Hard (grid size: 25x25, No. of mines = 104)\n";
            cout << "4) Custom\n";
            cout << "Enter your choice: ";
            cin >> choice;
            int width, height, mines;
            switch (choice)
            {
            case 1:
                width = height = 6;
                mines = 6;
                break;
            case 2:
                width = height = 12;
                mines = 24;
                break;
            case 3:
                width = height = 25;
                mines = 104;
                break;
            case 4:
                cout << "Enter custom grid size (6~25): ";
                cin >> width;
                height = width;
                cout << "Number of mines (integer division of total squares by 6):" << endl;
                mines = (width * height) / 6;
                break;
            default:
                continue;
            }
            Minesweeper game(width, height, mines);
            bool gameOver = false;
            while (!gameOver)
            {
                game.display();
                cout << "Choose an action:\n1. Open a cell\n2. Flag/Unflag a cell\n3. Exit to main menu\n";
                cin >> choice;
                switch (choice)
                {
                case 1:
                    cout << "Enter row and column numbers: ";
                    int x, y;
                    cin >> x >> y;
                    if (!game.openCell(x, y))
                    {
                        cout << "Game Over! You hit a mine!\n";
                        game.display(true);
                        gameOver = true;
                    }
                    if (game.checkWin())
                    {
                        cout << "Congratulations! You cleared the minefield!\n";
                        gameOver = true;
                    }
                    break;
                case 2:
                    cout << "Enter row and column numbers to flag/unflag: ";
                    cin >> x >> y;
                    game.toggleFlag(x, y);
                    break;
                case 3:
                    gameOver = true;
                    break;
                default:
                    cout << "Invalid option. Please choose again.\n";
                    break;
                }
            }
            break;
        }
        case 2:
        {
            cout << "Help:\n";
            cout << "- Choose 'Open a cell' to reveal what is underneath that cell.\n";
            cout << "- Choose 'Flag/Unflag a cell' to mark a cell you suspect contains a mine.\n";
            cout << "- Uncover all cells without mines to win the game.\n";
            cout << "- Hitting a mine results in a game over.\n";
            break;
        }
        case 3:
            cout << "Exiting game.\n";
            return 0;
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
