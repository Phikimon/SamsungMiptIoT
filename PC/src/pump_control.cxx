
#include <iostream>
#include <time.h>

time_t prog_start_time;
time_t current_time;
//clock_t time_start;
enum { DAY = 86400};
enum PUMP_MODE {
	MANUAL,
	AUTO
};

enum PUMP_STATE {
	ON,
	OFF
};

class Pump {
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
};

Pump :: Pump() {
	mode = MANUAL;
	on = off;
	manual_state = OFF;
	humidity = 0;
	frequency = 0;
	poliv_qt = 0;
	hum_min = hum_max = 0;
	duration = 1000;

}
Pump :: ~Pump() {
}

void Pump :: pump_control() {
	
		if(pump.mode == AUTO) {
			pump.auto_poliv () ;
		}
		if(pump.mode == MANUAL) {
			 pump.manual_poliv (); 
		}
}

void Pump :: auto_poliv () {
	
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
		//if (poliv_q  > frequency) poliv_q = 0;
	}
	
	if (humidity < hum_min) {
		//on pump
	}
}



void Pump :: manual_poliv () {
	if(manual_state == ON && on == false) {
		//on pump
		on = true;
	}
	else if(manual_state == OFF && on == true) {
		//off pump
		on = false;
	}
}
