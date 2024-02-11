/* Binairo
 *
 * Desc:
 *   This program implements a Binairo game with the gameboard of size 6 x 6.
 * Each square in the gameboard has zero, one, or empty. The aim is to add
 * zeros and ones in the empty squares by following the rules:
 * - each horizontal and vertical line can have at most three identical numbers
 * - each horizontal and vertical line can have at most two identical number
 *   sequentially.
 *   At first, the user is asked, if the gameboard will be filled with
 * randomly drawn characters, or with user-given 36 characters. In the first
 * option, a seed value for the random number generator will be asked next.
 * In the latter option, the user is asked to input 36 characters, the
 * validity of which is checked.
 *   On each round, the user is asked for the coordinates and the identity
 * of the character to be added, i.e. three characters. The player wins if
 * the gameboard has been filled following the rules above. The program does
 * not allow additions violating the above rules, but it is possible to end up
 * to a situation where no addition is possible any more.
 *   The program checks the user-given inputs. The character to be added must
 * be zero or one. The coordinates must be inside the gameboard, and the
 * square indicated by them must be empty.
 *
 * Program author
 * Name: Baqir Kazmi
 *
 * Notes about the program and it's implementation (if any):
 *
 * */
#include "gameboard.hh"
#include <iostream>

using namespace std;

// Tulosteet
// Output messages
const string QUIT = "Quitting ...";
const string OUT_OF_BOARD = "Out of board";
const string INVALID_INPUT = "Invalid input";
const string CANT_ADD = "Can't add";
const string WIN = "You won!";

// Converts the given numeric string to the corresponding integer
// (by calling stoi) and returns the integer.
// If the given string is not numeric, returns zero.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Removes empty characters (such as ' ' etc.) from the given string.
// Returns true if the given string has exactly one non-empty character,
// which is either '0' or '1', otherwise returns false.
bool find_fill_symbol(string& str)
{
    string fill_str = "";
    for(unsigned int i = 0; i < str.size(); ++i)
    {
        if(not isspace(str.at(i)))
        {
            fill_str += str.at(i);
        }
    }
    str = fill_str;
    return (fill_str.size() == 1 and
           (fill_str.at(0) == '0' or fill_str.at(0) == '1'));
}


// Enables the user to play the game, i.e. by repeatedly asking an element
// to be added and its position, until the game is over.
void play_game(GameBoard& board)
{
    board.print();
    while(not board.is_game_over())
    {
        string x_str = "";
        string y_str = "";
        string rest_input = "";
        cout << "Enter two coordinates and a fill symbol, or q to quit: ";

        //
        // Reading x coordinate as a string and checking if it was quit command
        cin >> x_str;
        if(x_str.at(0) == 'q' or x_str.at(0) == 'Q')
        {
            cout << QUIT << endl;
            return;
        }

        // Reading y coordinate
        cin >> y_str;
        //
        // Changing coordinates from string to int and checking if they are
        // inside the board
        unsigned int x = stoi_with_check(x_str);
        unsigned int y = stoi_with_check(y_str);
        if(not (1 <= x and x <= SIZE and 1 <= y and y <= SIZE))
        {
            cout << OUT_OF_BOARD << endl;
            getline(cin, rest_input);
            continue;
        }

        // Reading the rest of the input line including fill symbol and
        // checking if the rest input consists exactly of one '0' or '1'
        getline(cin, rest_input);
        if(not find_fill_symbol(rest_input))
        {
            cout << INVALID_INPUT << endl;
            continue;
        }

        // At this point, we know that the input is valid, but we don't know
        // if it is possible to add the given symbol on the given position
        char symbol = rest_input[0];
        Element_type fill_symbol = (symbol == '0') ? ZERO : ONE;
        if(board.add_element(x, y, fill_symbol))
        // If the given symbol was possible to add, print the changed gameboard
        board.print();
    }
    // If the game ends up to a totally filled gameboard, the player won
    cout << WIN << endl;
}

// Gives the user a possibility to select a filling way.
// Returns true if filling succeeded, otherwise returns false.
bool select_start(GameBoard& board)
{
    string choice = "";
    cout << "Select start (R for random, I for input): ";
    getline(cin, choice);
    if(choice != "R" and choice != "r" and choice != "I" and choice != "i")
    {
        return false;
    }
    else if(choice == "R" or choice == "r")
    {
        string seed_string = "";
        cout << "Enter a seed value: ";
        getline(cin, seed_string);

// Fill gameboard and return if it succeeded or not
        int seed = stoi_with_check(seed_string);
                if(seed == 0)
                {
                    cout << "Bad seed" << endl;
                    return false;
                }
                return board.fill_random(seed);
            }
    else // if(choice == "I" or choice == "i")
    {
        string input = "";
        cout << "Input: ";
        getline(cin, input);

        // (Note that at this point, input includes quote marks or such)
        // TODO: Fill gameboard and return if it succeeded or not
        return board.fill_input(input);
    }
}

// Short and simple main function.
int main()
{
     srand(time(0));
    GameBoard board;
    while(not select_start(board)); 
                                  
    play_game(board);
    return 0;
}
