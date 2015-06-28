#include <SPI.h>
#include "RF24.h"
#include "RF24SN.h"
#include <SFE_BMP180.h>
#include <Wire.h>

#define RF24SN_NODE_ID 2
#define RF24SN_SERVER_ADDR 0xF0F0F0F000LL
#define RF24_CE_PIN 12
#define RF24_CS_PIN 13
#define SMOKE_PIN 19
#define sensorId_smoke 4
#define sensorId_press 3

SFE_BMP180 pressure;
RF24 radio(RF24_CE_PIN, RF24_CS_PIN);
RF24SN rf24sn( &radio, RF24SN_SERVER_ADDR, RF24SN_NODE_ID);

void setup(void)
{
	pressure.begin();
	rf24sn.begin();
	pinMode(SMOKE_PIN, INPUT);
}

void loop(void)
{
	double P, T;
	int smoke_val;
	long press;
	char status;
	bool smoke;
	smoke_val = analogRead(SMOKE_PIN);
	if (smoke_val > 600)
		smoke = true;
	else
		smoke = false;
	status = pressure.startPressure(1);
	if (status != 0)
	{
		delay(status);
		status = pressure.getPressure(P,T);
		if (status != 0)
		{
			press = int(P*100);
		}
		else
			press = 0;
	}
	else
		press = -1
	while (!publish(smoke, press));
	delay(500);	
}

bool publish(bool smoke, long press)
{
	bool b_press rf24sn.publish(sensorId_smoke, smoke);
	bool b_smoke rf24sn.publish(sensorId_press, press);
	return (b_press && b_smoke);
}
