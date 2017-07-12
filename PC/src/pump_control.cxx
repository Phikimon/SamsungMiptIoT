#include <iostream>
#include <ctime>

time_t prog_start_time;
time_t current_time;

enum { DAY = 86400};

enum PUMP_MODE {
	MANUAL,
	AUTO
};

enum PUMP_STATE {
	ON,
	OFF
};

class Pump
{
	public:
		Pump();
		~Pump();

		void auto_poliv();
		void manual_poliv();
		void pump_control();

		int mode;
		bool on;
		int manual_state;
		float humidity;
		int frequency;
		int poliv_qt;
		int hum_max;
		int hum_min;
		time_t duration;
		char ID;
};

Pump::Pump() {
	mode = PUMP_MODE::MANUAL;
	on = false;
	manual_state = PUMP_STATE::OFF;
	humidity = 0;
	frequency = 0;
	poliv_qt = 0;
	hum_min = hum_max = 0;
	duration = 60 * 5;
}

Pump::~Pump() {
}

void Pump::pump_control() {
	
		if (mode == PUMP_MODE::AUTO) {
			auto_poliv() ;
		}
		if (mode == PUMP_MODE::MANUAL) {
			manual_poliv();
		}
}

void Pump::auto_poliv() {
	
	current_time = time(NULL);
	if((current_time - prog_start_time - poliv_qt * (DAY / frequency)) > DAY/frequency) {
		time_t time_start = time(NULL);
		
		//on pump
		
		on = true;
		while(1) {
			current_time = time(NULL);
			if (current_time - time_start > duration) {
				
				//get humidity value again
				
				if (humidity > hum_max) {
					//off pump
					on = false;
					break;
				}
			}
		}
		poliv_qt++;
	}
	
	if (humidity < hum_min) {
		//on pump
	}
}



void Pump::manual_poliv () {
	if(manual_state == PUMP_STATE::ON && on == false) {
		//on pump
		on = true;
	} else if(manual_state == PUMP_STATE::OFF && on == true) {
		//off pump
		on = false;
	}
}
