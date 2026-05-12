#include "logic.h"
#include <stdlib.h>
#include <time.h>

void setDefaultTileValues(struct board* board) {
	for (int i = 0; i < board->height; i++) {
		for (int j = 0; j < board->width; j++) {
			board->matrix[i][j].state = TileState::Hidden;
			board->matrix[i][j].bHasMine = false;
		}
	}
}

int getMinesToPlaceNum(struct board* board) {
	return int((board->height * board->width) * 0.15);
}

void placeMinesOnBoard(struct board* board) {
	int leftToPlace = board->minesNum;
	while (leftToPlace > 0) {
		int xToPlace = rand() % board->height;
		int yToPlace = rand() % board->width;
		if (!board->matrix[xToPlace][yToPlace].bHasMine) {
			board->matrix[xToPlace][yToPlace].bHasMine = true;
			leftToPlace -= 1;
		}
	}
}

int getAdjacentMinesNum(struct board* board, int tileX, int tileY) {
	int mineCounter = 0;
	for (int i = -1; i <= 1; i++) {
		int newX = tileX + i;
		if (newX < 0 || newX >= board->height) {
			continue;
		}
		for (int j = -1; j <= 1; j++) {
			int newY = tileY + j;
			if (newY < 0 || newY >= board->width || (i == 0 && j == 0)) {
				continue;
			}
			if (board->matrix[newX][newY].bHasMine) {
				mineCounter += 1;
			}
		}
	}

	return mineCounter;
}

void setTileNumbers(struct board* board) {
	for (int i = 0; i < board->height; i++) {
		for (int j = 0; j < board->width, j++) {
			if (board->matrix[i][j].bHasMine) {
				continue;
			}
			board->matrix[i][j].adjacentMinesNum = getAdjacentMinesNum(board, i, j);
		}
	}
}

void initBoard(struct board* board) {
	board->height = 9;
	board->width = 9;
	board->minesNum = getMinesToPlaceNum(board);
	board->state = GameState::Running;

	setDefaultTileValues(board);
	placeMinesOnBoard(board);
	setTileNumbers(board);
}
