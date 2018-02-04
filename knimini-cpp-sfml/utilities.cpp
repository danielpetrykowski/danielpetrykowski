#include "utilities.h"

map::map(float size, std::string mapa, std::string texture)
{
	cellSize = size;
	loadMap(mapa, texture);
}

void map::loadMap(std::string sMap, std::string sTexture) //void map::loadMap(std::string filePath)
{

	//--------------------Mapa (krawêdzie) oraz rozmiar X i Y-----------------------------------------------------
	{
		int i = 0;
		int j = 0;

		int8_t doSzufladki = 0;
		std::string temp = "";
		char pobranyZnak;
		bool czySpac = true;

		std::string filePath = sMap;
		std::fstream fstr(filePath);

		while ((pobranyZnak = fstr.get()) != EOF)
		{
			if ((pobranyZnak == ' ') || (pobranyZnak == '\n') || (pobranyZnak == '\r') || (pobranyZnak == '\t') || (pobranyZnak == 'x'))
			{
				if (!czySpac)
				{
					doSzufladki = atoi(temp.c_str());
					mapDetails[i][j] = doSzufladki;
					j++;
				}
				if ((pobranyZnak == '\n')) {
					i++;
					X = j;
					j = 0;
				}
				czySpac = true;
				temp = "";
			}
			else
			{
				czySpac = false;
				temp += pobranyZnak;
			}
		}
		Y = i + 1;
		fstr.close();
	}

	//-------------------Tekstury------------------------------------------------------------------
	{
		int i = 0;
		int j = 0;

		int8_t doSzufladki = 0;
		std::string pom = "";
		char pobranyZnak;
		bool czySpac = true;

		std::string filePath = sTexture;
		std::fstream fstr(filePath);

		while ((pobranyZnak = fstr.get()) != EOF)
		{
			if ((pobranyZnak == ' ') || (pobranyZnak == '\n') || (pobranyZnak == '\r') || (pobranyZnak == '\t') || (pobranyZnak == 'x'))
			{
				if (!czySpac) {
					doSzufladki = atoi(pom.c_str());
					textureDetails[i][j] = doSzufladki;
					j++;
				}

				if ((pobranyZnak == '\n')) {
					i++;
					j = 0;
				}


				czySpac = true;
				pom = "";
			}
			else
			{
				czySpac = false;
				pom += pobranyZnak;
			}
		}
		fstr.close();
	}
}

int map::cellV(int first, int second)
{
	return mapDetails[first][second];
}

int map::binaryPositionSearch(float pos, int beg, int end)
{
	if (end - beg < 2)
	{
		if (beg == end || pos < end*cellSize)
			return beg;
		else
			return end;
	}
	int mid = (beg + end) / 2;
	if (pos < mid*cellSize)
		return binaryPositionSearch(pos, beg, mid);
	else if (pos > (mid + 1)*cellSize)
		return binaryPositionSearch(pos, mid, end);
	else
		return mid;
}

std::vector<sf::RectangleShape> map::draw()  //tutaj g³ówna moja robota i w sumie yolo
{
	std::vector<sf::RectangleShape> arr;
	sf::RectangleShape temp;
	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{
			int val = mapDetails[i][j];

			//right wall
			if (val > 8)
			{
				val -= 8;
				temp = sf::RectangleShape(sf::Vector2f(0.1f*cellSize / 2, cellSize));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*(j + 1), cellSize*i);
				arr.push_back(temp);
			}

			else if (val == 8)
			{
				val -= 8;
				temp = sf::RectangleShape(sf::Vector2f(0.1f*cellSize / 2, cellSize));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*(j + 1), cellSize*i);
				arr.push_back(temp);
				continue;
			}

			//left wall
			if (val > 4)
			{
				val -= 4;
				temp = sf::RectangleShape(sf::Vector2f(0.1f*cellSize / 2, cellSize));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*j, cellSize*i);
				arr.push_back(temp);
			}
			else if (val == 4)
			{
				val -= 4;
				temp = sf::RectangleShape(sf::Vector2f(0.1f*cellSize / 2, cellSize));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*j, cellSize*i);
				arr.push_back(temp);
				continue;
			}

			//ceiling
			if (val > 2)
			{
				val -= 2;
				temp = sf::RectangleShape(sf::Vector2f(cellSize, 0.1f*cellSize / 2));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*j, cellSize*i);
				arr.push_back(temp);
			}
			else if (val == 2)
			{
				val -= 2;
				temp = sf::RectangleShape(sf::Vector2f(cellSize, 0.1f*cellSize / 2));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*j, cellSize*i);
				arr.push_back(temp);
				continue;
			}

			//floor
			if (val == 1)
			{
				val -= 1;
				temp = sf::RectangleShape(sf::Vector2f(cellSize, 0.1f*cellSize / 2));
				temp.setFillColor(sf::Color(255, 255, 255));
				temp.setPosition(cellSize*j, cellSize*(i + 1));
				arr.push_back(temp);
			}
		}
	}
	return arr;
}

std::vector<sf::RectangleShape> map::loadTextures(sf::Texture* texture1, sf::Texture* texture2, sf::Texture* texture3)
{
	std::vector<sf::RectangleShape> arr;
	sf::RectangleShape temp;

	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{
			int t = textureDetails[i][j];

			//albo podloga albo niebo
			temp = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
			temp.setPosition(cellSize*j, cellSize*i);

			switch (t)
			{
			case 0:
				temp.setFillColor(sf::Color(135, 206, 250));	//niebo b³êkit
			break;
			case 1:
				temp.setTexture(texture1);
			break;
			case 2:
				temp.setTexture(texture2);
			break;
			case 3:
				temp.setFillColor(sf::Color(26, 26, 66));   //niebo ksiêzyc (granat)
				break;
			case 4:
				temp.setTexture(texture3);
			break;
			case 5:
				temp.setFillColor(sf::Color(163, 37, 11));   //niebo mars (piaskowaty)
				break;
			}
			arr.push_back(temp);
		}
	}
	return arr;
}

float map::size()
{
	return cellSize;
}
int map::x()
{
	return X;
}
int map::y()
{
	return Y;
}

void updateCam(sf::View* cam, sf::Shape* pl, sf::RenderWindow* window)
{
	cam->setCenter(pl->getPosition()/* + static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)) / 20.f*/);
}
