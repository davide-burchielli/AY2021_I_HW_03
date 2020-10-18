/* =========================================================================
    author: Davide Burchielli

 * =========================================================================
*/

#include "project.h"
#include "RGB_driver.h"
#include "stdio.h"

#define PERIOD 255

static char message[20]= {'\0'};
Byte CurrentByte = 0;
static char ReceivedByte = 0;


 void RGB_Start()
    {
        PWM_RG_Start();
        PWM_B_Start();
    }
    
 void RGB_Stop()
    {
        PWM_RG_Stop();
        PWM_B_Stop();
    }
  
 void Return ()
    {
    CurrentByte = IDLE; 
    Timer_1_Stop();
    }
    
 void ResetColor()
    {
        for(uint8 i=0 ; i < 3; i++)
            ColorVector[i] = 0;
    }
 void UpdateColor()
    {
        for(uint8 i=0 ; i < 3; i++)
        { sprintf(message, "vect: %d\r\n", ColorVector[i]);
            UART_PutString(message);
        }
        PWM_RG_WriteCompare1(ColorVector[0]);
        PWM_RG_WriteCompare2(ColorVector[1]);
        PWM_B_WriteCompare(ColorVector[2]);
    }
 void ResetTimer()
    {
        Timer_1_WriteCounter(PERIOD);
    }
 CY_ISR (Custom_UART_ISR)
    {
             
     if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            ReceivedByte= UART_ReadRxData();
            
            switch ( CurrentByte )
            {
                case IDLE :           
                            switch ( ReceivedByte )
                            {
                                case 'v':
                                            UART_PutString("RGB LED Program $$$\r\n");
                                            break;
                                case 0xA0:
                                            ResetColor(); 
                                            ResetTimer();
                                            Timer_1_Start();
                                            CurrentByte++;  
                                            break;
                                default:
                                            UART_PutString("!! Error: Invalid Header !!\r\n");
                                            break;
                            }
                            break;
                default:
                        ResetTimer();
                        if (ReceivedByte < 0 || ReceivedByte > 255 )
                            {
                            UART_PutString(" !! Error: byte out of range [00 ; FF]!! \r\n");
                            Return();
                            }
                        else  
                            {
                            ColorVector[CurrentByte-1] = ReceivedByte;
                            CurrentByte++; 
                            }      
                        break;
                case B :
                          ResetTimer();
                          if (ReceivedByte == 0xC0)
                            {
                             CurrentByte++;
                             Timer_1_Stop();
                            }
                          else 
                            { 
                            UART_PutString("!! ERROR: WRONG TAIL!! \r\n");
                            Return(); 
                            }
                        break;                
                case TAIL: break;                       
            } 
        }
    }
    
 CY_ISR (Custom_TIMER_ISR)
    {
        Timer_1_ReadStatusRegister();
        
        UART_PutString("** TIME OUT **\r\n");
        Return(); 
    }

/* [] END OF FILE */
