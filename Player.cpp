#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	srand(time(NULL));
	int a = rand() % 4;
	switch (a)
	{
	case 0:
	{
		posX = 1; posY = 1; break;
	}
	case 1:
	{
		posX = 13; posY = 1; break;
	}
	case 2:
	{
		posX = 1; posY = 11; break;
	}
	case 3:
	{
		posX = 13, posY = 11; break;
	}
	}
	pBomb[0] = { NULL, };
}

Player::~Player()
{
}