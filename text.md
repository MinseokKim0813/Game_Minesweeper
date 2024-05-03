# Game_Minesweeper

## Introduction
This repository contains a Minesweeper game implemented in C++. The game utilizes principles of object-oriented programming (OOP), making extensive use of classes to encapsulate game functionalities. It features dynamic memory management and utilizes the C++ Standard Library's random number generation to place mines randomly on the game board.

## Features
- **Object-Oriented Design**: Structured using classes and objects that represent various components of the game such as the game board and game logic.
- **Random Mine Placement**: Each game session features a randomly generated minefield ensuring a unique challenge every time.
- **Dynamic Grid Management**: The game adapts dynamically to specified grid dimensions and number of mines.
- **Interactive Menu System**: Includes a main menu with options to play the game, access help information, or exit the application.

## How to Compile and Run
To compile and run the Minesweeper game, you will need a C++ compiler that supports C++11 or later. Follow these steps:

1. Clone the repository to your local machine:
   git clone https://github.com/Minseokkim0813/Game_Minesweeper.git
2. Navigate to the directory containing the game:
   cd Game_Minesweeper
3. Compile the game using a C++ compiler, for example g++:
   g++ -std=c++11 -o Minesweeper main.cpp
4. Run the game:
   ./Minesweeper

## How to Play
Start the Game: Choose "Play" from the main menu.
Open a Cell: Select the cell coordinates to reveal what lies beneath.
Flag a Mine: Mark suspected mines to avoid opening them by mistake.
Win the Game: Clear all cells without mines to win.

## Contributing
Contributions to this project are welcome! Please feel free to fork the repository, make changes, and submit pull requests. You can also send me an email if you have any questions or suggestions.

## Contact
For any issues, questions, or contributions, please contact mk7545@nyu.edu.
