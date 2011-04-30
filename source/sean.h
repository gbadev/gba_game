#ifndef __SEAN__H__
#define __SEAN__H__

#include "TektronicWave.h"

//START: Sean Sound Stuff

#define u16 unsigned short
#define u32 unsigned long

#define REG_TM0D *(volatile u16*)0x4000100
#define REG_TM0CNT *(volatile u16*)0x4000102
#define REG_DMA1SAD *(volatile u32*)0x40000BC
#define REG_DMA1DAD *(volatile u32*)0x40000C0
#define REG_DMA1CNT_H *(volatile u16*)0x40000C6

#define TIMER_ENABLE 0x80
#define DMA_DEST_FIXED 64
#define DMA_REPEAT 512
#define DMA_TIMING_SYNC_TO_DISPLAY 4096 | 8192
#define DMA_ENABLE 0x80000000
#define DMA_32 0x04000000
#define REG_SGCNT0_H *(volatile u16*)0x4000082
#define REG_SGCNT1 *(volatile u16*)0x4000084
#define DSOUND_A_RIGHT_CHANNEL 256
#define DSOUND_A_LEFT_CHANNEL 512
#define DSOUND_A_FIFO_RESET 2048
#define SOUND_MASTER_ENABLE 128

//define some interrupt registers
#define REG_IME *(volatile u16*)0x4000208
#define REG_IE *(u16*)0x4000200
#define REG_IF *(volatile u16*)0x4000202
#define REG_INTERRUPT *(volatile u32*)0x3007FFC
#define REG_DISPSTAT *(volatile u16*)0x4000004
#define INT_VBLANK 0x0001

#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_DATA __attribute__((section(".iwram")))
#define EWRAM_BSS __attribute__((section(".sbss")))

#define EWRAM_CODE __attribute__((section(".ewram"), long_call))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

extern IWRAM_CODE void MyHandler(void);
extern void PlaySound(const PCMSOUND * theSound);
extern void init_sound();

extern u32 SampleLength;
extern u32 SamplePosition;

//END: Sean Sound Stuff
#endif