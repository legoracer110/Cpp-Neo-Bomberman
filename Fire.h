#pragma once
class Fire
{
private:
	int posX = 0;
	int posY = 0;
public:
	Fire();
	~Fire();
	inline int getX() { return posX; }
	inline int getY() { return posY; }
	inline void setXY(int a, int b) { posX = a; posY = b; }
	long s_time;
};