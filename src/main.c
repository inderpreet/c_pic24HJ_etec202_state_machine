#include <stdint.h>
#include <xc.h>
#include "main.h"

// OUTPUTs
#define LED1        _LATB14

// INPUTS
#define SW1         _RB13
#define SEL1        _RB12

// Tristate buffers
#define LED1_TRIS   _TRISB14
#define SW1_TRIS    _TRISB13
#define SEL1_TRIS   _TRISB12

//------------------------------------------------------------------------------
// Custom types
typedef enum {
    INIT,
            LED_ON,
            LED_OFF,
            S1,
            S2,
            S3,
            S4,
            S5
            
}eStates;

typedef enum{
            LED_ON_STATE,
            LED_OFF_STATE,
}eHeartbeat;

// global variables
eStates system_state;
eHeartbeat heartbeat_state;

// functions 
void setup_timer(){
     /* Initialize Timer 1
     * Each configuration sets the MCU's oscillator to 32MHz (see the respective
     * "system.c" for configuration bits).  For the LED to blink
     * at a rate of 1Hz, it must be toggled at 2Hz.
     *
     * Calculation:
     * Fcy = Fosc / 2 = 16E6 [Hz]
     * T1CONbits.TCKPS = 3 divides the input clock by 256.
     * PR1 = Fcy[Hz] / 256 / 2[Hz] = 0x7A12;
     */
    _T1IF = 0;
    _T1IE = 0;
    TMR1 = 0x0000;
    PR1 = 0x7A12;               // Configures how fast or slow
    T1CONbits.TCKPS = 3;        // Pre-scaler for the Timer
    T1CONbits.TON = 1;          // Turns on the Timer
}

void config_lab7(){
    setup_timer();
    system_state = INIT;
    heartbeat_state = LED_OFF_STATE;
    LED1_TRIS = 0;   // OUTPUT
    SW1_TRIS =1;    // INPUT
    SEL1_TRIS = 1;  // INPUT
    LED_TRIS = 0;   // OUTPUT
}

void my_delay(){
    while(_T1IF == 0){} 
    _T1IF = 0;
}

void small_delay(){
    unsigned int i, j = 10;
    while ( j != 0){
        j--;
        i = 20000;
        while(i!=0){
            i--;
        }
    }
}

void do_heartbeat(){    
    if(_T1IF == 1 ){
        _T1IF = 0;
        switch ( heartbeat_state ){
            case LED_ON_STATE:
                LED = 1;
                heartbeat_state = LED_OFF_STATE;
                break;

            case LED_OFF_STATE:
            default:
                LED = 0;
                heartbeat_state = LED_ON_STATE;
                break;
        }
    }
}

void do_state_machine(){
    switch ( system_state ){
        case S1:
            small_delay();
            LED1=0;
            system_state = INIT;
            break;
        case INIT:
        default:
            small_delay();
            LED1 = 1;
            system_state = S1;
            break;
    }
}
// main

int main() 
{
    config_lab7();
    while(1){
        do_heartbeat();
        do_state_machine();
    }
    return 0;
}

/*******************************************************************************
 End of File
*/
