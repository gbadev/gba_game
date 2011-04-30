#include "sean.h"

// Global Sound Stuff
EWRAM_DATA u32 SampleLength = 0;
EWRAM_DATA u32 SamplePosition = 0;

IWRAM_CODE void MyHandler(void);

//////////////////////////////////////////////////
// Function: MyHandler
// Custom interrupt callback function
//////////////////////////////////////////////////

IWRAM_CODE void MyHandler(void)
{	
	u16 Int_Flag;
	
	//disable interrupts
	REG_IME = 0x00;
	
	//backup the interrupt flags
	Int_Flag = REG_IF;
	
	//look for vertical refresh
	if((REG_IF & INT_VBLANK) == INT_VBLANK)
	{
		//is a sample currently playing?
		/*
		if(SampleLength)
		{
			//increment the position, check if complete
			SamplePosition++;
			if (SamplePosition > SampleLength)
			{
				//stop playback: disable the timer and DMA
				REG_TM0CNT = 0;
				REG_DMA1CNT_H = 0;
				//reset length
				SampleLength = 0;
			}
		}
		*/
		
		if(!SampleLength--)
		{
			//stop playback: disable the timer and DMA
			REG_TM0CNT = 0;
			REG_DMA1CNT_H = 0;
			//reset length
			SampleLength = 0;
		}
		
	}
	
	//restore the interrupt flags
	REG_IF = Int_Flag;
	//enable interrupts
	REG_IME = 0x01;
	
	return;
}