#ifndef DISPLAY1_H
#define DISPLAY1_H

#include "runGameControl.h"
#include "timer.h"
class display1 : public rtos::task<>{
private:
    hwlib::terminal_from & display;
	rtos::channel<int,10> displayChannel;
	timer & Timer;
	rtos::clock clk;
	int Time;
	int Point=20;
	void countDown(){
        display << "\f" << "The Game Will\nStart In 30\nSeconds" << hwlib::flush;
        hwlib::wait_ms( 1000 );
        for( int i=3; i>=0; i--){
            display << "\f" << i << hwlib::flush;   
            hwlib::wait_ms( 1000 );
        }    
    }
	
	void coolDown(){
        for( int i=3; i>=0; i--){
            display << "\f"<< "weapon cooldown \n" << i <<" sec" << hwlib::flush;   
            hwlib::wait_ms( 1000 );
        }    
    }
	
	
	
	void GameOver(){
		display << "\f"<< " Game Over \n" << hwlib::flush;
	}
	
	void showTimePoints(){
		Time = Timer.getTime();
		display << "\f"<< "point: "<<Point << "\n" <<"time: "<< Time <<hwlib::flush;
	}
public:
    display1(auto & display,timer & Timer):
	task(6,"display1"),
	display(display),
	displayChannel(this,"displayChannel"),
	Timer(Timer),
	clk(this, 1000 , "clock"){}
    
	void sendDisplayCommand(const int & command){
		displayChannel.write(command);
	}
	
	
	void setTimePoint(const int & point,const int & time){
		Point = point;
		Time = time;
	}
	
	void main() override{
		for(;;){
			wait(clk);
			showTimePoints();
//			wait(displayChannel);
			switch(displayChannel.read()){
				case 1: {
					countDown();
					showTimePoints();
					break;
				}
				case 2: {
					coolDown();
					showTimePoints();
					break;
				}
				case 3: {
					GameOver();
					suspend();
					break;
				}
				default: {
					showTimePoints();
					break;
				}
			}
		}
	}
};

#endif