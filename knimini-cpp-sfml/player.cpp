#include "player.h"
#include <Windows.h>
#include <mutex>


kodlak::kodlak(float skala)
{
    life = LIFE;
    posX = POCZATKOWY_X;
    posY = POCZATKOWY_Y;
	Vx = 0;
	Vy = 0;
	ax = 0;
	ay = 0;
	Fr = 0;
	Fj = 0;
	Fsp = 0;
	Fsairx = 0;
	Fsairy = 0;
	N = 0;
	Frea = 0;
	Fg = MASA*GRAVITY;
	szerokosc = 0.25*skala;
	wysokosc = 1.6*skala;

    spX = 0;
    spY = 0;
    
	alive = true;
	win = false;
    standing = false;
    left = true; //posibility of left movement
    right = true; //posibility of right movement
}

void kodlak::moveRight()
{
	if (right) {
		Fr = 400;
	}
}

void kodlak::moveLeft()
{
	if (left) {
		Fr = -400;
	}
}

void kodlak::jump()
{
	if (standing) {
		Fj = 30000;
	}
}

void kodlak::updatePos(float skala, bool* modePhysics)
{
	if (!standing) {  //postaæ jest w powietrzu
		//za³o¿enia
		if (*modePhysics) {
			Fr = 0;
		}
		else {
			if (Fr > 0)
				Fr = 100;
			if (Fr < 0)
				Fr = -100;
		}
		Fsp = 0;
		Frea = 0;
		Fj = 0;
		//wspó³rzêdna x
		Fsairx = -6 * 3.1415*Vx*WSPOPO*PROMIEN;
		float Fw = Fr + Fsp + Fsairx;
		ax = Fw / MASA;
		spX = (Vx*DELTAT + (ax*DELTAT*DELTAT / 2))*skala;
		posX = (posX + spX);
		Vx = Vx + ax*DELTAT;
		//wspó³rzêdna y
		Fsairy = -6 * 3.1415*Vy*WSPOPO*PROMIEN;
		Fw = Fg + Frea + Fj + Fsairy;
		ay = Fw / MASA;
		spY = -(Vy*DELTAT + (ay*DELTAT*DELTAT / 2))*skala;
		posY = (posY + spY);
		Vy = Vy + ay*DELTAT;
	}
    else
    {  //postaæ stoi na ziemi
		//za³o¿enia
		N = -Fg;
		Frea = -Fg;
		//wspó³rzêdna x
		if (Vx == 0) {
			Fsp = 0;
		}
		else {
			Fsp = (((-Vx)/abs(Vx))*WSPTAR*N);
		}
		Fsairx = -6 * 3.1415*Vx*WSPOPO*PROMIEN;
		float Fw = Fr + Fsp + Fsairx;
		ax = Fw / MASA;
		spX = (Vx*DELTAT + (ax*DELTAT*DELTAT / 2))*skala;
		posX = (posX + spX);
		Vx = Vx + ax*DELTAT;

		if ((Fr == 0) && (Vx < 0.04) && (Vx > -0.04)) {  //funkcja hamuj¹ca ma³e prêdkoœci
			Vx = 0;
			ax = 0;
		}
		//wspó³rzêdna y
		Fsairy = -6 * 3.1415*Vy*WSPOPO*PROMIEN;
		Fw = Fg + Frea + Fj + Fsairy;
		ay = Fw / MASA;
		spY = -(Vy*DELTAT + (ay*DELTAT*DELTAT / 2))*skala;
		posY = (posY + spY);
		Vy = Vy + ay*DELTAT;	
    }
    standing = false;
    left = true;
    right = true;
	Fj = 0;
	Fr = 0;
}

void kodlak::stopFall()
{
    standing = true;
    spY = 0;
	Vy = 0;
	ay = 0;
}

void kodlak::stopLeft()
{
    left = false;
	Fr = 0;
	if (Vx < 0)
		Vx = 0;
	if (ax < 0)
		ax = 0;
    if (spX < 0)
    {
        spX = 0;
    }
}

void kodlak::stopRight()
{
    right = false;
		Fr = 0;
	if (Vx > 0)
		Vx = 0;
	if (ax > 0)
		ax = 0;
    if (spX > 0)
    {
        spX = 0;
    }
}

void kodlak::stopJump()
{	
	if (Vy > 0)
		Vy = 0;
	if (ay > 0)
		ay = 0;
}

void kodlak::kill()
{
    alive = false;
}

void kodlak::success() {
	win = true;
}

void kodlak::initialValue() {
	life = LIFE;
	posX = POCZATKOWY_X;
	posY = POCZATKOWY_Y;
	Vx = 0;
	Vy = 0;
	ax = 0;
	ay = 0;
	Fr = 0;
	Fj = 0;
	Fsp = 0;
	Fsairx = 0;
	Fsairy = 0;
	N = 0;
	Frea = 0;
	Fg = MASA*GRAVITY;

	spX = 0;
	spY = 0;

	alive = true;
	win = false;
	standing = false;
	left = true; //posibility of left movement
	right = true; //posibility of right movement
}

float kodlak::x()
{
    return posX;
}

float kodlak::y()
{
    return posY;
}

float kodlak::Getszerokosc()
{
	return szerokosc;
}

float kodlak::Getwysokosc()
{
	return wysokosc;
}

float kodlak::getSpeedY() {
	return Vy;
}

float kodlak::getAccelerationY() {
	return ay;
}

float kodlak::getSpeedX() {
	return Vx;
}

float kodlak::getAccelerationX() {
	return ax;
}

int kodlak::getLife() {
	return life;
}

void kodlak::savePositionPast() {
	if (Past.i == 9) 
		Past.modif = false;
	Past.x[Past.i] = x();
	Past.y[Past.i] = y();
	Past.Vx[Past.i] = getSpeedX();
	Past.Vy[Past.i] = getSpeedY();
	Past.ax[Past.i] = getAccelerationX();
	Past.ay[Past.i] = getAccelerationY();
	Past.Fr[Past.i] = Fr;
	Past.Fj[Past.i] = Fj;
	Past.Fsp[Past.i] = Fsp;
	Past.Fsairx[Past.i] = Fsairx;
	Past.Fsairy[Past.i] = Fsairy;
	Past.N[Past.i] = N;
	Past.Frea[Past.i] = Frea;
	Past.spX[Past.i] = spX;
	Past.spY[Past.i] = spY;
	Past.standing[Past.i] = standing;
	Past.left[Past.i] = left;
	Past.right[Past.i] = right;
	Past.i = (Past.i + 1) % 10;
}

void move(kodlak* character, map* carte, float skala, bool* modePhysics)
{
        farCollisionCheck(character, carte, &skala);  // punkt góra-lewa
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 		character->moveLeft();
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	character->moveRight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))		character->jump();
        character->updatePos(skala, modePhysics);
}

void farCollisionCheck(kodlak* ch1, map* carte, float* skala)
{
	float s = 0;
	float w = 0;
	point Point;
	for (int i = 1; i < 5; i++) {
		switch (i) {
		case 1:
			s = ch1->Getszerokosc();
			w = 0;
			break;
		case 2:
			s = 0;
			w = 0;
			break;
		case 3:
			s = 0;
			w = ch1->Getwysokosc();
			break;
		case 4:
			s = ch1->Getszerokosc();
			w = ch1->Getwysokosc();
			break;
		}
		int posX = carte->binaryPositionSearch(ch1->x() + s, 0, carte->x() - 1);
		int posY = carte->binaryPositionSearch(ch1->y() + w, 0, carte->y() - 1);
		
		switch (i) {
		case 1:
			Point.upRight[0] = posX;
			Point.upRight[1] = posY;
			break;
		case 2:
			Point.upLeft[0] = posX;
			Point.upLeft[1] = posY;
			break;
		case 3:
			Point.downLeft[0] = posX;
			Point.downLeft[1] = posY;
			break;
		case 4:
			Point.downRight[0] = posX;
			Point.downRight[1] = posY;
			break;
		}
	}
		closeCollisionCheck(ch1, &Point, carte, carte->size(), skala);
}

void closeCollisionCheck(kodlak* ch1, point* Point, map* carte, float size, float* skala)
{

    float coordXRight = (Point->upRight[0] + 1) * size;
    float coordYDown = (Point->downLeft[1] + 1) * size;
    float coordXLeft = Point->upLeft[0] * size;
    float coordYUp = Point->upLeft[1] * size;
	int val;

	for (int i = 0; i < 8; i++) {
		switch (i)
		{
		case 0: //górny-prawy róg
			val = carte->cellV(Point->upRight[1], Point->upRight[0]);
			break;
		case 1:   //górny-lewy róg
			val = carte->cellV(Point->upLeft[1], Point->upLeft[0]);
			break;
		case 2:   //dolny-prawy róg
			val = carte->cellV(Point->downRight[1], Point->downRight[0]);
			break;
		case 3:   //dolny-lewy róg
			val = carte->cellV(Point->downLeft[1], Point->downLeft[0]);
			break;
		case 4:  //górna œciana
			for (int j = Point->upLeft[0] + 1; j < Point->upRight[0]; j++) {
				if ((carte->cellV(Point->upLeft[1], j) == 8)) {
					val = 8;
					break;
				}
			}
			break;
		case 5:  //dolna œciana
			for (int j = Point->downLeft[0] + 1; j < Point->downRight[0]; j++) {
				if ((carte->cellV(Point->downLeft[1], j) == 1)) {
					val = 1;
					break;
				}
			}
			break;
		case 6:  //lewa œciana
			for (int j = Point->upLeft[1] + 1; j < Point->downLeft[1]; j++) {
				if ((carte->cellV(j, Point->upLeft[0]) == 4)) {
					val = 4;
					break;
				}
			}
			break;
		case 7:  //prawa œciana
			for (int j = Point->upRight[1] + 1; j < Point->downRight[1]; j++) {
				if ((carte->cellV(j, Point->upRight[0]) == 8)) {
					val = 8;
					break;
				}
			}
			break;
		}

		if (val == 0)
			continue;

			//right wall
			if (val > 8)
			{
				val -= 8;
				if (ch1->x() + 0.5 + ch1->Getszerokosc() + (ch1->getSpeedX()*DELTAT + ch1->getAccelerationX()*DELTAT*DELTAT / 2)**skala > coordXRight)
				{
					ch1->stopRight();
				}
			}
			else if (val == 8)
			{
				val -= 8;
				if (ch1->x() + 0.5 + ch1->Getszerokosc() + (ch1->getSpeedX()*DELTAT + ch1->getAccelerationX()*DELTAT*DELTAT / 2)**skala > coordXRight)
				{
					ch1->stopRight();
				}
				continue;
			}

			//left wall
			if (val > 4)
			{
				val -= 4;
				if (ch1->x() - 0.5 + (ch1->getSpeedX()*DELTAT + ch1->getAccelerationX()*DELTAT*DELTAT / 2)**skala < coordXLeft)
				{
					ch1->stopLeft();
				}
			}
			else if (val == 4)
			{
				val -= 4;
				if (ch1->x() - 0.5 + (ch1->getSpeedX()*DELTAT + ch1->getAccelerationX()*DELTAT*DELTAT / 2)**skala < coordXLeft)
				{
					ch1->stopLeft();
				}
				continue;
			}

			//ceiling
			if (val > 2)
			{
				val -= 2;
				if (ch1->y() - 0.5 - (ch1->getSpeedY()*DELTAT + ch1->getAccelerationY()*DELTAT*DELTAT / 2)**skala  < coordYUp)
				{
					ch1->stopJump();
				}
			}
			else if (val == 2)
			{
				val -= 2;
				if (ch1->y() - 0.5 - (ch1->getSpeedY()*DELTAT + ch1->getAccelerationY()*DELTAT*DELTAT / 2)**skala < coordYUp)
				{
					ch1->stopJump();
				}
				continue;
			}

			//floor
			if (val == 1)
			{
				val -= 1;
				if (0.5 + ch1->y() + ch1->Getwysokosc() - (ch1->getSpeedY()*DELTAT + ch1->getAccelerationY()*DELTAT*DELTAT/2)**skala > coordYDown)
				{
					ch1->stopFall();
				}
				continue;
			}
		
	}
}

void kodlak::modifeLifeAndPosition(int count) {
	if (life <= count) {
		kill();
		Past.i = 0;
		Past.modif = true;
	}
	else {
		if (Past.modif == true) {
			life = life - count;
			posX = Past.x[0];
			posY = Past.y[0];
			Vx = Past.Vx[0];
			Vy = Past.Vy[0];
			ax = Past.ax[0];
			ay = Past.ay[0];
			Fr = Past.Fr[0];
			Fj = Past.Fj[0];
			Fsp = Past.Fsp[0];
			Fsairx = Past.Fsairx[0];
			Fsairy = Past.Fsairy[0];
			N = Past.N[0];
			Frea = Past.Frea[0];
			spX = Past.spX[0];
			spY = Past.spY[0];
			standing = Past.standing[0];
			left = Past.left[0];
			right = Past.right[0];
			Past.modif = true;
			Past.i = 0;
		}
		else {
			life = life - count;
			posX = Past.x[Past.i];
			posY = Past.y[Past.i];
			Vx = Past.Vx[Past.i];
			Vy = Past.Vy[Past.i];
			ax = Past.ax[Past.i];
			ay = Past.ay[Past.i];
			Fr = Past.Fr[Past.i];
			Fj = Past.Fj[Past.i];
			Fsp = Past.Fsp[Past.i];
			Fsairx = Past.Fsairx[Past.i];
			Fsairy = Past.Fsairy[Past.i];
			N = Past.N[Past.i];
			Frea = Past.Frea[Past.i];
			spX = Past.spX[Past.i];
			spY = Past.spY[Past.i];
			standing = Past.standing[Past.i];
			left = Past.left[Past.i];
			right = Past.right[Past.i];
			Past.modif = true;
			Past.i = 0;
		}
	}
	std::cout << life << std::endl;
}

float calculateRotation(kodlak* pl, sf::RenderWindow* window)
{
    sf::Vector2f wPos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    float x = wPos.x - pl->x();
    float y = wPos.y - pl->y();
    return (atan2(y,x)*180)/3.14159265;
}



