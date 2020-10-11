#pragma once
class Enemy
{
	friend class Player;
private:
	int posX = 13;
	int posY = 11;
public:
	Enemy();
	~Enemy();
	inline int getX() { return this->posX; }
	inline int getY() { return this->posY; }
	inline void setX(int num) { this->posX += num; }
	inline void setY(int num) { this->posY += num; }
	inline void setX_Y(int num1, int num2) { posX = num1; posY = num2; }
};