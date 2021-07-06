#ifndef BUTTON_H
#define BUTTON_H

#include "rtos.hpp"
#include "runGameControl.h"

class button : public rtos::task<>{
private:
    runGameControl & runGame;
    hwlib::target::pin_in & Button;
public:
    button(runGameControl & runGame, hwlib::target::pin_in & Button):
    task(5,"button"),
    runGame(runGame),
	Button(Button)
    {}

    void main() override{
        for(;;){
            hwlib::wait_ms(20);
            Button.refresh();
            if(Button.read()){
                runGame.setButtonChannel();
                hwlib::wait_ms( 3000 ); 
            }
        }
    }
};



#endif
