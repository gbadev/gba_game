#include "sean.h"
#include "TektronicWave.h"

void init_sound()
{
	//create custom interrupt handler for vblank (FOR SOUND STUFF)
	REG_IME = 0x00;
	REG_INTERRUPT = (u32)MyHandler;
	REG_IE |= INT_VBLANK;
	REG_DISPSTAT |= 0x08;
	REG_IME = 0x01;
}

void PlaySound(const PCMSOUND *theSound)
{
	u16 samples;
	
	//output to both channels and reset the FIFO
	REG_SGCNT0_H = DSOUND_A_RIGHT_CHANNEL | DSOUND_A_LEFT_CHANNEL | DSOUND_A_FIFO_RESET;
	
	//enable all sound
	REG_SGCNT1 = SOUND_MASTER_ENABLE;
	
	//DMA1 source address
	REG_DMA1SAD = (u32)(theSound->pName);
	
	//DMA1 destination address
	REG_DMA1DAD = 0x40000A0;
	
	//write 32 bits into destination every vblank
	REG_DMA1CNT_H = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_TIMING_SYNC_TO_DISPLAY | DMA_ENABLE;
	
	//set the sample rate
	samples = 16777216 / (theSound->nSamplingRate);
	REG_TM0D = 65536 - samples;
	
	//keep track of the playback position and length
	SampleLength = (theSound->nLength) / samples * 15.57;
	
	//enable the timer
	REG_TM0CNT = TIMER_ENABLE;
	
	return;
}