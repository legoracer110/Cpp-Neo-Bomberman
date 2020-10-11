// SecondProducing.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "GameMain.h"

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c) { //Ŀ�� �Ⱥ��̰� �ϴ� �Լ� 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void Instruction_PvC()
{
	
	cout << "\t<<Player Control Instruction>>" << endl << endl << endl;
	cout << "\t\t\t ��" << endl;
	cout << "\t\tMove : ��  ��" << endl;
	cout << "\t\t\t ��" << endl << endl;

	cout << "\t\tBomb : SPACE" << endl << endl;
	cout << "\t\tSkill : Z" << endl << endl << endl;

	system("pause");
	system("cls");
}

void Instruction_PvP()
{
	cout << "\t<<Player Control Instruction>>" << endl << endl << endl;
	cout << "  [1P]\t\t\t  [2P]" << endl;
	cout << "\t\t W\t\t\t ��" << endl;
	cout << "\tMove : A   D\t\tMove : ��  ��" << endl;
	cout << "\t\t S\t\t\t ��" << endl << endl;

	cout << "\tBomb : G\t\tBomb : J" << endl << endl;
	cout << "\tSkill : H\t\tSkill : K" << endl << endl << endl;

	system("pause");
	system("cls");
}

int main()
{
	srand(time(NULL));
	
	setcursortype(NOCURSOR);
	
	GameMain gm;
	
	gm.titleMenu();
	
	system("cls");
	
	gm.selectMap();
	
	system("cls");
	
	gm.selectCharacter();
	
	system("cls");
	
	switch (gm.gameMode)
	{
	case PvC: gm.Init();
			Instruction_PvC();
		break;
	case PvP: gm.Init2();
			Instruction_PvP();
		break;
	}
	
	while (true)
	{
		gm.Update();
	
		//Sleep(32);
	
		if (gm.game_End == true)break;
	}
	
	cout << "���ӿ���" << endl;


	system("pause");
	return 0;
}

