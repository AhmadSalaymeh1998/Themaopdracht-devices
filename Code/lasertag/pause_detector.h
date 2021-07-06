#ifndef PAUSE_DETECTOR_H
#define PAUSE_DETECTOR_H

#include "hwlib.hpp"
#include "rtos.hpp"

#include "message_decoder.h"


class pause_detector : public rtos::task<> {
private:
	enum class State_PD {idle,signal};
    hwlib::target::pin_in & signal_in;
    rtos::clock clk;
    State_PD State;
	msg_decoder & decoder;
	
    public:
    pause_detector(hwlib::target::pin_in & pin,msg_decoder & decoder)
        : task(1,"pause_detector"),
		signal_in(pin),
		clk(this, 100, "clock"),
		State(State_PD::idle),
		decoder(decoder) 
		{}
	
	; 
	
    void main() override {
        int n = 0;  // pause duration
        for (;;) {
            wait(clk);
			hwlib::wait_us(100);
            switch (State) {
                case State_PD::idle:
                    if (signal_in.read() == 0) {
                        decoder.pause_detected(n);
						hwlib::cout << "pause duration: "<<n<<"\n";
                        State = State_PD::signal;
                    } else {
                        n += 100;
                    }
                    break;
                case State_PD::signal:
                    if (signal_in.read() == 1) {
                        n = 0;
                        State = State_PD::idle;
                    }
                    break;
            }
        }
    }
};


#endif