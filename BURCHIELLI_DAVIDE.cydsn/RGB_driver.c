/* ======================================================================================================
    author: Davide Burchielli

    This file defines the functions and the ISRs.
    Custom_UART_ISR is executed everytime the isr_UART is triggered by the arrival of a byte. 
    Custom_TIMER_ISR is executed everytime the isr_TIMER is triggered by a TC event, so every 5 seconds.  
 * ======================================================================================================
*/

#include "project.h"
#include "RGB_driver.h"

#define PERIOD 255

extern StateEnum State;  
uint8_t ReceivedByte = 0;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
extern _Bool flag, TimeOut;

 void RGB_Start() // Function that starts the PWM
    {
        PWM_RG_Start();
        PWM_B_Start();
    }
    
 void RGB_Stop() // Function that stops the PWM
    {
        PWM_RG_Stop();
        PWM_B_Stop();
    }
    
 void Return () // Function used to return the IDLE state and stops the Timer
    {
    State = IDLE; 
    Timer_1_Stop();
    }
    
 void ResetColor() // Function that resets ColorVector array 
    {
        for(uint8 i=0 ; i < 3; i++)
            ColorVector[i] = 0;
    }
    
 void UpdateColor() // Function that changes the Compare values of PWM_RG and PWM_B to update the RGB LED color
    {
        PWM_RG_WriteCompare1(ColorVector[0]);
        PWM_RG_WriteCompare2(ColorVector[1]);
        PWM_B_WriteCompare(ColorVector[2]);
    }
    
 void ResetTimer() // Function that resets the Timer counter to the value of the Period
    {
        Timer_1_WriteCounter(PERIOD);
    }
    
 // Define Custom_UART_ISR
 CY_ISR (Custom_UART_ISR) 
    {    
     if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            ReceivedByte= UART_ReadRxData();
            flag = 1;
        }
    }
    
 // Define Custom_TIMER_ISR 
 CY_ISR (Custom_TIMER_ISR)
    {
        Timer_1_ReadStatusRegister(); //Read Timer status register to bring interrupt line low
        TimeOut = 1;
    }

/* [] END OF FILE */
