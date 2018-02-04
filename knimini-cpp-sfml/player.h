#pragma once
#include "stdfax.h"
#include "utilities.h"


#define GRAVITY -9.81f
#define MASA 30.f
#define WSPTAR 0.6f
#define PROMIEN 0.15f
#define WSPOPO 0.5f
#define DELTAT 0.01f
#define LIFE 100
#define POCZATKOWY_X 0.5f
#define POCZATKOWY_Y 0.5f

struct positonPast {
	float x[9];
	float y[9];
	float Vx[9];
	float Vy[9];
	float ax[9];
	float ay[9];
	int Fr[9];
	int Fj[9];
	float Fsp[9];
	float Fsairx[9];
	float Fsairy[9];
	float N[9];
	float Frea[9];
	

	float spX[9];
	float spY[9];

	bool standing[9];
	bool left[9]; //posibility of left movement
	bool right[9];
	int i=0;
	bool modif = true;
};
//player class
class kodlak
{
private:
    int life, Fr, Fj;
    float posX, posY;
    float spX, spY;
	float Vx, Vy, Fsp, Fsairx, Fsairy, N, Frea, Fg;
	float szerokosc, wysokosc;
	float ax, ay;
    bool standing, left, right;
	positonPast Past;
public:
    bool alive;
	bool win;

    kodlak(float);

    void moveRight();
    void moveLeft();
    void updatePos(float, bool*);
    void stopFall();
    void stopLeft();
    void stopRight();
	void stopJump();
    void jump();
    void kill();
	void success();
	void initialValue();
	void modifeLifeAndPosition(int);
	void savePositionPast();

    float x();
    float y();
	float Getszerokosc();
	float Getwysokosc();
	float getSpeedY();
	float getAccelerationY();
	float getSpeedX();
	float getAccelerationX();
	int getLife();

};

struct point
{
	int upRight[2];
	int upLeft[2];
	int downRight[2];
	int downLeft[2];
};



//function for moving character + collision check (RUNS IN SEPARATE THREAD!!!!!!!)
void move(kodlak*, map*, float, bool*);

//checks for far collision
void farCollisionCheck(kodlak*, map*, float*);

//checks for close collision
void closeCollisionCheck(kodlak*, point*, map*, float ,float*);

//calculates rotation of arm in degrees
float calculateRotation(kodlak*, sf::RenderWindow*);


