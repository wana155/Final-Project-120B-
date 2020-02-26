
/*
 *    Name: Arturo Perez E-mail: apere129@ucr.edu
 *    Lab Section: 022
 *    Custom Project: Tetris Game
 *    Exercise Description: This program implements the Tetris Game on the ATmega 1284 micro controller.
 *
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

//version3-13.tarde.1

#include <avr/io.h>
#include "io.c"
#include "nokia5110.c"
#include "Tetris_Game_Functions.h"

//=======================Struct/Varibales==========//
typedef struct task {
    int state; // Current state of the task
    unsigned long period; // Rate at which the task should tick
    unsigned long elapsedTime; // Time since task's previous tick
    int (*TickFct)(int); // Function to call for task's tick
} task;

task tasks[5];

const unsigned char tasksNum = 5;
const unsigned long tasksPeriodGCD = 5;
const unsigned long lCDPeriod =50; 
const unsigned long JoystickPeriod=50;
const unsigned long ButtonPeriod=50;
const unsigned long ScreenSpacePeriod=25;
const unsigned long LevelSpeedPeriod=500;




//=======================Timer/Task scheduler===============================//
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
    // AVR timer/counter controller register TCCR1
    TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
    OCR1A = 125;    // Timer interrupt will be generated when TCNT1==OCR1A
    TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt
    TCNT1=0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
    TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
//void TimerISR() { TimerFlag = 1; }
void TimerISR() {
    unsigned char i;
    for (i = 0; i < tasksNum; ++i) {                     // Heart of the scheduler code
        if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriodGCD;
    }
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
    // CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
    _avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
    if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
        TimerISR(); // Call the ISR that the user uses
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
	//ADMUX |= (1<<MUX0);
}

unsigned char Press,resetG;
unsigned char Bcnt;
//=======================BUTTON PRESS SM==========================//
/* This State Machine reads button presses for the game and reset functionality.
 It sets global flags for communication with other SM's on the program. */
enum Button{Bstart,Binit, BPressed,Bwait};
	unsigned char TEMPA=0x00;
	int Button_Tick(int Bstate){            // Switch statement for transitions
		switch(Bstate){
			case Bstart:
			Bcnt=0;
			resetG=0;
			Bstate=Binit;
			break;
			
			case Binit:
	 		TEMPA=~PINA & 0x0C;
			 if (TEMPA==0x08)
			 { resetG=1;   }
			 if (TEMPA==0x04)
			 {Bstate=BPressed;}
			break;
			
			case BPressed:
			Bcnt=0;
			Bstate=Bwait;
			break;
			
			case Bwait:
		    if (Bcnt>1)
			{Bstate=Binit;}
		    else{Bstate=Bwait;}
			break;
			
			default:
            Bstate=Bstart;
			break;
		}
		switch(Bstate){            // Switch statement for Actions
			case Bstart:
			break;
			
			case Binit:
			Press=0;
			break;
			
			case BPressed:
			Press=1;
			break;
			
			case Bwait:
			Bcnt=Bcnt+1;
			break;
			
			default:
			break;
		}
		return Bstate;
		};
		
//============================ Joystick SM =============================//
/*This State Machine reads the ADC ports A0 and A1 and compares analog readings
 to different thresholds in order to define directions of the Joystick */
unsigned char move=0;
enum Joystick{JS_start,read_x,read_y} ;
unsigned long JSXcor,JSycord;
int Joystick_Tick(int Jstate) {
	
	switch(Jstate) {             // Switch statement for transitions
		case JS_start:
		Jstate = read_x;
		break;
		
		case read_x:
		ADMUX=0x00;
		Jstate = read_y;
		break;
		
		case read_y:
		ADMUX=0x01;
		Jstate=read_x;
		break;
            
        default:
        Jstate=JS_start;
        break;
	}
	switch (Jstate) {           //State Actions
		case JS_start:
		break;
	
		case read_x:
		JSXcor=ADC;
		if (JSXcor>927)
		{// LCD_DisplayString(1,"UP");  
		}
		else if (JSXcor<100)
		{move=1;}
		else if(JSycord<=927&&JSycord>=100)
		{ move=0;}
		break;
		
		case read_y:
		JSycord=ADC;
		if (JSycord>927)
		{ move=3;}
		else if (JSycord<100)
		{ move=2;}
		else { }
		break;
	}
	return Jstate;
}

//========================= Screen Space =================//
/* This State Machine is the main driver of the game.
 It reads inputs from other SM's to calculates the position of the shapes on
 the screen at any given time. The SM also calculates positions after changes
 in location and face of the shape. It keeps track of the squares left before
 coming in contact with another shape or boundary. Once the shape reaches its
 destination, the shape is aded to a two dimensional array (vertical) that
 represents the occupied squares on the game screen. Then, a new shape is created
 with a randomizer function. The State Machine also handles the gameOver state,
 closing animation screen, and a screen saver for waiting times.*/

signed char squaresLeft;
unsigned char Lcnt;
unsigned char v=0;
unsigned char l=7;
unsigned char gameOver=0;
unsigned char promt=0;
unsigned char Begin=0;

enum ScreenSpace{SS_start,SS_BeginGame,SS_new,SS_calculate,SS_Dropdown,SS_save,SS_closeDown,SS_gameover};
int ScreenSpace_Tick(int SSstate){
	switch(SSstate){                    // Switch statement for transitions
		case SS_start:
		nokia_lcd_clear();
		Tetris_screen_saver();
		SSstate = SS_BeginGame;
		break;
		
		case SS_BeginGame:
		if (Press)
		{SSstate=SS_new;
		Press=0;
		promt=0;
		Begin=1;
        //==Erase Verticall==//
        for (int i=0;i<8;i++)
        {for(int j=0;j<12;j++)
        {vertical[i][j]=0;}}}
		else{SSstate=SS_BeginGame;}
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_new:
		SSstate = SS_calculate;
		Lcnt=0;
		squaresLeft=12;
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_calculate:
		SSstate=SS_Dropdown;
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_Dropdown:
		if (squaresLeft>-1 && !move && !Press)
		{SSstate=SS_Dropdown;}
		else if (squaresLeft>-1 && (move||Press))
		{SSstate=SS_calculate;}
		else {SSstate=SS_save;}
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_save:
		if (gameOver)
		{SSstate=SS_closeDown;
		Lcnt=0;
		v=0;
		l=7;}
		else{SSstate=SS_new;
		score++;}
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_closeDown:
		if (Lcnt<32)
		{SSstate=SS_closeDown;}
		else{Tetris_screen_saver();
		SSstate=SS_gameover;}
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		case SS_gameover:
		if (Press)
		{SSstate=SS_BeginGame;
		Press=0;
		gameOver=0;
		Begin=1;}
		else{
		SSstate=SS_gameover;}
		if (resetG)
		{SSstate=SS_start;	}
		break;
		
		default:
		SSstate=SS_start;
		break;
	}
	switch(SSstate){               // Switch statement for Actions
		case SS_start:
        nokia_lcd_clear();
        Tetris_screen_saver();
		break;
		
		case SS_BeginGame:
		break;
		
		case SS_new:
		Tetris_makeShape(7);
		break;
		
		case SS_calculate:
		if (move==1)    //Move shape down
		{ if (squaresLeft>0)
			{current.Row+=1;
			squaresLeft=12;
		for (int i=0;i<current.colums_used;i++)      // Recalculate the the postion of the shape after moving down
		{squaresLeft=Tetris_min(squaresLeft,12-Tetris_Bottom_up(i+current.Colunm)-current.Row-used(i));}}
		}
		else if (move==2)//Move Shape Left
		{squaresLeft=12;
		if(current.Colunm+(current.colums_used-1)<7 && Tetris_safe_L_Move(current.Colunm,current.Row)){current.Colunm+=1;}//Check if it is safe to move left
		for (int i=0;i<current.colums_used;i++)    // Recalculate the the postion of the shape after moving left
		{squaresLeft=Tetris_min(squaresLeft,12-Tetris_Bottom_up(i+current.Colunm)-current.Row-used(i));}
		}
		else if (move==3)//Move Shape Right
		{squaresLeft=12;
		if (current.Colunm>0 && Tetris_safe_R_Move(current.Colunm,current.Row)){current.Colunm-=1;}//Check if it is safe to move right
		for (int i=0;i<current.colums_used;i++)// Recalculate the the postion of the shape after moving right
		{squaresLeft=Tetris_min(squaresLeft,12-Tetris_Bottom_up(i+current.Colunm)-current.Row-used(i));}
		}
		else if (Press)  //Rotae/change the shape's face
		{ squaresLeft=12;
        if (current.face==4){current.face=1;}
        else{current.face+=1;}
        changeFace(current.type,current.face);
        for (int i=0;i<current.colums_used;i++)// Recalculate the the postion of the shape after shifting face
        {squaresLeft=Tetris_min(squaresLeft,12-Tetris_Bottom_up(i+current.Colunm)-current.Row-used(i));}
        Press=0;}
		else{squaresLeft=13;
		for (int i=0;i<current.colums_used;i++)// Calculate the the postion after creating a new shape
		{squaresLeft=Tetris_min(squaresLeft,12-Tetris_Bottom_up(i+current.Colunm)-current.Row-used(i));}
		}
		move=0;
		break;
		
		case SS_Dropdown:
		if (Lcnt<speedLevel)
		{Lcnt++;}
		else{Lcnt=0;
		if (squaresLeft>0)
		{current.Row+=1;}
		squaresLeft--;
		Lcnt++;}
		break;
		
		case SS_save:
		// Fill in the array with the shape
		for (int i=0;i<current.colums_used;i++)
		{ for (int j=0;j<4;j++)
			{if (current.shapeA[i][j])
				{vertical[i+current.Colunm][j+current.Row]=1;}}}
		//check for complete lines//
		Tetris_check();
		//check for game end//
		for (int i=0;i<8;i++)
		{if (vertical[i][0]){gameOver=1;}}
		break;
		
		case SS_closeDown:
        // close/swipe the screen animation//
		if (Lcnt%2==0)
		{ if (v<8)
		{ for (int i=0;i<12;i++)
		  { vertical[v][i]=1; }
	     v++;}
		 else {for (int i=0;i<12;i++)
        { vertical[l][i]=0; }
        l--;
        promt=1;}}
		Lcnt++;
		break;
		
		case SS_gameover:
		break;
		
		default:
		break;
	}
	return SSstate;
	};

//================= Speed Level =================//
/* This State Machine controls the prompts on the second screen
 as well of displaying the score and level */

enum speedState{LS_start,LS_Begin,LS_Gamelevel,LS_over};

int  LS_Tick(int LSstate){
	switch(LSstate){                      //Switch for transitions
		case LS_start:
		LSstate=LS_Begin;
		resetG=0;
		break;
		
		case LS_Begin:
		if (Begin)
		{LSstate=LS_Gamelevel;
		score=0;
		level=1;
		speedLevel=30;
		Begin=0;}
		else
		{LSstate=LS_Begin;}
	if (resetG)
		{LSstate=LS_start;	}
		break;
		
		case LS_Gamelevel:
		if (gameOver)
		{LSstate=LS_over;}
		else{LSstate=LS_Gamelevel;}
		if (resetG)
		{LSstate=LS_start;	}
		break;
		
		case LS_over:
		if(Begin)
		{LSstate=LS_Begin;
		Begin=0;}
		else{LSstate=LS_over;}
		if (resetG)
		{LSstate=LS_start;	}
		break;
		
		default:
		LSstate=LS_start;
		break;
	}
	switch(LSstate){                     //Switch for State Actions
		case LS_start:
		break;
		
		case LS_Begin:
		LCD_DisplayString(1,"     TETRIS     ->PUSH TO START"); 
		break;
		
		case LS_Gamelevel:
		LCD_DisplayString(1,"SCORE:          LEVEL:");
		Tetris_score(8);
		LCD_Cursor(24);
		LCD_WriteData(level+'0');
		break;
		
		case LS_over:
		LCD_DisplayString(1,"  GAME OVER!    FINAL SCORE:   ");
		Tetris_score(29);
		break;
							
		default:
		break;
		}
return LSstate;
}
//=================LCD SM============//
/* This State Machine updates the Nokia 5110 LCD screen
 with the shapes and their current locations on the screen*/

enum LeState{Lstart,Print};

int  LCD_Tick(int state){
    switch(state){                      //Switch for transitions
            
        case Lstart:
			state=Print;
            break;
            
        case Print:
          state=Print;
            break;
            
        default:
            state=Lstart;
            break;
    }
    switch(state){                     //Switch for State Actions
        case Lstart:
            break;
            
        case Print:
		
		    nokia_lcd_clear();
	if (!promt)
{	for (int i=0;i<4;i++)            // Print the current shape
    { for (int j=0;j<4;j++)
    { if (current.shapeA[i][j])
    { int8_t c=Xlength*(j+current.Row);
    int8_t r=Ylength*(i+current.Colunm);
    if (c>=0)
    {Tetris_Draw_A_Sqaure(c,r);}}}}
		
    for (int i=0;i<8;i++)           //Print the rest of the squares in the game
    { for (int j=0;j<12;j++)
    {  if (vertical[i][j])
    {uint8_t c=Xlength*j;
    uint8_t r=Ylength*i;
    Tetris_Draw_A_Sqaure(c,r);}}}
}
	
else{for (int i=0;i<8;i++)
{ for (int j=0;j<12;j++)           // If promt, Print the contents of vertical, usually the screen saver.
{  if (vertical[i][j])
{uint8_t c=Xlength*j;
uint8_t r=Ylength*i;
Tetris_Draw_A_Sqaure(c,r);}}}}
			
nokia_lcd_render();
break;
            
        default:
        break;}
    return state;
}

int main(void)
{

    //=============== Initialize Structure With Tasks================///
    unsigned char i=0;
    tasks[i].state = JS_start;
    tasks[i].period = JoystickPeriod;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Joystick_Tick;	
	i++;
	tasks[i].state = Bstart;
	tasks[i].period = ButtonPeriod;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Button_Tick;
	i++;
	tasks[i].state = SS_start;
	tasks[i].period = ScreenSpacePeriod;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &ScreenSpace_Tick;
	i++;	
    tasks[i].state = Lstart;
    tasks[i].period = lCDPeriod;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &LCD_Tick;
	i++;
	tasks[i].state = LS_start;
	tasks[i].period = LevelSpeedPeriod;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &LS_Tick;
    
	
    DDRA = 0x00; PORTA = 0x0C; // Set A2-A3 for button input
    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines
   	
   	nokia_lcd_init();
   	nokia_lcd_clear();
   	
   	LCD_init();  
	ADC_init();		
    
    TimerSet(tasksPeriodGCD);
    TimerOn();
    
    while(1) {}
}
