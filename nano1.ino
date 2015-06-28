#include <dht.h>
#include <SPI.h>
#include "RF24.h"
#include "RF24SN.h"

#define RF24SN_NODE_ID 1
#define RF24SN_SERVER_ADDR 0xF0F0F0F000LL
#define RF24_CE_PIN 12
#define RF24_CS_PIN 13
#define DHT22_PIN 9
#define PIR_PIN 8
#define LIGHT_PIN 7
#define sensorId_temp 1
#define sensorId_humi 2
#define sensorId_pres 5
#define sensorId_light 6

dht DHT;
RF24 radio(RF24_CE_PIN, RF24_CS_PIN);
RF24SN rf24sn( &radio, RF24SN_SERVER_ADDR, RF24SN_NODE_ID);

void setup(void)
{
	rf24sn.begin();
	pinMode(DHT22_PIN, INPUT);
	pinMode(PIR_PIN, INPUT);
	pinMode(LIGHT_PIN, OUTPUT);
}

void loop(void)
{
	int temp, humi;
	bool light;
	bool pres = digitalRead(PIR_PIN);
	int dht = DHT.read22(DHT22_PIN);
	if (dht != DHTLIB_OK) then
	{
		temp = int (DHT.temperature*10);
		humi = int (DHT.humidity*10);
	}
	else
	{
		humi = -1;
		temp = -100;
	}
	while (!publish(temp, humi, pres));
	if (rf24sn.request(sensorId_light, &light)) then
	{
		if light then
			digitalWrite(LIGHT_PIN, HIGH);
		else
			digitalWrite(LIGHT_PIN, LOW);
	}
	delay(500);
}

bool publish(int temp, int humi, bool pres)
{
	bool b_temp rf24sn.publish(sensorId_temp, temp);
	bool b_humi rf24sn.publish(sensorId_humi, humi);
	bool b_pres rf24sn.publish(sensorId_pres, pres);
	return (b_temp && b_humi && b_pres);
}