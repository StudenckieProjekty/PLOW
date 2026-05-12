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
	Running = 0,
	Won = 1,
	Lost = 2
};

struct board {
	struct tile matrix[9][9];
	int width;
	int height;
	int minesNum;
	enum GameState state;
};

void initBoard();
