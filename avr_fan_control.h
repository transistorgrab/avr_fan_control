#define SENSOR_INDOOR	1
#define SENSOR_OUTDOOR	2
#define SENSORS_ALL		3
#define DATA_ERROR		0xAA
#define INDOOR_TEMP_MIN	50	/* all temperatures are read from the sensor in 1/10 degree C	*/
							/* so: 50 equals 5 degree C										*/
#define SENSOR_UPDATED	0x02

#define TEMP_TOO_LOW		0x01
#define HUM_OUT_TOO_HIGH	0x02
#define TEMP_OUT_TOO_HIGH	0x04

#define FAN_RUNNING	0x58
#define FAN_STOPPED	0x87

struct sensor_data
{
	int16_t temp;
	uint16_t hum;
};

struct sensor_data get_sensor(int sensor, int update);

/** defines for ports **
**
** hardware pin configuration:
** all not defined ports are not used or not available	*/

/** struct and define for setting single port bits with simple statements
    source: [http://www.mikrocontroller.net/attachment/27445/SBIT.C] */
struct bits {
	uint8_t b0:1;
	uint8_t b1:1;
	uint8_t b2:1;
	uint8_t b3:1;
	uint8_t b4:1;
	uint8_t b5:1;
	uint8_t b6:1;
	uint8_t b7:1;
} __attribute__((__packed__));

#define SBIT(port,pin) ((*(volatile struct bits*)&port).b##pin)

/** ports for power amplifier control and status	*/
#define SENSOR_INDOOR_WR	SBIT( PORTD, 0 )	/** write output for indoor sensor		*/
#define SENSOR_INDOOR_RD	SBIT( PIND,	 0 )	/** read input for indoor sensor		*/
#define SENSOR_OUTDOOR_WR	SBIT( PORTD, 2 )	/** write output for outdoor sensor		*/
#define SENSOR_OUTDOOR_RD   SBIT( PIND,  2 )	/** read input for indoor sensor		*/

#define FAN_CONTROL			SBIT( PORTB, 0 )	/** output to fan control circuit		*/

#define TEMP_WARNING		SBIT( PORTB, 2 )	/** output for temperature low warning	*/
#define HUM_WARNING			SBIT( PORTB, 4 )	/** output for humidity high warning	*/