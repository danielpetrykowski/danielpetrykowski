#include "Game.h"
#include <windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <rapidxml\rapidxml.hpp>
#include "rapidxml/rapidxml_print.hpp"


RenderWindow window(VideoMode(960, 540), "Gra KNI!");


Game::Game()
{
    state = MENU1;
	sMap = "";
	sTexture = "";
	parwin = 0;
	configurate = Game::readXMLconfig();
	timeSavePosition = 0;
}

Game::~Game()
{
}

void Game::runGame()
{
    while(state != END)
    {
        switch (state)
        {
        case MENU1:
            menu1();
            break;
        case MENU2:
            menu2();
            break;
        case GAME:
            gameStart();
            break;
		case SETTINGS:
			settings();
			break;
        }
    }
}

//////////////////XML///////////////////////////////

char* Game::plikDoChart(const char* nazwaPlik) {
	
	using namespace std;

	ifstream plik(nazwaPlik);
	if (plik.bad()) {
		cout << "Plik nie istnieje lub nie masz do niego praw!" << endl;
		exit(-1);
	}

	filebuf* pbuf = plik.rdbuf();
	long dlugosc = pbuf->pubseekoff(0, ios::end, ios::in);
	plik.seekg(0);

	char* wyjscie = new char[dlugosc + 1];
	plik.read(wyjscie, dlugosc);
	return wyjscie;
}

std::vector<Postac> Game::readXMLpostacie() {

	using namespace std;
	using namespace rapidxml;

	vector<Postac> vectorPostacie;
	string name;
	string sPostac;
	int id;

	const char * nazwa_pliku = "postacie.xml";
	auto_ptr <char> zawartosc_pliku(plikDoChart(nazwa_pliku));

	xml_document <> dokument;
	try {
		dokument.parse<0>(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}
	//cout << dokument;
	xml_node <> *postacie = dokument.first_node();

	for (xml_node <>* postac = postacie->first_node(); postac; postac = postac->next_sibling()) {
		xml_attribute<>* atrybut = postac->first_attribute();
		id = atoi(atrybut->value());
		xml_node <>* Name = postac->first_node();
		name = Name->value();
		xml_node <>* SPostac = Name->next_sibling();
		sPostac = SPostac->value();
		Postac temp(id, name, sPostac);
		vectorPostacie.push_back(temp);
	}
	return vectorPostacie;
}

std::vector<Poziom> Game::readXMLpoziomy() {

	using namespace std;
	using namespace rapidxml;

	vector<Poziom> vectorPoziomy;
	string name;
	string sPoziom;
	string sMap;
	string sTexture;
	int win;
	int id;

	const char * nazwa_pliku = "poziomy.xml";
	auto_ptr <char> zawartosc_pliku(plikDoChart(nazwa_pliku));

	xml_document <> dokument;
	try {
		dokument.parse<0>(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}
	//cout << dokument;
	xml_node <> *poziomy = dokument.first_node();

	for (xml_node <>* poziom = poziomy->first_node(); poziom; poziom = poziom->next_sibling()) {
		xml_attribute<>* atrybut = poziom->first_attribute();
		id = atoi(atrybut->value());
		xml_node <>* Name = poziom->first_node();
		name = Name->value();
		xml_node <>* SPoziom = Name->next_sibling();
		sPoziom = SPoziom->value();
		xml_node <>* SMap = SPoziom->next_sibling();
		sMap = SMap->value();
		xml_node <>* STexture = SMap->next_sibling();
		sTexture = STexture->value();
		xml_node <>* Swin = STexture->next_sibling();
		win = atoi(Swin->value());
		Poziom temp(id, name, sPoziom, sMap, sTexture, win);
		vectorPoziomy.push_back(temp);
	}
	return vectorPoziomy;
}

struct config Game::readXMLconfig() {

	using namespace std;
	using namespace rapidxml;

	config config;
	std::string modePhysics;

	const char * nazwa_pliku = "config.xml";
	auto_ptr <char> zawartosc_pliku(plikDoChart(nazwa_pliku));

	xml_document <> dokument;
	try {
		dokument.parse<0>(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}
	//cout << dokument;
	xml_node <> *settings = dokument.first_node();

//	for (xml_node <>* poziom = poziomy->first_node(); poziom; poziom = poziom->next_sibling()) {
		xml_node <>* set = settings->first_node();
		modePhysics = set->value();
//		xml_node <>* SPoziom = Name->next_sibling();
		if (modePhysics == "ON")
			config.modePhysics = true;
		else
			config.modePhysics = false;

	return config;
}

void Game::modifayXMLconfig(struct config* configurate) {

	using namespace std;
	using namespace rapidxml;

	const char * nazwa_pliku = "config.xml";
	auto_ptr <char> zawartosc_pliku(plikDoChart(nazwa_pliku));

	xml_document <> dokument;
	try {
		dokument.parse < parse_fastest >(zawartosc_pliku.get());
	}
	catch (parse_error p) {
		p.what();
	}
	//cout << dokument;
	xml_node <> * settings = dokument.first_node();
	xml_node <> * set = settings->first_node();
	if (configurate->modePhysics) 
		set->value("ON");
	else
		set->value("OFF");
	cout << dokument;

	std::ofstream myfile;
	myfile.open("config.xml");
	myfile << dokument;
	myfile.close();
	dokument.clear();
}

/////////////////REASUME////////////////////////////

void Game::windowResume(kodlak* character, Vector2 <float> cameraCenter, View* view) {


	float x = cameraCenter.x;
	float y = cameraCenter.y;

	sf::Font font;
	font.loadFromFile("LobsterTwo-Regular.otf");

	int opcja = 0;

	sf::Sprite background;                     //tło
	sf::Texture texture;
	texture.loadFromFile("otworcach.png");
	background.setTexture(texture);
	background.setPosition(x-(SKALAVIEW*12/2)*SKALA, y-(SKALAVIEW*8/2)*SKALA);
	background.setScale(((SKALAVIEW*SKALA * 0.6) / 50), ((SKALAVIEW*SKALA * 0.6) / 50));

	sf::Text tekst[3];                                                         //teksty
	std::string str[] = { "Resume", "Restart", "Menu" };
	for (int i = 0; i < 3; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize((SKALAVIEW*SKALA*40)/50);
		tekst[i].setFillColor(sf::Color::White);
		tekst[i].setString(str[i]);
		tekst[i].setPosition( x-((tekst[i].getGlobalBounds().width) / 2), y-2*SKALAVIEW*SKALA+(i *SKALAVIEW*SKALA));
	}

	sf::Sprite strzalka;                           //stzałki
	sf::Texture strzalkaTexture;
	strzalkaTexture.loadFromFile("strzalkaWhite.png");
	strzalka.setTexture(strzalkaTexture);
	strzalka.scale( ((SKALAVIEW*SKALA * 0.5) / 50), ((SKALAVIEW*SKALA * 0.5) / 50));
	strzalka.setPosition(x - ((tekst[0].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width -(SKALA*SKALAVIEW)/3, y - 2 * SKALA*SKALAVIEW + strzalka.getGlobalBounds().height/2);

	while (window.isOpen() && (state==RESUME))
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed /*|| (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))*/) {  //zamykanie okna, przez krzyżyk 
				state = END;
				character->kill();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    //wydarzenie naciśnięcie klawisza strzaałki w góre lub dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  //naciśnięty klawisz w dół
				{
					++opcja;
					opcja %= 3;
				}
				else
				{
					--opcja;
					if (opcja == -1)
						opcja = 2;
				}

				switch (opcja)
				{
				case 0: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 2 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				case 1: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 2 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				case 2: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 2 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))   //wydarzenie wciśnięcie entera
			{

				if (opcja == 2) {
					state = MENU2;   //zamykanie okna przez klawisz
					character->kill();
				}

				if (opcja == 1) {
					character->kill();
					state = GAME;	

				}

				if (opcja == 0) {
					state = GAME;
				}
			}
		}
		window.clear();

		window.setView(*view);
		window.draw(background);
		for (int i = 0; i < 3; i++) {
			window.draw(tekst[i]);
		}
		window.draw(strzalka);
		window.display();
	}
}

////////////////GAME OVER////////////////////////////

bool Game::gameOver(kodlak* character, map* carte) {
	bool minus;
	if (character->y() > (carte->y() + 10)*SKALA) {
		character->modifeLifeAndPosition(10);
		 minus = true;
		if (!character->alive)
			state = GAME_OVER;
	}
	else {
		 minus = false;
	}
	return minus;
}

void Game::windowGameOver(kodlak* character, Vector2 <float> cameraCenter, View* view) {


	float x = cameraCenter.x;
	float y = cameraCenter.y;

	sf::Font font;
	font.loadFromFile("LobsterTwo-Regular.otf");

	int opcja = 0;

	sf::Sprite background;                     //tło
	sf::Texture texture;
	texture.loadFromFile("otworcach.png");
	background.setTexture(texture);
	background.setPosition(x - (SKALAVIEW * 12 / 2)*SKALA, y - (SKALAVIEW * 8 / 2)*SKALA);
	background.setScale(((SKALAVIEW*SKALA * 0.6) / 50), ((SKALAVIEW*SKALA * 0.6) / 50));

	sf::Text tekstGameOver;
	tekstGameOver.setFont(font);
	tekstGameOver.setCharacterSize((SKALAVIEW*SKALA * 60) / 50);
	tekstGameOver.setFillColor(sf::Color::White);
	tekstGameOver.setString("GAME OVER");
	tekstGameOver.setPosition(x - ((tekstGameOver.getGlobalBounds().width) / 2), y - 3 * SKALAVIEW*SKALA);

	sf::Text tekst[2];                                                         //teksty
	std::string str[] = { "Restart", "Menu" };
	for (int i = 0; i < 2; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize((SKALAVIEW*SKALA * 40) / 50);
		tekst[i].setFillColor(sf::Color::White);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(x - ((tekst[i].getGlobalBounds().width) / 2), y - 1 * SKALAVIEW*SKALA + (i *SKALAVIEW*SKALA));
	}

	sf::Sprite strzalka;                           //stzałki
	sf::Texture strzalkaTexture;
	strzalkaTexture.loadFromFile("strzalkaWhite.png");
	strzalka.setTexture(strzalkaTexture);
	strzalka.scale(((SKALAVIEW*SKALA * 0.5) / 50), ((SKALAVIEW*SKALA * 0.5) / 50));
	strzalka.setPosition(x - ((tekst[0].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + strzalka.getGlobalBounds().height / 2);

	while (window.isOpen() && (state == GAME_OVER))
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed /*|| (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))*/) {  //zamykanie okna, przez krzyżyk 
				state = END;
				character->kill();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    //wydarzenie naciśnięcie klawisza strzaałki w góre lub dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  //naciśnięty klawisz w dół
				{
					++opcja;
					opcja %= 2;
				}
				else
				{
					--opcja;
					if (opcja == -1)
						opcja = 1;
				}

				switch (opcja)
				{
				case 0: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				case 1: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))   //wydarzenie wciśnięcie entera
			{

				if (opcja == 1) {
					state = MENU2;   //zamykanie okna przez klawisz
				}

				if (opcja == 0) {
					state = GAME;

				}

				
			}
		}
		window.clear();

		window.setView(*view);
		window.draw(background);
		window.draw(tekstGameOver);
		for (int i = 0; i < 2; i++) {
			window.draw(tekst[i]);
		}
		window.draw(strzalka);
		window.display();
	}
}

////////////////WIN//////////////////////////////////

void Game::win(kodlak* character, map* carte) {

	if (character->x() > (parwin*SKALA)) {
		state = WIN;
		character->success();
	}
}

void Game::windowWin(kodlak* character, Vector2 <float> cameraCenter, View* view) {


	float x = cameraCenter.x;
	float y = cameraCenter.y;

	sf::Font font;
	font.loadFromFile("LobsterTwo-Regular.otf");

	int opcja = 0;

	sf::Sprite background;                     //tło
	sf::Texture texture;
	texture.loadFromFile("otworcach.png");
	background.setTexture(texture);
	background.setPosition(x - (SKALAVIEW * 12 / 2)*SKALA, y - (SKALAVIEW * 8 / 2)*SKALA);
	background.setScale(((SKALAVIEW*SKALA * 0.6) / 50), ((SKALAVIEW*SKALA * 0.6) / 50));

	sf::Text tekstGameOver;
	tekstGameOver.setFont(font);
	tekstGameOver.setCharacterSize((SKALAVIEW*SKALA * 60) / 50);
	tekstGameOver.setFillColor(sf::Color::White);
	tekstGameOver.setString("WIN");
	tekstGameOver.setPosition(x - ((tekstGameOver.getGlobalBounds().width) / 2), y - 3 * SKALAVIEW*SKALA);

	sf::Text tekst[2];                                                         //teksty
	std::string str[] = { "Restart", "Menu" };
	for (int i = 0; i < 2; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize((SKALAVIEW*SKALA * 40) / 50);
		tekst[i].setFillColor(sf::Color::White);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(x - ((tekst[i].getGlobalBounds().width) / 2), y - 1 * SKALAVIEW*SKALA + (i *SKALAVIEW*SKALA));
	}

	sf::Sprite strzalka;                           //stzałki
	sf::Texture strzalkaTexture;
	strzalkaTexture.loadFromFile("strzalkaWhite.png");
	strzalka.setTexture(strzalkaTexture);
	strzalka.scale(((SKALAVIEW*SKALA * 0.5) / 50), ((SKALAVIEW*SKALA * 0.5) / 50));
	strzalka.setPosition(x - ((tekst[0].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + strzalka.getGlobalBounds().height / 2);

	while (window.isOpen() && (state == WIN))
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed /*|| (event.type == sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))*/) {  //zamykanie okna, przez krzyżyk 
				state = END;
				character->kill();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    //wydarzenie naciśnięcie klawisza strzaałki w góre lub dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  //naciśnięty klawisz w dół
				{
					++opcja;
					opcja %= 2;
				}
				else
				{
					--opcja;
					if (opcja == -1)
						opcja = 1;
				}

				switch (opcja)
				{
				case 0: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				case 1: strzalka.setPosition(x - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - (SKALA*SKALAVIEW) / 3, y - 1 * SKALA*SKALAVIEW + (opcja * SKALA*SKALAVIEW) + strzalka.getGlobalBounds().height / 2);
					break;
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))   //wydarzenie wciśnięcie entera
			{

				if (opcja == 1) {
					state = MENU2;   //zamykanie okna przez klawisz
				}

				if (opcja == 0) {
					state = GAME;

				}


			}
		}
		window.clear();

		window.setView(*view);
		window.draw(background);
		window.draw(tekstGameOver);
		for (int i = 0; i < 2; i++) {
			window.draw(tekst[i]);
		}
		window.draw(strzalka);
		window.display();
	}
}

//////////////// MENU 1 /////////////////////////////

void Game::menu1()
{
	sf::Font font;
	font.loadFromFile("LobsterTwo-Regular.otf");

	int opcja = 0;

	sf::Sprite background;                     //tło
	sf::Texture texture;
	texture.loadFromFile("background6.jpg");
	background.setTexture(texture);
	background.scale(1.1f, 1.1f);

	sf::Text tekst[4];                                                         //teksty
	std::string str[] = { "Start", "Settings", "Authors", "Exit" };
	for (int i = 0; i < 4; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(46);
		tekst[i].setFillColor(sf::Color::Black);
		tekst[i].setString(str[i]);
		tekst[i].setPosition(480 - ((tekst[i].getGlobalBounds().width) / 2) , 210 + (i * 45));
	}

	sf::Text otworcach;                                //tekst o twórcach
	otworcach.setFont(font);
	otworcach.setCharacterSize(30);
	otworcach.setString("Gra zrealizowana w ramach projektu rekrutacyjnego\n do Kola Naukowego Informatyków\n na Politechnice Warszawskiej.\n\t\t\t Sklad:\n\t\tPM: Michal Boruta\n\t\t\tZespól:\n\t\tDaniel Petrykowski\n\t\tPiotr Slebzak\n\t\tLukasz Zaparucha");
	otworcach.setPosition(200, 100);

	sf::Sprite strzalka;                           //stzałki
	sf::Texture strzalkaTexture;
	strzalkaTexture.loadFromFile("strzalka.png");
	strzalka.setTexture(strzalkaTexture);
	strzalka.scale(0.5f, 0.5f);
	strzalka.setPosition(480 - ((tekst[0].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width-20, 210+strzalka.getGlobalBounds().height/2);

	sf::Sprite tlo;                                 //tło (o twórcach)
	sf::Texture tloTexture;
	tloTexture.loadFromFile("otworcach.png");
	tlo.setTexture(tloTexture);
	tlo.setPosition(0, -100);

	while (state == MENU1 && window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || (event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape))   //zamykanie okna, przez krzyżyk 
				state = END;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    //wydarzenie naciśnięcie klawisza strzaałki w góre lub dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  //naciśnięty klawisz w dół
				{
					++opcja;
					opcja %= 4;
				}
				else
				{
					--opcja;
					if (opcja == -1)
						opcja = 3;
				}

				switch (opcja)
				{
				case 0: strzalka.setPosition(480 - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - 20, 210 + (opcja * 45)+ strzalka.getGlobalBounds().height / 2);
					break;
				case 1: strzalka.setPosition(480 - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - 20, 210 + (opcja * 45) + strzalka.getGlobalBounds().height / 2);
					break;
				case 2: strzalka.setPosition(480 - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - 20, 210 + (opcja * 45) + strzalka.getGlobalBounds().height / 2);
					break;
				case 3: strzalka.setPosition(480 - ((tekst[opcja].getGlobalBounds().width) / 2) - strzalka.getGlobalBounds().width - 20, 210 + (opcja * 45) + strzalka.getGlobalBounds().height / 2);
					break;
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))   //wydarzenie wciśnięcie entera
			{

				if (opcja == 3) {
					state = END;   //zamykanie okna przez klawisz
				}

				if (opcja == 2) {
					window.draw(tlo);
					window.draw(otworcach);
					window.display();
					Sleep(500);   //zmienic na zależnosc od wcisnietego klawisza

					do {
					} while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return));  //Nie ma dowolnego znaku w SFML'u ????
					Sleep(500);
				}

				if (opcja == 1) {
					state = SETTINGS;
				}

				if (opcja == 0) {
					state = MENU2;   // otworzenie okna menu gry 

				}
			}
		}

		window.clear();
		window.draw(background);
		window.draw(strzalka);
		for (int i = 0; i < 4; i++) {
			window.draw(tekst[i]);
		}
		window.display();
	}
}

//////////////// MENU 2 /////////////////////////////

void Game::menu2()
{
	sf::Font font;						//czcionka
	font.loadFromFile("LobsterTwo-Regular.otf");

	std::vector<Postac> postacie = Game::readXMLpostacie();
	std::vector<Poziom> poziomy = Game::readXMLpoziomy();

	Postac postacAktywna(postacie[0]);
	Poziom poziomAktywny(poziomy[0]);

	sf::Sprite poziom;					//grafika tła
	sf::Texture texture;
	texture.loadFromFile(poziomAktywny.getsPoziom());
	poziom.setTexture(texture);
	poziom.setScale(1, 1);

	sf::Sprite postac;					//grafika postaci
	sf::Texture postacTexture;
	postacTexture.loadFromFile(postacAktywna.getsPostac());
	postac.setTexture(postacTexture);
	postac.setScale(0.55, 0.55);
	postac.setPosition(-50, 0);

	sf::Text tekst[4];					//tekst
	std::string str[] = { "Selection character", "Selection level", "Start", "Back" };
	for (int i = 0; i < 4; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(48);
		//tekst[i].setColor(sf::Color::Black);
		tekst[i].setString(str[i]);
	}
	tekst[0].setPosition(350, 160);
	tekst[1].setPosition(400, 270);
	tekst[2].setPosition(475, 380);
	tekst[3].setPosition(90, 470);

	sf::Text postacName;				//tekst nazwa postaci
	postacName.setFont(font);
	postacName.setCharacterSize(48);
	postacName.setString(postacAktywna.getName());
	postacName.setPosition(350 + 166 - (postacName.getGlobalBounds().width / 2), 210);

	sf::Text poziomName;				//tekst nazwa poziomu
	poziomName.setFont(font);
	poziomName.setCharacterSize(48);
	poziomName.setString(poziomAktywny.getName());
	poziomName.setPosition(350 + 166 - (poziomName.getGlobalBounds().width / 2), 320);

	sf::Sprite strzalkaL;				//grafika strzałki lewej
	sf::Texture strzalkaLTexture;
	strzalkaLTexture.loadFromFile("strzalkal.png");
	strzalkaL.setTexture(strzalkaLTexture);
	strzalkaL.setScale(0.5, 0.5);
	strzalkaL.setPosition(350 + 166 - (postacName.getGlobalBounds().width / 2) - 80, 235);

	sf::Sprite strzalkaR;				//grafika strzałki prawej
	sf::Texture strzalkaRTexture;
	strzalkaRTexture.loadFromFile("strzalka.png");
	strzalkaR.setTexture(strzalkaRTexture);
	strzalkaR.setScale(0.6, 0.5);
	strzalkaR.setPosition(350 + 166 + (postacName.getGlobalBounds().width / 2) + 10, 235);


	int opcja = 0;
	int idPostaci = 0;
	int idPoziomu = 0;
	int liczbaPostaci = postacie.size();
	int liczbaPoziomow = poziomy.size();

	while (state == MENU2)
	{


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || (event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape))   //zamkniecie okna przez krzyżyk
				state = END;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   //naciśnięcie strzałki w górę bądź dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   //naciśnięcie strzałki w dół
				{
					++opcja;
					opcja %= 4;
				}
				else   //jeżeli strzałka  górę
				{
					--opcja;
					if (opcja == -1)
						opcja = 3;
				}

				switch (opcja)
				{
				case 0: {strzalkaL.setPosition(350 + 166 - (postacName.getGlobalBounds().width / 2) - 80, 235);
					strzalkaR.setPosition(350 + 166 + (postacName.getGlobalBounds().width / 2) + 10, 235); }
						break;
				case 1: {strzalkaL.setPosition(350 + 166 - (poziomName.getGlobalBounds().width / 2) - 80, 345);
					strzalkaR.setPosition(350 + 166 + (poziomName.getGlobalBounds().width / 2) + 10, 345); }
						break;
				case 2: {strzalkaR.setPosition(365, 400);
					strzalkaL.setPosition(595, 400); }
						break;
				case 3: {strzalkaR.setPosition(5, 485);
					strzalkaL.setPosition(220, 485); }
						break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    //naciśnięcie strzałki w lewo lub w prawo 
			{
				if (opcja == 0) {   //wybor postaci
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  //naciśnięcie strzałki w lewo
					{
						--idPostaci;
						if (idPostaci == -1)
							idPostaci = (liczbaPostaci - 1);
					}
					else   //naciśnięcie strzałki w prawo
					{
						++idPostaci;
						idPostaci %= (liczbaPostaci);
					}
					
					postacAktywna = postacie[idPostaci];   //ustawienia tła wzależności od wybranej postaci
					
					postacTexture.loadFromFile(postacAktywna.getsPostac());
					postacName.setString(postacAktywna.getName());
					postacName.setPosition(350 + 166 - (postacName.getGlobalBounds().width / 2), 210);
					{strzalkaL.setPosition(350 + 166 - (postacName.getGlobalBounds().width / 2) - 80, 235);
					strzalkaR.setPosition(350 + 166 + (postacName.getGlobalBounds().width / 2) + 10, 235); }
				}
				if (opcja == 1) { //wybor poziomu
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))   //nacisniecie strzałki w lewo
					{
						--idPoziomu;
						if (idPoziomu == -1)
							idPoziomu = (liczbaPoziomow - 1);
					}
					else  //naciśniecie strzalki w prawo
					{
						++idPoziomu;
						idPoziomu %= (liczbaPoziomow);
					}

					poziomAktywny = poziomy[idPoziomu];    //ustawienia poziomu w zaleznosci od wybranego poziomu
					
					texture.loadFromFile(poziomAktywny.getsPoziom());
					poziomName.setString(poziomAktywny.getName());
					poziomName.setPosition(350 + 166 - (poziomName.getGlobalBounds().width / 2), 320);
					{strzalkaL.setPosition(350 + 166 - (poziomName.getGlobalBounds().width / 2) - 80, 345);
					strzalkaR.setPosition(350 + 166 + (poziomName.getGlobalBounds().width / 2) + 10, 345); }
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))  //nacisniecia klawisza enter
			{
				
				if (opcja == 3) {  //wybor "powrotu"
					state = MENU1;

				}
				if (opcja == 2) {   //wybor "start" 
					state = GAME;
					sMap = poziomAktywny.getsMap();
					sTexture = poziomAktywny.getsTexture();
					parwin = poziomAktywny.getWin();
				}
			}
		}


		window.clear();
		window.draw(poziom);
		window.draw(postac);
		for (int i = 0; i < 4; i++) {
			window.draw(tekst[i]);
		}
		window.draw(postacName);
		window.draw(poziomName);
		window.draw(strzalkaL);
		window.draw(strzalkaR);
		window.display();
	}
}

////////////////SETTINGS/////////////////////////////

void Game::settings()
{
	sf::Font font;						//czcionka
	font.loadFromFile("LobsterTwo-Regular.otf");

	int opcja = 0;
	int idPhysicsMode;

	sf::Sprite background;					//grafika tła
	sf::Texture texture;
	texture.loadFromFile("otworcach.png");
	background.setTexture(texture);
	background.setScale(1, 1);

	sf::Text tekst[3];					//tekst
	std::string str[] = { "Settings", "Physics Mode", "Back" };
	for (int i = 0; i < 3; i++) {
		tekst[i].setFont(font);
		tekst[i].setCharacterSize(48);
		tekst[i].setFillColor(sf::Color::White);
		tekst[i].setString(str[i]);
	}
	tekst[0].setPosition(500 - (tekst[0].getGlobalBounds().width / 2), 80);
	tekst[1].setPosition(500 - (tekst[1].getGlobalBounds().width / 2), 210);
	tekst[2].setPosition(150 - (tekst[2].getGlobalBounds().width / 2), 460);
	

	sf::Text PhysicsMode;				//tekst wyboru trybu ON/OFF
	PhysicsMode.setFont(font);
	PhysicsMode.setCharacterSize(48);
	if (configurate.modePhysics) {
		PhysicsMode.setString("ON");
		idPhysicsMode = 1;
	}
	else {
		PhysicsMode.setString("OFF");
		idPhysicsMode = 0;
	}
	PhysicsMode.setPosition(500 - (PhysicsMode.getGlobalBounds().width / 2), 280);

	
	sf::Sprite strzalkaL;				//grafika strzałki lewej
	sf::Texture strzalkaLTexture;
	strzalkaLTexture.loadFromFile("strzalkal.png");
	strzalkaL.setTexture(strzalkaLTexture);
	strzalkaL.setScale(0.5, 0.5);
	strzalkaL.setPosition(500 - (PhysicsMode.getGlobalBounds().width / 2) - 80, 295);

	sf::Sprite strzalkaR;				//grafika strzałki prawej
	sf::Texture strzalkaRTexture;
	strzalkaRTexture.loadFromFile("strzalka.png");
	strzalkaR.setTexture(strzalkaRTexture);
	strzalkaR.setScale(0.6, 0.5);
	strzalkaR.setPosition(500 + (PhysicsMode.getGlobalBounds().width / 2) + 10, 295);

	while (state == SETTINGS)
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || (event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape))   //zamkniecie okna przez krzyżyk
				state = END;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))   //naciśnięcie strzałki w górę bądź dół
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   //naciśnięcie strzałki w dół
				{
					++opcja;
					opcja %= 2;
				}
				else   //jeżeli strzałka  górę
				{
					--opcja;
					if (opcja == -1)
						opcja = 1;
				}

				switch (opcja)
				{
				case 0: {strzalkaL.setPosition(500 - (PhysicsMode.getGlobalBounds().width / 2) - 80, 295);
					strzalkaR.setPosition(500 + (PhysicsMode.getGlobalBounds().width / 2) + 10, 295); }
						break;
				case 1: {strzalkaR.setPosition(5, 485);
					strzalkaL.setPosition(220, 485); }
						break;
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))    //naciśnięcie strzałki w lewo lub w prawo 
			{
				if (opcja == 0) {   //wybor postaci
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  //naciśnięcie strzałki w lewo
					{
						--idPhysicsMode;
						if (idPhysicsMode == -1)
							idPhysicsMode = (1);
					}
					else   //naciśnięcie strzałki w prawo
					{
						++idPhysicsMode;
						idPhysicsMode %= (2);
					}

					if (idPhysicsMode == 1) {
						configurate.modePhysics = true;
						PhysicsMode.setString("ON");
					}
					else {
						configurate.modePhysics = false;
						PhysicsMode.setString("OFF");
					}

					PhysicsMode.setPosition(500 - (PhysicsMode.getGlobalBounds().width / 2), 280);
					{strzalkaL.setPosition(500 - (PhysicsMode.getGlobalBounds().width / 2) - 80, 295);
					strzalkaR.setPosition(500 + (PhysicsMode.getGlobalBounds().width / 2) + 10, 295); }
				}
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))  //nacisniecia klawisza enter
			{
				if (opcja == 1) {  //wybor "powrotu"
					state = MENU1;
					modifayXMLconfig(&configurate);
				}
			}
		}


		window.clear();
		window.draw(background);
		for (int i = 0; i < 3; i++) {
			window.draw(tekst[i]);
		}
		window.draw(PhysicsMode);
		window.draw(strzalkaL);
		window.draw(strzalkaR);
		window.display();
	}
}

/////////////////PositionPast///////////////////////

void Game::positionPast(clock_t czas, kodlak* character) {
	if (czas > timeSavePosition) {
		character->savePositionPast();
		timeSavePosition = clock() + TIME / 10 * CLOCKS_PER_SEC;
	}
	
}

///////////////////// GRA //////////////////////////

void Game::gameStart()
{

	kodlak character(SKALA);
	map carte = map(1.f*SKALA, sMap, sTexture);

	//body = placeholder of character
	sf::RectangleShape body(sf::Vector2f(character.Getszerokosc(), character.Getwysokosc()));
	body.setFillColor(sf::Color(100, 250, 50));

	//arm = temporary arm for testing
	sf::RectangleShape arm(sf::Vector2f(2, 0.1));
	arm.setFillColor(sf::Color(0, 255, 255));

	//drawing level based on the map
	sf::Texture texture1;                         //textury deklarowane co do poziomu
	texture1.loadFromFile("texture1.2.jpg");
	sf::Texture texture2;                         //textury deklarowane co do poziomu
	texture2.loadFromFile("texture2.2.jpg");
	sf::Texture texture3;                         //textury deklarowane co do poziomu
	texture3.loadFromFile("texture3.2.jpg");

	std::vector<sf::RectangleShape> level = carte.draw();
	std::vector<sf::RectangleShape> levelTextures = carte.loadTextures(&texture1, &texture2, &texture3);

	//panel
	sf::Sprite panel1;                                
	sf::Texture panel1Texture;
	panel1Texture.loadFromFile("panel1.png");
	panel1.setTexture(panel1Texture);
	panel1.setScale(0.35, 0.35);

	sf::Sprite bpanel1;
	sf::Texture bpanel1Texture;
	bpanel1Texture.loadFromFile("bpanel1.png");
	bpanel1.setTexture(bpanel1Texture);
	bpanel1.setScale(0.35, 0.35);

	//life bar
	sf::RectangleShape barlife(sf::Vector2f(246,25));
	barlife.setFillColor(sf::Color(0, 128, 255));


	//adding camera view
	sf::View view;
	view.setSize(SKALAVIEW*12*SKALA, SKALAVIEW*8*SKALA);

	//movement of character on separate thread
	//std::thread(&move, &character, &carte).detach();


	while (window.isOpen() && state == GAME)
	{
		character.initialValue();
		/*time_t start;
		time_t koniec;*/
		barlife.setSize(sf::Vector2f(246, 25));

		while (character.alive && (!character.win))
		{

			sf::Event event;

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					character.kill();
					state = END;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
				{
					sf::Vector2 <float> cameraCenter = view.getCenter();
					state = RESUME;
					windowResume(&character, cameraCenter, &view);
				}
			}

				move(&character, &carte, SKALA, &configurate.modePhysics);
				body.setPosition(character.x(), character.y());
				arm.setPosition(character.x(), character.y());
				arm.setRotation(calculateRotation(&character, &window));
				//std::cout << calculateRotation(&character, &window) << std::endl;
				window.clear();
				updateCam(&view, &body, &window);

				sf::Vector2 <float> cameraCenter = view.getCenter();
				int x = carte.binaryPositionSearch(cameraCenter.x, 0, carte.x() - 1);
				int y = carte.binaryPositionSearch(cameraCenter.y, 0, carte.y() - 1);
				int beginX = x - SKALAVIEW * 12 / 2 - 2;
				if (beginX < 0)
					beginX = 0;
				int beginY = y - SKALAVIEW * 8 / 2 - 2;
				if (beginY < 0)
					beginY = 0;
				int endX = x + SKALAVIEW * 12 / 2 + 2;
				if (endX > carte.x())
					endX = carte.x();
				int endY = y + SKALAVIEW * 8 / 2 + 2;
				if (endY > carte.y())
					endY = carte.y();

				panel1.setPosition(cameraCenter.x - (SKALA*SKALAVIEW * 12 / 2), cameraCenter.y - SKALA*SKALAVIEW * 8 / 2);
				bpanel1.setPosition(cameraCenter.x - (SKALA*SKALAVIEW * 12 / 2), cameraCenter.y - SKALA*SKALAVIEW * 8 / 2);
				barlife.setPosition(cameraCenter.x - (SKALA*SKALAVIEW * 5.09 / 2), cameraCenter.y - SKALA*SKALAVIEW * 7.6 / 2);

				for (int i = beginY*carte.x(); i < endY*carte.x(); i=i+carte.x()) {
					for (int j = beginX; j < endX; j++)
					{
						window.draw(levelTextures[i+j]);
					}
				}



				for (int i = 0; i < level.size(); i++)  //jezeli będzie zbyt wolno działał program zmienić na rysowanie tylko fragmentu który jest widziany
				{
					window.draw(level[i]);
				}

				window.setView(view);
				window.draw(body);
				window.draw(arm);
				window.draw(bpanel1);
				window.draw(barlife);
				window.draw(panel1);
				window.display();

				if (gameOver(&character, &carte)) {
					int a =(256 * character.getLife()/100);
					barlife.setSize(sf::Vector2f(a, 25));
				}
				win(&character, &carte);
				positionPast(clock(), &character);                                    //zapis pozycji
			
		}
		if (state == GAME_OVER) {//GRA W PRZYPADKU ZABICIA 
			sf::Vector2 <float> cameraCenter = view.getCenter();
			windowGameOver(&character, cameraCenter, &view);
		}
		
		if (state == WIN) {
			sf::Vector2 <float> cameraCenter = view.getCenter();
			windowWin(&character, cameraCenter, &view);
		}

	}
	view.setSize(960, 540);
	view.setCenter(480, 270);
	window.setView(view);
}
