/*
 *         File: my_lora_device.h
 *      Created: 2020-02-18
 *       Author: Simon Perneel
 *               modified version from DRAMCO EFM32-RN2483-LoRa-Node
 *  Description: configuration of LoRaWAN parameters
 */

#ifndef _MY_LORA_DEVICE_H_
#define _MY_LORA_DEVICE_H_


/* OVER_THE_AIR_ACTIVATION:
 * 	- set to 1 to use the Over-the-Air activation procedure -- default SF
 * 	- set to 0 to use the Personalization activation procedure -- not recommended
 */
#define OVER_THE_AIR_ACTIVATION	  	1
#define DEFAULT_DATA_RATE			SF12_BW125



/*****************************************************************/

#if OVER_THE_AIR_ACTIVATION == 1
// Copy your settings here
/* 64-bit device ID */
#define LORAWAN_DEVICE_EUI			"00AFABD948590349"
/* 64-bit application ID */
#define LORAWAN_APPLICATION_EUI     "70B3D57ED002BC0E"
/* 128-bit AES key */
#define LORAWAN_APPLICATION_KEY     "8CEAA2F9ADA9F48984AC57715327C0EA"
// Don't change
#define JOIN_MECHANISM				OTAA

#elif OVER_THE_AIR_ACTIVATION == 0
// Copy your settings here
/* 32-bit device address */
#define LORAWAN_DEVICE_ADDRESS     	"26011074"
/* 128-bit AES key */
#define LORAWAN_NWKSKEY             "A6B13497CF7698BFD33841E1F9ABA9E0"
/* 128-bit AES key */
#define LORAWAN_APPSKEY             "468300457A2ED92DD7F8392034DF0AFA"
// Don't change
#define JOIN_MECHANISM				ABP
#else
#error "OVER_THE_AIR_ACTIVATION has the wrong value"
#endif


/*****************************************************************************************
 * DO NOT CHANGE !
 *****************************************************************************************/
#if OVER_THE_AIR_ACTIVATION == 1
#define LORA_INIT_MY_DEVICE                                         \
{	JOIN_MECHANISM,						/* Activation mechanism */	\
	DEFAULT_DATA_RATE,					/* Data Rate on start-up */ \
	LORAWAN_DEVICE_EUI,					/* Device EUI */            \
	LORAWAN_APPLICATION_EUI,			/* Application EUI */       \
	LORAWAN_APPLICATION_KEY,			/* Application key */       \
	"00000000",							/* Device address */ 		\
	"00000000000000000000000000000000",	/* Network session key*/ 	\
	"00000000000000000000000000000000",	/* App session key*/ 		\
}
#else
#define LORA_INIT_MY_DEVICE                                         \
{	JOIN_MECHANISM,						/* Activation mechanism */	\
	DEFAULT_DATA_RATE,					/* Data Rate on start-up */ \
	"",                 				/* Device EUI */            \
	"",                					/* Application EUI */       \
	"",									/* Application key */       \
	LORAWAN_DEVICE_ADDRESS,				/* Device address */ 		\
	LORAWAN_NWKSKEY, 					/* Network session key*/ 	\
	LORAWAN_APPSKEY, 					/* App session key*/ 		\
}
#endif

#endif /* _MY_LORA_DEVICE_H_ */
