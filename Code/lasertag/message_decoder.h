#ifndef MESSAGE_DECODER_H
#define MESSAGE_DECODER_H

#include "hwlib.hpp"
#include "rtos.hpp"
//#include "pause_detector.h"
#include "runGameControl.h"


class msg_decoder : public rtos::task<>{
enum class state_MD {idle, signal};
    private:
    state_MD state; 
	
	
	
	bool getBit(uint8_t index, const uint16_t & byte){
		return (byte & (1 << (index)));
	}
	
	
	bool isSame(const uint32_t & msg){
		for(int i = 0; i < 15; i++){
			if( !(getBit(i, uint16_t(msg)) == getBit(i, uint16_t(msg >> 16)))){
				return false;
			}
		}
		return true;
	}
	
	rtos::channel<int, 200> pauses;
	runGameControl & rungame;
	
    public:
    msg_decoder(runGameControl & rungame)
        : task(2,"msg_decoder"), 
		state(state_MD::idle), 
		pauses(this,"pauses"),
		rungame(rungame) {}
	
	void pause_detected(int length){
		pauses.write(length);
	}
	
	
	void main() override {
        for (;;) {
//			suspend();
//			resume();
			int pause, n, m = 0;
			pause = pauses.read();
			int time1;
//			hwlib::cout << "in decoder pause: "<< pause<<"\n";
            switch (state) {
                case state_MD::idle:
//					hwlib::cout << "in decoder idle \n";
                    if (pause < 6000) {
                        n = m = 0;
                        state = state_MD::signal;
						time1 = hwlib::now_us();
						hwlib::cout << "------------------------------\n" ;
						break;
                    }
						else{
						hwlib::wait_us(100);
                        state = state_MD::idle;
						break;
                    }
                    break;
                case state_MD::signal:
				int time2 = hwlib::now_us();
					if(time2 >= (time1+50000)){
						state = state_MD::idle;
						break;
					}
//					hwlib::cout << "in decoder signal \n";
                    if (n >= 16) {
                        state = state_MD::idle;
//						if(isSame(m)){
//							rungame.message_received(m);
//							
							hwlib::cout << "received message: "<<m << "\n" ;
//							
//						}
//						hwlib::cout << "message: "<<m << "\n" ;
						rungame.message_received(m);
						m=0;
                    } else {
                        m <<= 1;
						hwlib::cout << "111111111111111111111111111111111\n" ;
                        m |= pause > 1200 ? 0 : 1;
						hwlib::cout << "22222222222222222222222222222222\n" ;
                        n++;
                    }
                    break;
            }
        }
    }



};

#endif