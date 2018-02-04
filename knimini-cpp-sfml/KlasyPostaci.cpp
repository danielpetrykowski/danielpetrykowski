#include "KlasyPostaci.h"
#include <SFML/Graphics.hpp>
#include <windows.h>

///////////// POSTAC ////////////////////////////

Postac::Postac(int a, std::string b, std::string c)
{
	id = a;
	Name = b;
	sPostac = c; 
}

Postac::Postac() {
	sPostac = "";
	Name = "";
	id = 0;
}

Postac::~Postac()
{
};

std::string Postac::getsPostac()
{
    return sPostac;
}

std::string Postac::getName()
{
    return Name;
}


//////////////// POZIOM /////////////////////////////


Poziom::Poziom(int a, std::string b, std::string c, std::string d, std::string e, int f)
{
	int id = a;
    Name = b;
    sPoziom = c;
	sMap = d;
	sTexture = e;
	win = f;
}

Poziom::Poziom() {
	sPoziom = "";
	Name = "";
	id = 0;
	sMap = "";
	sTexture = "";
	win = 0;
}

Poziom::~Poziom()
{
};

std::string Poziom::getsPoziom()
{
    return sPoziom;
}
std::string Poziom::getName()
{
    return Name;
}

std::string Poziom::getsMap()
{
	return sMap;
}

std::string Poziom::getsTexture() 
{
	return sTexture;
}

int Poziom::getWin() {
	return win;
}

