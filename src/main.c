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

#define NORMAL      0x7A12
#define SLOWER      0xF424
#define FASTER      0x3D09

//------------------------------------------------------------------------------
// Custom types
typedef enum {
    IDLE,
            BLINK,
            WAIT_FOR_RELEASE,
            SET_SLOWER,
            SET_FASTER,
            IDLE_NEW,
            BLINK_NEW,
            WAIT_FOR_RELEASE_AND_GO_TO_IDLE,
            
            
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
    system_state = IDLE;
    heartbeat_state = LED_OFF_STATE;
    LED1_TRIS = 0;   // OUTPUT
    SW1_TRIS = 1;    // INPUT
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
        //_T1IF = 0;
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
        case IDLE:
            if(_T1IF == 1){
                _T1IF = 0;
                // LED1 = !LED1;
                system_state = BLINK;
                small_delay();
            }
            if(SW1 == 1){
                system_state = WAIT_FOR_RELEASE;
            }
            break;
            
        case BLINK:
            LED1 = ! LED1;
            system_state = IDLE;
            break;
        
        case WAIT_FOR_RELEASE:
            if(SW1 == 1){   // When Button is Released
                if(LED1 == 0){  // Slower
                    system_state = SET_SLOWER;
                } else {        // Faster
                    system_state = SET_FASTER;
               }
            }            
            break;
        
        case SET_SLOWER:
            PR1 = SLOWER;
            system_state = IDLE_NEW;
            break;
        case SET_FASTER:
            PR1 = FASTER; 
            system_state = IDLE_NEW;
            break;
        case IDLE_NEW:
            if(_T1IF == 1){
                _T1IF = 0;
                // LED1 = !LED1;
                system_state = BLINK_NEW;
                small_delay();
            }
            if (SW1 == 0 ){
                system_state  = WAIT_FOR_RELEASE_AND_GO_TO_IDLE;
            }
            break;
        case BLINK_NEW:
            system_state = IDLE_NEW;
            break;
        case WAIT_FOR_RELEASE_AND_GO_TO_IDLE:
            if(SW1 == 1){
                system_state = IDLE;
            }
            break;
        default:
            system_state = IDLE;
    }
}
// main

int main() 
{
    volatile unsigned int temp;
    config_lab7();
    while(1){
//        do_heartbeat();
//        do_state_machine();
        LED = SEL1;
        temp = SEL1;
    }
    return 0;
}

/*******************************************************************************
 End of File
*/
