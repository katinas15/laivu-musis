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
	char zaidimoLaukas[10][10];	// laukas kuris yra isvedamas zaidejui, jame mato kur sove ir kur pataike
	char fl[10][10];		//laukas kuriame yra surasyti visi prieso laivai
public:
	string message = " ";	//reikalingas isvesti ar teisingai pasirinko koordinates
	char get_status(int x, int y)//isveda char pasirinktos pozicijos
	{
		return fl[x][y];	
	};
	void show_field()	//isveda zaidimo lauka i kuri zaidejas saudo
	{	
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//reikalinga spalvoms nustatyti
		cout << "Prieso laukas\n";
		cout << "  0123456789\n";

		for (int y = 0; y < 10; y++) {
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				
				if (zaidimoLaukas[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196); //jei pataikyta isveda raudona spalva
				cout << zaidimoLaukas[x][y];
				SetConsoleTextAttribute(hConsole, 15 + 1 * 16); //grazinama i melynai balta spalva
			}
			cout << endl;
		}

	};
	void end_game()	//pasibaigus zaidimui parodoma kur buvo visi prieso laivai
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//reikalinga spalvoms nustatyti
		cout << "Prieso laukas\n";
		cout << "  0123456789\n";

		for (int y = 0; y < 10; y++) {
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				if(zaidimoLaukas[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196); //jei pataikyta isveda raudona spalva
				if (zaidimoLaukas[x][y] == LANG_TUSC) {	//parodomi like laivai
					if(fl[x][y] == LANG_LAIV) SetConsoleTextAttribute(hConsole, 46);

					cout << fl[x][y];
					}
				else cout << zaidimoLaukas[x][y];

				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);//grazinama i melynai balta spalva
			}
			cout << endl;
		}

	};
	void clear_field()	//istrinami visi lauke buve char
	{
		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++) {
				zaidimoLaukas[x][y] = LANG_TUSC;
				fl[x][y] = LANG_TUSC;
			}
	};
	bool put_ship(int x, int y, int ilg, bool yra_vert) //tikrinama ar laivas gali buti padetas
	{
		bool pavyko = true;
		if (x < 0 || x>9 || y < 0 || y>9 || ilg < 1 || ilg>4 || (yra_vert && (ilg + y) > 9) || (!yra_vert && (ilg + x) > 9)) {	//tiktinama ar parinkta tinkama koordinate
			return false;
		}
		else
			for (int i = 0; i < ilg && pavyko; i++)	//tikrinama ar salia nera laivu
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
	void place_ships()	//bandoma padeti laiva atsitiktinai pasirinktoje pozicijoje
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
	bool cannon_fire(int x, int y) {	//saunama i atitinkama koordinate
		message = " ";
		if (x < 0 || x>9 || y < 0 || y>9) {	//jei neiteisingai pasirinkta koordinate
			message = "Netinkama koordinate";
			return false;
		}

		else if (zaidimoLaukas[x][y] != LANG_TUSC) {	//jei jau i sita langeli buvo sauta
			message = "Pasirinkite kita langeli";
			return false;
		}

		else if (fl[x][y] == LANG_LAIV) {	//jei tai laivas tai pataike
			zaidimoLaukas[x][y] = LANG_PTKT;
			return true;
		}

		else if (fl[x][y] == LANG_TUSC) {	//jei sauta i tuscia langeli
			zaidimoLaukas[x][y] = LANG_SAUTA;
			return true;
		}

		return true;
	}
	void message_show() {	//isvedamas cannon_fire nustatytas message ir iskarto istrinamas
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
	char get_status(int x, int y)//isveda char pasirinktos pozicijos
	{
		return playerField[x][y];
	};
	void show_field() //isveda zaidimo lauka i kuri zaidejas saudo
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //reikalinga spalvoms nustatyti
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);	//reikalinga nustatant cout pozicija
		COORD coord;	//cout pozicijos koordinates
		coord.X = 15;
		coord.Y = 0;
		SetConsoleCursorPosition(hout, coord);	//nustatoma kursoriaus pozicija
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
				if (playerField[x][y] == LANG_LAIV) SetConsoleTextAttribute(hConsole, 46);	//parenkamos atitinkamos isvedimo spalvos
				if (playerField[x][y] == LANG_PTKT) SetConsoleTextAttribute(hConsole, 196);
				cout << playerField[x][y];
				SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
			}
			cout << endl;
		}

	};
	void show_start(bool message)	//reikalinga zaidimo pradziai kai zaidejas delioja laivus
	{
		
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//reikalinga spalvoms nustatyti
		SetConsoleTextAttribute(hConsole, 15 + 1 * 16);
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);//reikalinga nustatant cout pozicija
		COORD coord;//cout pozicijos koordinates
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(hout, coord);//nustatoma kursoriaus pozicija
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
		cout << "1-mouse = padeti laiva" << endl;	//deliojimo instrukcijos
		cout << "2-mouse = pakeisti krypti" << endl;
		cout << endl << endl;
		cout << "Jei neveikia pele: 2-mouse ant consoles->Properties->Options->'uncheck' Quick Edit Mode" << endl;	//kartais neveikia mouse
		if (message) {//jei buvo pasirinkta netinkama laivo dejimo pozicija
			SetConsoleTextAttribute(hConsole, 15 + 4*16);
			coord.X = 15;
			coord.Y = 5;
			SetConsoleCursorPosition(hout, coord);
			cout << "Netinkama pozicija";
			SetConsoleTextAttribute(hConsole, 15 + 1 * 16); //set back to black background and white text
		}
	};
	void clearscreen() {	//istrinamas ekranas
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
	void clear_field()	//istrinami playerfield esami char
	{
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				playerField[x][y] = LANG_TUSC;
			}
		}
	};
	void destroyed(int x, int y) {//pazymi aplinkinius langelius jei laivas buvo nusautas tam kad AI nesuadytu aplinkui nusauta laiva kur pagal salyga negali buti laivu
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

	bool ar_vienvietis(int x, int y) {	//patikrina ar tai vienvietis laivas, jei taip iskart pazymima aplinkui kaip nusautas
		int counter = 0;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if ((playerField[x + i][y + j] == LANG_PTKT || playerField[x + i][y + j] == LANG_LAIV) && (x + i < 10 && y + j < 10 && x + i >= 0 && y + j >= 0)) counter++; //ieskoma ar tikrai vienvietis
			}
		}

		if (counter == 1) {
			destroyed(x, y);	//nutrinami aplinkiniai langeliai
			return true;
		}
		return false;
	};
	bool suvis(int x, int y) {//AI saudo
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
		if (x > 9 || x < 0 || y>9 || y < 0) return false;

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
	bool put_ship(int x, int y, int ilg, bool yra_vert) //tikrina ar gali padeti laiva atitinkamoje pozicijoje
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
	void place_ships()	//zaidejas peles mygtuko paspaudimu deda laivus savo lauke
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

		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);	//mouse input
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
				while (input) {	//tol kol nepaspaustas 1-mouse
					
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
						if (!krypt) {						//kiekviena karta kai yra pajudinama pelyte, parodomas laivas kuri gali padeti zaidejas toje pozicijoje
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

						if (GetKeyState(VK_LBUTTON) < 0)	//jei 1-mouse mygtukas laivas pastatomas
						{
							if (!put_ship(x, y, ilg, krypt)) {
								message = true;
							} else input = false;
							
						}

						else if (GetKeyState(VK_RBUTTON) < 0)	//jei 2-mouse pakeiciama kryptis laivo
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
	void reset(int x, int y) {	//resetinamas AI saudymas, kad vel saudytu random
		destroyed(x, y);
		vertikalus = false;
		kitaPuse = false;
		pKartas = true;
		for (int i = 0; i < 4; i++) sautaPuse[i] = 0;
		AI_shoot();

	};
	void AI_shoot() {

		bool pataike = true;
		if (playerLaivuSk <= 0) pataike = false;
		while (pataike) {	
			pataike = false;
			if (pKartas) {	//jei tai pirmas suvis AI sauna random
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
			else if (!pKartas) {	//jei tai yra antras suvis jau saudoma pagal tam tikra logika

				if (kitaPuse) {	 // jei buvo saudoma i viena puse ir nera aisku ar laivas buvo nusautas, saudoma i priesinga puse negu pries tai
					if (vertikalus) {
						if (playerField[x][y + 1] == LANG_PTKT && y + 1 <= 9) {
							int i = 1;     // pradeda sauti vienu i apacia
							while (suvis(x, y - i)) {
								
								i++;
								if (y - i < 0) break;//kad nesaudytu uz ribu
							}
						}
						else if (y - 1 >= 0) {    //einame zemyn
							int i = 1;
							while (suvis(x, y + i)) {
								
								i++;
								if (y + i > 0) break;//kad nesaudytu uz ribu
							}
						}
						//horizontaliai
					}
					else {
						if (playerField[x + 1][y] == LANG_PTKT && x + 1 <= 9) {
							int i = 1;     // einame i kaire
							while (suvis(x - i, y)) {
								
								i++;
								if (x - i < 0) break;//kad nesaudytu uz ribu
							}
						}
						else if (x + 1 <= 9) { // einame i desine
							int i = 1;
							while (suvis(x + i, y)) {
								
								i++;
								if (x + i > 9) break;//kad nesaudytu uz ribu
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
						//jei buvo pataikyta, atsitiktinai parenkama i kuria puse toliau saudyti
						int random = rand() % 4;    //ieskoma nesauta puse 
						while (sautaPuse[random] != 0) {
							random = rand() % 4;
						}

						//saudys i virsu
						if (random == 0) {
							sautaPuse[0] = 1;
							if (playerField[x][y - 1] == LANG_SAUTA || y - 1 < 0)  retry = true;	//jei pasirinktoje puseje jau langelis sautas arba iseina uz ribu
							else {
								vertikalus = suvis(x, y - 1);

								if (vertikalus) {
									
									int i = 2;     //nes trecias suvis todel = 2
									if (y - i < 0) {
										kitaPuse = true;
								
										//kad nesaudytu uz ribu
									}
									else {

										while (suvis(x, y - i) && i <= 4) {
											
											i++;
											if (y - i < 0) kitaPuse = true;//kad nesaudytu uz ribu
										}

									}


									if (kitaPuse) {	//jei pasiekiama lauko riba saudoma i kita puse
										i = 1;
										while (suvis(x, y + i) && i <= 4) {
											i++;
										}
									}

									if (y + 1 > 9) reset(x, y); // jei pabaiga
									else if (playerField[x][y + 1] == LANG_SAUTA || y + 1 > 9) reset(x, y); //jei priesingoje puseje jau sauta 
									else if (i == 4) reset(x, y);	//jei tai yra keturvietis laivas, jis sunaikinamas ir aplinkui pazymima
									else if (sautaPuse[1] == 1) reset(x, y); //jau buvo sauta i priesinga puse grizti nereikia
									else if (y == 9) reset(x, y); //jei yra pacioje apacioje zemyn nereikia saudyti 
									else if (kitaPuse) reset(x, y);
									else if (i < 4)   kitaPuse = true;	//jei neaisku ar buvo pilnai nusautas laivas todel bandoma sauti i priesinga puse


								}


							}
						}

						//saudys i apacia
						if (random == 1) {
							sautaPuse[1] = 1;
							if (playerField[x][y + 1] == LANG_SAUTA || y + 1 > 9)  retry = true;//jei pasirinktoje puseje jau langelis sautas arba iseina uz ribu
							else {
								vertikalus = suvis(x, y + 1);

								if (vertikalus) {
									int i = 2;     //nes trecias suvis todel = 2
									if (y + i > 9) {
										kitaPuse = true;
											//kad nesaudytu uz ribu
									}
									else {

										while (suvis(x, y + i) && i <= 4) {
											
											i++;
											if (y + i > 9) break;//kad nesaudytu uz ribu
										}
									}

									if (kitaPuse) {//jei pasiekiama lauko riba saudoma i kita puse
										i = 1;
										while (suvis(x, y - i) && i <= 4) {
											i++;
										}
									}

									if (y - 1 < 0) reset(x, y); // jei pabaiga
									else if (playerField[x][y - 1] == LANG_SAUTA)  reset(x, y); //jei priesingoje puseje jau sauta arba kitoje puseje pabaiga
									else if (kitaPuse) reset(x, y);
									else if (i < 4)   kitaPuse = true;//jei neaisku ar buvo pilnai nusautas laivas todel bandoma sauti i priesinga puse
									else if (i == 4) reset(x, y);//jei tai yra keturvietis laivas, jis sunaikinamas ir aplinkui pazymima
									else if (sautaPuse[0] == 1) reset(x, y); //jau buvo sauta i priesinga puse grizti nereikia
									else if (y == 0) reset(x, y); //jei yra pacioje virsuje i virsu nereikia saudyti 
			
								}
							}

						}

						// saudys i desine
						if (random == 2) {
							sautaPuse[2] = 1;
							if (playerField[x + 1][y] == LANG_SAUTA || x + 1 > 9) {
								retry = true;//jei pasirinktoje puseje jau langelis sautas arba iseina uz ribu
							}
							else {
								vertikalus = !(suvis(x + 1, y));

								if (!vertikalus) {
									int i = 2;     //nes trecias suvis todel = 2
									if (x + i > 9) {
										kitaPuse = true;
										//kad nesaudytu uz ribu
									}
									else {

										while (suvis(x + i, y) && i <= 4) {
											
											i++;
											if (x + i > 9) break;//kad nesaudytu uz ribu
										}

									}

									if (kitaPuse) {//jei pasiekiama lauko riba saudoma i kita puse
										i = 1;
										while (suvis(x - i, y) && i <= 4) {
											i++;
										}
									}


									if (x + 1 > 9) reset(x, y); // jei pabaiga
									else if (playerField[x - 1][y] == LANG_SAUTA || x - 1 < 0) reset(x, y); //jei priesingoje puseje jau sauta 
									else if (kitaPuse) reset(x, y);
									else if (i < 4)   kitaPuse = true;   //jei neaisku ar buvo pilnai nusautas laivas todel bandoma sauti i priesinga puse
									else if (i == 4) reset(x, y);   //jau buvo sauta i priesinga puse grizti nereikia
									else if (sautaPuse[3] == 1) reset(x, y); //jau buvo sauta i priesinga puse grizti nereikia
									else if (x == 0) reset(x, y); //jei yra kaireje
							
								}
							}

						}

						// saudys i kaire
						if (random == 3) {
							sautaPuse[3] = 1;
							if (playerField[x - 1][y] == LANG_SAUTA || x - 1 < 0)  retry = true;//jei pasirinktoje puseje jau langelis sautas arba iseina uz ribu
							else {
								vertikalus = !(suvis(x - 1, y));

								if (!vertikalus) {
									int i = 2;     //nes trecias suvis todel = 2
									if (x - i < 0) {
										kitaPuse = true;
					
										//kad nesaudytu uz ribu
									}
									else {

										while (suvis(x - i, y) && i <= 4) {
											
											i++;
											if (x - i < 0) break; //kad nesaudytu uz ribu
										}

									}

									if (kitaPuse) {//jei pasiekiama lauko riba saudoma i kita puse
										i = 1;
										while (suvis(x + i, y) && i <= 4) {
											i++;
										}
									}

									if (x - 1 < 0) reset(x, y); // jei pabaiga
									else if (playerField[x + 1][y] == LANG_SAUTA || x + 1 > 9) reset(x, y); //jei priesingoje puseje jau sauta 
									else if (kitaPuse) reset(x, y);
									else if (i < 4)   kitaPuse = true; //jei neaisku ar buvo pilnai nusautas laivas todel bandoma sauti i priesinga puse
									else if (i == 4) reset(x, y);//jau buvo sauta i priesinga puse grizti nereikia
									else if (sautaPuse[2] == 1) reset(x, y);//jau buvo sauta i priesinga puse grizti nereikia
									else if (x == 9) reset(x, y); //jei yra pacioje desineje
							
								}
							}
						}
					
					}//retry
				}//else kitaPuse
			} // else if !pKartas
		}//while
	}//ai shoot
}; // objektas

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
				if (AILaivuSk <= 0) gameOver(mano, cat);
				ReadConsoleInput(hin, &InputRecord, 1, &Events);
				switch (InputRecord.EventType) {
				case MOUSE_EVENT: // mouse input 

					if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)	//jei paspaustas 1-mouse sauna i ta koordinate
					{
						x = InputRecord.Event.MouseEvent.dwMousePosition.X - 2;
						y = InputRecord.Event.MouseEvent.dwMousePosition.Y - 2;
						input = false;
					}
					break;

				}
			}

			FlushConsoleInputBuffer(hin);

			if (AILaivuSk <= 0) gameOver(mano, cat);
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
		//AI saudymas
		if (AILaivuSk > 0) cat.AI_shoot();


	}

	gameOver(mano, cat);
	return 0;
}

