#define _CRT_SECURE_NO_WARNINGS
#include "Hex.h"
#include <iostream>
#include <cstring>

#define SPACE ' ' 
#define RED 'r'
#define BLUE 'b'
#define CHECK '0'

#define MAXSIZE 11
#define MAXQUERY 45
#define MAXQUERY_SCANF "%45s"

void Hex::deleteBoard(char** boardToDelete) const {
	for (int i = 0; i < size; i++) {
		delete[] boardToDelete[i];
	}
	delete[] boardToDelete;
}
char Hex::getOpponent(char player) {
	if (player == RED) return BLUE;
	else return RED;
}

void Hex::getBoardSize() {
	char c = 0;
	int space = 0;

	while (c != '-') {
		c = (char)getchar();
		if (c == SPACE) space++;
		else if (c == '\n') space = 0;
	}
	for (int i = 0; i < MAXSIZE; i++) {
		if (space == 3 * i) {
			size = i + 1;
		}
	}
}
void Hex::loadBoard() {
	int red = 0, blue = 0;

	while ((char)getchar() != '\n') {}

	for (int i = 0; i < 2 * size - 1; i++) {
		char c = 0;
		int j = 0;

		while (c != '\n') {
			c = (char)getchar();
			if (c == '<') {
				c = (char)getchar();
				if (c == SPACE) {
					c = (char)getchar();

					if (i < size) {
						board[j][i - j] = c;
					}
					else {
						board[i - size + 1 + j][size - 1 - j] = c;
					}

					if (c == RED) red++;
					if (c == BLUE) blue++;
				}
				j++;
			}
		}
	}
	while ((char)getchar() != '\n') {}

	pawns_number = red + blue;
	if (red == blue || red == blue + 1) {
		is_board_correct = true;
	}
	else is_board_correct = false;

	lastplayer = BLUE;
	if (pawns_number % 2) {
		lastplayer = RED;
	}
}

bool Hex::checkWinPath(char** boardToCheck, int x, int y, char player) {
	if (boardToCheck[x][y] != player) return false;
	boardToCheck[x][y] = CHECK;

	if (player == RED) {
		if (x == 0) return true;
	}
	else if (player == BLUE) {
		if (y == 0) return true;
	}

	if (x + 1 < size) {
		if (checkWinPath(boardToCheck, x + 1, y, player)) return true;
	}
	if (x > 0) {
		if (checkWinPath(boardToCheck, x - 1, y, player)) return true;
	}
	if (y + 1 < size) {
		if (checkWinPath(boardToCheck, x, y + 1, player)) return true;
	}
	if (y > 0) {
		if (checkWinPath(boardToCheck, x, y - 1, player)) return true;
	}
	if (x + 1 < size && y + 1 < size) {
		if (checkWinPath(boardToCheck, x + 1, y + 1, player)) return true;
	}
	if (x > 0 && y > 0) {
		if (checkWinPath(boardToCheck, x - 1, y - 1, player)) return true;
	}

	return false;
}
char Hex::isGameOver() {	
	char** newBoard = new char* [size];
	for (int i = 0; i < size; i++) {
		newBoard[i] = new char[size];
		for (int j = 0; j < size; j++) {
			newBoard[i][j] = board[i][j];
		}
	}

	for (int i = 0; i < size; i++) {
		if (checkWinPath(newBoard, size - 1, i, RED)) {
			deleteBoard(newBoard);
			return RED;
		}
		if (checkWinPath(newBoard, i, size - 1, BLUE)) {
			deleteBoard(newBoard);
			return BLUE;
		}
	}

	deleteBoard(newBoard);
	return SPACE;
}
char Hex::isGameOver(char** otherBoard) {
	char** newBoard = new char* [size];
	for (int i = 0; i < size; i++) {
		newBoard[i] = new char[size];
		for (int j = 0; j < size; j++) {
			newBoard[i][j] = otherBoard[i][j];
		}
	}

	for (int i = 0; i < size; i++) {
		if (checkWinPath(newBoard, size - 1, i, RED)) {
			deleteBoard(newBoard);
			return RED;
		}
		if (checkWinPath(newBoard, i, size - 1, BLUE)) {
			deleteBoard(newBoard);
			return BLUE;
		}
	}

	deleteBoard(newBoard);
	return SPACE;
}
bool Hex::isBoardPossible() {
	char color = isGameOver();
	if (color != SPACE) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (board[j][i] == color) {
					board[j][i] = SPACE;

					if (isGameOver() == SPACE && lastplayer == color) {
						board[j][i] = color;
						return true;
					}
					board[j][i] = color;
				}
			}
		}
		return false;
	}
	else return true;
}

#define MIN_PAWNS_1_MOVE 2 * (size - 1) - 1
#define MIN_PAWNS_2_MOVES 2 * (size - 2) - 1
#define MAX_PAWNS_1_MOVE size * size - 1
#define MAX_PAWNS_2_MOVES size * size - 3

bool Hex::opponentCanBlock2Moves(char player) {
	char opponent = getOpponent(player);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[j][i] == SPACE) {
				board[j][i] = player;

				if (isGameOver() == player) {
					board[j][i] = opponent;

					if (isGameOver() == SPACE) {
						for (int k = 0; k < size; k++) {
							for (int l = 0; l < size; l++) {
								if (board[l][k] == SPACE) {
									board[l][k] = player;

									if (isGameOver() == player) {
										board[j][i] = SPACE;
										board[l][k] = SPACE;
										return false;
									}
									board[l][k] = SPACE;
								}
							}
						}
					}
				}
				board[j][i] = SPACE;
			}
		}
	}
	return true;
}

bool Hex::winNaive1Move(char player) {
	if(lastplayer != player) {
		if(pawns_number < MIN_PAWNS_1_MOVE + 1 || pawns_number > MAX_PAWNS_1_MOVE) return false;
	}
	else if(pawns_number < MIN_PAWNS_1_MOVE || pawns_number > MAX_PAWNS_1_MOVE - 1) return false;

	for (int i = 0; i < size; i++) { 
		for (int j = 0; j < size; j++) { 
			if (board[j][i] == SPACE) { 
				board[j][i] = player;
				if (isGameOver() == player) { 
					board[j][i] = SPACE;
					return true;
				}
				board[j][i] = SPACE; 
			}
		}
	}
	return false;
}
bool Hex::winNaive2Moves(char player) {
	if(lastplayer != player) {
		if(pawns_number < MIN_PAWNS_2_MOVES + 1 || pawns_number > MAX_PAWNS_2_MOVES) return false;
	}
	else if(pawns_number < MIN_PAWNS_2_MOVES || pawns_number > MAX_PAWNS_2_MOVES - 1) return false;

	char** newBoard = new char* [size];
	for (int i = 0; i < size; i++) {
		newBoard[i] = new char[size];
		for (int j = 0; j < size; j++) {
			newBoard[i][j] = board[i][j];
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (newBoard[j][i] == SPACE) {
				newBoard[j][i] = player;
				if (isGameOver(newBoard) == SPACE) {
					for (int k = 0; k < size; k++) {
						for (int l = 0; l < size; l++) {
							if (newBoard[l][k] == SPACE) {
								newBoard[l][k] = player;

								if (isGameOver(newBoard) == player) {
									deleteBoard(newBoard);
									return true;
								}

								newBoard[l][k] = SPACE;
							}
						}
					} 
					newBoard[j][i] = CHECK;
				}
				else newBoard[j][i] = SPACE;
			}
		}
	} 
	deleteBoard(newBoard);
	return false;
}
bool Hex::winPerfect1Move(char player) {
	if(lastplayer != player) {
		if(pawns_number < MIN_PAWNS_1_MOVE + 1 || pawns_number > MAX_PAWNS_1_MOVE) return false;
	}
	else if(pawns_number < MIN_PAWNS_1_MOVE || pawns_number > MAX_PAWNS_1_MOVE - 1) return false;

	int plr = 0, opp = 0;
	char opponent = getOpponent(player);

	if (lastplayer != player) {
		bool res = winNaive1Move(player);
		return res;
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (board[j][i] == SPACE) {
					board[j][i] = opponent;

					bool checker = false;
					for (int k = 0; k < size; k++) {
						for (int l = 0; l < size; l++) {
							if (board[l][k] == SPACE) {
								board[l][k] = player;
								if (isGameOver() == player) {
									checker = true;
									plr++;
								}

								board[l][k] = SPACE;
							}
							if (checker) break;
						}
						if (checker) break;
					}

					board[j][i] = SPACE;
					opp++;
				}
			}
		}
	}

	if (plr == opp) {
		return true;
	}
	return false;
}
bool Hex::winPerfect2Moves(char player) {
	if(lastplayer != player) {
		if(pawns_number < MIN_PAWNS_2_MOVES + 1 || pawns_number > MAX_PAWNS_2_MOVES) return false;
	}
	else if(pawns_number < MIN_PAWNS_2_MOVES || pawns_number > MAX_PAWNS_2_MOVES - 1) return false;

	char opponent = getOpponent(player);

	if (player != lastplayer) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (board[j][i] == SPACE) {
					board[j][i] = player;

					if (!opponentCanBlock2Moves(player)) {
						board[j][i] = SPACE;
						return true;
					}
					board[j][i] = SPACE;
				}
			}
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (board[j][i] == SPACE) {
					board[j][i] = opponent;

					if (isGameOver() == SPACE) {
						this->lastplayer = opponent;
						pawns_number++; 
						if (!winPerfect2Moves(player)) {
							this->lastplayer = player;
							pawns_number--; 
							board[j][i] =  SPACE;
							return false;
						}
						pawns_number--;
						this->lastplayer = player;
					}
					else {
						board[j][i] = SPACE;
						return false;
					}
					board[j][i] = SPACE;
				}
			}
		}
		return true;
	}
	return false;
}

Hex::Hex() {
	getBoardSize();
	if (board) deleteBoard(board);
	board = new char* [size]; 
	for (int i = 0; i < size; i++) { 
		board[i] = new char[size]; 
	} 
	loadBoard();
}
void Hex::queryService() {
	char query[MAXQUERY];
	scanf(MAXQUERY_SCANF, &query);

	if (strcmp(query, "BOARD_SIZE") == 0) {
		printf("%d\n", size);
	}
	else if (strcmp(query, "PAWNS_NUMBER") == 0) {
		printf("%d\n", pawns_number);
	}
	else if (strcmp(query, "IS_BOARD_CORRECT") == 0) {
		if (is_board_correct) printf("YES\n");
		else printf("NO\n");
	}
	else if (strcmp(query, "IS_GAME_OVER") == 0) {
		if (is_board_correct) {
			switch (isGameOver()) {
			case RED:
				printf("YES RED\n");
				break;
			case BLUE:
				printf("YES BLUE\n");
				break;
			case SPACE:
				printf("NO\n");
				break;
			}
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "IS_BOARD_POSSIBLE") == 0) {
		if (is_board_correct && isBoardPossible()) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winNaive1Move(RED)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winNaive1Move(BLUE)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winNaive2Moves(RED)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winNaive2Moves(BLUE)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winPerfect1Move(RED)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winPerfect1Move(BLUE)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winPerfect2Moves(RED)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
	else if (strcmp(query, "CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT") == 0) {
		if (is_board_correct && isGameOver() == SPACE && winPerfect2Moves(BLUE)) {
			printf("YES\n");
		}
		else printf("NO\n");
	}
}
Hex::~Hex() {
	deleteBoard(board);
}