/* =====================================================================================
    author: Davide Burchielli
 * =====================================================================================
*/

#include "project.h"
#include "RGB_driver.h"
#include "stdio.h"

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts

    RGB_Start();    // Start PWM_RG and PWM_B
 
    UART_Start();
    UART_PutString("Send color intensities or type 'RGB LED Program $$$' to open the GUI \r\n");
   
    isr_TIMER_StartEx(Custom_TIMER_ISR); //Start the ISR of the Timer
    isr_UART_StartEx(Custom_UART_ISR);   //Start the ISR of the UART

    extern Byte CurrentByte;
    
    
    for(;;)  // empty infinite loop
    {
            //sprintf(message, "period: %x\r\n", Timer_1_ReadCounter());
            //UART_PutString(message);
        /*if (CurrentByte == TAIL)
        {
          UART_PutString("aggiorno i colori \r\n");
          UpdateColor();
          CurrentByte = IDLE;
        }*/
        
    }
}

/* [] END OF FILE */
