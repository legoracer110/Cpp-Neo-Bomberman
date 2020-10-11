#pragma once
class Bomb
{
private:
	int posX;
	int posY;
public:
	Bomb();
	~Bomb();

	inline int getX() { return posX; }
	inline int getY() { return posY; }
	inline void setX(int num) { posX = num; }
	inline void setY(int num) { posY = num; }

	long s_time;
};
