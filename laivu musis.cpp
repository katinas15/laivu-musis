#include "pch.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>
#include <type_traits>
#include <windows.h>

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
	string message = " ";
	char get_status(int x, int y)
	{
		return fl[x][y];
	};
	void show_field()
	{
		cout << "  0123456789\n";

		for (int y = 0; y < 10; y++) {
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
						if (playerField[x + i][y + j + k] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i) <= 9 && (y + j + k) <= 9 && (x + i) >= 0 && (y + j + k) >= 0) playerField[x + i][y + j + k] = LANG_SAUTA;
					}
				}
				k++;
			}

			k = 0; //eina i virsu
			while (playerField[x][y - k] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (playerField[x + i][y + j - k] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i) <= 9 && (y + j - k) <= 9 && (x + i) >= 0 && (y + j - k) >= 0) playerField[x + i][y + j - k] = LANG_SAUTA;
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
						if (playerField[x + i - k][y + j] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i - k) <= 9 && (y + j) <= 9 && (x + i - k) >= 0 && (y + j + k) >= 0) playerField[x + i - k][y + j] = LANG_SAUTA;
					}
				}
				k++;
			}

			k = 0; //eina i desine
			while (playerField[x + k][y] == LANG_PTKT) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (playerField[x + i + k][y + j] != LANG_PTKT && playerField[x + i][y + j + k] != LANG_LAIV && (x + i + k) <= 9 && (y + j) <= 9 && (x + i + k) >= 0 && (y + j + k) >= 0) playerField[x + i + k][y + j] = LANG_SAUTA;
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
				if ((playerField[x + i][y + j] == LANG_PTKT || playerField[x + i][y + j] == LANG_LAIV) && (x+i < 10 && y+j < 10 && x+i >= 0 && y+j >= 0)) counter++;
			}
		}

		if (counter == 1) {
			cout << "vienvietis nusautas" << endl;
			reset(x, y);
			return true;
		}
		return false;
	}
	bool suvis(int x, int y) {
		cout << x << " " << y << endl;
		if (playerField[x][y] == LANG_TUSC) {
			playerField[x][y] = LANG_SAUTA;
			return false;
		}
		else if (playerField[x][y] == LANG_LAIV) {
			playerField[x][y] = LANG_PTKT;
			playerLaivuSk--;
			return true;
		}
		return false;
	}
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
	void reset(int x, int y) {
		destroyed(x, y);
		vertikalus = false;
		kitaPuse = false;
		pKartas = true;
		for (int i = 0; i < 4; i++) sautaPuse[i] = 0;
		
	}
	void AI_shoot() {

		bool pataike = true;
		if (playerLaivuSk <= 0) pataike = false;
		while (pataike) {
			pataike = false;
			if (pKartas) {
				x = rand() % 10;
				y = rand() % 10;
				while (playerField[x][y] == LANG_SAUTA || playerField[x][y] == LANG_PTKT || x>9 || x<0 || y>9||y<0) {
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
				retry:
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
						if (playerField[x][y - 1] == LANG_SAUTA || y - 1 < 0)  goto retry;
						vertikalus = suvis(x, y - 1);

						if (vertikalus) {
							int i = 2;     //nes antras suvis todel = 2
							while (suvis(x, y - i) && i < 4) {
								if (y - i < 0) reset(x, y);
								i++;
							}

							if (i < 4)   kitaPuse = true;
							if (i == 4) reset(x, y);
							if (sautaPuse[1] == 1) reset(x, y); //jau buvo sauta i priesinga puse grizti nereikia
						}

					}

					//saudys i apacia
					if (random == 1) {
						sautaPuse[1] = 1;
						if (playerField[x][y + 1] == LANG_SAUTA || y + 1 > 9)  goto retry;
						vertikalus = suvis(x, y + 1);

						if (vertikalus) {
							int i = 2;     //nes antras suvis todel = 2
							while (suvis(x, y + i) && i < 4) {
								if (y + i > 9) reset(x, y);
								i++;
							}

							if (i < 4)   kitaPuse = true;
							if (i == 4) reset(x, y);
							if (sautaPuse[0] == 1) reset(x, y);
						}

					}

					// saudys i desine
					if (random == 2) {
						sautaPuse[2] = 1;
						if (playerField[x + 1][y] == LANG_SAUTA || x + 1 > 9)  goto retry;
						vertikalus = !(suvis(x + 1, y));

						if (!vertikalus) {
							int i = 2;     //nes antras suvis todel = 2
							while (suvis(x + i, y) && i < 4) {
								if (x + i > 9) reset(x, y);
								i++;
							}

							if (i < 4)   kitaPuse = true;
							if (i == 4) reset(x, y);
							if (sautaPuse[3] == 1) reset(x, y);
						}

					}

					// saudys i kaire
					if (random == 3) {
						sautaPuse[3] = 1;
						if (playerField[x - 1][y] == LANG_SAUTA || x - 1 < 0)  goto retry;
						vertikalus = !(suvis(x - 1, y));

						if (!vertikalus) {
							int i = 2;     //nes antras suvis todel = 2
							while (suvis(x - i, y) && i < 4) {
								if (x - i > 0) reset(x, y);
								i++;
							}

							if (i < 4)   kitaPuse = true;
							if (i == 4) reset(x, y);
							if (sautaPuse[2] == 1) reset(x, y);
						}
					}
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


void screen(Claukas mano, playerField cat) {

	system("cls");
	cat.show_field();
	cout << endl << endl;
	mano.show_field();
	if(cat.playerLaivuSk > 0) cout << "pasirinkite koordinates x ir y" << endl;
	mano.message_show();
}

void gameOver(Claukas mano, playerField cat) {
	screen(mano, cat);
	cout << "Zaidimas baigesi" << endl;
	if (cat.playerLaivuSk <= 0) cout << "Defeat" << endl;
	else cout << "Win" << endl;
	system("PAUSE");
	exit(1);
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
	int AILaivuSk = 20;

	while (cat.playerLaivuSk > 0 && AILaivuSk > 0) {

		int x, y;
		bool pataike = true;

		//zaidejas
		while (pataike) {
			screen(mano, cat);
			pataike = false;
			cin >> x >> y;
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

		//AI
		if (AILaivuSk > 0) cat.AI_shoot();


	}

	gameOver(mano,cat);
	return 0;
}

/*
sutvarkyti kad neleistu rasyti characteriu

nenaudoti goto

kartais ant pabaigos crashina

neleisti rasyti characteriu
 */
