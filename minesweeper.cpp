#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Minesweeper
{
private:
    std::vector<std::vector<int> > grid;
    std::vector<std::vector<bool> > revealed;
    std::vector<std::vector<bool> > flagged;
    int width, height, mines;

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
                }
            }
        }
    }

public:
    Minesweeper(int w, int h, int m) : width(w), height(h), mines(m)
    {
        grid.resize(height, std::vector<int>(width, 0));
        revealed.resize(height, std::vector<bool>(width, false));
        flagged.resize(height, std::vector<bool>(width, false));
        placeMines();
    }

    void display(bool showMines = false)
    {
        // Print the top column labels
        std::cout << "   "; // Space for row numbers
        for (int y = 0; y < width; y++) {
            std::cout << " " << y << "  ";
        }
        std::cout << std::endl;

        // Print the top border
        std::cout << "  +";
        for (int y = 0; y < width; y++) {
            std::cout << "---+";
        }
        std::cout << std::endl;

        for (int x = 0; x < height; x++)
        {
            std::cout << x << " |";
            for (int y = 0; y < width; y++)
            {
                if (revealed[x][y])
                {
                    if (grid[x][y] == -1)
                    {
                        std::cout << " * |"; // Mine
                    }
                    else
                    {
                        std::cout << " " << grid[x][y] << " |"; // Number of mines around
                    }
                }
                else if (flagged[x][y])
                {
                    std::cout << " F |"; // Flagged
                }
                else
                {
                    std::cout << "   |"; // Unrevealed
                }
            }
            std::cout << std::endl;

            // Print border after each row
            std::cout << "  +";
            for (int y = 0; y < width; y++) {
                std::cout << "---+";
            }
            std::cout << std::endl;
        }
    }


    bool openCell(int x, int y)
    {
        if (!isValid(x, y) || revealed[x][y] || flagged[x][y])
        {
            return true;
        }

        revealed[x][y] = true;
        if (grid[x][y] == -1)
        {
            return false;
        }

        if (grid[x][y] == 0)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if (isValid(nx, ny) && !revealed[nx][ny])
                    {
                        openCell(nx, ny);
                    }
                }
            }
        }
        return true;
    }

    void toggleFlag(int x, int y)
    {
        if (!revealed[x][y])
        {
            flagged[x][y] = !flagged[x][y];
        }
    }

    bool checkWin()
    {
        for (int x = 0; x < height; x++)
        {
            for (int y = 0; y < width; y++)
            {
                if (!revealed[x][y] && grid[x][y] != -1)
                {
                    return false;
                }
            }
        }
        return true;
    }
};

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Assume the rest of the Minesweeper class is defined here.

int main() {
    srand(time(nullptr)); // Seed the random number generator

    while (true) {
        std::cout << "Main Menu\n";
        std::cout << "1. Play\n";
        std::cout << "2. Help\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int width = 10, height = 10, mines = 10;
                Minesweeper game(width, height, mines);
                bool gameOver = false;
                while (!gameOver) {
                    game.display();
                    std::cout << "Choose an action:\n1. Open a cell\n2. Flag/Unflag a cell\n3. Exit to main menu\n";
                    std::cin >> choice;
                    switch (choice) {
                        case 1:
                            std::cout << "Enter row and column numbers: ";
                            int x, y;
                            std::cin >> x >> y;
                            if (!game.openCell(x, y)) {
                                std::cout << "Game Over! You hit a mine!\n";
                                game.display(true);
                                gameOver = true;
                            }
                            if (game.checkWin()) {
                                std::cout << "Congratulations! You cleared the minefield!\n";
                                gameOver = true;
                            }
                            break;
                        case 2:
                            std::cout << "Enter row and column numbers to flag/unflag: ";
                            std::cin >> x >> y;
                            game.toggleFlag(x, y);
                            break;
                        case 3:
                            gameOver = true;
                            break;
                        default:
                            std::cout << "Invalid option. Please choose again.\n";
                            break;
                    }
                }
                break;
            }
            case 2: {
                std::cout << "Help:\n";
                std::cout << "- Choose 'Open a cell' to reveal what is underneath that cell.\n";
                std::cout << "- Choose 'Flag/Unflag a cell' to mark a cell you suspect contains a mine.\n";
                std::cout << "- Uncover all cells without mines to win the game.\n";
                std::cout << "- Hitting a mine results in a game over.\n";
                break;
            }
            case 3:
                std::cout << "Exiting game.\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
        }
    }

    return 0;
}
