#ifndef _GLOBAL_H_
#define	_GLOBAL_H_

#define	ioRMIN			FP00

//#define	ioChannel		FP02
//#define	ioPower			FP03
//#define ioSCLK 			FP45
//#define ioIO 				FP44
//#define ioRST 			FP43


#define P_SCLK      FP43
#define P_IO 				FP44
#define P_RST 			FP45


#define ioKey1			FP01
#define ioKey2			FP02
#define ioKey3			FP03
#define ioLed				FP47

#define	ioSensor		FP46
#define	ioWLamp			FP05
#define	ioCLamp			FP06
#define	ioBeep			FP07
#define	ioNightLight	FP04
//#define LED_BLUE		FP42
#define LED_BLUE		FP40
#define LED_RED			FP41
//#define LED_GREEN		FP40
#define LED_GREEN		FP42

#define MODE_OFF			0x0
#define	MODE_ON				0x1
#define MODE_NIGHT		0x2
#define MODE_COLOR		0x3
#define MODE_ALL			0x4

#define COM_MAX				6

#define	SEG_MAX				200
#define SEG2_MAX			78

#define	COLOR_JUMP				0x0
#define	COLOR_JUMP_HOLD		0x1
#define COLOR_CHANGE			0x2
#define COLOR_CHANGE_HOLD 0x3


#define COLOR_RED			0
#define COLOR_ORANGE	1
#define COLOR_GREEN		2
#define COLOR_DINGO		3
#define	COLOR_BLUE		4
#define COLOR_PUPLE		5
#define COLOR_WHITE		6
#define Read_Second     0x81


extern BYTE D_Seg,D_Seg2,D_Com,D_Mode,C_Mode,Cool_Old,Warm_Old,Ad_Start,Second,D_Seg3,N_Mode,Night_Step,S_Mode;
//extern bit F_Color_Renew,F_Auto_Sleep;
#define	feeddog()	WDTR = 0x5a
void ms_delay(BYTE ms);
#endif