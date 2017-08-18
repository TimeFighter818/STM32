#ifndef __ACTION_H__
#define __ACTION_H__
#include "stm32f10x.h"
#include "DCMotor.h"

void action_GoStraight(unsigned char nGrid);
void action_Turn90(int8_t nTurn90);


#endif
