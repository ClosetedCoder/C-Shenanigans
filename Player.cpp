#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
    : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}


class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer:: HumanPlayer(string nm, const Game& g) : Player(nm, g)
{}

 bool HumanPlayer :: isHuman() const
{
    return true; 
}

 bool HumanPlayer::placeShips(Board& b)
 {
     char shipDirection; 
     int row_coord; 
     int col_coord; 
     Point coordinate; 
     bool valid; 
     cout << Player::name() + " must place " + to_string(Player::game().nShips()) + " ships.";
     cout << endl; 
     for (int i = 0; i < Player::game().nShips(); i++)
     {
         b.display(false); 
         cout << "Enter h or v for direction of " + Player::game().shipName(i) + " (length " + to_string(Player::game().shipLength(i)) + "): ";
         cin >> shipDirection; 
         cin.clear();
         cin.ignore(10000, '\n');
         while ((shipDirection != 'h') && (shipDirection != 'v'))
         {
             cout << "Direction must be h or v.";
             cout << endl;
             cout << "Enter h or v for direction of " + Player::game().shipName(i) + " (length " + to_string(Player::game().shipLength(i)) + "): ";
             cin >> shipDirection; 
             cin.clear();
             cin.ignore(10000, '\n');
         }
         if (shipDirection == 'h')
         {
             cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
             valid = getLineWithTwoIntegers(row_coord, col_coord);
             while (!valid)
             {
                 cout << "You must enter two integers.";
                 cout << endl; 
                 cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                 valid = getLineWithTwoIntegers(row_coord, col_coord);
             }
             coordinate = Point(row_coord, col_coord);
             while (!(b.placeShip(coordinate, i, HORIZONTAL)))
             {
                 cout << "The ship can not be placed there.";
                 cout << endl; 
                 cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                 valid = getLineWithTwoIntegers(row_coord, col_coord);
                 while (!valid)
                 {
                     cout << "You must enter two integers.";
                     cout << endl; 
                     cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                     valid = getLineWithTwoIntegers(row_coord, col_coord);
                 }
                 coordinate = Point(row_coord, col_coord);
             }
         }
         if (shipDirection == 'v')
         {
             cout << "Enter row and column of topmost cell (e.g., 3 5): ";
             valid = getLineWithTwoIntegers(row_coord, col_coord);
             while (!valid)
             {
                 cout << "You must enter two integers.";
                 cout << endl;
                 cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                 valid = getLineWithTwoIntegers(row_coord, col_coord);
             }
             coordinate = Point(row_coord, col_coord);
             while (!(b.placeShip(coordinate, i, VERTICAL)))
             {
                 cout << "The ship can not be placed there.";
                 cout << endl;
                 cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                 valid = getLineWithTwoIntegers(row_coord, col_coord);
                 while (!valid)
                 {
                     cout << "You must enter two integers.";
                     cout << endl;
                     cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                     valid = getLineWithTwoIntegers(row_coord, col_coord);
                 }
                 coordinate = Point(row_coord, col_coord);
             }
         }
     }
     b.display(false);
     return true; 
 }

 Point HumanPlayer::recommendAttack()
 {
     int row_coord; 
     int col_coord; 
     bool valid; 
     Point attack_coord; 
     cout << "Enter the row and column to attack (e.g., 3 5): ";
     valid = getLineWithTwoIntegers(row_coord, col_coord); 
     while (!valid)
     {
         cout << "You must enter two integers.";
         cout << endl;
         cout << "Enter the row and column to attack (e.g., 3 5): ";
         cout << endl; 
         valid = getLineWithTwoIntegers(row_coord, col_coord);
     }
     attack_coord = Point(row_coord, col_coord);
     return attack_coord; 
 }

 void HumanPlayer :: recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
 {
      //do nothing
 }

 void HumanPlayer::recordAttackByOpponent(Point p)
 {
     //do nothing 
 }


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

 class MediocrePlayer : public Player
 {
 public:
     MediocrePlayer(string nm, const Game& g);
     virtual bool isHuman() const;
     virtual bool placeShips(Board& b);
     virtual Point recommendAttack();
     virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
     virtual void recordAttackByOpponent(Point p); 
     bool helperPlaceShips(int index, Board& b);
 private:
     int state; 
     vector<Point> attackedPositions; 
     vector<Point> StateTwoOptions; 
     Point hit_location; 
 };

 MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), state(1)
 {}

 bool MediocrePlayer::isHuman() const
 {
     return false; 
 }

 bool MediocrePlayer::placeShips(Board& b)
 {
     int attempts = 0; 
     //atempt 50 times to place ships 
     while (attempts <= 50)
     {
         b.block();
         //failure
         if (!helperPlaceShips(0, b))
         {
             attempts++;
             b.unblock();
         }
         //success
         else
         {
             b.unblock();
             return true; 
         }
     }
     return false; 
 }

 //recursive algorithm (helper function) 
 bool MediocrePlayer :: helperPlaceShips(int index, Board& b)
 {
     if (index == Player::game().nShips())
         return true;
     //for loop that runs through entire board
     for(int row_incrementer = 0; row_incrementer < Player::game().rows(); row_incrementer++)
     {
         for (int col_incrementer = 0; col_incrementer < Player::game().cols(); col_incrementer++)
         {
             
             //horizontal case 
             if (b.placeShip(Point(row_incrementer, col_incrementer), index, HORIZONTAL))
             {
                 bool returnValue = helperPlaceShips(index + 1, b);
                 if (returnValue)
                     return true;
                 else
                     b.unplaceShip(Point(row_incrementer, col_incrementer), index, HORIZONTAL);
             }
             //vertical case
             if (b.placeShip(Point(row_incrementer, col_incrementer), index, VERTICAL))
             {
                 bool returnValue = helperPlaceShips(index + 1, b);
                 if (returnValue)
                     return true;
                 else
                     b.unplaceShip(Point(row_incrementer, col_incrementer), index, VERTICAL);
             }
         }
     }
     return false; 
 }

 Point MediocrePlayer :: recommendAttack()
 {
     Point attackpos; 
     int counter = 0;
     bool match_found = false; 
     if (state == 1)
     {
         //find unique attack point
         attackpos = Player::game().randomPoint();
         while(counter == 0)
         {
             for (int i = 0; i < attackedPositions.size(); i++)
             {
                 counter++;
                 if ((attackedPositions[i].r == attackpos.r) && (attackedPositions[i].c == attackpos.c))
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0; 
                     break;
                 }
             }
             if (counter == attackedPositions.size())
                 break;
         }
         attackedPositions.push_back(attackpos); 
         return attackpos; 
     }

     if (state == 2)
     {
         //find unique attack point
         attackpos = Player::game().randomPoint();
         while (counter == 0)
         {
             for (int i = 0; i < attackedPositions.size(); i++)
             {
                 counter++;
                 if ((attackedPositions[i].r == attackpos.r) && (attackedPositions[i].c == attackpos.c))
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                     break;
                 }
             }
             if (counter == attackedPositions.size())
             {
                 //check if the random attack point is able to be done given the vector StateTwoOptions
                 for (int k = 0; k < StateTwoOptions.size(); k++)
                 {
                     if ((StateTwoOptions[k].r == attackpos.r) && (StateTwoOptions[k].c == attackpos.c))
                     {
                         match_found = true; 
                         break; 
                     }
                 }
                 if (match_found)
                     break; 
                 else
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                 }
             }
         }
         attackedPositions.push_back(attackpos);
         return attackpos;
     }
     return Point(0, 0);
 }

 void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
 {
     if ((state == 1) && (shotHit == false))
         return;
     if ((state == 1) && shotHit && shipDestroyed)
         return; 
     if ((state == 1) && shotHit && (!shipDestroyed))
     {
         state = 2; 
         hit_location = p; 
         //fill vectors with positions that can be attacked given the algorithm 
         for (int i = 1; i < 5; i++)
         {
             if (game().isValid(Point(hit_location.r + i, hit_location.c)))
                 StateTwoOptions.push_back(Point(hit_location.r + i, hit_location.c));
             if (game().isValid(Point(hit_location.r - i, hit_location.c)))
                 StateTwoOptions.push_back(Point(hit_location.r - i, hit_location.c));
             if (game().isValid(Point(hit_location.r, hit_location.c + i)))
                 StateTwoOptions.push_back(Point(hit_location.r, hit_location.c + i));
             if (game().isValid(Point(hit_location.r, hit_location.c - i)))
                 StateTwoOptions.push_back(Point(hit_location.r, hit_location.c - i));
         }
         return; 
     }

     if ((state == 2) && (!shotHit))
         return; 
     if ((state == 2) && shotHit && (!shipDestroyed))
         return; 
     if ((state == 2) && shotHit && shipDestroyed)
     {
         state = 1; 
         StateTwoOptions.clear();
         return; 
     }
 }

 void MediocrePlayer:: recordAttackByOpponent(Point p)
 {
     //do nothing 
 }

//*********************************************************************
//  GoodPlayer
//*********************************************************************

 class GoodPlayer : public Player
 {
 public:
     GoodPlayer(string nm, const Game& g);
     virtual bool isHuman() const;
     virtual bool placeShips(Board& b);
     virtual Point recommendAttack();
     virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
     virtual void recordAttackByOpponent(Point p);
     bool helperPlaceShips(int index, Board& b);
 private:
     int state; 
     Point firstHit; 
     vector <Point> pointsOfOptimalAttack; 
     vector <Point> attackedPositions; 
     vector <Point> pointsOfOptimalAttack_2; 
     vector <Point> pointsOfOptimalAttack_3; 
 };



 GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), state(1)
 {
 }

 bool GoodPlayer::isHuman() const
 {
     return false; 
 }

 bool GoodPlayer :: placeShips(Board& b)
 {
     int attempts = 0;
     while (attempts <= 1000000)
     {
         //failure
         if (!helperPlaceShips(0, b))
         {
             attempts++;
         }
         //success
         else
         {
             return true;
         }
     }
     return false;
 }

 //helper function to place ships 
 bool GoodPlayer::helperPlaceShips(int index, Board& b)
 {
     if (index == Player::game().nShips())
         return true;
     Point place_point = Player::game().randomPoint();
     //horizontal case 
     if (b.placeShip(place_point, index, HORIZONTAL))
             {
                 bool returnValue = helperPlaceShips(index + 1, b);
                 if (returnValue)
                     return true;
                 else
                     b.unplaceShip(place_point, index, HORIZONTAL);
             }
     //vertical case
     if (b.placeShip(place_point, index, VERTICAL))
             {
                 bool returnValue = helperPlaceShips(index + 1, b);
                 if (returnValue)
                     return true;
                 else
                     b.unplaceShip(place_point, index, VERTICAL);
             }
     return false;
 }

 Point GoodPlayer::recommendAttack()
 {
     Point attackpos;
     int counter = 0;
     bool match_found = false; 
     if (state == 1)
     {
         //find unique attack point
         attackpos = Player::game().randomPoint();
         while (counter == 0)
         {
             for (int i = 0; i < attackedPositions.size(); i++)
             {
                 counter++;
                 if ((attackedPositions[i].r == attackpos.r) && (attackedPositions[i].c == attackpos.c))
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                     break;
                 }
             }
             if (counter == attackedPositions.size())
                 break;
         }
         attackedPositions.push_back(attackpos);
         return attackpos;
     }

     //attack only 1 square in cardinal directions around hit location (state 2) to search for ship 
     if (state == 2)
     {
         int count_1 = 0;
         //what if all spots in the vector have been attacked? 
         for (int i = 0; i < pointsOfOptimalAttack_2.size(); i++)
         {
             for (int u = 0; u < attackedPositions.size(); u++)
             {
                 if ((pointsOfOptimalAttack_2[i].r == attackedPositions[u].r) && (pointsOfOptimalAttack_2[i].c == attackedPositions[u].c))
                     count_1++;
             }
         }
         if (count_1 == pointsOfOptimalAttack_2.size())
         {
             state = 1;
             return recommendAttack();
         }
         //find unique attack point
         attackpos = Player::game().randomPoint();
         while (counter == 0)
         {
             for (int i = 0; i < attackedPositions.size(); i++)
             {
                 counter++;
                 if ((attackedPositions[i].r == attackpos.r) && (attackedPositions[i].c == attackpos.c))
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                     break;
                 }
             }
             if (counter == attackedPositions.size())
             {
                 //check if the random attack point is able to be done given the vector pointsOfOptimalAttack_2
                 for (int k = 0; k < pointsOfOptimalAttack_2.size(); k++)
                 {
                     if ((pointsOfOptimalAttack_2[k].r == attackpos.r) && (pointsOfOptimalAttack_2[k].c == attackpos.c))
                     {
                         match_found = true;
                         break;
                     }
                 }
                 if (match_found)
                     break;
                 else
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                 }
             }
         }
         attackedPositions.push_back(attackpos);
         return attackpos;
     }

     //if we have locked onto a target, initiate state 3 (attack along cardinal direction until ship is destroyed) 
     if (state == 3)
     {
         int count_1 = 0;
         //what if all spots in the vector have been attacked? 
         for (int i = 0; i < pointsOfOptimalAttack_3.size(); i++)
         {
             for (int u = 0; u < attackedPositions.size(); u++)
             {
                 if ((pointsOfOptimalAttack_3[i].r == attackedPositions[u].r) && (pointsOfOptimalAttack_3[i].c == attackedPositions[u].c))
                     count_1++;
             }
         }
         if (count_1 == pointsOfOptimalAttack_3.size())
         {
             state = 1;
             return recommendAttack();
         }
         //find unique attack point
         attackpos = Player::game().randomPoint();
         while (counter == 0)
         {
             for (int i = 0; i < attackedPositions.size(); i++)
             {
                 counter++;
                 if ((attackedPositions[i].r == attackpos.r) && (attackedPositions[i].c == attackpos.c))
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                     break;
                 }
             }
             if (counter == attackedPositions.size())
             {
                 //check if the random attack point is able to be done given the vector pointsOfOptimalAttack_3
                 for (int k = 0; k < pointsOfOptimalAttack_3.size(); k++)
                 {
                     if ((pointsOfOptimalAttack_3[k].r == attackpos.r) && (pointsOfOptimalAttack_3[k].c == attackpos.c))
                     {
                         match_found = true;
                         break;
                     }
                 }
                 if (match_found)
                     break;
                 else
                 {
                     attackpos = Player::game().randomPoint();
                     counter = 0;
                 }
             }
         }
         attackedPositions.push_back(attackpos);
         return attackpos;
     }
     return Point(0, 0);
 }

 void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
 {
     if ((state == 1) && (shotHit == false))
         return;
     if ((state == 1) && shotHit && shipDestroyed)
         return;
     if ((state == 1) && shotHit && (!shipDestroyed))
     {
         state = 2;
         firstHit = Point(p.r, p.c);
         if (game().isValid(Point(p.r, (p.c + 1))))
             pointsOfOptimalAttack_2.push_back(Point(p.r, (p.c + 1)));
         if (game().isValid(Point((p.r + 1), p.c)))
             pointsOfOptimalAttack_2.push_back(Point((p.r + 1), p.c));
         if (game().isValid(Point(p.r, (p.c - 1))))
             pointsOfOptimalAttack_2.push_back(Point(p.r, (p.c - 1)));
         if (game().isValid(Point((p.r - 1), p.c)))
             pointsOfOptimalAttack_2.push_back(Point((p.r - 1), p.c));
         return;
     }

     if ((state == 2) && (!shotHit))
         return;
     if ((state == 2) && shotHit && (!shipDestroyed))
     {
         //cardinal direction of ship is locked 
         state = 3;
         if ((p.r == (firstHit.r - 1)) || (p.r == (firstHit.r + 1)))
         {
             //bottom and top direction
             for (int i = 1; i < 5; i++)
             {
                 if (game().isValid(Point(firstHit.r + i, firstHit.c)))
                     pointsOfOptimalAttack_3.push_back(Point(firstHit.r + i, firstHit.c));
                 if (game().isValid(Point(firstHit.r - i, firstHit.c)))
                     pointsOfOptimalAttack_3.push_back(Point(firstHit.r - i, firstHit.c));
             }
         }
         if ((p.c == (firstHit.c - 1)) || (p.c == (firstHit.c + 1)))
         {
             //left and right direction 
             for (int i = 1; i < 5; i++)
             {
                 if (game().isValid(Point(firstHit.r, firstHit.c + i)))
                     pointsOfOptimalAttack_3.push_back(Point(firstHit.r, firstHit.c + i));
                 if (game().isValid(Point(firstHit.r, firstHit.c - i)))
                     pointsOfOptimalAttack_3.push_back(Point(firstHit.r, firstHit.c - i));
             }
         }
         pointsOfOptimalAttack_2.clear();
         return;
     }
     if ((state == 2) && shotHit && shipDestroyed)
     {
         state = 1;
         pointsOfOptimalAttack_2.clear();
         return;
     }

     if ((state == 3) && shotHit && (!shipDestroyed))
     {
         return;
     }

     if ((state == 3) && (!shotHit))
     {
         return; 
     }

     if ((state == 3) && shotHit && shipDestroyed)
     {
         pointsOfOptimalAttack_3.clear(); 
         state = 1; 
         return; 
     }
 }

 void GoodPlayer::recordAttackByOpponent(Point p)
 {
     //do nothing 
 }

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
        type != types[pos]; pos++)
        ;
    switch (pos)
    {
    case 0:  return new HumanPlayer(nm, g);
    case 1:  return new AwfulPlayer(nm, g);
    case 2:  return new MediocrePlayer(nm, g);
    case 3:  return new GoodPlayer(nm, g);
    default: return nullptr;
    }
}


