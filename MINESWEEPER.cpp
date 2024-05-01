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

public:
    // check if the selected land is within the grid
    bool isValid(int x, int y)
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return true;
        }

        return false;
    }

    // Constructor to initialize the game with given dimensions and number of mines
    Minesweeper(int w, int h, int m)
    {
        width = w;
        height = h;
        mines = m;
        remainingSafeSquares = 0;

        // dynamic 2d arrays initialization
        // array of int pointers
        grid = new int *[height];
        // array of bool pointers
        revealed = new bool *[height];
        // array of bool pointers
        flagged = new bool *[height];

        // for each row
        for (int i = 0; i < height; i++)
        {
            // each row contains another array
            grid[i] = new int[width];
            revealed[i] = new bool[width];
            flagged[i] = new bool[width];

            // for each column
            for (int j = 0; j < width; j++)
            {
                // set every element to default values
                grid[i][j] = 0;
                revealed[i][j] = false;
                flagged[i][j] = false;
            }
        }

        // randomly place mines on the grid
        placeMines();
    }

    // destructor
    ~Minesweeper()
    {
        // deallocate each row array
        for (int i = 0; i < height; i++)
        {
            delete[] grid[i];
            delete[] revealed[i];
            delete[] flagged[i];
        }

        // deallocate the array itself
        delete[] grid;
        delete[] revealed;
        delete[] flagged;
    }

    // randomly place mines on the grid
    // randomly generate x and y coordinates
    // then check if the land already has mine or not
    // if it has mine, randomly generate coordinates again
    // if it doesn't have mine, place it there
    void placeMines()
    {
        srand((unsigned)time(NULL)); // random number generator

        // for each mine
        for (int i = 0; i < mines; i++)
        {
            int x, y;
            do
            {
                // random number coordinates
                x = rand() % height;
                y = rand() % width;
            } while (grid[x][y] == -1); // repeat until placing at unmined cell
            grid[x][y] = -1;
        }

        // for each land
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                // if the land doesn't have mine
                if (grid[x][y] != -1)
                {
                    // assign the number for the land
                    // which is the number of adjacent mines from the selected land
                    grid[x][y] = countMines(x, y);

                    // keep a track of the number of non-mine square
                    remainingSafeSquares++;
                }
            }
        }
    }

    // count number of mines adjacent to the selected land
    int countMines(int x, int y)
    {
        int count = 0;

        // for adjacent land x-coordinates
        for (int dx = -1; dx <= 1; dx++)
        {
            // for adjacent land y-coordinates
            for (int dy = -1; dy <= 1; dy++)
            {
                // skip the selected land coordinate itself
                if (dx == 0 && dy == 0)
                {
                    continue;
                }

                // adjacent land x and y coordinates
                int adjLandX = x + dx, adjLandY = y + dy;

                // if it is within the grid and has mine
                if (isValid(adjLandX, adjLandY) && grid[adjLandX][adjLandY] == -1)
                {
                    // no. of adjacent mines++
                    count++;
                }
            }
        }

        return count;
    }

    // displaying the grid
    void display()
    {
        // show the remaining safe cells
        cout << "\nRemaining Safe Squares: " << remainingSafeSquares << endl
             << endl;

        // print the column numbers at the top
        cout << "    ";
        for (int y = 0; y < width; y++)
        {
            // for single digit column numbers
            if (y < 10)
            {
                // extra space
                cout << " " << y << "  ";
            }
            // for double digit column numbers
            else
            {
                cout << y << "  ";
            }
        }
        cout << endl;

        // print the top border
        cout << "   +";
        for (int y = 0; y < width; y++)
        {
            cout << "---+";
        }
        cout << endl;

        // for each row
        for (int x = 0; x < height; x++)
        {
            // for sing digit row
            if (x < 10)
            {
                // extra space
                cout << " " << x << " |";
            }
            // for double digit row
            else
            {
                cout << x << " |";
            }

            // for each column of the current row
            for (int y = 0; y < width; y++)
            {
                // if the cell is revealed
                if (revealed[x][y])
                {
                    // if the cell has mine
                    if (grid[x][y] == -1)
                    {
                        // display *
                        cout << " * |";
                    }
                    // if not mined
                    else
                    {
                        // display the number for the cell (no. of mines around it)
                        cout << " " << grid[x][y] << " |";
                    }
                }
                // if cell is flagged
                else if (flagged[x][y])
                {
                    // display F
                    cout << " F |";
                }
                // if cell is unopened
                else
                {
                    // display empty
                    cout << "   |";
                }
            }
            cout << endl;

            // print border after each row
            cout << "   +";
            for (int y = 0; y < width; y++)
            {
                cout << "---+";
            }
            cout << endl;
        }
    }

    // checking for mine
    bool checkMined(int x, int y)
    {
        // if the cell has mine
        if (grid[x][y] == -1)
        {
            return true;
        }

        return false;
    }

    // opening the cell
    void openCell(int x, int y)
    {
        // if the selected cell is outside the grid or opened or flagged
        // don't try opening it
        if (!isValid(x, y) || revealed[x][y] || flagged[x][y])
        {
            return;
        }

        // open the cell
        revealed[x][y] = true;

        // if the cell has mine
        if (checkMined(x, y))
        {
            return;
        }

        // decrement count of unrevealed safe squares
        remainingSafeSquares--;

        // if opened the cell with number 0
        // it should open the adjacent cells as well
        if (grid[x][y] == 0)
        {
            // for adjacent x coordinate cells
            for (int dx = -1; dx <= 1; dx++)
            {
                // for adjacent y coordinate cells
                for (int dy = -1; dy <= 1; dy++)
                {
                    // for each adjacent cell coordinate
                    int adjLandX = x + dx, adjLandY = y + dy;

                    // if it is valid and unopened yet
                    if (isValid(adjLandX, adjLandY) && !revealed[adjLandX][adjLandY])
                    {
                        // recursively open the cell
                        openCell(adjLandX, adjLandY);
                    }
                }
            }
        }

        return;
    }

    // flagging / unflagging
    void toggleFlag(int x, int y)
    {
        // if not opened
        if (!revealed[x][y])
        {
            // toggle the flag
            flagged[x][y] = !flagged[x][y];
        }
    }

    // check if the player has won the game
    bool checkWin()
    {
        // if there is no remaining safe cell
        if (remainingSafeSquares == 0)
        {
            return true;
        }

        return false;
    }
};

// main function
int main()
{
    // random number generator
    srand((unsigned)time(NULL));

    // repeat
    while (true)
    {
        // display main menu
        cout << "Main Menu\n\n";
        cout << "1. Play\n";
        cout << "2. Help\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        // play the game
        case 1:
        {
            cout << "\nWelcome to Minesweeper Mania!!!\n";
            cout << "\nSelect Difficulty:\n";
            cout << "1) Easy (grid size: 6x6, No. of mines = 6)\n";
            cout << "2) Medium (grid size: 12x12, No. of mines = 24)\n";
            cout << "3) Hard (grid size: 25x25, No. of mines = 104)\n";
            cout << "4) Custom\n";
            cout << "\nEnter your choice: ";
            cin >> choice;

            // validation check
            while (choice < 1 || choice > 4)
            {
                cout << "Invalid choice. Please select again: ";
                cin >> choice;
            }

            int width, height, mines;
            switch (choice)
            {
            // easy
            case 1:
                width = height = 6;
                mines = 6;
                break;
            // medium
            case 2:
                width = height = 12;
                mines = 24;
                break;
            // hard
            case 3:
                width = height = 25;
                mines = 104;
                break;
            // custom
            case 4:
                cout << " Please enter the row/column (6~25): " << endl;
                cin >> width;
                // validation check
                while (width < 6 || width > 25)
                {
                    cout << "Invalid choice. Please select again: ";
                    cin >> width;
                }
                height = width;
                mines = (width * height) / 6;
                break;
            default:
                continue;
            }

            // create game object
            Minesweeper game(width, height, mines);
            bool gameOver = false;

            // while not over yet
            while (!gameOver)
            {
                game.display();
                cout << "\nChoose an action:\n1. Open a cell\n2. Flag/Unflag a cell\n3. Exit to main menu\n";
                cin >> choice;

                // validation check
                while (choice < 1 || choice > 3)
                {
                    cout << "Invalid choice. Please select again: ";
                    cin >> choice;
                }
                switch (choice)
                {
                // open a cell
                case 1:
                    cout << "Enter row and column number to select the square: ";
                    int x, y;
                    cin >> x >> y;

                    // validation check
                    while (!game.isValid(x, y))
                    {
                        cout << "Invalid choice. Please select again: ";
                        cin >> x >> y;
                    }

                    // open the cell
                    game.openCell(x, y);

                    // if the cell has mine
                    if (game.checkMined(x, y))
                    {
                        // game over
                        cout << "\nGame Over! You hit a mine!\n";
                        game.display();
                        gameOver = true;
                    }

                    // if the player opened all the safe cells
                    if (game.checkWin())
                    {
                        cout << "\nCongratulations! You cleared the minefield!\n";
                        gameOver = true;
                    }
                    break;
                // flag
                case 2:
                    cout << "Enter row and column numbers to flag/unflag: ";
                    cin >> x >> y;

                    // validation check
                    while (!game.isValid(x, y))
                    {
                        cout << "Invalid choice. Please select again: ";
                        cin >> x >> y;
                    }
                    game.toggleFlag(x, y);
                    break;
                // exit
                case 3:
                    gameOver = true;
                    break;
                }
            }
            break;
        }
        // help menu with instructions
        case 2:
        {
            cout << endl;
            cout << "Welcome to Minesweeper Mania!!!\nMinesweeper is a classic single-player puzzle game where the objective is to clear a rectangular grid containing hidden mines without detonating any of them.\n\n";
            cout << "How to Use the Program:\n";
            cout << "1. Upon starting the program, you'll be presented with a main menu offering several options.\n";
            cout << "2. Choose 'Play' to start a new game, 'Help' to view instructions, or 'Exit' to leave the game.\n";
            cout << "3. If you choose 'Play', you'll be prompted to select a difficulty level: Easy, Medium, Hard, or Custom.\n";
            cout << "4. In Custom mode, you can specify the grid size and number of mines.\n";
            cout << "5. During the game, you'll be able to open cells, flag or unflag cells as mines, or return to the main menu.\n\n";
            cout << "How to Play the Game:\n";
            cout << "1. The game grid consists of cells, some of which contain mines.\n";
            cout << "2. Your goal is to uncover all cells that do not contain mines.\n";
            cout << "3. To uncover a cell, select 'Open a cell' and enter the row and column numbers of the cell you want to reveal.\n";
            cout << "4. If the cell you open contains a mine, you lose the game.\n";
            cout << "5. If the cell does not contain a mine, it will display a number indicating how many mines are adjacent to it.\n";
            cout << "6. Use the numbers to strategically uncover safe cells.\n";
            cout << "7. You can flag cells that you suspect contain mines to prevent accidentally opening them.\n";
            cout << "8. To flag or unflag a cell, choose 'Flag/Unflag a cell' and enter the row and column numbers.\n";
            cout << "9. Continue uncovering cells and flagging mines until all non-mine cells are revealed.\n";
            cout << "10. If you successfully uncover all safe cells without hitting any mines, you win the game!\n\n";
            cout << "Remember:\n";
            cout << "- Be cautious when opening cells, as hitting a mine ends the game.\n";
            cout << "- Use logic and deduction to determine the locations of mines.\n";
            cout << "- Pay attention to the numbers displayed in uncovered cells to help identify safe moves.\n\n";
            break;
        }
        // exit program
        case 3:
            cout << "Exiting game.\n";
            return 0;
        // on invalid input
        default:
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
