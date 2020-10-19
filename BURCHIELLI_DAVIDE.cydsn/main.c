/* ==========================================================================================
    author: Davide Burchielli

    This file starts the Components and their ISRs and when a byte is received by the UART, 
    it manages the operations according to the current state.
 * ==========================================================================================
*/

#include "project.h"
#include "RGB_driver.h"


StateEnum State = 0; // Define and initialize the variable State which identifies the current state.
_Bool flag = 0;     // flag variable is set = 1 by Custom_UART_ISR when a byte is recived.
_Bool TimeOut = 0; // TimeOut variable is set = 1 by Custom_TIMER_ISR every 5 seconds.
extern uint8_t ReceivedByte; // Global variable.

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts
    RGB_Start();       // Start PWM_RG and PWM_B
 
    UART_Start();      //Start the UART
    UART_PutString("Send:\n - Color packet: A0 RR GG BB C0\n - 'v' to open the GUI \r\n");
   
    isr_TIMER_StartEx(Custom_TIMER_ISR); //Start the ISR of the Timer
    isr_UART_StartEx(Custom_UART_ISR);   //Start the ISR of the UART
    
    // At power-on the RGB LED must be switch off: 
    ResetColor();
    UpdateColor();
    
    for(;;) 
    {       
            while(!flag)  // While no byte has been received
            {
                if (TimeOut)
                {
                   TimeOut = 0;
                   Return(); // Return to the IDLE state
                   UART_PutString("** TIME OUT **\r\n");
                }
            }
            
            switch ( State )  // One byte has been received by the UART
            {
                case IDLE : // The program exits from the IDLE states only when the right Header byte is received (0xA0)          
                            switch ( ReceivedByte )
                            {
                                case 'v':
                                            UART_PutString("RGB LED Program $$$\r\n");
                                            break;
                                case 0xA0:  //The Header byte has been received                                           
                                            ResetColor(); 
                                            ResetTimer();
                                            Timer_1_Start();  //The timer starts and it will generate an interrupt every 5 seconds
                                            State++;  //switch to the next state: HEADER
                                            break;
                                default:
                                            UART_PutString("!! ERROR: Wrong Header !!\r\n");
                                            break;
                            }
                            flag = 0;
                            break;
                default: // The default case is executed for the state: HEADER, RED and GREEN 
                        
                        ResetTimer(); // Reset the counter value of the Timer to the Period value (255).
                        if (ReceivedByte < 0 || ReceivedByte > 255 )
                            {
                            UART_PutString(" !! ERROR: byte out of range [00 ; FF]!! \r\n");
                            Return(); // Return to the IDLE state
                            }
                        else  
                            {
                            // Save the byte received as [State-1]th element of the Color Vector array
                            ColorVector[State-1] = ReceivedByte; 
                            State++; 
                            }      
                        flag = 0;
                        break;
                case BLUE : 
                          ResetTimer();
                          // The program switches to the TAIL state only when the right Tail byte is received (0xC0)
                          if (ReceivedByte == 0xC0)
                            {
                             State++;  //switch to TAIL state
                             Timer_1_Stop();
                            }
                          else 
                            { 
                            UART_PutString("!! ERROR: Wrong Tail !! \r\n");
                            Return(); // Return to the IDLE state
                            flag = 0;
                            break;
                            }
                case TAIL:
                           UpdateColor(); // Update the RGB LED color
                           State = IDLE;  // Return to the IDLE state
                           flag = 0;
                           break;
            }
    }
}
/* [] END OF FILE */
