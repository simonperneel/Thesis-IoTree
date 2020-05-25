/*****************************************************************************
 * @file main.c
 * @brief main software file for master thesis IoTree
 * @author Arne de Brabanter & Simon Perneel
 * @version 1.0 : Code for measuring the sensors
 * 			2.0 : Code for LoRaWAN communication with RN2483
 ******************************************************************************/
// Standard libraries
#include "stdbool.h"
#include "stdlib.h"
#include "math.h"

// SDK libraries for efm32
#include "em_device.h"
#include "em_chip.h"
#include "em_I2C.h"
#include "em_rtc.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"

// System libraries
#include "system.h"
#include "pin_mapping.h"
#include "util.h"
#include "gpio.h"
#include "hardwareid.h"
#include "iic.h"
#include "delay.h"
#include "led.h"

// LoRa communication
#include "lora.h"
#include "lpp.h"
#include "my_lora_device.h"

// Sensors
#include "lis3dh.h"
#include "TMP006.h"
#include "HDC2080.h"
#include "SoilMoist.h"
#include "CCS811.h"

// Debugging: dbprintln, dbprintInt,...
#include "debug_dbprint.h"

#define A_MINUTE 60
#define A_SECOND 1000

/************************** ADJUSTABLE ***************************************************/
#define WAKE_UP_PERIOD_S 30*A_MINUTE // time between measurements
#define CUSTOM_BOARD 0 //0: Happy gecko, 1: custom board --> !!! ook in pinmapping.h aanpassen !!!
#define TILT_TRESH 45 // Threshold for maximum tilt before alert (°)
#define CODE_JONGE_BOOM 1 // 1/0: with or without soil moisture sensor
/*****************************************************************************************/

// global variables
uint16_t Co2;
uint16_t LPP_BUFFERSIZE = 32;
float globalTemp, globalHum, maxtilt;
double boomTemp, globTemp, globHum, grondVochtigheid = 0;
volatile bool interrupt_triggered, windhoos_meas;

// application states
typedef enum app_states {
	INIT_SYSTEM,
	JOIN,
	INIT_SENSORS,
	MEASURE,
	SEND,
	WINDHOOS,
	SLEEP,
	WAKE_UP,
	SLEEP_REMAINING
} APP_State_t;

static volatile APP_State_t appState; /* Static so it's always kept in memory*/

/*********************************************************************************
 *
 * 								MAIN APPLICATION
 *
 *********************************************************************************/

int main(void) {
	/* Application variables */
	// Value to keep the remaining sleep time when waking up using the accelerometer
	uint32_t remainingSleeptime = 0;
	// flags to check if sensors are connected
	bool co2ON, temphumON, boomtempON, bewegingON;
	int counter_CO2, counter_Soil;

	LoRaSettings_t loraSettings = LORA_INIT_MY_DEVICE;
	LoRaStatus_t loraStatus;
	LPP_Buffer_t appData;
	memset(&appData, 0, sizeof(appData));

	appState = INIT_SYSTEM;

	while (1) {
		switch (appState) {

		case INIT_SYSTEM: {
			System_Init();

#if CUSTOM_BOARD == 1
			initLED(); //initialize LED of custom board
#endif

#if CUSTOM_BOARD == 0
#if DEBUG_DBPRINT == 1
			dbprint_INIT(USART1, 4, true, false); //dbprint Happy Gecko
#endif
#else
#if DEBUG_DBPRINT == 1
			dbprint_INIT(USART1, 3, true, false); //dbprint Custom board
#endif
#endif
			// initialize variables
			windhoos_meas = false;
			counter_CO2 = counter_Soil = 0;

			// power accelerometer
			powerSensor(true, LIS3DH_PORT, LIS3DH_PIN);

#if DEBUG_DBPRINT == 1
			dbprintln("System initialized!");
#endif

			appState = INIT_SENSORS;
		}
			break;

		case INIT_SENSORS: {
			// initialize I²C
			setupIIC();
			//power other sensors
			powerSensor(true, CCS811_PORT, CCS811_PIN);
			delay(40); /* power-up delay of 40ms */
			powerSensor(true, HDC2080_PORT, HDC2080_PIN);
			delay(40);
			powerSensor(true, TMP006_PORT, TMP006_PIN);
			delay(40);

			delay(1000); // init delay

			/*************
			 * init accelerometer
			 *************/
			if (checkHW_ID(LIS3DH_IIC_ADDRESS, LIS3DH_REG_WHOAMI, 0x3333)) {
				init_accelerometer();
				init_interrupt_accel();
				bewegingON = true;
			} else {
				bewegingON = false;

			}
			/*************
			 * init sensor HDC2080
			 *************/
			start_globTemp();
			delay(200);
			if (checkHW_ID(HDC2080_IIC_ADDRESS, HDC2080_HW_ID_LOW, HDC2080_HW_ID_CODE)) {
				temphumON = true;  //co2 sensor is connected

			} else {
				temphumON = false;
				powerSensor(false, HDC2080_PORT, HDC2080_PIN);
			}
			/*************
			 * init sensor CO2
			 *************/
			delay(200);
			start_CO2();
			if (checkHW_ID(CCS811_IIC_ADDRESS, CCS811_HW_ID, 0x8100)) {
				init_Co2();
				co2ON = true;
				powerSensor(false, CCS811_PORT, CCS811_PIN);
			} else {
				co2ON = false; //co2 sensor is not connected
				powerSensor(false, CCS811_PORT, CCS811_PIN);
			}
			/*************
			 * init sensor tree temperature (IR)
			 *************/
			delay(40);
			start_boomTemp();
			if (checkHW_ID(TMP006_IIC_ADDRESS, TMP006_DEVICE_ID, 0x0067)) {
				boomtempON = true; //IR sensor is connected
			} else {
				boomtempON = false; //IR sensor is not connected
				powerSensor(false, TMP006_PORT, TMP006_PIN);
			}

			appState = JOIN;
		}
			break;

		case JOIN: {
			/* Initialize LoRa communication */
			loraStatus = LoRa_Init(loraSettings);

			if (loraStatus != JOINED) {
			// error
			}

			appState = MEASURE;
		}
			break;

		case MEASURE: {
			powerSensor(true, HDC2080_PORT, HDC2080_PIN);
			delay(40);
			powerSensor(true, TMP006_PORT, TMP006_PIN);
			delay(40);

#if CODE_JONGE_BOOM == 1 // define this at the top of this document --> 1 = measure soil moisture
			if (counter_Soil == 0) {
				powerSensor(true, SOIL_PORT, SOIL_PIN);
				delay(40);
				for (int i = 0; i < 4; i++) {
					grondVochtigheid += measure_soil_IIC();
					delay(250);
				}
				grondVochtigheid /= 4; // average over 1s
				powerSensor(false, SOIL_PORT, SOIL_PIN); // turn off power pin soil moisture
			}
			counter_Soil++;

			if (counter_Soil == 24) { //after 24 times 30min = 12 hours measure again
				counter_Soil = 0;
			}
#endif

			if (temphumON) {
				globTemp = 0;
				globHum = 0;
				init_globTemp();
				for (int i = 0; i < 4; i++) { // calculate average value over 1 second with 4 measurements
					measureGlobTemp();
					globTemp += globalTemp;
					globHum += globalHum;
					delay(250);
				}
				globTemp /= 4; //average value
				globHum /= 4; //average value
				globTemp -= 1; //offset
				powerSensor(false, HDC2080_PORT, HDC2080_PIN); // turn off power pin HDC2080
			}

			if (boomtempON) {
				boomTemp = 0;
				for (int i = 0; i < 4; i++) { // calculate average value over 1 second with 4 measurements
					boomTemp += calculateboomTemp();
					delay(250);
				}
				boomTemp /= 4;
				powerSensor(false, TMP006_PORT, TMP006_PIN);
			}

			if (counter_CO2 == 1) {
				powerSensor(true, CCS811_PORT, CCS811_PIN); //power sensor
				delay(40);
				if (co2ON) {
					sleep(20 * A_MINUTE); // sensor has run-in time of 20 min

					Co2 = 0;
					for (int i = 0; i < 10; i++) { // calculate average value over 10 second with 10 measurements
						Co2 += measureCo2();
						delay(1000);
					}
					Co2 /= 10; //average CO2 value
					powerSensor(false, CCS811_PORT, CCS811_PIN);
				}
			}

			counter_CO2++;
			// after 144 times*30 minutes= 3 days, measure again
			if (counter_CO2 == 144) {
				counter_CO2 = 0;
			}

			powerSensor(false, SOIL_PORT, SOIL_PIN);
			powerSensor(false, SDA_PORT, SDA_PIN); // turn off IIC SDA
			powerSensor(false, SCL_PORT, SCL_PIN); // turn off IIC SCL
			appState = SEND;
		}
			break;

		case SEND: {
			/* Send measurement data to "the cloud" */
			// Initialize Buffer
			if (!LPP_InitBuffer(&appData, LPP_BUFFERSIZE)) {
#if DEBUG_DBPRINT == 1
				dbwarn("Buffer niet geinitialiseerd");
#endif
			}

			// Convert Sensor data to LPP format
			globTemp=globHum=boomTemp=25.3;
			Co2=450;
			int16_t globTempLPP = (int16_t) round((globTemp * 10));	//0.1 precision
			uint8_t humidityLPP = (uint8_t) (round(globHum * 2));//0.5 precision
			int16_t boomTempLPP = (int16_t) round((boomTemp * 10)); //0.1 precision
			uint16_t Co2LPP = (uint16_t) (Co2 * 10);			//0.1 precision
			int16_t maxtiltLPP = (int16_t) (maxtilt * 100);		//0.01 precision

			// Fill Buffer
			if (boomtempON) {
				if (!LPP_AddTreeTemperature(&appData, boomTempLPP)) {
				// error
				}
			}
			if (temphumON) {
				if (!LPP_AddAmbientTemperature(&appData, globTempLPP)) { // Add ambient temperature
				//error
				}
				if (!LPP_AddAirHumidity(&appData, humidityLPP)) { // Add air humidity
#if DEBUG_DBPRINT == 1
					dbprintln("Kan luchtvochtigheidsdata niet toevoegen");
#endif
				}
			}

			if (co2ON) {
				if (counter_CO2 == 2) {
					if (!LPP_AddCo2(&appData, Co2LPP)) {		// add CO2 data
#if DEBUG_DBPRINT == 1
						dbwarn("kan co2 niet toevoegen");
#endif
					}
				}
			}

#if CODE_JONGE_BOOM == 1
			if (counter_Soil == 1) {
				grondVochtigheid=25.4;
				uint8_t grondVochtigheidLPP = (uint8_t) (round(grondVochtigheid * 2));
				if (!LPP_AddSoilMoisture(&appData, grondVochtigheidLPP)) { // Add soil moisture
				}
			}
#endif
			if (windhoos_meas == true) { // tilt was measured
				if (!LPP_AddAnalog(&appData, maxtiltLPP)) {
				//error
				}
				//clear variables
				maxtilt = 0;
				windhoos_meas = false;
			}

			/* Send data to TTN*/
			if (LoRa_SendLppBuffer(appData, LORA_UNCONFIRMED) != SUCCESS) {
			//error
			}

			// power LoRa modem off
			LoRa_disable();

			// Go to sleep
			appState = SLEEP;
		}
			break;

		case SLEEP: {
			sleep(WAKE_UP_PERIOD_S); //sleep in EM2
			appState = WAKE_UP;

		}
			break;

		case WAKE_UP: {

			if (RTC_checkWakeup()) {

				RTC_clearWakeup(); /* Clear variable */
				remainingSleeptime = 0; // Reset passed sleeptime since it's an RTC wakeup

				appState = INIT_SENSORS; // Take measurements on "case WAKEUP" exit
			}
			if (bewegingON) {
				if (interrupt_triggered) {
					setupIIC();			// enable I²C
					delay(100);
					RTC_Enable(false); 	//Disable the counter
					appState = WINDHOOS; //measure accelerometer
				}
			}
		}
			break;

		case WINDHOOS: {
			windhoos_meas = true;

			for (int i = 0; i < 25; i++) { // measure 5 seconds with frequency of 5Hz
				measure_Accelerometer(); //measure accelerometer
				delay(180);
			}

			if (maxtilt >= TILT_TRESH) { // send immediately
				/* join network */
				loraStatus = LoRa_Init(loraSettings);
				/* Send data to TTN */
				int16_t maxtiltLPP = (int16_t) (maxtilt * 100);
				LPP_AddAnalog(&appData, maxtiltLPP);
				if (LoRa_SendLppBuffer(appData, LORA_UNCONFIRMED) != SUCCESS) {
				// error
				}
				//clear variables
				maxtilt = 0;
				windhoos_meas = false;
				LoRa_disable(); // power LoRa modem down
			}

			interrupt_triggered = false; // reset triggered variable
			//Disable I²C
			powerSensor(false, SDA_PORT, SDA_PIN);
			powerSensor(false, SCL_PORT, SCL_PIN);
			appState = SLEEP_REMAINING;

		}
			break;

		case SLEEP_REMAINING: {
			remainingSleeptime += RTC_getPassedSleeptime(); /* Add the time spend sleeping to the variable */

			/* Go back to sleep for the remaining time until another measurement (we only take measurements on RTC/button wake-up) */
			sleep(WAKE_UP_PERIOD_S - remainingSleeptime);
			appState = WAKE_UP;
		}
			break;
		}
	}
}
