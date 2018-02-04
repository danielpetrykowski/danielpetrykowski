#include <SFML/Graphics.hpp>
#include "KlasyPostaci.h"
#include "stdfax.h"
#include "player.h"
#include "utilities.h"

#define SKALA 50.f //50 
#define SKALAVIEW 1.5f   //1.5f
#define TIME 3.f

using namespace sf;

struct config
	{
		bool modePhysics;
	};

class Game
{
public:
	Game();
	~Game();
	void runGame();

protected:
	enum GameState {MENU1, MENU2, GAME,GAME_OVER,END, RESUME, WIN, SETTINGS};
	GameState state;
	int parwin;
	std::string sMap;
	std::string sTexture;
	config configurate;
	clock_t timeSavePosition;

private:

	char* plikDoChart(const char*);
	std::vector<Postac> readXMLpostacie();
	std::vector<Poziom> readXMLpoziomy();
	config readXMLconfig();
	void modifayXMLconfig(struct config*);
	bool gameOver(kodlak*, map*);
	void win(kodlak*, map*);
	void windowResume(kodlak*, Vector2 <float>, View*);
	void windowGameOver(kodlak*, Vector2 <float>, View*);
	void windowWin(kodlak*, Vector2 <float>, View*);
	void menu1();
	void menu2();
	void settings();
	void gameStart();
	void positionPast(clock_t, kodlak*);
};
