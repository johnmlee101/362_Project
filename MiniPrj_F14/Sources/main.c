/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014                    
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < 12 >

 Project Name: < Candy Dispenser >

 Team Members:

   - Team/Doc Leader: < Kyunghoon Jung >      Signature: Kyunghoon Jung
   
   - Software Leader: < John Lee >      Signature: John Lee

   - Interface Leader: < Josh Wrobleski >     Signature: Josh Wroblewski

   - Peripheral Leader: < Kyunghoon Jung >    Signature: Kyunghoon Jung


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to .... < ? >


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

***********************************************************************

  Date code started: < ? >

  Update history (add an entry every time a significant change is made):

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >


***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void setPosition(int x);
void bco(char x);		// SCI buffered character output
void disp(void);
void shiftout(char);	// LCD drivers (written previously)
void lcdwait(void);
void send_byte(char);
void send_i(char);
void chgline(char);
void print_c(char);
void pmsglcd(char[]);
int getValue(int port);
void delay(void);


/* Variable declarations */
int servoPosition = 0;
int dispenseHold = 0;
int mmHold = 0;
int key[4] = {0,0,0,0};
int currentKey = 0;
int rightPin = 0;
int wrongPin = 0;
int reset = 0;
//int column1 = 0;
//int column2 = 0;
//int column3 = 0;
int RTICNT = 0;
int lastKey = 0;
int defa[4] = {1,2,3,4};
int newCounter = 0;
   	   			 		  			 		       

/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 = 0x80	// LCD line 1 cursor position
#define LINE2 = 0xC0	// LCD line 2 cursor position

//#define row4 PTT_PTT1
#define row3 PTT_PTT5
#define row2 PTT_PTT6
#define row1 PTT_PTT7
#define row4 PTT_PTT1



	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

/* Initialize peripherals */
            
/* Initialize interrupts */

//RTI Initialisations for 2.048 ms
  RTICTL = 0x41;
  CRGINT = CRGINT | 0x80;
  
//PWM initializations
  MODRR_MODRR0 = 0x01;
  PWME_PWME0 = 0x01;
  PWMPOL_PPOL0 = 0x01;
  PWMPER0 = 0xFF;
 	PWMDTY0 = 0x00;
 	PWMPRCLK = 0x07;
 	PWMCLK = 0x01;
 	PWMSCLA = 0x07;
  
//SPI Baud rate 6Mbs
  DDRM   = 0xFF;
  SPICR1 = 0x50;
  SPICR2 = 0x00;
  SPIBR  = 0x01;
  
//LCD initializations
  PTT_PTT4 = 1;
  PTT_PTT3 = 0;
  send_i(LCDON);
  send_i(TWOLINE);
  send_i(LCDCLR);
  lcdwait();

  ATDCTL2 = 0x80;
  ATDCTL3 = 0x10;
  ATDCTL4 = 0x85;
  
  DDRT = 0xFF;
  
	      
	      
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  	DisableInterrupts
	initializations(); 		  			 		  		
	EnableInterrupts;
  reset = 1;
  disp();
  row1 = 0;
  row2 = 0;
  row3 = 0;
  //row4 = 1;
 for(;;) {
  
/* < start of your main loop > */ 
  
  

  
   } /* loop forever */
   
}   /* do not leave main */




/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flagt 
  	CRGFLG = CRGFLG | 0x80;

    if(RTICNT > 71){
      RTICNT = 0;
      
      if(dispenseHold <= 0) {
        setPosition(0); 
      row1 = 1;
      delay();
      ATDCTL5 = 0x00;
      while(!ATDSTAT0_SCF) {
      }
      //delay();
      
      //THIS BLOCK OF CODE IS SCANNING FOR THE KEYPAD
      
      if(ATDDR0H >= 0xD6 && lastKey != 1)
      {
        key[currentKey] = 1;
        currentKey++;
        lastKey = 1;
        
      }
      ATDCTL5 = 0x01;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR1H >= 0xD6 && lastKey != 2)
      {
        key[currentKey] = 2;
        currentKey++;
        lastKey = 2;
      }        
      ATDCTL5 = 0x02;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR0H >= 0xD6 && lastKey != 3)
      {
        key[currentKey] = 3;
        currentKey++;
        lastKey = 3;
      }
      row1 = 0;
      delay();
      row2 = 1;
      delay();
      ATDCTL5 = 0x00;
      while(!ATDSTAT0_SCF) {
      }
      
      if(ATDDR0H >= 0xD6 && lastKey != 4)
      {
        key[currentKey] = 4;
        currentKey++;
        lastKey = 4;
      }
      ATDCTL5 = 0x01;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR1H >= 0xD6 && lastKey != 5)
      {
        key[currentKey] = 5;
        currentKey++;
        lastKey = 5;
      }
      ATDCTL5 = 0x02;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR0H >= 0xD6 && lastKey != 6)
      {
        key[currentKey] = 6;
        currentKey++;
        lastKey = 6;
      }
      row2 = 0;
      delay();
      row3 = 1;
      delay();
      ATDCTL5 = 0x00;
      while(!ATDSTAT0_SCF) {
      }
      
      if(ATDDR0H >= 0xD6 && lastKey != 7)
      {
        key[currentKey] = 7;
        currentKey++;
        lastKey = 7;
      }
      ATDCTL5 = 0x01;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR1H >= 0xD6 && lastKey != 8)
      {
        key[currentKey] = 8;
        currentKey++;
        lastKey = 8;
      }
      ATDCTL5 = 0x02;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR0H >= 0xD6 && lastKey != 9)
      {
        key[currentKey] = 9;
        currentKey++;
        lastKey = 9;
      }
      row3 = 0;
      delay();
      
      
      
      row4 = 1;
      delay();
      ATDCTL5 = 0x00;
      while(!ATDSTAT0_SCF) {
      }
      
      if(ATDDR0H >= 0xD6 && lastKey != 10)
      {
        //key[currentKey] = 10;
        key[0] = 0;
        key[1] = 0;
        key[2] = 0;
        key[3] = 0;
        currentKey = 0;
        lastKey = 10;
      }
      ATDCTL5 = 0x01;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR1H >= 0xD6 && lastKey != 0)
      {
        key[currentKey] = 0;
        currentKey++;
        lastKey = 0;
      }
      ATDCTL5 = 0x02;
      while(!ATDSTAT0_SCF) {
      }
      if(ATDDR0H >= 0xD6 && lastKey != 11 && key[0] == 2 && key[1] == 0 && key[2] == 1 && key[3] == 4)
      {
        currentKey = 0;
        newCounter = 1;
        lastKey = 11;
      } 
      row4 = 0;
      delay();
      
      
      if(newCounter == 1 && currentKey == 4) {
         defa[0] = key[0];
         defa[1] = key[1];
         defa[2] = key[2];
         defa[3] = key[3];
         key[0] = 0;
         key[1] = 0;
         key[2] = 0;
         key[3] = 0;
         newCounter = 0;
      }
      
      
      if(currentKey >= 4) {

       currentKey = 0; 
       wrongPin = 1;
       disp();
      }
      
      
      //if keypad has right code 
      if(key[0] == defa[0])
      {
        outchar('1');
        if(key[1] == defa[1])
        {
          outchar('2');
          if(key[2] == defa[2])
          {
            outchar('3');
            if(key[3] == defa[3])
            {
              outchar('4');
              setPosition(2);
              dispenseHold = 50; 
              rightPin = 1;
              key[0] = 0;
              key[3] = 0;
              disp(); 
            }
          }
        }
      } 
      
      
      } else {
       dispenseHold--; 
      }
      
      
      
    }
    RTICNT++;


}

/*
***********************************************************************                       
  TIM interrupt service routine	  		
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
  	// clear TIM CH 7 interrupt flag 
 	TFLG1 = TFLG1 | 0x80; 
 

}

/*
***********************************************************************                       
  SCI interrupt service routine		 		  		
***********************************************************************
*/

interrupt 20 void SCI_ISR(void)
{
 


}

/*
***********************************************************************
 Character I/O Library Routines for 9S12C32 
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
***********************************************************************
*/

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}


/*
***********************************************************************                       
  Set the new servo position	 		  		
***********************************************************************
*/

void setPosition(int x) {
    //for x, 0 = -90, 1 = 0; 2 = 90;
    if(x == 0){
     //set PWM to 1ms
     PWMDTY0 = 255/23;   //19
    } else if (x == 1){
     //set PWM to 1.5ms
     PWMDTY0 = (255/19)*1.5; 
    } else {
     //set PWM to 2ms
     PWMDTY0 = 255/9; 
    }
  
}


/*
***********************************************************************                       
  Set the new servo position	 		  		
***********************************************************************
*/
int getValue(int port) {  
    //port 1 is AN00 port2 is AN01 port3 is AN02
    ATDCTL5 = 0x10;
    
    while(!ATDSTAT0_SCF) {
    }
    outchar(ATDDR0H);
    if(port == 1) {
      if(ATDDR0H > 0x4C){
        return 1; 
      } else {
        return 0;
      }    
    }
    if(port == 2) {
      if(ATDDR1H > 0x4C){
        return 1; 
      } else {
        return 0;
      }    
    }
     if(port == 3) {
      if(ATDDR2H > 0x4C){
        return 1; 
      } else {
        return 0;
      }    
    }
 }


/*
***********************************************************************
  Display the LCD
***********************************************************************
*/

void disp() {

send_i(LCDCLR);
chgline(0x80);

if(rightPin)
{
  pmsglcd("Enjoy your");
  chgline(0xC0);
  pmsglcd("Candy!");
}

if(wrongPin)
{
  pmsglcd("PIN number");
  chgline(0xC0);
  pmsglcd("is incorrect");
}

if(reset)
{
 pmsglcd("Please enter");
  chgline(0xC0);
  pmsglcd("PIN number."); 
}

  rightPin = 0;
  wrongPin = 0;
  reset = 0;
}

void shiftout(char x)

{
 
  // test the SPTEF bit: wait if 0; else, continue
  // write data x to SPI data register
  // wait for 30 cycles for SPI data to shift out 
  while(SPISR_SPTEF == 0){
  }
  SPIDR = x;
  lcdwait();

}

/*
***********************************************************************
  lcdwait: Delay for approx 2 ms
***********************************************************************
*/

void lcdwait()
{
    int n = 8;
    int m = 6000;
    while(n!=0){
      while(m){
        m--;
      }
      n--;
    }
 
}

/*
***********************************************************************
  delay: Delay for approx 0.05 ms
***********************************************************************
*/

void delay()
{
    int n = 100;
    int m = 100;//150;
    while(n!=0){
      while(m){
        m--;
      }
      n--;
    }
 
}

/*
*********************************************************************** 
  send_byte: writes character x to the LCD
***********************************************************************
*/

void send_byte(char x)
{
     // shift out character
     // pulse LCD clock line low->high->low
     // wait 2 ms for LCD to process data
    shiftout(x);
    PTT_PTT4 = 0;
    PTT_PTT4 = 1;
    PTT_PTT4 = 0;
    lcdwait();
}

/*
***********************************************************************
  send_i: Sends instruction byte x to LCD  
***********************************************************************
*/

void send_i(char x)
{
        // set the register select line low (instruction data)
        // send byte
    PTT_PTT2 = 0;
    send_byte(x);
}

/*
***********************************************************************
  chgline: Move LCD cursor to position x
  NOTE: Cursor positions are encoded in the LINE1/LINE2 variables
***********************************************************************
*/

void chgline(char x)
{
    send_i(CURMOV);
    send_i(x);
    outchar('\r');
    outchar('\n');

}

/*
***********************************************************************
  print_c: Print (single) character x on LCD            
***********************************************************************
*/
 
void print_c(char x)
{
    PTT_PTT2 = 1;
    send_byte(x);
    outchar(x);

}

/*
***********************************************************************
  pmsglcd: print character string str[] on LCD
***********************************************************************
*/

void pmsglcd(char str[])
{
    int i = 0;
    while(str[i] != '\0'){
      print_c(str[i]);
      i++;
    }

}


/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}