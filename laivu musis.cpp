// laivu musis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>
#include "pch.h"
using namespace std;

const char LANG_TUSC = ' ';
const char LANG_LAIV = 'O';
const char LANG_SAUTA = '.';
const char LANG_PTKT = 'X';
class Claukas
{
private:
	char zaidimoLaukas[10][10];
	char fl[10][10];
public:
	string message;
	char get_status(int x, int y)
	{
		return fl[x][y];
	};
	void show_field()
	{
		cout << "  0123456789\n";
		for (int y = 0; y < 10; y++)
		{
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				cout << zaidimoLaukas[x][y];
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
			cout << "Ilgis:" << ilg << " x,y:" << x << ',' << y << " Vert:" << yra_vert << endl;
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
		if (message != "") cout << message << endl;
		message = "";
	}

};



class playerField
{
private:
	char playerField[10][10];
	bool kryptis = false;
public:
	char get_status(int x, int y)
	{
		return playerField[x][y];
	};
	void show_field()
	{
		cout << "  0123456789\n";
		for (int y = 0; y < 10; y++)
		{
			cout << y << " ";
			for (int x = 0; x < 10; x++) {
				cout << playerField[x][y];
			}
			cout << endl;
		}

	};
	void clear_field()
	{
		for (int y = 0; y < 10; y++)
			for (int x = 0; x < 10; x++) {
				playerField[x][y] = LANG_TUSC;
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
			cout << "Ilgis:" << ilg << " x,y:" << x << ',' << y << " Vert:" << yra_vert << endl;
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
	void AI_shoot(int playerLaivuSk) {
		bool pataike = true;
		bool kartojimas = false;
		while (pataike) {
			pataike = false;
			if (!kartojimas) {
				int x, y;
				x = rand() % 10;
				y = rand() % 10;
				cout << x << " " << y << endl;
				while (playerField[x][y] == LANG_SAUTA || playerField[x][y] == LANG_PTKT) {
					x = rand() % 10;
					y = rand() % 10;
				}
				if (playerField[x][y] == LANG_TUSC) {
					playerField[x][y] = LANG_SAUTA;
					pataike = false;
				}
				else if (playerField[x][y] == LANG_LAIV) {
					playerField[x][y] = LANG_PTKT;
					pataike = true;
					kartojimas = true;
				}
				if (pataike) playerLaivuSk--;
			} else {

			}

		}


		
	}
};

void cheatcode(Claukas &mano,int x, int y,int &AILaivuSk) {
	
	//cheat code nuke
	if (x == 1337 && y == 1488) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				mano.cannon_fire(i, j);
				//cout << mano.get_status(i,j) << endl;
				if (mano.get_status(i, j) == LANG_LAIV) {
					AILaivuSk--;
				}
			}
		}
	}


}


void screen(Claukas mano, playerField cat){
	
	system("cls");
	cat.show_field();
	cout << endl << endl;
	mano.show_field();
	cout << "pasirinkite koordinates x ir y" << endl;
	mano.message_show();
}

int main()
{
	srand(time(NULL));
	Claukas mano;
	playerField cat;
	cat.clear_field();
	cat.place_ships();
	mano.clear_field();
	mano.place_ships();
	int playerLaivuSk = 20;
	int AILaivuSk = 20;

	while (playerLaivuSk > 0 && AILaivuSk > 0) {

		int x, y;
		bool pataike = true;

		//zaidejas
		while (pataike) {
			screen(mano,cat);
			pataike = false;
			cin >> x >> y;

			cheatcode(mano, x, y, AILaivuSk);

			if (AILaivuSk == 0) break;

			mano.cannon_fire(x, y);
			
			if (mano.get_status(x, y) == LANG_LAIV) {
				AILaivuSk--;
				pataike = true;
			}
		}

		//AI
		if(AILaivuSk > 0) cat.AI_shoot(playerLaivuSk);

		
	}

	screen(mano, cat);

	cout << "Zaidimas baigesi" << endl;
	if (AILaivuSk  <= 0) cout << "Win" << endl;
	else cout << "Defeat" << endl;
	system("PAUSE");
	return 0;
}

/*
1. pataiko
2. paleisti AI
3. patikrinti ar tai buvo 1 langelio laivas ir ar galima sauti salia, jei taip pazymeti aplinkui kad dar karta nesautu, jei ne random sauna
4. sauna random kryptimi
4.1 jei nepataiko kita ejima bandoma kita kryptis
4.2 jei pataiko saudo toliau ta pacia kryptimi kol nepataiko
4.2.1 jei tai ne visas laivas gryztama atgal ir saudoma i priesinga puse
4.3 kaskart tikrinimas vykdomas ar nenusautas laivas



*/



















