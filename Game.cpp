#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    //Ship objects will store data about newly created ship types 
    class Ship
    {
    public: 
        Ship(int len, char sym, string s_name) : ship_length(len), ship_symbol(sym), ship_name(s_name)
        {}
        int ship_length; 
        char ship_symbol; 
        string ship_name; 
    };
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private: 
    int game_rows; 
    int game_cols;
    vector<Ship> ship_types; //dynamically allocated array that stores ship types 
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols) : game_rows(nRows), game_cols(nCols)
{
   //Must not exceed MAXROWS and MAXCOLS
    if ((nRows > MAXROWS) || (nCols > MAXCOLS))
    {
        exit(1); 
    }
}

int GameImpl::rows() const
{
    return game_rows; 
}

int GameImpl::cols() const
{
    return game_cols; 
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (length <= 0)//must be positive and must allow ship to fit into the board 
        return false; 
    if ((isprint(symbol) == 0) || (symbol == 'X') || (symbol == 'o') || (symbol == '.')) //must be a printable character other than the ones that aren't allowed 
        return false; 
    //add to array of ship types if size of vector is 0 (no ships added)
    if (ship_types.size() == 0)
    {
        ship_types.push_back(Ship(length, symbol, name));
        return true;
    }
    //traverse through vector to make sure no matching symbols if vector is not empty 
    for (size_t k = 0; k < ship_types.size(); k++)
    {
        if (ship_types[k].ship_symbol == symbol)
return false;
    }
    ship_types.push_back(Ship(length, symbol, name));
    return true;
}

int GameImpl::nShips() const
{
    return ship_types.size();
}

int GameImpl::shipLength(int shipId) const
{
    //shipId must be within the range between 0 and the vector size for defined behavior 
    if (shipId < 0 || shipId >= ship_types.size())
    {
        cerr << "invalid shipId passed to shipLength function";
        return -1;
    }
    return ship_types[shipId].ship_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    if (shipId < 0 || shipId >= ship_types.size())
    {
        cerr << "invalid shipId passed to shipSymbol function";
        return '!';
    }
    return ship_types[shipId].ship_symbol;
}

string GameImpl::shipName(int shipId) const
{
    if (shipId < 0 || shipId >= ship_types.size())
    {
        cerr << "invalid shipId passed to shipName function";
        return "error";
    }
    return ship_types[shipId].ship_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    //p1 will have the board b1 and p2 will have the board b2
    if (!(p1->placeShips(b1)))
    {
        return nullptr;
    }
    if (!(p2->placeShips(b2)))
    {
        return nullptr;
    }
    bool validAttack;
    bool shotHit;
    bool shipDestroyed;
    int destroyedShipId;
    Point target; 
    //b1 and b2 to be assembled by the 2 players (place ships) 
    // while loop 
        //p1 attack b2 
        //display b2 board 
        // //repeat code 
        //p2 attack b1 
        //display b1 board 
    //keep repeating until win (exit condition for the while loop)
    //should be while loop here
    // assumes player algorithms will not make an invalid attack 
    //should include recordAttackResult and recordAttackbyOpponent in this function 
    while ((!(b1.allShipsDestroyed())) && (!(b2.allShipsDestroyed())))
    {
        if (p1->isHuman())
        {
            cout << p1->name() + "'s turn. Board for " + p2->name() + ":";
            cout << endl;
            b2.display(true);
        }
        else
        {
            cout << p1->name() + "'s turn. Board for " + p2->name() + ":";
            cout << endl;
            b2.display(false);
        }
        target = p1->recommendAttack();
        validAttack = b2.attack(target, shotHit, shipDestroyed, destroyedShipId);
        if ((p1->isHuman()) && validAttack && shotHit)
        {
            if (shipDestroyed)
            {
                cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and destroyed the " + p1->game().shipName(destroyedShipId) + ", resulting in:";
                cout << endl;
                b2.display(true);
            }
            else
            {
                cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and hit something, resulting in:";
                cout << endl;
                b2.display(true);
            }
        }
        else if ((p1->isHuman()) && validAttack && (!shotHit))
        {
            cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and missed, resulting in:"; 
            cout << endl; 
            b2.display(true); 
        }
        else if ((!(p1->isHuman())) && validAttack && shotHit)
        {
            if (shipDestroyed)
            {
                cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and destroyed the " + p1->game().shipName(destroyedShipId) + ", resulting in:";
                cout << endl;
                b2.display(false);
                p1->recordAttackResult(target, true, true, true, destroyedShipId);
            }
            else
            {
                cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and hit something, resulting in:";
                cout << endl;
                b2.display(false);
                p1->recordAttackResult(target, true, true, false, destroyedShipId);
            }
        }
        else if ((!(p1->isHuman())) && validAttack && (!shotHit))
        {
            cout << p1->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and missed, resulting in:";
            cout << endl;
            b2.display(false);
            p1->recordAttackResult(target, true, false, false, destroyedShipId);
        }
        else if ((p1->isHuman()) && !validAttack)
        {
            cout << p1->name() + " wasted a shot at (" + to_string(target.r) + "," + to_string(target.c) + ").";
            cout << endl; 
            b2.display(true);
        }
        else 
        {
            cout << p1->name() + " wasted a shot at (" + to_string(target.r) + "," + to_string(target.c) + ").";
            cout << endl; 
            b2.display(false); 
            p1->recordAttackResult(target, false, false, false, destroyedShipId);
        }
        if (b2.allShipsDestroyed())
        {
            break; 
        }
        if (shouldPause)
        {
            waitForEnter();
        }

        //other player's turn 
        if (p2->isHuman())
        {
            cout << p2->name() + "'s turn. Board for " + p1->name() + ":";
            cout << endl;
            b1.display(true);
        }
        else
        {
            cout << p2->name() + "'s turn. Board for " + p1->name() + ":";
            cout << endl;
            b1.display(false);
        }
        target = p2->recommendAttack();
        validAttack = b1.attack(target, shotHit, shipDestroyed, destroyedShipId);
        if ((p2->isHuman()) && validAttack && shotHit)
        {
            if (shipDestroyed)
            {
                cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and destroyed the " + p2->game().shipName(destroyedShipId) + ", resulting in:";
                cout << endl;
                b1.display(true);
            }
            else
            {
                cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and hit something, resulting in:";
                cout << endl;
                b1.display(true);
            }
        }
        else if ((p2->isHuman()) && validAttack && (!shotHit))
        {
            cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and missed, resulting in:";
            cout << endl;
            b1.display(true);
        }
        else if ((!(p2->isHuman())) && validAttack && shotHit)
        {
            if (shipDestroyed)
            {
                cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and destroyed the " + p2->game().shipName(destroyedShipId) + ", resulting in:";
                cout << endl;
                b1.display(false);
                p2->recordAttackResult(target, true, true, true, destroyedShipId);
            }
            else
            {
                cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and hit something, resulting in:";
                cout << endl;
                b1.display(false);
                p2->recordAttackResult(target, true, true, false, destroyedShipId);
            }
        }
        else if ((!(p2->isHuman())) && validAttack && (!shotHit))
        {
            cout << p2->name() + " attacked (" + to_string(target.r) + "," + to_string(target.c) + ") and missed, resulting in:";
            cout << endl;
            b1.display(false);
            p2->recordAttackResult(target, true, false, false, destroyedShipId);
        }
        else if ((p2->isHuman()) && !validAttack)
        {
            cout << p2->name() + " wasted a shot at (" + to_string(target.r) + "," + to_string(target.c) + ").";
            cout << endl;
            b1.display(true);
        }
        else
        {
            cout << p2->name() + " wasted a shot at (" + to_string(target.r) + "," + to_string(target.c) + ").";
            cout << endl;
            b1.display(false);
            p2->recordAttackResult(target, false, false, false, destroyedShipId);
        }
        if (b1.allShipsDestroyed())
        {
            break;
        }
        if (shouldPause)
        {
            waitForEnter();
        }
    }
    //if the losing player is human, display the winner's board, showing everything 
    if (b1.allShipsDestroyed())
    {
        if (p1->isHuman())
            b2.display(false);
        return p2; 
    }
    if (b2.allShipsDestroyed())
    {
        if (p2->isHuman())
            b1.display(false);
        return p1;
    }
    return p1; 
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1 || nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1 || nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows() && length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
            << endl;
        return false;
    }
    if (!isascii(symbol) || !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
            << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X' || symbol == '.' || symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
            << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr || p2 == nullptr || nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

