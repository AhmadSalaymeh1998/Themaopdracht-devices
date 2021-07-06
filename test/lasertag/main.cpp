#include "hwlib.hpp"
#include "classes.h"

int main( void ){
	namespace target = hwlib::target;
	auto scl = target::pin_oc( target::pins::scl );
	auto sda = target::pin_oc( target::pins::sda );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
		
	auto w = hwlib::glcd_oled( i2c_bus, 0x3c );
	auto font    = hwlib::font_default_8x8();
	auto display = hwlib::terminal_from( w, font );
	auto button =    target::pin_in( target::pins::d10 );
	
}