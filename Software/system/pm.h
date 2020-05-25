/*
 *  Created on: 17-03-2019
 *      Author: Simon Perneel
 *  Description: header file for pm.c
 */

#ifndef _PM_H_
#define _PM_H_

typedef enum PM_subsystems{
	PM_RN2483,
} PM_SubSystem_t;

void PM_Init(void);

void PM_Enable(PM_SubSystem_t pmss);

void PM_Disable(PM_SubSystem_t pmss);

#endif /* _PM_H_ */
