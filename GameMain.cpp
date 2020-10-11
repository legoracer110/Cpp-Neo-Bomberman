#include "stdafx.h"
#include "GameMain.h"

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int keyCnt;

enum EMapAttribute
{
	E_Empty = 0,
	E_Player = 1,
	E_Wall = 2,
	E_Destroyable = 3,
	E_Bomb = 4,
	E_Fire = 5,
	E_Enemy = 6,
	E_PowerItem = 7,
	E_PlusBombItem = 8,
	E_Well = 9,
	E_Player2 = 10,
};

enum EDirection
{
	LEFT = 75,
	RIGHT = 77,
	UP = 72,
	DOWN = 80,
};

enum EDirection2
{
	P1_LEFT = 'a',
	P1_RIGHT = 'd',
	P1_UP = 'w',
	P1_DOWN = 's',
	P1_ATTACK = 'g',
	P1_SKILL = 'h',
	P2_LEFT = 75,
	P2_RIGHT = 77,
	P2_UP = 72,
	P2_DOWN = 80,
	P2_ATTACK = 'j',
	P2_SKILL = 'l',
};




void drawMap2(Player* pPlayer, Player* pPlayer2, int map[HEIGHT][WIDTH], int cpymap[HEIGHT][WIDTH], int itemmap[HEIGHT][WIDTH], bool gameEnd,
	Bomb* pBomb[MAX_BOMB], Bomb* pBomb2[MAX_BOMB], Fire* pFire[MAX_BOMB], Fire* pFire2[MAX_BOMB], int s_map, int s_char, int s_char2)
{
	int prob;
	int prob2;
	prob = rand() % 3;			//		아이템 뜰 확률 SEED
	prob2 = rand() % 3;

	if (s_char == 1)					//		player1 케릭터 1 일때의 패시브 설정(1)
	{
		pPlayer->BombAmount = 2;
	}
	else if (s_char == 4)
	{
		pPlayer->Power = 2;				//		player1 케릭터 1 일때의 패시브 설정(2)
	}

	if (s_char2 == 1)
	{
		pPlayer2->BombAmount = 2;		//		player2 케릭터 1 일때의 패시브 설정(1)
	}
	else if (s_char2 == 4)
	{
		pPlayer2->Power = 2;			//		player2 케릭터 1 일때의 패시브 설정(1)
	}

	if (pPlayer->status2 == 0 && map[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_Fire)	//	플레이어 1이 죽는 조건 (불길에 닿았을때)
	{
		gotoxy(30, 26);
		cout << "Player1이 죽었습니다!" << endl;
		system("pause");
		return;
	}

	if (pPlayer2->status2 == 0 && map[pPlayer2->getY()][pPlayer2->getX()] == EMapAttribute::E_Fire)	//	플레이어 2가 죽는 조건 (불길에 닿았을때)
	{
		gotoxy(30, 26);
		cout << "Player2이 죽었습니다!" << endl;
		system("pause");
		return;
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			for (int m = 0; m < pPlayer->BombAmount; m++)		//	플레이어 1
			{
				if (pBomb[m] == NULL && map[i][j] == EMapAttribute::E_Bomb)		//	폭탄 사라진 위치
					map[i][j] = EMapAttribute::E_Empty;
				if (pFire[m] == NULL && map[i][j] == EMapAttribute::E_Fire)		//	불길 사라진 위치
				{
					if (itemmap[i][j] == EMapAttribute::E_Destroyable)
					{
						switch (prob)
						{
						case 0: map[i][j] = EMapAttribute::E_PowerItem; break;
						case 1: map[i][j] = EMapAttribute::E_PlusBombItem; break;
						default: map[i][j] = EMapAttribute::E_Empty; break;
						}
						itemmap[i][j] = EMapAttribute::E_Empty;
					}
					else map[i][j] = EMapAttribute::E_Empty;
				}
			}

			for (int m = 0; m < pPlayer2->BombAmount; m++)		//	플레이어 2
			{
				if (pBomb2[m] == NULL && map[i][j] == EMapAttribute::E_Bomb)		//	폭탄 사라진 위치
					map[i][j] = EMapAttribute::E_Empty;
				if (pFire2[m] == NULL && map[i][j] == EMapAttribute::E_Fire)		//	불길 사라진 위치
				{
					if (itemmap[i][j] == EMapAttribute::E_Destroyable)
					{
						switch (prob2)
						{
						case 0: map[i][j] = EMapAttribute::E_PowerItem; break;
						case 1: map[i][j] = EMapAttribute::E_PlusBombItem; break;
						default: map[i][j] = EMapAttribute::E_Empty; break;
						}
						itemmap[i][j] = EMapAttribute::E_Empty;						//	아이템이 없어지는 문제가 되는 코드
					}
					else map[i][j] = EMapAttribute::E_Empty;
				}
			}

		}
	}

	map[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Player;		//	플레이어 1 위치

	map[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Player2;		//	플레이어 2 위치

	for (int i = 0; i < pPlayer->BombAmount; i++)		//	플레이어 1
	{
		if (pBomb[i] != NULL)				//		폭탄 위치
		{
			map[pBomb[i]->getY()][pBomb[i]->getX()] = EMapAttribute::E_Bomb;
		}
	}

	for (int i = 0; i < pPlayer2->BombAmount; i++)		//	플레이어 2
	{
		if (pBomb2[i] != NULL)				//		폭탄 위치
		{
			map[pBomb2[i]->getY()][pBomb2[i]->getX()] = EMapAttribute::E_Bomb;
		}
	}


	for (int k = 0; k<pPlayer->BombAmount; k++)				//		플레이어 1
	{
		if (pFire[k] != NULL)								//		폭탄 불길 위치
		{
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 오른쪽 좌표
			{
				if (map[pFire[k]->getY()][pFire[k]->getX() + i] == EMapAttribute::E_Wall)
				{
					break;
				}

				else if (map[pFire[k]->getY()][pFire[k]->getX() + i] == EMapAttribute::E_Destroyable)
				{
					map[pFire[k]->getY()][pFire[k]->getX() + i] = EMapAttribute::E_Fire;
					break;
				}
				else
					map[pFire[k]->getY()][pFire[k]->getX() + i] = EMapAttribute::E_Fire;

			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire[k]->getY()][pFire[k]->getX() - i] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY()][pFire[k]->getX() - i] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 위쪽 좌표
			{
				if (map[pFire[k]->getY() - i][pFire[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY() - i][pFire[k]->getX()] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire[k]->getY() + i][pFire[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY() + i][pFire[k]->getX()] = EMapAttribute::E_Fire;
			}
		}
	}




	for (int k = 0; k<pPlayer2->BombAmount; k++)				//		플레이어 2
	{
		if (pFire2[k] != NULL)								//		폭탄 불길 위치
		{
			for (int i = 0; i <= pPlayer2->Power; i++)		//	불길 오른쪽 좌표
			{
				if (map[pFire2[k]->getY()][pFire2[k]->getX() + i] == EMapAttribute::E_Wall)
				{
					break;
				}

				else if (map[pFire2[k]->getY()][pFire2[k]->getX() + i] == EMapAttribute::E_Destroyable)
				{
					map[pFire2[k]->getY()][pFire2[k]->getX() + i] = EMapAttribute::E_Fire;
					break;
				}
				else
					map[pFire2[k]->getY()][pFire2[k]->getX() + i] = EMapAttribute::E_Fire;

			}
			for (int i = 0; i <= pPlayer2->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire2[k]->getY()][pFire2[k]->getX() - i] == EMapAttribute::E_Wall)
					break;
				else map[pFire2[k]->getY()][pFire2[k]->getX() - i] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer2->Power; i++)		//	불길 위쪽 좌표
			{
				if (map[pFire2[k]->getY() - i][pFire2[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire2[k]->getY() - i][pFire2[k]->getX()] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer2->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire2[k]->getY() + i][pFire2[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire2[k]->getY() + i][pFire2[k]->getX()] = EMapAttribute::E_Fire;
			}
		}
	}

	if (s_map == 3)											//	우물 그림모양 유지(초기화)
	{
		map[3][3] = EMapAttribute::E_Well;
		map[9][3] = EMapAttribute::E_Well;
		map[3][11] = EMapAttribute::E_Well;
		map[9][11] = EMapAttribute::E_Well;
	}

	for (int i = 0; i < HEIGHT; i++)						//	맵 출력
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (cpymap[i][j] != map[i][j])
			{
				gotoxy(j + 8, i + 8);
				switch (map[i][j])
				{
				case EMapAttribute::E_Empty: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000); cout << "  "; break;
				case EMapAttribute::E_Player: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000C);
					if (s_char == 1)cout << "⊙";
					else if (s_char == 2)
					{
						if (pPlayer->status == 0)	cout << "◈";
						else
						{
							if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
							else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B);
							else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
							cout << "□";
						}
					}
					else if (s_char == 3)cout << "＠";
					else if (s_char == 4)cout << "◐";
					else if (s_char == 5)
					{
						if (pPlayer->status2 == 0)	cout << "★";
						else if (pPlayer->status2 == 1)	cout << "☆";
					}
					break;
				case EMapAttribute::E_Wall: if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004);
											else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
											else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008);
											cout << "■"; break;
				case EMapAttribute::E_Destroyable: if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
												   else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B);
												   else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
												   cout << "□"; break;
				case EMapAttribute::E_Bomb: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008); cout << "♣"; break;
				case EMapAttribute::E_Fire: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0006); cout << "▒"; break;
				case EMapAttribute::E_Enemy: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A); cout << "○"; break;
				case EMapAttribute::E_PowerItem: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004); cout << "ⓟ"; break;
				case EMapAttribute::E_PlusBombItem: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0005); cout << "ⓑ"; break;
				case EMapAttribute::E_Well: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009); cout << "●"; break;
				case EMapAttribute::E_Player2: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009);
					if (s_char2 == 1)cout << "⊙";
					else if (s_char2 == 2)
					{
						if (pPlayer2->status == 0)	cout << "◈";
						else
						{
							if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
							else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B);
							else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
							cout << "□";
						}
					}
					else if (s_char2 == 3)cout << "＠";
					else if (s_char2 == 4)cout << "◐";
					else if (s_char2 == 5)
					{
						if (pPlayer2->status2 == 0)	cout << "★";
						else if (pPlayer2->status2 == 1)	cout << "☆";
					}
					break; 
				}
			}
		}
		cout << endl;
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			cpymap[i][j] = map[i][j];
		}
	}

	// Status 창

	gotoxy(10, 21);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
	cout << "P1 " << "ⓑ" << pPlayer->BombAmount << " ⓟ" << pPlayer->Power;
	cout << "   P2 " << "ⓑ" << pPlayer2->BombAmount << " ⓟ" << pPlayer2->Power;
}

void drawMap(Player* pPlayer, Enemy* pEnemy1,
	int map[HEIGHT][WIDTH], int cpymap[HEIGHT][WIDTH], int itemmap[HEIGHT][WIDTH],
	Bomb* pBomb[MAX_BOMB], Fire* pFire[MAX_BOMB],
	Bomb* pBomb2[MAX_BOMB], Fire* pFire2[MAX_BOMB], int s_map, int s_char)
{
	int prob;
	if (pEnemy1 != NULL)
	{
		if (pPlayer->status2==0 && map[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_Fire
			|| map[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_Enemy)	//	플레이어가 죽는 조건 (불에 닿거나 적을 만났을때)
		{
			gotoxy(30, 26);
			cout << "플레이어가 죽었습니다!" << endl;
			// delete pPlayer, pEnemy1, pBomb, pBomb2, pFire, pFire2;
			system("pause");
			return;
		}

		if (map[pEnemy1->getY()][pEnemy1->getX()] == EMapAttribute::E_Fire)		//	적 1이 죽는 조건
		{
			gotoxy(25, 10);
			cout << "적 1이 죽었습니다!" << endl;
			system("pause");
			return;

			//delete pEnemy1;
			//pEnemy1 = NULL;
		}
	}


	if (pEnemy1 != NULL)
		map[pEnemy1->getY()][pEnemy1->getX()] = EMapAttribute::E_Enemy;		//	적 1 위치

	prob = rand() % 3;			//		아이템 뜰 확률 SEED

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			for (int m = 0; m < pPlayer->BombAmount; m++)
			{
				if (pBomb[m] == NULL && map[i][j] == EMapAttribute::E_Bomb)		//	폭탄 사라진 위치
					map[i][j] = EMapAttribute::E_Empty;
				if (pFire[m] == NULL && map[i][j] == EMapAttribute::E_Fire)		//	불길 사라진 위치
				{
					if (itemmap[i][j] == EMapAttribute::E_Destroyable)
					{
						switch (prob)
						{
						case 0: map[i][j] = EMapAttribute::E_PowerItem; break;
						case 1: map[i][j] = EMapAttribute::E_PlusBombItem; break;
						default: map[i][j] = EMapAttribute::E_Empty; break;
						}
						itemmap[i][j] = EMapAttribute::E_Empty;
					}
					else map[i][j] = EMapAttribute::E_Empty;
				}

			}

			if (pEnemy1 == NULL && map[i][j] == EMapAttribute::E_Enemy)		//	적1 사라진 위치
			{
				map[i][j] = EMapAttribute::E_Empty;
			}
			if (pPlayer == NULL && map[i][j] == EMapAttribute::E_Player)	//	플레이어 사라진 위치(...?)
				map[i][j] = EMapAttribute::E_Empty;
		}
	}


	//gotoxy(20, 20);
	//cout << pItem[0]->getX() << ", " << pItem[0]->getY() << endl;

	map[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Player;		//	플레이어 위치

	for (int i = 0; i < pPlayer->BombAmount; i++)
	{
		if (pBomb[i] != NULL)				//		폭탄 위치
		{
			map[pBomb[i]->getY()][pBomb[i]->getX()] = EMapAttribute::E_Bomb;
		}
	}

	for (int k = 0; k<pPlayer->BombAmount; k++)			//		폭탄 불길 위치
	{
		if (pFire[k] != NULL)
		{
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 오른쪽 좌표
			{
				if (map[pFire[k]->getY()][pFire[k]->getX() + i] == EMapAttribute::E_Wall)
				{
					break;
				}

				else if (map[pFire[k]->getY()][pFire[k]->getX() + i] == EMapAttribute::E_Destroyable)
				{
					map[pFire[k]->getY()][pFire[k]->getX() + i] = EMapAttribute::E_Fire;
					break;
				}
				else
					map[pFire[k]->getY()][pFire[k]->getX() + i] = EMapAttribute::E_Fire;

			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire[k]->getY()][pFire[k]->getX() - i] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY()][pFire[k]->getX() - i] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 위쪽 좌표
			{
				if (map[pFire[k]->getY() - i][pFire[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY() - i][pFire[k]->getX()] = EMapAttribute::E_Fire;
			}
			for (int i = 0; i <= pPlayer->Power; i++)		//	불길 아래쪽 좌표
			{
				if (map[pFire[k]->getY() + i][pFire[k]->getX()] == EMapAttribute::E_Wall)
					break;
				else map[pFire[k]->getY() + i][pFire[k]->getX()] = EMapAttribute::E_Fire;
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (cpymap[i][j] != map[i][j])
			{
				gotoxy(j+8, i+8);
				switch (map[i][j])
				{
				case EMapAttribute::E_Empty: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000); cout << "  "; break;
				case EMapAttribute::E_Player: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000C);
					if (s_char == 1)cout << "⊙";
					else if (s_char == 2)
					{
						if (pPlayer->status == 0)	cout << "◈";
						else
						{
							if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
							else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B);
							else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
							cout << "□";
						}
					}
					else if (s_char == 3)cout << "＠";
					else if (s_char == 4)cout << "◐";
					else if (s_char == 5)
					{
						if (pPlayer->status2 == 0)	cout << "★";
						else if (pPlayer->status2 == 1)	cout << "☆";
					}
					break;
				case EMapAttribute::E_Wall: if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004);
											else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
											else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008);
											cout << "■"; break;
				case EMapAttribute::E_Destroyable: if (s_map == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
												   else if (s_map == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B);
												   else if (s_map == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
												   cout << "□"; break;
				case EMapAttribute::E_Bomb: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008); cout << "♣"; break;
				case EMapAttribute::E_Fire: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0006); cout << "▒"; break;
				case EMapAttribute::E_Enemy: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A); cout << "○"; break;
				case EMapAttribute::E_PowerItem: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004); cout << "ⓟ"; break;
				case EMapAttribute::E_PlusBombItem: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0005); cout << "ⓑ"; break;
				case EMapAttribute::E_Well: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009); cout << "●"; break;
				}
			}
		}
		cout << endl;
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			cpymap[i][j] = map[i][j];
		}
	}

	gotoxy(10, 21);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
	cout << "P1 " << "ⓑ" << pPlayer->BombAmount << " ⓟ" << pPlayer->Power;
}


GameMain::GameMain()
	:pPlayer(NULL),
	pPlayer2(NULL),
	pEnemy1(NULL)
{
	pBomb[MAX_BOMB] = { NULL, };
	pFire[MAX_BOMB] = { NULL, };
	pBomb2[MAX_BOMB] = { NULL, };
	pFire2[MAX_BOMB] = { NULL, };
	game_End = false;
}

GameMain::~GameMain()
{
}

void GameMain::Init()
{
	keyCnt = 0;
	game_End = false;
	pPlayer = new Player();
	pEnemy1 = new Enemy();
	while (true)
	{
		srand(time(NULL));
		int a = rand() % 4;
		switch (a)
		{
		case 0:
		{
			pEnemy1->setX_Y(1, 1); break;
		}
		case 1:
		{
			pEnemy1->setX_Y(13, 1); break;
		}
		case 2:
		{
			pEnemy1->setX_Y(1, 11); break;
		}
		case 3:
		{
			pEnemy1->setX_Y(13, 11); break;
		}
		}
		if (pEnemy1->getX() == pPlayer->getX() && pEnemy1->getY() == pPlayer->getY())continue;
		else break;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch (s_map)
			{
			case 1:itemmap[i][j] = map[i][j]; break;
			case 2:itemmap[i][j] = map2[i][j]; break;
			case 3:itemmap[i][j] = map3[i][j]; break;
			}
			playMap[i][j] = itemmap[i][j];
		}
	}
}

void GameMain::Init2()
{
	game_End = false;
	pPlayer = new Player();
	pPlayer2 = new Player();
	pPlayer->setX_Y(1, 1);
	pPlayer2->setX_Y(13, 11);

	//gotoxy(20, 20);
	//cout << "플레이어 1 좌표 : " << pPlayer->getX() << ", " << pPlayer->getY();
	//cout << "플레이어 2 좌표 : " << pPlayer2->getX() << ", " << pPlayer->getY();
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			switch (s_map)
			{
			case 1:itemmap[i][j] = map[i][j]; break;
			case 2:itemmap[i][j] = map2[i][j]; break;
			case 3:itemmap[i][j] = map3[i][j]; break;
			}
			playMap[i][j] = itemmap[i][j];
		}
	}
}

void GameMain::Update()
{
	switch (gameMode)
	{
	case PvC:
		drawMap(pPlayer, pEnemy1, playMap, cpymap, itemmap, pBomb, pFire, pBomb2, pFire2, s_map, s_char);
		Single_Player_Update();
		Enemy_Update();
		Bomb_Update();
		break;
	case PvP:
		drawMap2(pPlayer, pPlayer2, playMap, cpymap, itemmap, game_End, pBomb, pBomb2, pFire, pFire2, s_map, s_char, s_char2);
		Player1_Update();
		Bomb_Update();
		break;
	}

	//if (pPlayer == NULL || pEnemy1 == NULL)game_End = true;
}

void GameMain::Single_Player_Update()
{
	int rand_x;
	int rand_y;

	int infi_time = 0;

	if (_kbhit())
	{
		char input = _getch();

		switch (input)
		{

		case EDirection::LEFT:
			if (s_map == 3)
			{
				if (pPlayer->getX() - 1 == 3 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() - 1 == 11 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() - 1 == 3 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() - 1 == 11 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Wall && playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX(-1);
			}
			break;
		case EDirection::RIGHT:

			if (s_map == 3)
			{
				if (pPlayer->getX() + 1 == 3 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() + 1 == 11 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() + 1 == 3 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() + 1 == 11 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Wall && playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX(1);
			}
			break;
		case EDirection::UP:
			if (s_map == 3)
			{
				if (pPlayer->getX() == 3 && pPlayer->getY() - 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() - 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() == 3 && pPlayer->getY() - 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() - 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Wall && playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setY(-1);
			}
			break;
		case EDirection::DOWN:
			if (s_map == 3)
			{
				if (pPlayer->getX() == 3 && pPlayer->getY() + 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() + 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() == 3 && pPlayer->getY() + 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() + 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}
			if (playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Wall && playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setY(1);
			}
			break;
		case SPACE:
		{
			for (int i = 0; i < pPlayer->BombAmount; i++)
			{
				if (pBomb[i] == NULL)
				{
					pBomb[i] = new Bomb();
					pBomb[i]->setX(pPlayer->getX());
					pBomb[i]->setY(pPlayer->getY());
					break;
				}
			}
			break;
		}
		case 'z':
		{
			if (s_char == 2) // 변신
			{
				if (pPlayer->status == 0) pPlayer->status = 1;
				else pPlayer->status = 0;
			}
			if (s_char == 3) // 순간이동
			{
				while (true)
				{
					rand_x = rand() % WIDTH;
					rand_y = rand() % HEIGHT;
					if (map[rand_y][rand_x] != EMapAttribute::E_Wall&&map[rand_y][rand_x] != EMapAttribute::E_Well&&map[rand_y][rand_x] != EMapAttribute::E_Destroyable&&
						map[rand_y][rand_x] != EMapAttribute::E_Bomb && rand_y != 13)
						break;
				}
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX_Y(rand_x, rand_y);
			}
			if (s_char == 5) // 무적
			{
				if (pPlayer->status2 == 0)pPlayer->status2 = 1;
				else if (pPlayer->status2 == 1)pPlayer->status2 = 0;
			}
			break;
		}
		default:
			break;
		}
	}
	if (s_map == 3)
	{
		if (pPlayer->getX() == 3 && pPlayer->getY() == 3) pPlayer->setX_Y(11, 9);
		if (pPlayer->getX() == 11 && pPlayer->getY() == 3) pPlayer->setX_Y(3, 9);
		if (pPlayer->getX() == 3 && pPlayer->getY() == 9) pPlayer->setX_Y(11, 3);
		if (pPlayer->getX() == 11 && pPlayer->getY() == 9) pPlayer->setX_Y(3, 3);
	}

	if (playMap[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_PowerItem && pPlayer->Power<4)
		pPlayer->Power++;
	if (playMap[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_PlusBombItem && pPlayer->BombAmount <= MAX_BOMB)
		pPlayer->BombAmount++;
}

void GameMain::Player1_Update()
{
	int rand_x;
	int rand_y;

	int rand_x2;
	int rand_y2;

	int infi_time = 0;

	if (_kbhit())
	{
		char input = _getch();

		switch (input)
		{

		case EDirection2::P1_LEFT:
			if (s_map == 3)
			{
				if (pPlayer->getX() - 1 == 3 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() - 1 == 11 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() - 1 == 3 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() - 1 == 11 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Wall && playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY()][pPlayer->getX() - 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX(-1);
			}
			break;
		case EDirection2::P1_RIGHT:
			if (s_map == 3)
			{
				if (pPlayer->getX() + 1 == 3 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() + 1 == 11 && pPlayer->getY() == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() + 1 == 3 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() + 1 == 11 && pPlayer->getY() == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Wall && playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY()][pPlayer->getX() + 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX(1);
			}
			break;
		case EDirection2::P1_UP:
			if (s_map == 3)
			{
				if (pPlayer->getX() == 3 && pPlayer->getY() - 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() - 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() == 3 && pPlayer->getY() - 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() - 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}

			if (playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Wall && playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY() - 1][pPlayer->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setY(-1);
			}
			break;
		case EDirection2::P1_DOWN:
			if (s_map == 3)
			{
				if (pPlayer->getX() == 3 && pPlayer->getY() + 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 9);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() + 1 == 3)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 9);
				}
				if (pPlayer->getX() == 3 && pPlayer->getY() + 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(11, 3);
				}
				if (pPlayer->getX() == 11 && pPlayer->getY() + 1 == 9)
				{
					playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
					pPlayer->setX_Y(3, 3);
				}
			}
			if (playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Wall && playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer->getY() + 1][pPlayer->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setY(1);
			}
			break;
		case EDirection2::P1_ATTACK:
		{
			for (int i = 0; i < pPlayer->BombAmount; i++)
			{
				if (pBomb[i] == NULL)
				{
					pBomb[i] = new Bomb();
					pBomb[i]->setX(pPlayer->getX());
					pBomb[i]->setY(pPlayer->getY());
					break;
				}
			}
			break;
		}
		case EDirection2::P1_SKILL:
		{
			if (s_char == 2) // 변신
			{
				if (pPlayer->status == 0) pPlayer->status = 1;
				else pPlayer->status = 0;
			}
			if (s_char == 3) // 순간이동
			{
				while (true)
				{
					rand_x = rand() % WIDTH;
					rand_y = rand() % HEIGHT;
					if (map[rand_y][rand_x] != EMapAttribute::E_Wall&&map[rand_y][rand_x] != EMapAttribute::E_Well&&map[rand_y][rand_x] != EMapAttribute::E_Destroyable&&
						map[rand_y][rand_x] != EMapAttribute::E_Bomb && rand_y != 13)
						break;
				}
				playMap[pPlayer->getY()][pPlayer->getX()] = EMapAttribute::E_Empty;
				pPlayer->setX_Y(rand_x, rand_y);
			}
			if (s_char == 5) // 무적
			{
				if (pPlayer->status2 == 0)pPlayer->status2 = 1;
				else if (pPlayer->status2 == 1)pPlayer->status2 = 0;
			}
			break;
		}
		case EDirection2::P2_LEFT:
			if (playMap[pPlayer2->getY()][pPlayer2->getX() - 1] != EMapAttribute::E_Wall && playMap[pPlayer2->getY()][pPlayer2->getX() - 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer2->getY()][pPlayer2->getX() - 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Empty;
				pPlayer2->setX(-1);
			}
			break;
		case EDirection2::P2_RIGHT:
			if (playMap[pPlayer2->getY()][pPlayer2->getX() + 1] != EMapAttribute::E_Wall && playMap[pPlayer2->getY()][pPlayer2->getX() + 1] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer2->getY()][pPlayer2->getX() + 1] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Empty;
				pPlayer2->setX(1);
			}
			break;
		case EDirection2::P2_UP:
			if (playMap[pPlayer2->getY() - 1][pPlayer2->getX()] != EMapAttribute::E_Wall && playMap[pPlayer2->getY() - 1][pPlayer2->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer2->getY() - 1][pPlayer2->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Empty;
				pPlayer2->setY(-1);
			}
			break;
		case EDirection2::P2_DOWN:
			if (playMap[pPlayer2->getY() + 1][pPlayer2->getX()] != EMapAttribute::E_Wall && playMap[pPlayer2->getY() + 1][pPlayer2->getX()] != EMapAttribute::E_Destroyable
				&& playMap[pPlayer2->getY() + 1][pPlayer2->getX()] != EMapAttribute::E_Bomb)
			{
				playMap[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Empty;
				pPlayer2->setY(1);
			}
			break;
		case EDirection2::P2_ATTACK:
		{
			for (int i = 0; i < pPlayer2->BombAmount; i++)
			{
				if (pBomb2[i] == NULL)
				{
					pBomb2[i] = new Bomb();
					pBomb2[i]->setX(pPlayer2->getX());
					pBomb2[i]->setY(pPlayer2->getY());
					break;
				}
			}
			gotoxy(20, 20);
			break;
		}
		case EDirection2::P2_SKILL:
		{
			if (s_char2 == 2) // 변신
			{
				if (pPlayer2->status == 0) pPlayer2->status = 1;
				else pPlayer2->status = 0;
			}
			if (s_char2 == 3) // 순간이동
			{
				while (true)
				{
					rand_x2 = rand() % WIDTH;
					rand_y2 = rand() % HEIGHT;
					if (map[rand_y2][rand_x2] != EMapAttribute::E_Wall&&map[rand_y2][rand_x2] != EMapAttribute::E_Well&&map[rand_y2][rand_x2] != EMapAttribute::E_Destroyable&&
						map[rand_y2][rand_x2] != EMapAttribute::E_Bomb && rand_y2 != 13)
						break;
				}
				playMap[pPlayer2->getY()][pPlayer2->getX()] = EMapAttribute::E_Empty;
				pPlayer2->setX_Y(rand_x2, rand_y2);
			}
			if (s_char2 == 5) // 무적
			{
				if (pPlayer2->status2 == 0)pPlayer2->status2 = 1;
				else if (pPlayer2->status2 == 1)pPlayer2->status2 = 0;
			}
			break;
		}
		default:
			break;
		}
		
	}
	//if (clock() - infi_time > 2000) pPlayer->status2 = 0;

	if (playMap[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_PowerItem && pPlayer->Power<4)
		pPlayer->Power++;
	if (playMap[pPlayer->getY()][pPlayer->getX()] == EMapAttribute::E_PlusBombItem && pPlayer->BombAmount <= MAX_BOMB)
		pPlayer->BombAmount++;



}

void GameMain::Enemy_Update()
{
	if (keyCnt>0) {
		keyCnt--;
		return;
	}

	keyCnt = 4;

	srand(time(NULL));
	int key = rand() % 4;
	switch (key)
	{
	case 0:
		if (playMap[pEnemy1->getY()][pEnemy1->getX() - 1] != EMapAttribute::E_Wall && playMap[pEnemy1->getY()][pEnemy1->getX() - 1] != EMapAttribute::E_Destroyable)
		{
			playMap[pEnemy1->getY()][pEnemy1->getX()] = EMapAttribute::E_Empty;
			pEnemy1->setX(-1);
		}
		break;
	case 1:
		if (playMap[pEnemy1->getY()][pEnemy1->getX() + 1] != EMapAttribute::E_Wall && playMap[pEnemy1->getY()][pEnemy1->getX() + 1] != EMapAttribute::E_Destroyable)
		{
			playMap[pEnemy1->getY()][pEnemy1->getX()] = EMapAttribute::E_Empty;
			pEnemy1->setX(1);
		}
		break;
	case 2:
		if (playMap[pEnemy1->getY() - 1][pEnemy1->getX()] != EMapAttribute::E_Wall && playMap[pEnemy1->getY() - 1][pEnemy1->getX()] != EMapAttribute::E_Destroyable)
		{
			playMap[pEnemy1->getY()][pEnemy1->getX()] = EMapAttribute::E_Empty;
			pEnemy1->setY(-1);
		}
		break;
	case 3:
		if (playMap[pEnemy1->getY() + 1][pEnemy1->getX()] != EMapAttribute::E_Wall && playMap[pEnemy1->getY() + 1][pEnemy1->getX()] != EMapAttribute::E_Destroyable)
		{
			playMap[pEnemy1->getY()][pEnemy1->getX()] = EMapAttribute::E_Empty;
			pEnemy1->setY(1);
		}
		break;
	}
}

void GameMain::Bomb_Update()
{
	int posX;
	int posY;

	int posX2;
	int posY2;

	for (int i = 0; i < pPlayer->BombAmount; i++)
	{
		if (pBomb[i] != NULL)
		{
			//cout << pBomb[0]->getX() << ", " << pBomb[0]->getY() << endl;
			posX = pBomb[i]->getX();
			posY = pBomb[i]->getY();
			if (s_char == 4)
			{
				if (clock() - pBomb[i]->s_time > 2000 || playMap[pBomb[i]->getY()][pBomb[i]->getX()] == EMapAttribute::E_Fire)
				{
					delete pBomb[i];
					pBomb[i] = NULL;
					//cout << "폭탄 터짐" << endl;
					pFire[i] = new Fire();
					pFire[i]->setXY(posX, posY);
				}
			}
			else
				if (clock() - pBomb[i]->s_time > 1000 || playMap[pBomb[i]->getY()][pBomb[i]->getX()] == EMapAttribute::E_Fire)
				{
					delete pBomb[i];
					pBomb[i] = NULL;
					//cout << "폭탄 터짐" << endl;
					pFire[i] = new Fire();
					pFire[i]->setXY(posX, posY);
				}
		}

		if (pFire[i] != NULL)
		{
			if (clock() - pFire[i]->s_time > 1000)
			{
				delete pFire[i];
				pFire[i] = NULL;
			}
		}
	}

	if (pPlayer2 != NULL)
	{

		for (int i = 0; i < pPlayer2->BombAmount; i++)
		{
			if (pBomb2[i] != NULL)
			{
				//cout << pBomb[0]->getX() << ", " << pBomb[0]->getY() << endl;
				posX2 = pBomb2[i]->getX();
				posY2 = pBomb2[i]->getY();
				if (s_char2 == 4)
				{
					if (clock() - pBomb2[i]->s_time > 2000 || playMap[pBomb2[i]->getY()][pBomb2[i]->getX()] == EMapAttribute::E_Fire)
					{
						delete pBomb2[i];
						pBomb2[i] = NULL;
						//cout << "폭탄 터짐" << endl;
						pFire2[i] = new Fire();
						pFire2[i]->setXY(posX, posY);
					}
				}
				else
					if (clock() - pBomb2[i]->s_time > 1000 || playMap[pBomb2[i]->getY()][pBomb2[i]->getX()] == EMapAttribute::E_Fire)
					{
						delete pBomb2[i];
						pBomb2[i] = NULL;
						//cout << "폭탄 터짐" << endl;
						pFire2[i] = new Fire();
						pFire2[i]->setXY(posX2, posY2);
					}
			}

			if (pFire2[i] != NULL)
			{
				if (clock() - pFire2[i]->s_time > 1000)
				{
					delete pFire2[i];
					pFire2[i] = NULL;
				}
			}
		}
	}
}

void GameMain::titleMenu(void) { // 게임타이틀
	int i, j;

	int text_battle_x = 3;
	int text_battle_y = 2;

	int text_bomber_x = 16;
	int text_bomber_y = 9;

	int text_menu_x = 13;
	int text_menu_y = 17;

	int menuP = PvC;
	int totalMenu = 2;
	int menuCnt = 0;

	int text_battle_order[9] = { 0,1,2,3,4,2,1,4,2 };
	int text_battle[5][6][25] = {
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,

		1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,
		0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
		0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,
		0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,
		0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,0,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,
		1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,

		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,
		0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,
		0,0,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,
		0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,
		0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1
	};

	int text_bomber[5][25] = {
		1,1,0,0,0,1,0,0,1,0,0,0,1,0,1,1,0,0,1,1,1,0,1,1,0,
		1,0,1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,1,0,1,
		1,1,0,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,1,0,1,1,0,
		1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,
		1,1,0,0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1
	};

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0003);
	for (int k = 1; k<9; k++) {
		for (i = 0; i<6; i++) {
			for (j = 0; j<25; j++) {
				if (text_battle[text_battle_order[k]][i][j] != text_battle[text_battle_order[k - 1]][i][j]) {
					gotoxy(text_battle_x + j, text_battle_y + i);
					if (text_battle[text_battle_order[k]][i][j] == 1) printf("■");
					else if (text_battle[text_battle_order[k]][i][j] == 0) printf("  ");
				}
			}
		}
		Sleep(100);
	}
	Sleep(200);



	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
	gotoxy(text_menu_x, text_menu_y); printf("    1. PLAYER VS COMPUTER");
	gotoxy(text_menu_x, text_menu_y + 2); printf("    2. PLAYER 1 VS PLAYER 2");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
	gotoxy(30, 7); printf("produced by 앙쁘띠성환 ★");


	//keyInstructions(menuP);
	//gotoxy(21, 23); printf("  http://blog.naver.com/azure0777");

	for (int cnt = 0;; cnt++) {
		if (menuCnt>0) menuCnt--;
		Sleep(10);

		if ((cnt + 0) % 60 == 0) {
			switch (menuP) {
			case PvC:
				gotoxy(text_menu_x, text_menu_y);
				break;
			case PvP:
				gotoxy(text_menu_x, text_menu_y + 2);
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
			printf(" →");
		}
		if ((cnt + 30) % 60 == 0) {
			switch (menuP) {
			case PvC:
				gotoxy(text_menu_x, text_menu_y);
				break;
			case PvP:
				gotoxy(text_menu_x, text_menu_y + 2);
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
			printf("→ ");
		}
		if (cnt % 75 == 0) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cnt % 7 + 2);
			for (i = 0; i<5; i++) {
				for (j = 0; j<25; j++) {
					gotoxy(text_bomber_x + j, text_bomber_y + i);
					if (text_bomber[i][j] == 1) printf("▣");
				}
			}
		}
		if (GetAsyncKeyState(VK_UP) && menuCnt == 0) {
			menuCnt = 20;
			gotoxy(text_menu_x, text_menu_y); printf("   ");
			gotoxy(text_menu_x, text_menu_y + 2); printf("   ");
			menuP = (menuP + 1) % totalMenu;
		}
		if (GetAsyncKeyState(VK_DOWN) && menuCnt == 0) {
			menuCnt = 20;
			gotoxy(text_menu_x, text_menu_y); printf("   ");
			gotoxy(text_menu_x, text_menu_y + 2); printf("   ");
			menuP = (menuP + totalMenu - 1) % totalMenu;
		}
		if (GetAsyncKeyState(VK_RETURN)) {
			gameMode = menuP;
			break;
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);
}

void GameMain::selectMap()
{
	int text_map_x = 5;
	int text_map_y = 2;

	int text_select_x = 20;
	int text_select_y = 2;

	int map1_x = 1;
	int map1_y = 11;

	int map2_x = 18;
	int map2_y = 11;

	int map3_x = 35;
	int map3_y = 11;

	int text_map[5][13] =
	{
		1,0,0,0,1,0,0,1,0,0,1,1,0,
		1,1,0,1,1,0,1,0,1,0,1,0,1,
		1,0,1,0,1,0,1,1,1,0,1,1,0,
		1,0,1,0,1,0,1,0,1,0,1,0,0,
		1,0,1,0,1,0,1,0,1,0,1,0,0
	};

	int text_select[5][23] =
	{
		1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,1,1,0,1,1,1,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,
		1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,0,1,0,
		0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,
		1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,0,0,1,0

	};

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<13; j++)
		{
			gotoxy(text_map_x + j, text_map_y + i);
			if (text_map[i][j] == 1) cout << "▣";
		}
	}

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<23; j++)
		{
			gotoxy(text_select_x + j, text_select_y + i);
			if (text_select[i][j] == 1) cout << "▣";
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			gotoxy(map1_x + j, map1_y + i);
			switch (map[i][j])
			{
			case EMapAttribute::E_Empty: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000); cout << "  "; break;
			case EMapAttribute::E_Wall: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004); cout << "■"; break;
			case EMapAttribute::E_Destroyable: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E); cout << "□"; break;
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			gotoxy(map2_x + j, map2_y + i);
			switch (map2[i][j])
			{
			case EMapAttribute::E_Empty: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000); cout << "  "; break;
			case EMapAttribute::E_Wall: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F); cout << "■"; break;
			case EMapAttribute::E_Destroyable: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000B); cout << "□"; break;
			}
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			gotoxy(map3_x + j, map3_y + i);
			switch (map3[i][j])
			{
			case EMapAttribute::E_Empty: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0000); cout << "  "; break;
			case EMapAttribute::E_Wall: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008); cout << "■"; break;
			case EMapAttribute::E_Destroyable: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A); cout << "□"; break;
			case EMapAttribute::E_Well: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009); cout << "●"; break;
			}
		}
	}

	gotoxy(4, 27);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000F);
	cout << "1. Map : Village" << endl;
	gotoxy(22, 27);
	cout << "2. Map : Winter" << endl;
	gotoxy(38, 27);
	cout << "3. Map : Jungle" << endl;

	gotoxy(17, 9);
	cout << "Enter Map Num : ";
	while (true)
	{
		cin >> s_map;
		if (s_map == 1 || s_map == 2 || s_map == 3)break;
	}
}

void GameMain::selectCharacter()
{
	int text_bot_x = 5;
	int text_bot_y = 2;

	int text_select_x = 20;
	int text_select_y = 2;

	int box1_x = 3;
	int box1_y = 11;

	int box2_x = 3;
	int box2_y = 18;

	int box3_x = 3;
	int box3_y = 25;

	int box4_x = 27;
	int box4_y = 11;

	int box5_x = 27;
	int box5_y = 18;

	int text_bot[5][11] =
	{
		1,1,0,0,0,1,0,0,1,1,1,
		1,0,1,0,1,1,1,0,0,1,0,
		1,1,0,0,1,0,1,0,0,1,0,
		1,0,1,0,1,1,1,0,0,1,0,
		1,1,1,0,0,1,0,0,0,1,0
	};

	int text_select[5][23] =
	{
		1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,0,1,1,0,1,1,1,
		1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,
		1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,0,1,0,
		0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,
		1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,0,0,1,0

	};

	int box[5][5] =
	{
		1,1,1,1,1,
		1,0,0,0,1,
		1,0,0,0,1,
		1,0,0,0,1,
		1,1,1,1,1
	};

	int explain[5][16]
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004);

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<11; j++)
		{
			gotoxy(text_bot_x + j, text_bot_y + i);
			if (text_bot[i][j] == 1) cout << "▣";
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0006);

	for (int i = 0; i<5; i++)
	{
		for (int j = 0; j<23; j++)
		{
			gotoxy(text_select_x + j, text_select_y + i);
			if (text_select[i][j] == 1) cout << "▣";
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoxy(box1_x + j, box1_y + i);
			if (box[i][j] == 1) cout << "▣";
			gotoxy(box2_x + j, box2_y + i);
			if (box[i][j] == 1) cout << "▣";
			gotoxy(box3_x + j, box3_y + i);
			if (box[i][j] == 1) cout << "▣";
			gotoxy(box4_x + j, box4_y + i);
			if (box[i][j] == 1) cout << "▣";
			gotoxy(box5_x + j, box5_y + i);
			if (box[i][j] == 1) cout << "▣";
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			gotoxy(box1_x + j + 6, box1_y + i);
			if (explain[i][j] == 1) cout << "▣";
			gotoxy(box2_x + j + 6, box2_y + i);
			if (explain[i][j] == 1) cout << "▣";
			gotoxy(box3_x + j + 6, box3_y + i);
			if (explain[i][j] == 1) cout << "▣";
			gotoxy(box4_x + j + 6, box4_y + i);
			if (explain[i][j] == 1) cout << "▣";
			gotoxy(box5_x + j + 6, box5_y + i);
			if (explain[i][j] == 1) cout << "▣";
		}
	}


	gotoxy(box1_x + 1, box1_y + 1);
	cout << "1>";
	gotoxy(box2_x + 1, box2_y + 1);
	cout << "2>";
	gotoxy(box3_x + 1, box3_y + 1);
	cout << "3>";
	gotoxy(box4_x + 1, box4_y + 1);
	cout << "4>";
	gotoxy(box5_x + 1, box5_y + 1);
	cout << "5>";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000C);
	gotoxy(box1_x + 2, box1_y + 2);
	cout << "⊙";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000A);
	gotoxy(box2_x + 2, box2_y + 2);
	cout << "◈";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009);
	gotoxy(box3_x + 2, box3_y + 2);
	cout << "＠";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0005);
	gotoxy(box4_x + 2, box4_y + 2);
	cout << "◐";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000E);
	gotoxy(box5_x + 2, box5_y + 2);
	cout << "☆";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0008);

	gotoxy(box1_x + 8, box1_y + 2);
	cout << "Bomb[2] Pow[1] [Normal]" << endl;
	gotoxy(box2_x + 8, box2_y + 2);
	cout << "Bomb[1] Pow[1] [Change]" << endl;
	gotoxy(box3_x + 8, box3_y + 2);
	cout << "Bomb[1] Pow[1] [Teleport]" << endl;
	gotoxy(box4_x + 8, box4_y + 2);
	cout << "Bomb[1] Pow[2] [Slower]" << endl;
	gotoxy(box5_x + 8, box5_y + 2);
	cout << "Bomb[1] Pow[1] [Escaper]" << endl;

	gotoxy(17, 9);
	cout << "Enter Player Character Num : ";
	cin >> s_char;
	if (gameMode == PvP)
	{
		gotoxy(17, 10);
		cout << "Enter Player2 Character Num : ";
		cin >> s_char2;
	}
}