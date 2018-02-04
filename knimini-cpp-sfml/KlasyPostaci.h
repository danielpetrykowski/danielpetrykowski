#include <SFML/Graphics.hpp>

class Postac 
{
public:
    Postac(int, std::string, std::string); 
	Postac();
    ~Postac();
    std::string getsPostac();    //zwraca scieszke do pliku z obrazkiem postaci w stringu
    std::string getName();    //zwraca scieszke do pliku z obrazkiem nazwy postaci w stringu

private:
	int id;
    std::string sPostac;   //scieszka do pliku z obrazkiem postaci w stringu
    std::string Name;		//scieszka do pliku z obrazkiem nazwy postaci w stringu
};

class Poziom 
{
public:
    Poziom(int, std::string, std::string, std::string,std::string, int);
	Poziom();
    ~Poziom();
    std::string getsPoziom();   //zwraca scieszke do pliku z obrazkiem poziomu w stringu
    std::string getName();   //zwraca scieszke do pliku z obrazkiem nazwy poziomu w stringu
	std::string getsMap();
	std::string getsTexture();
	int getWin();

private:
	int win;
	int id;
    std::string sPoziom;   //scieszka do pliku z obrazkiem poziomu w stringu
    std::string Name;    //scieszka do pliku z obrazkiem nazwy poziomu w stringu
	std::string sMap;
	std::string sTexture;
	
};
