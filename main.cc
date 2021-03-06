game.cc                                                                                             0000644 0001750 0001750 00000010250 13355253170 011004  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   // File: game.cxx

#include <cassert>    // Provides assert
#include <climits>    // Provides INT_MAX and INT_MIN
#include <iostream>   // Provides cin, cout
#include <queue>      // Provides queue<string>
#include <string>     // Provides string
#include "game.h"     // Provides definition of game class
using namespace std;

namespace main_savitch_14
{
//*************************************************************************
// STATIC MEMBER CONSTANTS
// const int game::SEARCH_LEVELS;

//*************************************************************************
// PUBLIC MEMBER FUNCTIONS

game::who game::play( )
// The play function should not be overridden. It plays one round of the
// game, with the human player moving first and the computer second.
// The return value is the winner of the game (or NEUTRAL for a tie).
{
	restart( );

	while (!is_game_over( ))
	{
		display_status( );
		whosTurn();
		if (last_mover( ) == COMPUTER) {
			countingPieces();
			make_human_move( );
		}
		else {
			make_computer_move( );
		}
	}
	display_status( );
	return HUMAN;
}



//*************************************************************************
// OPTIONAL VIRTUAL FUNCTIONS (overriding these functions is optional)

void game::display_message(const string& message) const
{
	cout << message;
}

string game::get_user_move( ) const
{
	string answer;

	display_message("If you cannot move, please press 'S'\n");
	display_message("Your move, please: ");
	getline(cin, answer);
	return answer;
}

game::who game::winning()const {

	int value = evaluate();

	if (value > 0) {
		return last_mover();
	} else if (value < 0) {
		return next_mover();
	} else {
		return NEUTRAL;
	}

}

//*************************************************************************
// PRIVATE FUNCTIONS (these are the same for every game)

int game::eval_with_lookahead(int look_ahead, int beat_this)
// Evaluate a board position with lookahead.
// --int look_aheads:  How deep the lookahead should go to evaluate the move.
// --int beat_this: Value of another move that we're considering. If the
// current board position can't beat this, then cut it short.
// The return value is large if the position is good for the player who just
// moved.
{
	queue<string> moves;   // All possible opponent moves
	int value;             // Value of a board position after opponent moves
	int best_value;        // Evaluation of best opponent move
	game* future;          // Pointer to a future version of this game

	// Base case:
	if (look_ahead == 0 || is_game_over( ))
	{
		if (last_mover( ) == COMPUTER)
			return evaluate( );
		else
			return -evaluate( );
	}

	// Recursive case:
	// The level is above 0, so try all possible opponent moves. Keep the
	// value of the best of these moves from the opponent's perspective.
	compute_moves(moves);
	// assert(!moves.empty( ));
	best_value = INT_MIN;
	while (!moves.empty( ))
	{
		future = clone( );
		future->make_move(moves.front( ));
		value = future->eval_with_lookahead(look_ahead - 1, best_value);
		delete future;
		if (value > best_value)
		{
			if (-value <= beat_this)
				return INT_MIN + 1; // Alpha-beta pruning
			best_value = value;
		}
		moves.pop( );
	}

	// The value was calculated from the opponent's perspective.
	// The answer we return should be from player's perspective, so multiply times -1:
	return -best_value;
}

void game::make_computer_move( )
{
	queue<string> moves;
	int value;
	int best_value;
	string best_move;
	game* future;

	// Compute all legal moves that the computer could make.
	compute_moves(moves);
	//assert(!moves.empty( ));

	// Evaluate each possible legal move, saving the index of the best
	// in best_index and saving its value in best_value.
	best_value = INT_MIN;
	while (!moves.empty( ))
	{
		future = clone( );
		future->make_move(moves.front( ));
		value = future->eval_with_lookahead(SEARCH_LEVELS, best_value);
		delete future;
		if (value >= best_value)
		{
			best_value = value;
			best_move = moves.front( );
		}
		moves.pop( );
	}

	// Make the best move.
	make_move(best_move);
}

void game::make_human_move( ) {
	string move;

	move = get_user_move( );
	while (!is_legal(move))
	{
		display_message("Illegal move.\n");
		move = get_user_move( );
	}
	make_move(move);
}

}



                                                                                                                                                                                                                                                                                                                                                        game.h                                                                                              0000644 0001750 0001750 00000006153 13355253170 010655  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   // File: game.h (part of the namespace main_savitch_14)


#ifndef MAIN_SAVITCH_GAME
#define MAIN_SAVITCH_GAME
#include <queue>   // Provides queue<string>
#include <string>  // Provides string

namespace main_savitch_14
{
class game
{
public:
	// ENUM TYPE
	enum who { HUMAN, NEUTRAL, COMPUTER }; // Possible game outcomes

	// CONSTRUCTOR and DESTRUCTOR
	game( ) { move_number = 0; }
	virtual ~game( ) { }

	// PUBLIC MEMBER FUNCTIONS
	// The play function should not be overridden. It plays one game,
	// with the human player moving first and the computer second.
	// The computer uses an alpha-beta look ahead algorithm to select its
	// moves. The return value is the winner of the game (or NEUTRAL for
	// a tie).
	who play( );

protected:
	// *******************************************************************
	// OPTIONAL VIRTUAL FUNCTIONS (overriding these is optional)
	// *******************************************************************
	virtual void display_message(const std::string& message) const;
	virtual std::string get_user_move( ) const;
	virtual who last_mover( ) const
	{ return (move_number % 2 == 1 ? HUMAN : COMPUTER); }
	virtual int moves_completed( ) const { return move_number; }
	virtual who next_mover( ) const
	{ return (move_number % 2 == 0 ? HUMAN : COMPUTER); }
	virtual who opposite(who player) const
	{ return (player == HUMAN) ? COMPUTER : HUMAN; }
	virtual void countingPieces() = 0;
	virtual void whosTurn() = 0;
	virtual who winning( ) const;

	// *******************************************************************
	// VIRTUAL FUNCTIONS THAT MUST BE OVERRIDDEND:
	// The overriding function should call the original when it finishes.
	// *******************************************************************
	// Have the next player make a specified move:
	virtual void make_move(const std::string& move) { ++move_number; }
	// Restart the game from the beginning:
	virtual void restart( ) { move_number = 0; }

	// *******************************************************************
	// PURE VIRTUAL FUNCTIONS
	// *******************************************************************
	// (these must be provided for each derived class)
	// Return a pointer to a copy of myself:
	virtual game* clone( ) const = 0;
	// Compute all the moves that the next player can make:
	virtual void compute_moves(std::queue<std::string>& moves) const = 0;
	// Display the status of the current game:
	virtual void display_status( ) const = 0;
	// Evaluate a board position:
	// NOTE: positive values are good for the computer.
	virtual int evaluate( ) const = 0;
	// Return true if the current game is finished:
	virtual bool is_game_over( ) const = 0;
	// Return true if the given move is legal for the next player:
	virtual bool is_legal(const std::string& move) const = 0;

	int move_number;                     // Number of moves made so far

private:

	// STATIC MEMBER CONSTANT
	static const int SEARCH_LEVELS = 4;  // Levels for look-ahead evaluation

	// PRIVATE FUNCTIONS (these are the same for every game)
    int eval_with_lookahead(int look_ahead, int beat_this);
    void make_computer_move( );
    void make_human_move( );
};
}

#endif

                                                                                                                                                                                                                                                                                                                                                                                                                     othello.cc                                                                                          0000644 0001750 0001750 00000023663 13355253170 011555  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   // Brock Ferrell
// CS2401
// November 23, 2015
// Project7

#include "othello.h"

namespace main_savitch_14
{

void Othello::display_status()const
{
  cout << BLUE << "   _______ _______ _______ _______ _______ _______ _______ _______\n";
  cout << BLUE << "   |  A  | |  B  | |  C  | |  D  | |  E  | |  F  | |  G  | |  H  |" << endl;

  for (int y = 0; y < 8; y++)
  {
    cout << BLUE << "  " << y + 1 << BLUE << '|' << RESET;
    for (int x = 0; x < 8; x++)
    {
      if (!gameBoard[x][y].is_black() && !gameBoard[x][y].is_white())
      {
        cout << B_RED << "     " << RESET;
      }
      else if (gameBoard[x][y].is_black())
      {
        cout << B_BLACK << WHITE << BOLD << "  B  " << RESET;
      }
      else if (gameBoard[x][y].is_white())
      {
        cout << B_WHITE << BLACK << BOLD << "  W  " << RESET;
      }

      if (x == 7)
      {
        cout << BLUE << "|     " << RESET;
      }
      else if (y < 8 && x <= 6)
      {
        cout << BLUE << "| |" << RESET;
      }
    }
    if (y < 8)
    {
      cout << BLUE << endl << "   |_____| |_____| |_____| |_____| |_____| |_____| |_____| |_____|\n";
    }
  }
  cout << "\nEnter a move in the format(letter,number) (Ex: A3)" << endl;
}

void Othello::make_move(const string& move) {

  int c, r;
  c = int(toupper(move[0]) - 'A');
  r = int(move[1] - '1');

  if (next_mover() == HUMAN) {
    if (move == "s" || move == "S") {
      make_skips();
    }

    else if (gameBoard[c + 1][r].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c++;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c - 1][r].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c--;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c][r + 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        r++;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c][r - 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        r--;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c + 1][r - 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c++;
        r--;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c - 1][r - 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c--;
        r--;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c - 1][r + 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c--;
        r++;
      } while (gameBoard[c][r].is_white());
    }

    else if (gameBoard[c + 1][r + 1].is_white()) {
      do {
        gameBoard[c][r].set_black();
        c++;
        r++;
      } while (gameBoard[c][r].is_white());
    }
  }

  if (next_mover() == COMPUTER) {
    if (move == "s" || move == "S") {
      make_skips();
    }

    else if (gameBoard[c + 1][r].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c++;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c - 1][r].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c--;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c][r + 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        r++;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c][r - 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        r--;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c + 1][r - 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c++;
        r--;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c - 1][r - 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c--;
        r--;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c - 1][r + 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c--;
        r++;
      } while (gameBoard[c][r].is_black());
    }

    else if (gameBoard[c + 1][r + 1].is_black()) {
      do {
        gameBoard[c][r].set_white();
        c++;
        r++;
      } while (gameBoard[c][r].is_black());
    }
    skips = 0;
  }
  move_number++;
}


void Othello::restart() {

  gameBoard[3][3].set_white();
  gameBoard[3][4].set_black();
  gameBoard[4][3].set_black();
  gameBoard[4][4].set_white();

  white = 2;
  black = 2;
  skips = 0;
  openSpots = 60;
}

bool Othello::is_legal(const string& move)const {

  int c, r;
  c = int(toupper(move[0]) - 'A');
  r = int(move[1] - '1');

// BLACK'S TURN
  if (next_mover() == HUMAN) {
    if (gameBoard[c][r].is_blank()) {
      if (gameBoard[c][r - 1].is_white()) {
        do {
          r--;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());

      }

      if (gameBoard[c][r + 1].is_white()) {
        do {
          r++;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());

      }

      if (gameBoard[c + 1][r].is_white()) {
        do {
          c++;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());

      }

      if (gameBoard[c - 1][r].is_white()) {
        do {
          c--;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }

      if (gameBoard[c + 1][r - 1].is_white()) {
        do {
          c++;
          r--;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }

      if (gameBoard[c - 1][r - 1].is_white()) {
        do {
          c--;
          r--;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }

      if (gameBoard[c - 1][r + 1].is_white()) {
        do {
          c--;
          r++;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }

      if (gameBoard[c + 1][r + 1].is_white()) {
        do {
          c++;
          r++;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }
    } else {
      return false;
    }
  }

// WHITE'S TURN
  if (next_mover() == COMPUTER) {
    if (gameBoard[c][r].is_blank()) {
      if (gameBoard[c][r - 1].is_white()) {
        do {
          r--;
          if (gameBoard[c][r].is_black()) {
            return true;
          }
        } while (gameBoard[c][r].is_white());
      }

      if (gameBoard[c][r + 1].is_black()) {
        do {
          r++;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }

      if (gameBoard[c + 1][r].is_black()) {
        do {
          c++;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }

      if (gameBoard[c - 1][r].is_black()) {
        do {
          c--;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }

      if (gameBoard[c + 1][r - 1].is_black()) {
        do {
          c++;
          r--;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());

      }

      if (gameBoard[c - 1][r - 1].is_black()) {
        do {
          c--;
          r--;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }

      if (gameBoard[c - 1][r + 1].is_black()) {
        do {
          c--;
          r++;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }

      if (gameBoard[c + 1][r + 1].is_black()) {
        do {
          c++;
          r++;
          if (gameBoard[c][r].is_white()) {
            return true;
          }
        } while (gameBoard[c][r].is_black());
      }
    } else {
      return false;
    }
  }
}

void Othello::countingPieces() {

  int black = 0;
  int white = 0;

  for (int b = 0; b < 8; b++) {
    for (int a = 0; a < 8; a++) {
      if (gameBoard[a][b].is_white()) {
        white++;
      } else if (gameBoard[a][b].is_black()) {
        black++;
      }
    }
  }
  cout << "Total move: " << moves_completed() << endl;
  openSpots = 64 - (black + white);
  cout << "Black: "  << black << " ---" << " White: " << white;
  cout << endl;
}

bool Othello::is_game_over()const {

  if (skips == 2 || openSpots == 0 || black == 0 || white == 0) {
    cout << "The game is over.\n";
    return true;
  }
  else {
    return false;
  }
}

void Othello::whosTurn() {
  if (next_mover() == COMPUTER) {
    cout << "\nWhite's Turn!" << endl;
  }

  if (next_mover() == HUMAN) {
    cout << "\nBlack's Turn!" << endl;
  }
}

void Othello::make_skips() {
  skips++;
}

game::who Othello::winning()const {

  if (black > white) {
    return last_mover();
  } else if (white > black) {
    return next_mover();
  } else {
    return NEUTRAL;
  }

}

int Othello::evaluate()const {

  int tmp_white = w;
  int tmp_black = b;

  if (gameBoard[0][0].is_white() || gameBoard[7][7].is_white() || gameBoard[0][7].is_white() || gameBoard[7][0].is_white()) {
    tmp_white += 5;
  }
  if (gameBoard[0][0].is_black() || gameBoard[7][7].is_black() || gameBoard[0][7].is_black() || gameBoard[7][0].is_black()) {
    tmp_black += 5;
  }

  if (b > w) {
    return b;
  } else if (w > b) {
    return w;
  }
}

void Othello::compute_moves(std::queue<std::string>& moves)const {
  string r;
  string c;
  string move;

  for (int i = 0; i < 8; i++) {
    c = i + 'A';
    for (int j = 0; j < 8; j++) {
      r = j + '1';
      move = c + r;
      if (is_legal(move)) {
        moves.push(move);
      }
      else {

      }
    }
  }
}

}
                                                                             othello.h                                                                                           0000644 0001750 0001750 00000001401 13355253170 011401  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   // Brock Ferrell
// CS2401
// November 23, 2015
// Project7

#ifndef OTHELLO_H
#define OTHELLO_H
#include "game.h"
#include "piece.h"
#include "colors.h"
#include <iostream>
using namespace std;

namespace main_savitch_14
{

class Othello: public game {
public:

	Othello() {}

	void display_status()const;
	int evaluate()const;
	bool is_game_over()const;
	bool is_legal(const string& move)const;
	void make_move(const string& move);
	void restart();
	void make_skips();
	void countingPieces();
	void whosTurn();
	game* clone()const{return new Othello(*this);}
	void compute_moves(std::queue<std::string>& moves)const;
	who winning()const;

protected:
	int black;
	int white;
	int skips;
	int openSpots;
	int b;
	int w;

private:
	piece gameBoard[8][8];
};
}

#endif


                                                                                                                                                                                                                                                               colors.h                                                                                            0000644 0001750 0001750 00000003634 13355253170 011246  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   //***********************************************************
//
//  File:     colors.h
//
//  Author:   Matthew Beldyk
//  Email:    mb245002@ohiou.edu
//
//  Usage:    I created this file to house some names string
//            constants with escape codes for colors in them
//            this makes it much easier for me to do colors.
//            I can still use the codes if I want, but this
//            works too.  try the statement:
//            cout<<BLUE<<"I like cookies"<<endl;
//
//		  You may use this whereever you want, but if you
//		  make any large improvements or whatever, I am
//		  curious, so email 'em my way, please.
//
//***********************************************************
//
//  all credit given to Matthew Beldyk for writing this file
//  he gave me permission to try out in my programs
//  just wanted to use to make everything look nice
//
//***********************************************************

#ifndef COLORS_H
#define COLORS_H

#include <string>
using namespace std;

const string BLINK     = "\e[5m";          //don't use this your
					   //professor will probibly
					   //beat you to death if
					   //you do feel the need to
					   //use blink!!!
const string BOLD      = "\e[1m";

const string RESET     = "\e[0m";
const string ERROR     = "\e[1;41;37m\a";
const string MENU       = "\e[44;37m";

const string BLACK      = "\e[30m";
const string RED        = "\e[31m";
const string GREEN      = "\e[32m";
const string YELLOW     = "\e[33m";
const string BLUE       = "\e[34m";
const string MAGENTA    = "\e[35m";
const string CYAN       = "\e[36m";
const string WHITE      = "\e[37m";

const string B_BLACK   = "\e[40m";
const string B_RED     = "\e[41m";
const string B_GREEN   = "\e[42m";
const string B_YELLOW  = "\e[43m";
const string B_BLUE    = "\e[44m";
const string B_MAGENTA = "\e[45m";
const string B_CYAN    = "\e[46m";
const string B_WHITE   = "\e[47m";

#endif //COLORS_H

                                                                                                    piece.h                                                                                             0000644 0001750 0001750 00000001077 13355253170 011031  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   // Brock Ferrell
// CS2401
// November 23, 2015
// Project7

#ifndef PIECE_H
#define PIECE_H
enum color {black, white, blank};

class piece {
public:
	piece() {theColor = blank;}

	void flip()
	{
		if (theColor == white) {
			theColor = black;
		}
		else if (theColor == black) {
			theColor = white;
		}
	}

	bool is_blank()const {return theColor == blank;}
	bool is_black()const {return theColor == black;}
	bool is_white()const {return theColor == white;}
	void set_white() {theColor = white;}
	void set_black() {theColor = black;}

private:
	color theColor;

};

#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                 Makefile                                                                                            0000644 0001750 0001750 00000000735 13355347673 011247  0                                                                                                    ustar   ryan                            ryan                                                                                                                                                                                                                   
game : main.o game.o othello.o
	g++ -o game main.o game.o othello.o

main.o : main.cc game.h othello.h
	g++ -c main.cc

game.o : game.cc game.h
	g++ -c game.cc

othello.o : othello.cc othello.h game.h colors.h piece.h
	g++ -c othello.cc othello.h game.h colors.h piece.h

archive: main.cc game.cc game.h othello.cc othello.h colors.h piece.h Makefile
	tar cvf main.cc game.cc game.h othello.cc othello.h colors.h piece.h Makefile

clean :
	rm game *.o *.gch
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   