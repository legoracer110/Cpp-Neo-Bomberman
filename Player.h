#pragma once

class Bomb;

class Player
{
private:
	int posX;
	int posY;

public:
	Player();
	~Player();

	int status = 0;
	int status2 = 0;

	Bomb* pBomb[MAX_BOMB];

	int Power = 1;
	int BombAmount = 1;

	inline int getX() { return this->posX; }
	inline int getY() { return this->posY; }
	inline void setX(int num) { this->posX += num; }
	inline void setY(int num) { this->posY += num; }
	inline void setX_Y(int num1, int num2) { this->posX = num1; this->posY = num2; }
};

