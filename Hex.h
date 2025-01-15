#pragma once

class Hex {
private:
	char** board = nullptr; 
	int size = 0; 
	int pawns_number;
	bool is_board_correct;
	char lastplayer; 

	void deleteBoard(char** boardToDelete) const;
	static char getOpponent(char player);

	void getBoardSize();
	void loadBoard();

	bool checkWinPath(char** boardToCheck, int x, int y, char player);
	char isGameOver();
	char isGameOver(char** otherBoard);
	bool isBoardPossible();

	bool opponentCanBlock2Moves(char player);

	bool winNaive1Move(char player);
	bool winNaive2Moves(char player);
	bool winPerfect1Move(char player);
	bool winPerfect2Moves(char player);

public:
	Hex();
	void queryService();
	~Hex();

};