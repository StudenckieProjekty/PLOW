#pragma once

enum TileState {
	Hidden = 0,
	Revealed = 1,
	Flagged = 2,
	QuestionMarked = 3
};

struct tile {
	int adjacentMinesNum;
	enum TileState state;
	bool bHasMine;
};

enum GameState {
	Menu = 0,
	Running = 1,
	Won = 2,
	Lost = 3,
	Exit = 4
};

struct board {
	struct tile matrix[9][9];
	int width;
	int height;
	int minesNum;
	enum GameState state;
};

void createBoard(struct board* board);

void initBoardAfterFirstClick(struct board* board, int clickedX, int clickedY);

void revealTile(struct board* board, int tileX, int tileY);
