//File "2048.cpp"

#include "2048.h"

//Cell section
Cell::Cell(Point pos, char key) : m_key(key), m_pos(pos) {} //Cell constructor

Point Cell::getPos() const { //Get cell position
	return m_pos;
}

char Cell::getKey() const { //Get cell key
	return m_key;
}

bool Cell::setPos(int x, int y) { //Set cell position
	if (x < 0 || x >= BOARD_WIDTH
		|| y < 0 || y >= BOARD_HEIGHT)
		return false;
	m_pos.x = x;
	m_pos.y = y;
	return true;
}

bool Cell::setKey(char key) {
	if (key >= 32 && key <= 126) {
		m_key = key;
		return true;
	}
	return false;
}

//Tile section
Tile::Tile(Point mainPos, int num) { //Tile constructor
	m_cell[CENTRAL].setKey(num - 1 + 'A');
	for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT && i != CENTRAL; i++)
		m_cell[i].setKey(' ');
	int tempX[TILE_WIDTH * TILE_HEIGHT];
	int k = 0;
	for (int i = 0; i < TILE_HEIGHT; i++)
		for (int j = -TILE_WIDTH / 2; j <= TILE_WIDTH / 2; j++)
			tempX[k++] = j;
	int tempY[TILE_WIDTH * TILE_HEIGHT];
	k = 0;
	for (int i = -TILE_HEIGHT / 2; i <= TILE_HEIGHT / 2; i++)
		for (int j = 0; j < TILE_WIDTH; j++)
			tempY[k++] = i;
	for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT; i++)
		m_cell[i].setPos(mainPos.x + tempX[i], mainPos.y + tempY[i]);
	m_num = num;
	m_isDrawn = false;
}

bool operator==(const Tile& t1, const Tile& t2) {
	int num1 = t1.getNum(), num2 = t2.getNum();
	return num1 == num2;
}

Tile operator+(const Tile& t1, const Tile& t2) {
	Tile t;
	for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT; i++) {
		t.m_cell[i].setPos(t1.m_cell[i].getPos().x, t1.m_cell[i].getPos().y);
	}
	t.m_cell[CENTRAL].setKey(t1.m_num + 'A');
	t.m_num = t1.m_num + 1;

	return t;
}

bool Tile::drawTile() {
	if (m_isDrawn)
		return false;
	for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT; i++) {
		goToXY(m_cell[i].getPos().x, m_cell[i].getPos().y);
		setColor((m_num - 1) * 16 + 15);
		if (!isEmpty())
			cout << m_cell[i].getKey();
		else {
			setColor(ColorCode_Black);
			cout << ' ';
		}
	}
	m_isDrawn = true;
	return true;
}

bool Tile::eraseTile() {
	if (!m_isDrawn)
		return false;
	for (int i = 0; i < TILE_WIDTH * TILE_HEIGHT; i++) {
		goToXY(m_cell[i].getPos().x, m_cell[i].getPos().y);
		setColor(ColorCode_Black);
		cout << m_cell[i].getKey();
	}
	m_isDrawn = false;
	return true;
}

Point Tile::getPos() const {
	return m_cell[CENTRAL].getPos();
}

int Tile::getNum() const {
	return m_num;
}

bool Tile::isEmpty() const {
	return m_num == -1;
}

void Tile::makeEmpty() {
	m_num = -1;
}

//GameBoard section
GameBoard::GameBoard() {
	srand(static_cast<unsigned int>(time(0)));
	int randPos[TILE_NUM_X];
	for (int i = 0; i < TILE_NUM_X; i++)
		randPos[i] = i;
	random_shuffle(&randPos[0], &randPos[TILE_NUM_X - 1]);
	for (int i = 0; i < TILE_NUM_Y; i++) {
		vector<Tile> tempVec;
		for (int j = 0; j < TILE_NUM_X; j++)
			if (i == randPos[j])
				tempVec.push_back(Tile({ START_POINT.x + DISTANCE_X * j, START_POINT.y + DISTANCE_Y * i }, 1));
			else
				tempVec.push_back(Tile({ START_POINT.x + DISTANCE_X * j, START_POINT.y + DISTANCE_Y * i }, -1));
		m_tiles.push_back(tempVec);
	}
}

bool GameBoard::drawBoard() {
	if (!m_isChanged)
		return false;
	for (int i = 0; i < TILE_NUM_Y; i++)
		for (int j = 0; j < TILE_NUM_X; j++) {
			m_tiles[i][j].eraseTile();
			m_tiles[i][j].drawTile();
		}
	m_isChanged = false;
}

bool GameBoard::swipeUp() {
	bool res = false;
	for (int j = 0; j < TILE_NUM_X; j++) {
		int bar = 0;
		for (int i = 0; i < TILE_NUM_Y; i++) {
			if (m_tiles[i][j].isEmpty())
				continue;
			int iterY = i - 1;
			while (iterY >= bar && m_tiles[iterY][j].isEmpty())
				iterY--;
			if (iterY + 1 != i)
				res = true;
			if (iterY >= bar && m_tiles[iterY][j] == m_tiles[i][j]) {
				m_tiles[iterY][j] = m_tiles[iterY][j] + m_tiles[i][j];
				m_tiles[i][j].makeEmpty();
				bar = iterY + 1;
				res = true;
			}
			else {
				int temp1 = m_tiles[i][j].getNum();
				int temp2 = m_tiles[iterY + 1][j].getNum();
				m_tiles[i][j] = Tile(m_tiles[i][j].getPos(), temp2);
				m_tiles[iterY + 1][j] = Tile(m_tiles[iterY + 1][j].getPos(), temp1);
			}
		}
	}
	if (res)
		m_isChanged = true;
	return res;
}

bool GameBoard::swipeDown() {
	bool res = false;
	for (int j = 0; j < TILE_NUM_X; j++) {
		int bar = TILE_NUM_Y - 1;
		for (int i = TILE_NUM_Y - 1; i >= 0; i--) {
			if (m_tiles[i][j].isEmpty())
				continue;
			int iterY = i + 1;
			while (iterY <= bar && m_tiles[iterY][j].isEmpty())
				iterY++;
			if (iterY - 1 != i)
				res = true;
			if (iterY <= bar && m_tiles[iterY][j] == m_tiles[i][j]) {
				m_tiles[iterY][j] = m_tiles[iterY][j] + m_tiles[i][j];
				m_tiles[i][j].makeEmpty();
				bar = iterY - 1;
				res = true;
			}
			else {
				int temp1 = m_tiles[i][j].getNum();
				int temp2 = m_tiles[iterY - 1][j].getNum();
				m_tiles[i][j] = Tile(m_tiles[i][j].getPos(), temp2);
				m_tiles[iterY - 1][j] = Tile(m_tiles[iterY - 1][j].getPos(), temp1);
			}
		}
	}
	if (res)
		m_isChanged = true;
	return res;
}

bool GameBoard::swipeRight() {
	bool res = false;
	for (int i = 0; i < TILE_NUM_Y; i++) {
		int bar = TILE_NUM_X - 1;
		for (int j = TILE_NUM_X - 1; j >= 0; j--) {
			if (m_tiles[i][j].isEmpty())
				continue;
			int iterX = j + 1;
			while (iterX <= bar && m_tiles[i][iterX].isEmpty())
				iterX++;
			if (iterX - 1 != j)
				res = true;
			if (iterX <= bar && m_tiles[i][iterX] == m_tiles[i][j]) {
				m_tiles[i][iterX] = m_tiles[i][iterX] + m_tiles[i][j];
				m_tiles[i][j].makeEmpty();
				bar = iterX - 1;
				res = true;
			}
			else {
				int temp1 = m_tiles[i][j].getNum();
				int temp2 = m_tiles[i][iterX - 1].getNum();
				m_tiles[i][j] = Tile(m_tiles[i][j].getPos(), temp2);
				m_tiles[i][iterX - 1] = Tile(m_tiles[i][iterX - 1].getPos(), temp1);
			}
		}
	}
	if (res)
		m_isChanged = true;
	return res;
}

bool GameBoard::swipeLeft() {
	bool res = false;
	for (int i = 0; i < TILE_NUM_Y; i++) {
		int bar = 0;
		for (int j = 0; j < TILE_NUM_X; j++) {
			if (m_tiles[i][j].isEmpty())
				continue;
			int iterX = j - 1;
			while (iterX >= bar && m_tiles[i][iterX].isEmpty())
				iterX--;
			if (iterX + 1 != j)
				res = true;
			if (iterX >= bar && m_tiles[i][iterX] == m_tiles[i][j]) {
				m_tiles[i][iterX] = m_tiles[i][iterX] + m_tiles[i][j];
				m_tiles[i][j].makeEmpty();
				bar = iterX + 1;
				res = true;
			}
			else {
				int temp1 = m_tiles[i][j].getNum();
				int temp2 = m_tiles[i][iterX + 1].getNum();
				m_tiles[i][j] = Tile(m_tiles[i][j].getPos(), temp2);
				m_tiles[i][iterX + 1] = Tile(m_tiles[i][iterX + 1].getPos(), temp1);
			}
		}
	}
	if (res)
		m_isChanged = true;
	return res;
}

void GameBoard::addTile() {
	int randX, randY;
	srand(static_cast<unsigned int>(time(0)));
	do {
		randX = rand() % TILE_NUM_X;
		randY = rand() % TILE_NUM_Y;
		if (m_tiles[randY][randX].isEmpty()) {
			m_tiles[randY][randX] = Tile(m_tiles[randY][randX].getPos(), rand() % 2 + 1);
			break;
		}
	} while (!m_tiles[randY][randX].isEmpty());
}

bool GameBoard::isFull() const{
	for (int i = 0; i < TILE_NUM_Y; i++)
		for (int j = 0; j < TILE_NUM_X; j++)
			if (m_tiles[i][j].isEmpty())
				return false;
	return true;
}

bool GameBoard::outOfMove() const {
	if (!isFull()) //If not full, return false
		return false;
	for (int i = 0; i < TILE_NUM_Y - 1; i++)
		for (int j = 0; j < TILE_NUM_X; j++)
			if (m_tiles[i][j] == m_tiles[i + 1][j])
				return false;
	for (int j = 0; j < TILE_NUM_X - 1; j++)
		for (int i = 0; i < TILE_NUM_Y; i++)
			if (m_tiles[i][j] == m_tiles[i][j + 1])
				return false;
	return true;
}