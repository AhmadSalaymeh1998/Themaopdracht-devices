#ifndef SENDIRMESSAGECONTROL_H
#define SENDIRMESSAGECONTROL_H

#include "rtos.hpp"
#include "hwlib.hpp"

class IRLed {
private:
    hwlib::target::d2_36kHz irled = hwlib::target::d2_36kHz();
public: 
    IRLed(){}
    void write(bool x){
        irled.write(x);
        irled.flush();
	}

};

class sendIRMessageControl : public rtos::task<>{
private:
    IRLed & irled;
    rtos::channel<uint16_t, 10> messagesSend;  
public:
    sendIRMessageControl(IRLed &irled):
        task(3,"sendIRMessageControl"),
        irled(irled),
        messagesSend(this, "messagesSend")  
    {}


private:
	
	void startBit(){
		irled.write(1);
        hwlib::wait_us(800);
        irled.write(0);
		hwlib::wait_us(5000);
//		irled.write(1);
//		hwlib::wait_us(200);
//		irled.write(0);
	}
	
    void sendZero(){
        irled.write(1);
        hwlib::wait_us(800);
        irled.write(0);
        hwlib::wait_us(1600);
    }  
    
    void sendOne(){
        irled.write(1);
        hwlib::wait_us(1600);
        irled.write(0);
        hwlib::wait_us(800);
    }
	

    void startProtocolSend(const uint16_t & message){
		startBit();
//		startBit();
        for(uint8_t i = 0; i < 2; i++){
            for(int j = 15; j >= 0; j--){ 
                if (message & (1 << j) ) {
                    sendOne();
//					hwlib::cout << "1";
                } else {
                    sendZero();
//					hwlib::cout << "0";
                }
            }
			if(i==0){
				hwlib::wait_ms(3);
			}
//			hwlib::cout << "\n";
        }
    }

public:
    void writeSendChannel(const uint16_t & message){
        messagesSend.write(message);
    }


    void main() override {
        for(;;){
			hwlib::wait_us(50);
			wait(messagesSend);
            uint16_t message = messagesSend.read();
//			hwlib::cout <<"send message: "<< message<<"\n";
            startProtocolSend(message);
			message = 0;
        }
    }

};


#endif
