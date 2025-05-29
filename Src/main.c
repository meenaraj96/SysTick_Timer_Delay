
/*
 * LED - PA5
 */

#include <stdint.h>

#define SYSTICK_LOAD_VALUE 16000

#define CTRL_ENABLE (1U<<0) //Control & Status Register
#define CTRL_CLKSRC (1U<<2) //Control & Status Register
#define CTRL_COUNTFLAG (1U<<16) //Control & Status Register

#define GPIOAEN (1U<<0)
#define LED (1U<<5) //PIN5

void systickdelayms(int delay);

uint32_t *pSTCSR = (uint32_t*)0xe000e010; //Control & Status Register
uint32_t *pSTRVR = (uint32_t*)0xe000e014; //Reload Value Register
uint32_t *pSTCVR = (uint32_t*)0xe000e018; //Current Value Register
uint32_t *pSTCR  = (uint32_t*)0xe000e01c; //Caliberation Value Register

uint32_t *pAHB1ENR = (uint32_t*)0x40023830;
uint32_t *pGPIOAMODER = (uint32_t*)0x40020000;
uint32_t *pGPIOAODR = (uint32_t*)0x40020014;

void systickdelayms(int delay)
{
	*pSTRVR |= SYSTICK_LOAD_VALUE; //Reload Value Register

	*pSTCVR |= 0; //Current Value Register

	//Control & Status Register
	*pSTCSR |= CTRL_ENABLE; //enable the systick counter
	*pSTCSR |= CTRL_CLKSRC; //select processor clock

	for (int i=0;i<delay;i++)
	{
		while((*pSTCSR & CTRL_COUNTFLAG)==0){} //COUNTFLAG Returns 1 if timer counted to 0
	}

	*pSTCSR |= 0; //Control & Status Register
}


int main(void)
{
	*pAHB1ENR |= GPIOAEN; //Enable GPIOA
	//clear and set MODER - 01: General purpose output mode
	*pGPIOAMODER &= ~(1U<<11);
	*pGPIOAMODER |=  (1U<<10);

	while(1)
	{
		*pGPIOAODR ^= LED;
		systickdelayms(500);
	}
}
