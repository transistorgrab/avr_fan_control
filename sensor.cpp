// this file contains all defines regarding sensor access
// the sensor is a AM2302 temperature/humidity sensor with a one wire interface

// get_sensor returns the sensor reading either for the indoor or outdoor sensor
// and returns the temperature and the humidity value in one 16 bit value
// if called with the update parameter it will read the data from the sensor(s)
// update must not be faster than 2 seconds between two readings per sensor

#include <avr/io.h>
#include "avr_fan_control.h"

struct sensor_data read_sensor(int sensor)
{
	struct sensor_data s_data;	/* contains the sensor data in a 16 bit format			*/
	uint8_t humidity_high = 0;	/* contain the bits received from the sensor	*/
	uint8_t humidity_low = 0;
	uint8_t temp_high = 0;
	uint8_t temp_low = 0;
	uint8_t parity = 0;
	
	/* implements the read routine for the one wire sensor AM2302					*/
	/* Protocol:																	*/
	/*	- open drain output required, 5 kOhm pull-up								*/
	/*	- Master: start bit: low level for 800 us..20 ms							*/
	/*			  from bus release until ack: 20..200 us							*/
	/*	- Slave: acknowledge: 80 us low, 80 us high	(75..85 us)						*/
	/*			 then 5 bytes of data: humidity H, hum. L, temp H, temp L, parity	*/
	/*			 low bit:  low = 50 us (48..55 us), high = 26 us (22..30 us)		*/
	/*			 high bit: low = 50 us (48..55 us), high = 70 us (68..75 us)		*/
	/*			 sensor needs 50 us (45..55 us) to release the bus					*/
	/*			  |   ack   |  0   |   1    |										*/
	/*			         ___      _      ___      ____								*/
	/*            \     /   \    / \    /   \    /									*/
	/*             \___/     \__/   \__/     \~~/									*/
	
	
	
	if (parity != (humidity_high+humidity_low+temp_high+temp_low))	/* if data is not correct return error code	*/
	{
		s_data.hum  = DATA_ERROR;
		s_data.temp = DATA_ERROR;
		return s_data;
	}
	/*	put all received data in the return variable	*/
	s_data.hum   = ((0x0011&humidity_high)<<8);
	s_data.hum  |= humidity_low;
	s_data.temp  = ((0x0011&temp_high)<<8);
	s_data.temp |= temp_low;	
	 	
	return s_data;
}

/* returns the reading from the sensors.
*  before using any reading for indoor or outdoor the first call to this function 
*  must be with the update parameter set.
*/

struct sensor_data get_sensor(int sensor, int update)
{
	struct sensor_data static sensor_indoor;
	struct sensor_data static sensor_outdoor;
	
	if (update)
	{
		switch (update)
		{
			case SENSORS_ALL:
				sensor_indoor	= read_sensor(SENSOR_INDOOR);
				sensor_outdoor	= read_sensor(SENSOR_OUTDOOR);
				break;
			case SENSOR_INDOOR:
				sensor_indoor	= read_sensor(SENSOR_INDOOR);
				break;
			case SENSOR_OUTDOOR:
				sensor_outdoor	= read_sensor(SENSOR_OUTDOOR);
				break;
			default:
				break;
		}
		
		return sensor_outdoor;
	}
	switch (sensor)
	{
		case SENSOR_INDOOR:
			return sensor_indoor;
			break;
		case SENSOR_OUTDOOR:
			return sensor_outdoor;
			break;
		default:
			return sensor_outdoor;
			break;
	}	
	
}
