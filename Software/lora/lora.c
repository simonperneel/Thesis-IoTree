/***************************************************************************//**
 * @file lora.c
 * @brief LoRa methods for RN2483 modem
 * @version 2.0
 * @author
 *   Simon Perneel
 *
 * ******************************************************************************
 * @section Versions
 *
 *   @li v1.0: file from DRAMCO EFM32-RN2483-LoRa-Node
 *   @li v2.0: added some methods: LoRa_disable: turn modem completely off when sleeping => rejoin needed after wakeup
 *   							 : SF increases when gateway not in range
 *
 * ****************************************************************************/

#include "system.h"
#include "rn2483.h"
#include "lpp.h"
#include "lora.h"
#include "dbprint.h"
#include "debug_dbprint.h"
#include "gpio.h"
#include "pin_mapping.h"
#include "em_leuart.h"
#include "pm.h"


#define LORA_BUFFERSIZE		128
#define MAX_JOIN_RETRIES	3		//max join retries at certain SF

char loraReceiveBuffer[LORA_BUFFERSIZE];
bool firstjoin = true;

LoRaStatus_t LoRa_Init(LoRaSettings_t init) {

	PM_Enable(PM_RN2483);  //power RN2483 Modem

	/*after first join, ABP is used when modem is powered */
	/* this is not really a join; it just loads the AppSKey, NwkSKey and DevAddr from the first OTAA join*/
	if (!firstjoin) {
		init.activationMethod = ABP;
	}
	RN2483_Init();   // init LEUART with RN2483

	/* join network */
	int retries = 0;
	while (retries <= MAX_JOIN_RETRIES) {
		if (RN2483_Setup(init, loraReceiveBuffer, LORA_BUFFERSIZE)== JOIN_ACCEPTED) {
			if (firstjoin){
				RN2483_SaveMac(loraReceiveBuffer, LORA_BUFFERSIZE); // save mac settings to EEPROM after first OTAA join
			}
			firstjoin = false;

			break;
		}

		delay(500);
		retries++;

		if (retries == MAX_JOIN_RETRIES) {
			retries = 0;
			init.dataRate--;  //try a higher SF
			if (init.dataRate == SF12_BW125) {

				return ERROR;
			}
		}
	}

	return JOINED;
}

LoRaStatus_t LoRa_SendLppBuffer(LPP_Buffer_t b, bool ackNoAck){
	if(ackNoAck == LORA_CONFIRMED){ // not tested yet !!
		if(RN2483_TransmitConfirmed(b.buffer, b.fill, loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_RX){
			return ERROR;
		}
		return SUCCESS;
	}
	else{
		if(RN2483_TransmitUnconfirmed(b.buffer, b.fill, loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_TX_OK){
			return ERROR;
		}
		return SUCCESS;
	}
}

void LoRa_Sleep(uint32_t durationMs, volatile bool * wakeUp){
	RN2483_Sleep(durationMs, wakeUp, loraReceiveBuffer, LORA_BUFFERSIZE);
}

LoRaStatus_t LoRa_WakeUp(void){
	if(RN2483_Wake(loraReceiveBuffer, LORA_BUFFERSIZE) != MAC_OK){
		return ERROR;
	}
	return SUCCESS;
}

void LoRa_disable(void){
	LEUART_Reset(RN2483_UART);
	LEUART_Enable(RN2483_UART, leuartDisable);
	PM_Disable(PM_RN2483);
	GPIO_PinOutClear(RN2483_RESET_PORT, RN2483_RESET_PIN);
	GPIO_PinOutClear(RN2483_RX_PORT, RN2483_RX_PIN);
	GPIO_PinOutClear(RN2483_TX_PORT, RN2483_TX_PIN);
}




