#include "hwlib.hpp"
#include "runGameControl.h"
#include "button.h" 
#include "rtos.hpp"
#include "pause_detector.h"

int main( void ){
	
	hwlib::wait_ms(1000);
	
	namespace target = hwlib::target;
	auto scl = target::pin_oc( target::pins::scl );
	auto sda = target::pin_oc( target::pins::sda );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
		
	auto w = hwlib::glcd_oled( i2c_bus, 0x3c );
	auto font    = hwlib::font_default_8x8();
	auto display = hwlib::terminal_from( w, font );
	

    auto receiverInput  = target::pin_in ( hwlib::target::pins::d11 );
	
	gameState GameState;
	timer Timer(GameState);
	auto Button = target::pin_in( target::pins::d4 );
	
	auto ir = IRLed();
    auto senderTask = sendIRMessageControl(ir);
	
	auto display1Task =  display1(display,Timer);
	
	auto registerGameTask = registerGameControl(display1Task, GameState, 1, 1);
	auto runGameTask = runGameControl(registerGameTask,display1Task,GameState,senderTask,Timer);
	auto decoderTask = msg_decoder(runGameTask);
    auto detectorTask = pause_detector(receiverInput,decoderTask);
	auto buttonTask = button(runGameTask,Button);

    rtos::run();
}

