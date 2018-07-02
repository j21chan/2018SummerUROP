/*
 Name:		UCI_ESP8266_Test.ino
 Created:	7/2/2018 2:04:24 PM
 Author:	jc
*/

#include <SPI.h>
#include <ADXL362.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// MAX30100 PulseOximeter
PulseOximeter pox;
uint32_t tsLastReport = 0;


// ADXL Accelerator
ADXL362 xl;
int16_t temp;
int16_t XValue, YValue, ZValue, Temperature;
int16_t chipSelpin = 15;


// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
	Serial.println("Beat!");
}

void setup()
{
	Serial.begin(74880);

	// Initialize the PulseOximeter instance and register a beat-detected callback
	if (pox.begin(PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES) == false)
	{
		Serial.println("False");
	}

	pox.setOnBeatDetectedCallback(onBeatDetected);

	// Initialize ADXL 362
	xl.begin(chipSelpin);                   // setup spi protocol, issue device soft reset
	xl.beginMeasure();              // switch adxl362 to measure mode  

}

void loop()
{
	// Make sure to call update as fast as possible
	pox.update();

	// read all three axis in burst to ensure all measurements correspond to same sample time
	xl.readXYZTData(XValue, YValue, ZValue, Temperature);

	// Asynchronously dump heart rate and oxidation levels to the serial
	// For both, a value of 0 means "invalid"
	if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
		Serial.print("Heart rate:");
		Serial.print(pox.getHeartRate());
		Serial.print("bpm / SpO2:");
		Serial.print(pox.getSpO2());
		Serial.println("%");
		tsLastReport = millis();
	}

	// Print ADXL362 Sensor Data
	Serial.print("XVALUE=");
	Serial.print(XValue);
	Serial.print("\tYVALUE=");
	Serial.print(YValue);
	Serial.print("\tZVALUE=");
	Serial.print(ZValue);
	Serial.print("\tTEMPERATURE=");
	Serial.println(Temperature);
	delay(100);                // Arbitrary delay to make serial monitor easier to observe

	Serial.println();
}