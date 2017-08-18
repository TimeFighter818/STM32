#include "action.h"

uint8_t nIRData,nIRData_Last;
void action_GoStraight(unsigned char nGrid)
{
	unsigned char nGridPassed;
	nGridPassed = 0;
	while(nGridPassed<nGrid)
	{
		nIRData = 0;
		nIRData = nIRData | (GPIO_ReadInputData(GPIOA)>>13);
		nIRData = nIRData | ((GPIO_ReadInputData(GPIOC)>>7) & 0x38);
		
		if(nIRData & 0x02)
			DCMotor_SetSpeed(30,38,0,0);
		else if(nIRData & 0x08)
			DCMotor_SetSpeed(38,30,0,0);
		else if(nIRData & 0x04)
			DCMotor_SetSpeed(30,30,0,0);
		else
			DCMotor_SetSpeed(0,0,0,0);
		
		if((nIRData&0x1F) == 0x1F && (nIRData_Last&0x1F) != 0x1F)
			nGridPassed++;
		nIRData_Last = nIRData;	
	}
	DCMotor_SetSpeed(0,0,0,0);
}
void action_Turn90(int8_t nTurn90)
{
	unsigned char nTurnPassed;
	nTurnPassed = 0;
	while(nTurnPassed<abs(nTurn90))
	{
		nIRData = 0;
		nIRData = nIRData | (GPIO_ReadInputData(GPIOA)>>13);
		nIRData = nIRData | ((GPIO_ReadInputData(GPIOC)>>7) & 0x38);
		if(nTurn90>0)
			DCMotor_SetSpeed(-10,25,0,0);
		else if(nTurn90<0)
			DCMotor_SetSpeed(25,-10,0,0);
		else
			break;
		
		if((nIRData&0x04) && !(nIRData_Last&0x04))
			nTurnPassed++;
		nIRData_Last = nIRData;	
	}

	DCMotor_SetSpeed(0,0,0,0);	
	
}
