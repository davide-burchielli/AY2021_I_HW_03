/* =====================================================================================
    author: Davide Burchielli
 * =====================================================================================
*/

#include "project.h"
#include "RGB_driver.h"
#include "stdio.h"

char message[20]= {'\0'};

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts

    RGB_Start();    // Start PWM_RG and PWM_B
 
    UART_Start();
    UART_PutString("Send:\n 1) Color packets: A0 RR GG BB C0\n 2)'v' to open the GUI \r\n");
   
    isr_TIMER_StartEx(Custom_TIMER_ISR); //Start the ISR of the Timer
    isr_UART_StartEx(Custom_UART_ISR);   //Start the ISR of the UART
    
    //Timer_1_Start();
    
    extern Byte CurrentByte;
    
    for(;;) 
    {
       //sprintf(message,"counter: %d \r\n" , Timer_1_ReadCounter());
       //UART_PutString(message);        
        if (CurrentByte == TAIL)
        {
          UpdateColor();
          CurrentByte = IDLE;
        }
    }
}

/* [] END OF FILE */
