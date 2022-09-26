#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    const Game& m_game;
    char game_board[MAXROWS][MAXCOLS];
    vector<int> placed_ships; 
    vector<Point> attacked_positions; 
};

BoardImpl::BoardImpl(const Game& g) : m_game(g)
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            game_board[r][c] = '.'; 
        }
    }
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
                game_board[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
    int total_cells = m_game.rows() * m_game.cols(); 
    int blocked_cells = 0; 
    // Block cells with 50% probability
    while (blocked_cells != (total_cells / 2))
    {
        for (int r = 0; r < m_game.rows(); r++)
        {
            for (int c = 0; c < m_game.cols(); c++)
            {
                if (blocked_cells != (total_cells / 2))
                {
                    if ((randInt(2) == 0) && (game_board[r][c] != '#'))
                    {
                        game_board[r][c] = '#';
                        blocked_cells++;
                    }
                }
            }
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (game_board[r][c] == '#')
                game_board[r][c] = '.';
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
   //validate shipId
    if (shipId < 0 || shipId >= m_game.nShips())
        return false; 
    //make sure point is valid 
    if (topOrLeft.r < 0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols())
        return false; 
    //make sure ship is not placed outside board 
    if (dir == HORIZONTAL && ((topOrLeft.c + m_game.shipLength(shipId)) <= m_game.cols()))
    {
        //make sure there is no overlap on anything (other ships, blocked positions)
        for (int t = topOrLeft.c; t < (topOrLeft.c + m_game.shipLength(shipId)); t++)
        {
            if (game_board[topOrLeft.r][t] != '.')
                return false; 
        }

        if (placed_ships.size() == 0)
        {
            placed_ships.push_back(shipId);
            for (int m = topOrLeft.c; m < (topOrLeft.c + m_game.shipLength(shipId)); m++)
            {
                game_board[topOrLeft.r][m] = m_game.shipSymbol(shipId); 
            }
            return true; 
        }
        //make sure ship has not previously been placed already on the board 
        for (int k = 0; k < placed_ships.size(); k++)
        {
            if (shipId == placed_ships[k])
                return false; 
        }
        placed_ships.push_back(shipId);
        for (int m = topOrLeft.c; m < (topOrLeft.c + m_game.shipLength(shipId)); m++)
        {
            game_board[topOrLeft.r][m] = m_game.shipSymbol(shipId);
        }
        return true;
    }

    if (dir == VERTICAL && ((topOrLeft.r + m_game.shipLength(shipId)) <= m_game.rows()))
    {
        //make sure there is no overlap on anything (other ships, blocked positions)
        for (int t = topOrLeft.r; t < (topOrLeft.r + m_game.shipLength(shipId)); t++)
        {
            if (game_board[t][topOrLeft.c] != '.')
                return false;
        }

        if (placed_ships.size() == 0)
        {
            placed_ships.push_back(shipId);
            for (int m = topOrLeft.r; m < (topOrLeft.r + m_game.shipLength(shipId)); m++)
            {
                game_board[m][topOrLeft.c] = m_game.shipSymbol(shipId);
            }
            return true;
        }
        //make sure ship has not previously been placed already on the board 
        for (int k = 0; k < placed_ships.size(); k++)
        {
            if (shipId == placed_ships[k])
                return false;
        }
        placed_ships.push_back(shipId);
        for (int m = topOrLeft.r; m < (topOrLeft.r + m_game.shipLength(shipId)); m++)
        {
            game_board[m][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
        return true;
    }
    return false; 
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    bool ShipFound = false; 
    vector<int>::iterator ship_location = placed_ships.begin();
    if (placed_ships.size() == 0)
        return false;
    //check if such a shipId has been placed on the board
    for (int k = 0; k < placed_ships.size(); k++, ship_location++)
    {
        if (placed_ships[k] == shipId)
        {
            ShipFound = true;
            break;
        }
    }
    if (!ShipFound)
        return false; 
    //check to see if board even contains coordinate 
    if (topOrLeft.r < 0 || topOrLeft.c < 0 || topOrLeft.r >= m_game.rows() || topOrLeft.c >= m_game.cols())
        return false;
    //check to see if point on the board has the given ship symbol 
    if (game_board[topOrLeft.r][topOrLeft.c] == m_game.shipSymbol(shipId))
    {
        //check if point, dir is valid and if the next spot based on dir and point has the ship's symbol 
        if (dir == HORIZONTAL && ((topOrLeft.c + m_game.shipLength(shipId)) <= m_game.cols()) && game_board[topOrLeft.r][topOrLeft.c + 1] == m_game.shipSymbol(shipId))
        {
            //remove ship and erase from vector 
            for (int k = topOrLeft.c; k < (topOrLeft.c + m_game.shipLength(shipId)); k++)
            {
                game_board[topOrLeft.r][k] = '.'; 
            }
            placed_ships.erase(ship_location); 
        }   
        //check if point, dir is valid and if the next spot based on dir and point has the ship's symbol 
        if (dir == VERTICAL && ((topOrLeft.r + m_game.shipLength(shipId)) <= m_game.rows()) && game_board[topOrLeft.r + 1][topOrLeft.c] == m_game.shipSymbol(shipId))
        {
            //remove ship and erase from vector 
            for (int k = topOrLeft.r; k < (topOrLeft.r + m_game.shipLength(shipId)); k++)
            {
                game_board[k][topOrLeft.c] = '.';
            }
            placed_ships.erase(ship_location);
        }
        return false; 
    }
    return false; 
}

void BoardImpl::display(bool shotsOnly) const
{
    if (!(shotsOnly))
    {
        cout << "  ";
        for (int t = 0; t < m_game.cols(); t++)
        {
            cout << t;
        }
        cout << endl;
        for (int m = 0; m < m_game.rows(); m++)
        {
            cout << m << " ";
            for (int k = 0; k < m_game.cols(); k++)
            {
                cout << game_board[m][k];
            }
            cout << endl;
        }
    }
    else
    {
        cout << "  ";
        for (int t = 0; t < m_game.cols(); t++)
        {
            cout << t;
        }
        cout << endl;
        for (int m = 0; m < m_game.rows(); m++)
        {
            cout << m << " ";
            for (int k = 0; k < m_game.cols(); k++)
            {
                if ((game_board[m][k] == '.') || (game_board[m][k] == 'X') || (game_board[m][k] == 'o'))
                {
                    cout << game_board[m][k];
                }
                else
                    cout << '.'; 
            }
            cout << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    vector<Point> ::iterator attack_pointer = attacked_positions.begin(); 
    //check to see if board even contains coordinate 
    if (p.r < 0 || p.c < 0 || p.r >= m_game.rows() || p.c >= m_game.cols())
        return false;
    //if this is first attack for a board, proceed with the attack 
    if (attacked_positions.size() == 0)
    {
        //missed attack 
        if (game_board[p.r][p.c] == '.')
        {
            game_board[p.r][p.c] = 'o';
            attacked_positions.push_back(p);
            shipDestroyed = false; 
            shotHit = false; 
            return true; 
        }
        else
        { //boat has been hit 
            //has this ship been entirely destroyed? 
            char boat_symbol = game_board[p.r][p.c]; 
            int symbols_left = 0; 
            for (int t = 0; t < m_game.rows(); t++)
            {
                for (int g = 0; g < m_game.cols(); g++)
                {
                    if (game_board[t][g] == boat_symbol)
                    {
                        symbols_left++;
                    }
                }
            }
            //ship has been entirely destroyed 
            if (symbols_left == 1)
            {
                for (int h = 0; h < placed_ships.size(); h++)
                {
                    if (m_game.shipSymbol(placed_ships[h]) == boat_symbol)
                    {
                        shipId = placed_ships[h];
                        shipDestroyed = true; 
                        break; 
                    }
                }
            }
            else
            {
                shipDestroyed = false;
            }
            game_board[p.r][p.c] = 'X';
            attacked_positions.push_back(p); 
            shotHit = true; 
            return true; 
        }
    }
    for (int k = 0; k < attacked_positions.size(); k++, attack_pointer++)
    {
        //location has been hit previously so return false 
        if (((*attack_pointer).r == p.r) && ((*attack_pointer).c == p.c))
         {
            return false; 
         }
    }
    //missed attack 
    if (game_board[p.r][p.c] == '.')
    {
        game_board[p.r][p.c] = 'o';
        attacked_positions.push_back(p);
        shipDestroyed = false;
        shotHit = false;
        return true;
    }
    else
    { //boat has been hit 
        //has this ship been entirely destroyed? 
        char boat_symbol = game_board[p.r][p.c];
        int symbols_left = 0;
        for (int t = 0; t < m_game.rows(); t++)
        {
            for (int g = 0; g < m_game.cols(); g++)
            {
                if (game_board[t][g] == boat_symbol)
                {
                    symbols_left++;
                }
            }
        }
        //ship has been entirely destroyed 
        if (symbols_left == 1)
        {
            for (int h = 0; h < placed_ships.size(); h++)
            {
                if (m_game.shipSymbol(placed_ships[h]) == boat_symbol)
                {
                    shipId = placed_ships[h];
                    shipDestroyed = true;
                    break;
                }
            }
        }
        else
        {
            shipDestroyed = false;
        }
        game_board[p.r][p.c] = 'X';
        attacked_positions.push_back(p);
        shotHit = true;
        return true;
    }
}

bool BoardImpl::allShipsDestroyed() const
{
    bool ship_left = false;
    for (int t = 0; t < m_game.rows(); t++)
    {
        for (int g = 0; g < m_game.cols(); g++)
        {
            if ((game_board[t][g] != '.') && (game_board[t][g] != 'o') && (game_board[t][g] != 'X'))
            {
                ship_left = true; 
                break; 
            }
        }
    }
    return (!ship_left); 
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
