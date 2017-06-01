#include "2048.h"

int main() {
	fixConsoleWindow();
	hideCursor();

	GameBoard board;
	while (true) {
		board.drawBoard();
		if (board.outOfMove())
			break;
		if (_kbhit()) {
			char c = toupper(_getch());
			if (c == 'W')
				if(board.swipeUp())
					board.addTile();
			if (c == 'D')
				if(board.swipeRight())
					board.addTile();
			if (c == 'S')
				if(board.swipeDown())
					board.addTile();;
			if (c == 'A')
				if(board.swipeLeft())
					board.addTile();;
		}
	//	Sleep(10);
	}

	return 0;
}