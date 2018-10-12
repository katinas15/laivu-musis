#include "stdafx.h"
//#include "pch.h"
#include <cwchar>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>
#include <type_traits>
#include <windows.h>

using namespace std;

const char LANG_TUSC = 0xDB;
const char LANG_LAIV = 'O';
const char LANG_SAUTA = ' ';
const char LANG_PTKT = 'X';



class Claukas
{
private:
	char zaidimoLaukas[10][10];
	char fl[10][10];
public:
	string message = " ";
	char get_status(int x, int y)
	{
		return fl[x][y];
	};
	void show_field()
	{	
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		cout << "Prieso laukas\n";
		cout << "  0123456789\n";

		for (int y = 0; y < 10; y++) {
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				
				if (zaidimoLaukas[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196);
				cout << zaidimoLaukas[x][y];
				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
			}
			cout << endl;
		}

	};
	void end_game()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		cout << "Prieso laukas\n";
		cout << "  0123456789\n";

		for (int y = 0; y < 10; y++) {
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				if(zaidimoLaukas[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196);
				if (zaidimoLaukas[x][y] == LANG_TUSC) {
					if(fl[x][y] == LANG_LAIV) SetConsoleTextAttribute(hConsole, 46);

					cout << fl[x][y];
					}
				else cout << zaidimoLaukas[x][y];

				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
			}
			cout << endl;
		}

	};
	void clear_field()
	{
		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++) {
				zaidimoLaukas[x][y] = LANG_TUSC;
				fl[x][y] = LANG_TUSC;
			}
	};
	bool put_ship(int x, int y, int ilg, bool yra_vert)
	{
		bool pavyko = true;
		if (x < 0 || x>9 || y < 0 || y>9 || ilg < 1 || ilg>4 || (yra_vert && (ilg + y) > 9) || (!yra_vert && (ilg + x) > 9)) {
			return false;
		}
		else
			for (int i = 0; i < ilg && pavyko; i++)
			{
				if (yra_vert) // Vertikalus
				{
					if (fl[x][y + i] != LANG_TUSC || // center
						fl[x][int(abs(y + i - 1))] != LANG_TUSC || // up
						fl[x + 1 - (x + 1) / 10][int(abs(y + i - 1))] != LANG_TUSC || // up-right
						fl[x + 1 - (x + 1) / 10][y + i] != LANG_TUSC || // right
						fl[x + 1 - (x + 1) / 10][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down-right
						fl[x][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down
						fl[int(abs(x - 1))][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down-left
						fl[int(abs(x - 1))][y + i] != LANG_TUSC || // left
						fl[int(abs(x - 1))][int(abs(y + i - 1))] != LANG_TUSC) // up-left
						return false;
				}
				else // Horizontalus
				{
					if (fl[x + i][y] != LANG_TUSC || // center
						fl[x + i][int(abs(y - 1))] != LANG_TUSC || // up
						fl[x + i + 1 - (x + i + 1) / 10][int(abs(y - 1))] != LANG_TUSC || // up-right
						fl[x + i + 1 - (x + i + 1) / 10][y] != LANG_TUSC || // right
						fl[x + i + 1 - (x + i + 1) / 10][y + 1 - (y + 1) / 10] != LANG_TUSC || // down-right
						fl[x + i][y + 1 - (y + 1) / 10] != LANG_TUSC || // down
						fl[int(abs(x + i - 1))][y + 1 - (y + 1) / 10] != LANG_TUSC || // down-left
						fl[int(abs(x + i - 1))][y] != LANG_TUSC || // left
						fl[int(abs(x + i - 1))][int(abs(y - 1))] != LANG_TUSC) // up-left
						return false;
				}
			}

		if (pavyko) // "Dedam" laivą
		{
			for (int i = 0; i < ilg; i++) {

				if (yra_vert)
					fl[x][y + i] = LANG_LAIV;
				else
					fl[x + i][y] = LANG_LAIV;


			}
			//cout << "Ilgis:" << ilg << " x,y:" << x << ',' << y << " Vert:" << yra_vert << endl;
		}
		return pavyko;
	}
	void place_ships()
	{
		int x, y;
		bool krypt;
		for (int ilg = 4; ilg >= 1; ilg--) {
			for (int kiek = 1; kiek <= 4 - ilg + 1; kiek++)
			{

				x = rand() % 10;
				y = rand() % 10;

				krypt = (rand() % 2) == 0;
				if (!put_ship(x, y, ilg, krypt))
					kiek--;
			}
		}


	}
	bool cannon_fire(int x, int y) {
		message = " ";
		if (x < 0 || x>9 || y < 0 || y>9) {
			message = "Netinkama koordinate";
			return false;
		}

		else if (zaidimoLaukas[x][y] != LANG_TUSC) {
			message = "Pasirinkite kita langeli";
			return false;
		}

		else if (fl[x][y] == LANG_LAIV) {
			zaidimoLaukas[x][y] = LANG_PTKT;
			return true;
		}

		else if (fl[x][y] == LANG_TUSC) {
			zaidimoLaukas[x][y] = LANG_SAUTA;
			return true;
		}

		return true;
	}
	void message_show() {
		if (message != " ") cout << message << endl;
		message = " ";
	}

};

class playerField
{
private:
	int x, y;
	char playerField[10][10];
	bool kryptis = false;
	bool vertikalus = false;
	bool kitaPuse = false;
	int sautaPuse[4] = { 0 };
	bool pKartas = true;

public:
	int playerLaivuSk = 20;
	char get_status(int x, int y)
	{
		return playerField[x][y];
	};
	void show_field()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord;
		coord.X = 15;
		coord.Y = 0;
		SetConsoleCursorPosition(hout, coord);
		cout << "Zaidejo laukas\n";
		coord.Y = 1;
		SetConsoleCursorPosition(hout, coord);
		cout << "  0123456789\n";
		for (int y = 0; y < 10; y++)
		{
			coord.X = 15;
			coord.Y = y+2;
			SetConsoleCursorPosition(hout, coord);
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				if (playerField[x][y] == LANG_LAIV) SetConsoleTextAttribute(hConsole, 46);
				if (playerField[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196);
				cout << playerField[x][y];
				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
			}
			cout << endl;
		}

	};
	void show_start(bool message)
	{
		
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(hout, coord);
		cout << "Zaidejo laukas\n";

		coord.Y = 1;
		SetConsoleCursorPosition(hout, coord);
		cout << "  0123456789\n";
		for (int y = 0; y < 10; y++)
		{
			coord.X = 0;
			coord.Y = y + 2;
			SetConsoleCursorPosition(hout, coord);
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				if(playerField[x][y] == LANG_LAIV) SetConsoleTextAttribute(hConsole, 46);
				cout << playerField[x][y];
				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
			}
			cout << endl;
		}
		cout << "1-mouse = padeti laiva" << endl;
		cout << "2-mouse = pakeisti krypti" << endl;
		cout << endl << endl;
		cout << "Jei neveikia pele: 2-mouse ant consoles->Properties->Options->'uncheck' Quick Edit Mode" << endl;
		if (message) {
			SetConsoleTextAttribute(hConsole, 15 + 4*16);
			coord.X = 15;
			coord.Y = 5;
			SetConsoleCursorPosition(hout, coord);
			cout << "Netinkama pozicija";
			SetConsoleTextAttribute(hConsole, 15 + 1 * 16); //set back to black background and white text
		}
	};
	void clearscreen() {
		COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
		SetConsoleCursorPosition(hConsole, coordScreen);
		return;
	}
	void clear_field()
	{
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				playerField[x][y] = LANG_TUSC;
			}
		}
	};
	void destroyed(int x, int y) {//pazymi aplinkinius langelius
		if (vertikalus) {
			int k = 0; //einame zemyn
			while (playerField[x][y + k] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
						COORD coord;
						coord.X = 15 + x + i + 2;
						coord.Y = 0 + y + 2 + j + k;
						SetConsoleCursorPosition(hout, coord);
						if (playerField[x + i][y + j + k] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i) <= 9 && (y + j + k) <= 9 && (x + i) >= 0 && (y + j + k) >= 0) { playerField[x + i][y + j + k] = LANG_SAUTA; cout << LANG_SAUTA; }
					}
				}
				k++;
			}

			k = 0; //eina i virsu
			while (playerField[x][y - k] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
						COORD coord;
						coord.X = 15 + x + i + 2;
						coord.Y = 0 + y + 2 + j - k;
						SetConsoleCursorPosition(hout, coord);
						if (playerField[x + i][y + j - k] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i) <= 9 && (y + j - k) <= 9 && (x + i) >= 0 && (y + j - k) >= 0){ playerField[x + i][y + j - k] = LANG_SAUTA; cout << LANG_SAUTA;}
					}
				}
				k++;
			}

		}
		else {
			int k = 0; //einame i kaire
			while (playerField[x - k][y] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
						COORD coord;
						coord.X = 15 + x + i - k + 2;
						coord.Y = 0 + y + 2 + j;
						SetConsoleCursorPosition(hout, coord);
						if (playerField[x + i - k][y + j] != LANG_PTKT && playerField[x + i - k][y + j] != LANG_LAIV && (x + i - k) <= 9 && (y + j) <= 9 && (x + i - k) >= 0 && (y + j) >= 0) { playerField[x + i - k][y + j] = LANG_SAUTA; cout << LANG_SAUTA; }
					}
				}
				k++;
			}

			k = 0; //eina i desine
			while (playerField[x + k][y] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
						COORD coord;
						coord.X = 15 + x + i + k + 2;
						coord.Y = 0 + y + 2 + j;
						SetConsoleCursorPosition(hout, coord);
						if (playerField[x + i + k][y + j] != LANG_PTKT && playerField[x + i + k][y + j] != LANG_LAIV && (x + i + k) <= 9 && (y + j) <= 9 && (x + i + k) >= 0 && (y + j) >= 0) { playerField[x + i + k][y + j] = LANG_SAUTA; cout << LANG_SAUTA; }
					}
				}
				k++;
			}
		}
	}

	bool ar_vienvietis(int x, int y) {
		int counter = 0;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if ((playerField[x + i][y + j] == LANG_PTKT || playerField[x + i][y + j] == LANG_LAIV) && (x + i < 10 && y + j < 10 && x + i >= 0 && y + j >= 0)) counter++;
			}
		}

		if (counter == 1) {
			reset(x, y);
			return true;
		}
		return false;
	};
	bool suvis(int x, int y) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO     cursorInfo;
		COORD coord;
		coord.X = 15 + x + 2;
		coord.Y = 0 + y + 2;
		cursorInfo.bVisible = true; // set the cursor visibility
		SetConsoleCursorInfo(hout, &cursorInfo);
		SetConsoleCursorPosition(hout, coord);
		SetConsoleTextAttribute(hConsole, 196);
		//cout << x << " " << y << endl;
		if (playerField[x][y] == LANG_TUSC) {
			playerField[x][y] = LANG_SAUTA;
			cout << LANG_SAUTA;
			Sleep(500);
			return false;
		}
		else if (playerField[x][y] == LANG_LAIV) {
			playerField[x][y] = LANG_PTKT;
			cout << LANG_PTKT;
			Sleep(500);
			playerLaivuSk--;
			return true;
		}
		return false;
	}
	bool put_ship(int x, int y, int ilg, bool yra_vert)
	{
		bool pavyko = true;
		if (x < 0 || x>9 || y < 0 || y>9 || ilg < 1 || ilg>4 || (yra_vert && (ilg + y - 1) > 9) || (!yra_vert && (ilg + x - 1) > 9)) {
			return false;
		}
		else
			for (int i = 0; i < ilg && pavyko; i++)
			{
				if (yra_vert) // Vertikalus
				{
					if (playerField[x][y + i] != LANG_TUSC || // center
						playerField[x][int(abs(y + i - 1))] != LANG_TUSC || // up
						playerField[x + 1 - (x + 1) / 10][int(abs(y + i - 1))] != LANG_TUSC || // up-right
						playerField[x + 1 - (x + 1) / 10][y + i] != LANG_TUSC || // right
						playerField[x + 1 - (x + 1) / 10][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down-right
						playerField[x][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down
						playerField[int(abs(x - 1))][y + i + 1 - (y + i + 1) / 10] != LANG_TUSC || // down-left
						playerField[int(abs(x - 1))][y + i] != LANG_TUSC || // left
						playerField[int(abs(x - 1))][int(abs(y + i - 1))] != LANG_TUSC) // up-left
						return false;
				}
				else // Horizontalus
				{
					if (playerField[x + i][y] != LANG_TUSC || // center
						playerField[x + i][int(abs(y - 1))] != LANG_TUSC || // up
						playerField[x + i + 1 - (x + i + 1) / 10][int(abs(y - 1))] != LANG_TUSC || // up-right
						playerField[x + i + 1 - (x + i + 1) / 10][y] != LANG_TUSC || // right
						playerField[x + i + 1 - (x + i + 1) / 10][y + 1 - (y + 1) / 10] != LANG_TUSC || // down-right
						playerField[x + i][y + 1 - (y + 1) / 10] != LANG_TUSC || // down
						playerField[int(abs(x + i - 1))][y + 1 - (y + 1) / 10] != LANG_TUSC || // down-left
						playerField[int(abs(x + i - 1))][y] != LANG_TUSC || // left
						playerField[int(abs(x + i - 1))][int(abs(y - 1))] != LANG_TUSC) // up-left
						return false;
				}
			}

		if (pavyko) // "Dedam" laivą
		{
			for (int i = 0; i < ilg; i++) {

				if (yra_vert)
					playerField[x][y + i] = LANG_LAIV;
				else
					playerField[x + i][y] = LANG_LAIV;


			}
			//cout << "Ilgis:" << ilg << " x,y:" << x << ',' << y << " Vert:" << yra_vert << endl;
		}
		return pavyko;
	}
	void place_ships()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO     cursorInfo;
		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		cursorInfo.bVisible = true; // set the cursor visibility
		SetConsoleCursorInfo(hout, &cursorInfo);
		SetConsoleCursorPosition(hout, coord);

		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD InputRecord;
		DWORD Events;
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(hout, &cci);
		SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
		bool message = false;
		show_start(message);
		
		int x, y;
		for (int ilg = 4; ilg >= 1; ilg--) {
			for (int kiek = 1; kiek <= 4 - ilg + 1; kiek++)
			{
				message = false;
				bool input = true;
				bool krypt = false;
				while (input) {
					
					ReadConsoleInput(hin, &InputRecord, 1, &Events);
					switch (InputRecord.EventType) {
					case MOUSE_EVENT: // mouse input 
						clearscreen();
						show_start(message);
						x = InputRecord.Event.MouseEvent.dwMousePosition.X - 2;
						y = InputRecord.Event.MouseEvent.dwMousePosition.Y - 2;
						coord.X = x + 2;
						coord.Y = y + 2;
						cursorInfo.bVisible = true; // set the cursor visibility
						SetConsoleCursorPosition(hout, coord);
						SetConsoleTextAttribute(hConsole, 46);
						if (!krypt) {
							for (int i = 0; i < ilg; i++) {
								cout << LANG_LAIV;
							}
						}
						else {
							for (int i = 0; i < ilg; i++) {
								cout << LANG_LAIV;
								coord.Y++;
								SetConsoleCursorPosition(hout, coord);
							}
						}
						SetConsoleTextAttribute(hConsole, 15);

						if (GetKeyState(VK_LBUTTON) < 0)
						{
							
							if (!put_ship(x, y, ilg, krypt)) {
								message = true;
							} else input = false;
							
						}

						else if (GetKeyState(VK_RBUTTON) < 0)
						{
							if (krypt) krypt = false;
							else krypt = true;
						}
						break;
					}
				}
				
			}
		}


	}
	void reset(int x, int y) {
		destroyed(x, y);
		vertikalus = false;
		kitaPuse = false;
		pKartas = true;
		for (int i = 0; i < 4; i++) sautaPuse[i] = 0;

	};
	void AI_shoot() {

		bool pataike = true;
		if (playerLaivuSk <= 0) pataike = false;
		while (pataike) {
			pataike = false;
			if (pKartas) {
				x = rand() % 10;
				y = rand() % 10;
				while (playerField[x][y] == LANG_SAUTA || playerField[x][y] == LANG_PTKT || x>9 || x<0 || y>9 || y<0) {
					if (playerLaivuSk <= 0) break;
					x = rand() % 10;
					y = rand() % 10;
				}

				pataike = suvis(x, y);
				if (playerLaivuSk <= 0) break;
				pKartas = !pataike;
				if (pataike) {
					pKartas = ar_vienvietis(x, y);
					if (playerLaivuSk <= 0) {
						pataike = false;
						break;
					}
				}

			}
			else if (!pKartas) {

				if (kitaPuse) {
					if (vertikalus) {
						if (playerField[x][y + 1] == LANG_PTKT && y + 1 <= 9) {
							int i = 1;     // pradeda sauti vienu i apacia
							while (suvis(x, y - i)) {
								i++;
							}
						}
						else if (y - 1 >= 0) {    //einame zemyn
							int i = 1;
							while (suvis(x, y + i)) {
								i++;
							}
						}
						//horizontaliai
					}
					else {
						if (playerField[x + 1][y] == LANG_PTKT && x + 1 <= 9) {
							int i = 1;     // einame i kaire
							while (suvis(x - i, y)) {
								i++;
							}
						}
						else if (x + 1 <= 9) { // einame i desine
							int i = 1;
							while (suvis(x + i, y)) {
								i++;
							}
						}

					}
					//clear all since the ship was destrOyed

					reset(x, y);

				}
				else {
					bool retry = true;
					while (retry) {
						retry = false;
						if (playerLaivuSk <= 0) {
							pataike = false;
							break;
						}
						//ieskoma nesauta puse
						int random = rand() % 4;    //ieskoma nesauta puse
						while (sautaPuse[random] != 0) {
							random = rand() % 4;
						}

						//saudys i virsu
						if (random == 0) {
							sautaPuse[0] = 1;
							if (playerField[x][y - 1] == LANG_SAUTA || y - 1 < 0)  retry = true;
							else {
								vertikalus = suvis(x, y - 1);

								if (vertikalus) {
									int i = 2;     //nes antras suvis todel = 2
									while (suvis(x, y - i) && i < 4) {
										if (y - i < 0) reset(x, y);
										i++;
									}

									if (i < 4)   kitaPuse = true;
									if (playerField[x][y + 1] == LANG_SAUTA) reset(x, y); //jei priesingoje puseje jau sauta 
									if (i == 4) reset(x, y);
									if (sautaPuse[1] == 1) reset(x, y); //jau buvo sauta i priesinga puse grizti nereikia
									if (y == 9) reset(x, y); //jei yra pacioje apacioje zemyn nereikia saudyti 
								}


							}
						}

						//saudys i apacia
						if (random == 1) {
							sautaPuse[1] = 1;
							if (playerField[x][y + 1] == LANG_SAUTA || y + 1 > 9)  retry = true;
							else {
								vertikalus = suvis(x, y + 1);

								if (vertikalus) {
									int i = 2;     //nes antras suvis todel = 2
									while (suvis(x, y + i) && i < 4) {
										if (y + i > 9) reset(x, y);
										i++;
									}
									if (playerField[x][y - 1] == LANG_SAUTA) reset(x, y); //jei priesingoje puseje jau sauta 
									if (i < 4)   kitaPuse = true;
									if (i == 4) reset(x, y);
									if (sautaPuse[0] == 1) reset(x, y);
									if (y == 0) reset(x, y); //jei yra pacioje virsuje i virsu nereikia saudyti 
								}
							}

						}

						// saudys i desine
						if (random == 2) {
							sautaPuse[2] = 1;
							if (playerField[x + 1][y] == LANG_SAUTA || x + 1 > 9)  retry = true;
							else {
								vertikalus = !(suvis(x + 1, y));

								if (!vertikalus) {
									int i = 2;     //nes antras suvis todel = 2
									while (suvis(x + i, y) && i < 4) {
										if (x + i > 9) reset(x, y);
										i++;
									}
									if (playerField[x-1][y] == LANG_SAUTA) reset(x, y); //jei priesingoje puseje jau sauta 

									if (i < 4)   kitaPuse = true;
									if (i == 4) reset(x, y);
									if (sautaPuse[3] == 1) reset(x, y);
									if (x == 0) reset(x, y); //jei yra kaireje
								}
							}

						}

						// saudys i kaire
						if (random == 3) {
							sautaPuse[3] = 1;
							if (playerField[x - 1][y] == LANG_SAUTA || x - 1 < 0)  retry = true;
							else {
								vertikalus = !(suvis(x - 1, y));

								if (!vertikalus) {
									int i = 2;     //nes antras suvis todel = 2
									while (suvis(x - i, y) && i < 4) {
										if (x - i > 0) reset(x, y);
										i++;
									}
									if (playerField[x+1][y] == LANG_SAUTA) reset(x, y); //jei priesingoje puseje jau sauta 

									if (i < 4)   kitaPuse = true;
									if (i == 4) reset(x, y);
									if (sautaPuse[2] == 1) reset(x, y);
									if (x == 9) reset(x, y); //jei yra pacioje desineje
								}
							}
						}
					
					}//retry
				}//else kitaPuse
			} // else if !pKartas
		}//while
	}//ai shoot
}; // objektas

void cheatcode(Claukas &mano, int x, int y, int &AILaivuSk) {

	//cheat code nuke
	if (x == 1337 && y == 1488) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				mano.cannon_fire(i, j);
				if (mano.get_status(i, j) == LANG_LAIV) {
					AILaivuSk--;
				}
			}
		}
	}


}

void clearscreen() {
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}

void screen(Claukas mano, playerField cat) {

	clearscreen();
	mano.show_field();
	cat.show_field();
	if (cat.playerLaivuSk > 0) cout << "Paspauskite kur norite sauti" << endl;
	mano.message_show();
}

void gameOver(Claukas mano, playerField cat) {
	clearscreen();
	mano.end_game();
	cat.show_field();
	cout << "Zaidimas baigesi" << endl;
	if (cat.playerLaivuSk <= 0) cout << "Defeat" << endl;
	else cout << "Win" << endl;
	system("PAUSE");
	exit(1);
}




int main()
{
	

	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 600, 500, TRUE);

	SetConsoleTextAttribute(console, 15 + 1 * 16);
	clearscreen();

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 12;                   // Width of each character in the font
	cfi.dwFontSize.Y = 24;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	srand(time(NULL));
	Claukas mano;
	playerField cat;
	cat.clear_field();
	cat.place_ships();
	mano.clear_field();
	mano.place_ships();
	int AILaivuSk = 20;

	while (cat.playerLaivuSk > 0 && AILaivuSk > 0) {

		int x = -1, y = -1;
		bool pataike = true;

		//zaidejas
		while (pataike) {
			screen(mano, cat);
			pataike = false;
			bool input = true;
			while(input){

				ReadConsoleInput(hin, &InputRecord, 1, &Events);
				switch (InputRecord.EventType) {
				case MOUSE_EVENT: // mouse input 

					if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						//cout << InputRecord.Event.MouseEvent.dwMousePosition.X << " " << InputRecord.Event.MouseEvent.dwMousePosition.Y << endl;
						x = InputRecord.Event.MouseEvent.dwMousePosition.X - 2;
						y = InputRecord.Event.MouseEvent.dwMousePosition.Y - 2;
						input = false;
					}
					break;

				/*case KEY_EVENT:
					switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
					{
						case VK_RETURN:
						cin >> x >> y;
						input = false;
						break;
					}
					break;*/
					


				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
					;
					break;

				case FOCUS_EVENT:  // disregard focus events 

				case MENU_EVENT:   // disregard menu events 

					break;


				}
			}

			FlushConsoleInputBuffer(hin);

			cheatcode(mano, x, y, AILaivuSk);
			if (AILaivuSk == 0) break;
			if (!mano.cannon_fire(x, y)) {
				pataike = true; //leidziama sauti dar karta jei neteisingai ivestos kooridantes
			}
			else {
				if (mano.get_status(x, y) == LANG_LAIV) {
					AILaivuSk--;
					pataike = true;
				}
			}
		}

		screen(mano, cat);
		//AI
		if (AILaivuSk > 0) cat.AI_shoot();


	}

	gameOver(mano, cat);
	return 0;
}

/*
kartais freezina

Flickering
*/
