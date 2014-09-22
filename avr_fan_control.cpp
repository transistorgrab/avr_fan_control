/*
 * Butterfly_Fan_Control.cpp
 * This is the main file for a controller application that controls a fan
 * depending on sensor reading. If the indoor and outdoor conditions are feasible
 * the fan is switched on.
 * The sensor readings are in 1/10 of the unit, so temperature is 100 for 10°C
 * and humidity is 500 for 50%.
 * 
 * Created: 13.09.2014 13:18:32
 *  Author: André
 */ 



#include <avr/io.h>
#include "avr_fan_control.h"
//#include <avr/sleep.h>


/** init function will initialize all output and input pins
    set up timers
*/
int init_controller(void)
{
	/** set up the input and output pins	*/
	/** set up the timer					*/
	/** get the RTC time					*/
}

/** set_fan(int enable) will switch the fan on when the enable parameter is > 0
	but will check before if the conditions allow the fan to be activated.
	if the conditions are not met an error code will be returned.
*/
int set_fan(int enable)
{
	struct sensor_data s_data;		/* data from sensor			*/
	int16_t sensor_indoor_temp;		/* temperature data indoor	*/
	uint16_t sensor_indoor_hum;		/* humidity data indoor		*/
	int16_t sensor_outdoor_temp;	/* temperature data outdoor	*/
	uint8_t sensor_outdoor_hum;		/* humidity data outdoor	*/
	
	s_data = get_sensor(SENSOR_INDOOR, 0);	/* get the current sensor readings	*/
	if (s_data.hum == DATA_ERROR) return DATA_ERROR; /* if there is an error it is always for both temperature and humidity	*/
	sensor_indoor_temp	= s_data.temp;
	sensor_indoor_hum	= s_data.hum;

	s_data = get_sensor(SENSOR_OUTDOOR, 0);
	if (s_data.hum == DATA_ERROR) return DATA_ERROR; /* if there is an error it is always for both temperature and humidity	*/
	sensor_outdoor_temp	= s_data.temp;
	sensor_outdoor_hum	= s_data.hum;

	/* there are several requirements to meet before the fan can be activated	*/
	/* the indoor temperature must not go below INDOOR_TEMP_MIN, so if the		*/
	/*     outdoor temperature and indoor temperature are below INDOOR_TEMP_MIN */
	/*     the fan must not run													*/
	if ((sensor_indoor_temp <= INDOOR_TEMP_MIN) && (sensor_outdoor_temp <= INDOOR_TEMP_MIN))
		return TEMP_TOO_LOW;
	/* if the outdoor temperature is more than 5 degrees lower but not more than 10 degrees C	*/
	/* than the indoor temperature but the humidity is more than 75% the fan must not run		*/
	if ((sensor_indoor_temp <= sensor_outdoor_temp + 50) && (sensor_outdoor_hum >= 750))
		return HUM_OUT_TOO_HIGH;
	/* if the outdoor temperature is within 5 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 60% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 50) && (sensor_outdoor_hum >= 600))
		return HUM_OUT_TOO_HIGH;	
	/* if the outdoor temperature is within 10 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 45% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 100) && (sensor_outdoor_hum >= 450))
		return HUM_OUT_TOO_HIGH;
	/* if the outdoor temperature is within 15 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 35% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 150) && (sensor_outdoor_hum >= 350))
		return HUM_OUT_TOO_HIGH;
	/* if the outdoor temperature is within 20 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 25% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 200) && (sensor_outdoor_hum >= 250))
		return HUM_OUT_TOO_HIGH;
	/* if the outdoor temperature is within 25 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 18% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 250) && (sensor_outdoor_hum >= 180))
		return HUM_OUT_TOO_HIGH;
	/* if the outdoor temperature is within 30 degrees higher of the indoor temperature and the	*/
	/* humidity outdoors is more than 15% the fan must not run									*/
	if ((sensor_indoor_temp >= sensor_outdoor_temp - 300) && (sensor_outdoor_hum >= 150))
		return HUM_OUT_TOO_HIGH;
	/* if there are temperatures more than 30 degrees higher than the indoor temperature this	*/
	/* will be regarded as not covered	*/
	if (sensor_indoor_temp < sensor_outdoor_temp - 300)
		return TEMP_OUT_TOO_HIGH;
	/* if those requirements and the exclusions above are meat it is assumed that it is save	*/
	/* to turn the fan on																		*/
		
	if (enable)
	{
		FAN_CONTROL = 1;
		return FAN_RUNNING;
	}
	else
	{	FAN_CONTROL = 0;
		return FAN_STOPPED;
	}
}

int main(void)
{
    while(1)
    {
        set_fan(1);
		//set_sleep_mode(SLEEP_MODE_IDLE);
		//sleep_mode();
    }
}