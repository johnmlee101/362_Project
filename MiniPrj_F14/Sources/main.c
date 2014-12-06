/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2014                    
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < 12 >

 Project Name: < M&M Dispenser >

 Team Members:

   - Team/Doc Leader: < Kyunghoon Jung >      Signature: Kyunghoon Jung
   
   - Software Leader: < John Lee >      Signature: ______________________

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


/* Variable declarations */
int servoPosition = 0;
int dispenseHold = 0;
int mmHold = 0;
int key[4] = {0,0,0,0};
int currentKey = 0;
   	   			 		  			 		       

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
  PWMPOL_PPOL0 = 0x00;
  PWMPER0 = 0xFF;
 	PWMDTY0 = 0x00;
  
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
      if(servoPosition == 1) {      //the pulse is ending
        //set PWM_DUTY to 0;
        
        servoPosition = 0;
      }
      
      
      //row1 = 0;
      //add a small delay
      
      /*if(!column1)
      {
        key[currentKey] = 1;
      }
      if(!column2)
      {
        key[currentKey] = 2;
      }
      if(!column3)
      {
        key[currentKey] = 3;
      }
      row1 = 1;
      row2 = 0;
      //delay
      
      if(!column1)
      {
        key[currentKey] = 4;
        currentKey++;
      }
      if(!column2)
      {
        key[currentKey] = 5;
        currentKey++;
      }
      if(!column3)
      {
        key[currentKey] = 6;
        currentKey++;
      }
      row2 = 1;
      row3 = 0;
      //delay
      
      if(!column1)
      {
        key[currentKey] = 7;
        currentKey++;
      }
      if(!column2)
      {
        key[currentKey] = 8;
        currentKey++;
      }
      if(!column3)
      {
        key[currentKey] = 9;
        currentKey++;
      }
      row3 = 1;
      row4 = 0;
      //delay
      
      if(!column1)
      {
        key[currentKey] = 10; //Star
        currentKey = 0;
      }
      if(!column2)
      {
        key[currentKey] = 0;
        currentKey++;
      }
      if(!column3)
      {
        key[currentKey] = 11; //hash
        currentKey++;
      }
      
      row4 = 1;
      
      
      
      //if keypad has right code 
      if(key[0] == 1)
      {
        if(key[1] == 2)
        {
          if(key[2] == 3)
          {
            if(key[3] == 4)
            {
              setPosition(3);  
            }
          }
        }
      } */
      //hold for 1s
      if(dispenseHold == 0 && servoPosition == 0) {
        setPosition(0); //sets position to -90;
        dispenseHold = 5;
      } else if(dispenseHold > 0) {
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
     PWMDTY0 = 255/10; 
    } else if (x == 1){
     //set PWM to 1.5ms
     PWMDTY0 = (255/10)*1.5; 
    } else {
     //set PWM to 2ms
     PWMDTY0 = 255/5; 
    }
    servoPosition = 1;
  
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