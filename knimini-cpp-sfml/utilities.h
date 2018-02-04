#pragma once
#include "stdfax.h"

class map
{
private:
    float cellSize;
    int X, Y;

    //mapDetails: +1 - bottom; +2 - top; +4 - left; +8 - right
	int mapDetails[50][500];
	//wczytuje mape
	//void loadMap(std::string filePath);
	int textureDetails[50][500];
	// jakaTextura {niebo, ziemia};
	int dlugosc;
	int wysokosc;

public:

    //creates map (cell size, sMap, sTexture)
    map(float, std::string, std::string);

	void loadMap(std::string, std::string);

    //draw map based on mapDetails values
    std::vector<sf::RectangleShape> draw();

    //searches for character position in map (character position, begining, end); returns cell row/column
    int binaryPositionSearch(float, int, int);

    //returns value of cell (x, y)
    int cellV(int, int);

	//
	std::vector<sf::RectangleShape> loadTextures(sf::Texture*, sf::Texture*, sf::Texture*);

    float size();
    int x();
    int y();
};

//updates position of camera
void updateCam(sf::View*, sf::Shape*, sf::RenderWindow*);
