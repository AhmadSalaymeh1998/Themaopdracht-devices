#ifndef TIMER_H
#define TIMER_H

#include "gameState.h"

class timer : public rtos::task<> {
private:
	int time=120; 
	gameState & GameState;
public:
	timer(gameState & GameState):
	task(7,"timer"),
	GameState(GameState){}
	
	int getTime(){return time;}
	void setTime(int Time){
		time = Time;
	}
	
	void main() override {
		while(time > 0){
			hwlib::wait_ms(1000);
			time--;
		}
		if(time == 0){
			GameState = gameState::gameOver;
			hwlib::cout << "Game Over!"<<"\n";
			suspend();
		}
	}
};

#endif