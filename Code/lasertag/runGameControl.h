#ifndef RUNGAMECONTROL_H
#define RUNGAMECONTROL_H

#include "hwlib.hpp"
#include "rtos.hpp"
#include "sendIRMessageControl.h"
#include "receiveIRMessageControl.h"
#include "display1.h"
#include "registerGameControl.h"
//#include "message_decoder.h"
#include "gameState.h"
#include "timer.h"


class runGameControl : public rtos::task<> {
private:
	registerGameControl &	register_game_control; 
	sendIRMessageControl &	send_IR_message_control;
	display1 & display_1;
//	msg_decoder & msgDecoder;
	gameState & GameState;
	
	uint16_t message = 0b1000010000100000;
	
	rtos::channel<bool,1> buttonChannel;
	rtos::channel<uint16_t,10> messagesProtocolReceive;
	
	int points = 20;
	timer & Timer;
	int receivePN(const uint16_t & messagesProtocolReceive){
		int PN = messagesProtocolReceive & 0b0111110000000000;
		PN = PN >> 10;
		return PN;							 
	}												 

	int receiveWP(const uint16_t & messagesProtocolReceive){
		int WP = messagesProtocolReceive & 0b0000001111100000; 
		WP = WP >> 5;
		return WP;								 
	}
	
	bool makeSureYouGotShot(const uint16_t & messagesProtocolReceive){
		return (receivePN(messagesProtocolReceive) == receiveWP(messagesProtocolReceive));
	}
	
public:
	runGameControl(registerGameControl & register_game_control,display1 & display_1,gameState & GameState,sendIRMessageControl & send_IR_message_control,timer & Timer):
	task(4,"runGameControl"), 
	register_game_control(register_game_control),
	send_IR_message_control(send_IR_message_control),
	display_1(display_1),
//	msgDecoder(msgDecoder),
	GameState(GameState),
	buttonChannel(this,"buttonChannel"),
	messagesProtocolReceive(this,"messagesProtocolReceive"),
	Timer(Timer)
	{
		GameState = gameState::idle;
	}
	
	
	
	void setButtonChannel(){
		hwlib::cout << "button pressed. "<<"\n";
		buttonChannel.write(1);
	}
	
	int getWeaponPower(){
		return register_game_control.giveWeaponPower();
	}
	
	int getPlayerNumber(){
		return register_game_control.givePlayerNumber();
	}
	
	void message_received(uint16_t msg) {
		messagesProtocolReceive.write(msg);
	}
	
	   void main() override {
        for(;;){
			display_1.setTimePoint(points,Timer.getTime());
			hwlib::wait_us(50);
			hwlib::cout << "test \n";
			switch(GameState){
                case gameState::idle: {
                    auto evt = wait( buttonChannel );
                    if (evt == buttonChannel){
						buttonChannel.read();
                        GameState = gameState::startGame;
                    }
                    break;
                }

                case gameState::startGame: {
					hwlib::cout << "through startGame. \n";
					display_1.sendDisplayCommand(1);
//					hwlib::wait_ms(30000);
					hwlib::wait_ms(3000); //Test
					Timer.setTime(120);
                    GameState = gameState::runGame;
                    break;
                }
				
				
				case gameState::hit: {
                    display_1.sendDisplayCommand(2);
//					hwlib::cout << "coolDown \n";
                    GameState = gameState::runGame;
                    break;
                }
                case gameState::gameOver: {
                    display_1.sendDisplayCommand(3);
					break;
                }
				
				
                case gameState::runGame: {
					hwlib::cout << "through runGame. \n";
                    if (Timer.getTime() == 0){
                        GameState = gameState::gameOver;
						break;
                    } 
					hwlib::cout <<"...............................................................\n";
					auto evt = wait( buttonChannel + messagesProtocolReceive);
					if (evt == messagesProtocolReceive){
						hwlib::cout << "received \n";
						int receive = messagesProtocolReceive.read();
						
//						if(makeSureYouGotShot(receive)){
							points--;
							hwlib::cout << "received: "<<receive<<"\n";
//						}
                        if (points == 0){
                            GameState = gameState::gameOver;
                        }
                    }

					if (evt == buttonChannel){
						hwlib::cout << "through runGame buttonChannel.read().\n";
						send_IR_message_control.writeSendChannel(message);
						hwlib::cout << "after writeSendChannel. \n";
                        GameState = gameState::hit;
						hwlib::cout << "hit \n";
                    }
					break;
                }
            }
			if(GameState == gameState::gameOver){
				break;
			}
        }
//		hwlib::cout << "TIME IS OUT!!";
		suspend();
    }
};
#endif