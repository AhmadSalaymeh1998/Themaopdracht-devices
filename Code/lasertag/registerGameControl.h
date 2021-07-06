#ifndef REGISTERGAMECONTROL_H
#define REGISTERGAMECONTROL_H

#include "hwlib.hpp"
#include "rtos.hpp"
#include "gameState.h"
#include "display1.h"

class playerNumber{
private:
	int playernumber;
public:	
	playerNumber(int number=1): playernumber(number){}
	void setplayerNumber(int number){playernumber = number;}
	int getplayerNumber(){return playernumber;}
};

class weaponPower{
private:
	int weaponpower;
public:
	weaponPower(int number=1): weaponpower(number){}
	void setweaponPower(int number){weaponpower = number;}
	int getweaponPower(){return weaponpower;}
};



class registerGameControl : public rtos::task<> {
private:
	int WPnumber, PNnumber;
	weaponPower WP;
	playerNumber PN;
	gameState & GameState;
	display1 & display_1; 
	
public:
	registerGameControl(display1 & display_1, gameState & GameState, const int WPnumber= 1,const int PNnumber= 1):
		task(8,"registerGameControl"), 
		WPnumber(WPnumber), 
		PNnumber(PNnumber),
		GameState(GameState), 
		display_1(display_1)
	{	
		WP.setweaponPower(WPnumber);
		PN.setplayerNumber(PNnumber);
	}
	
	void setPlayMode(const int player_Number, const int weapon_Power){
		WP.setweaponPower(weapon_Power);
		PN.setplayerNumber(player_Number);
	}
	int givePlayerNumber(){ return PN.getplayerNumber(); }
	int giveWeaponPower(){ return WP.getweaponPower(); }
	
	void main() override{
		for(;;){
			hwlib::wait_us(50);
//			if (GameState == gameState::startGame){
//				display_1.countDown(); 
//				GameState = gameState::runGame;
//				
//			}
//			if(GameState == gameState::runGame){
				hwlib::cout << "RegisterGame suspended"<<"\n";
				suspend();
//			}
		}
		
	}
};

#endif