/* =========================================================================
    author: Davide Burchielli

 * =========================================================================
*/

#include "project.h"
#include "RGB_driver.h"

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
    
 void Error ()
    {
    CurrentByte = 0; 
    Timer_1_Stop();
    UART_PutString("Error: byte out of rangec\r\n");
    }
    
 void ClearVector()
    {
        for(uint8 i=0 ; i < 3; i++)
            ColorVector[i] = 0;
    }
 void UpdateColor()
    {
        PWM_RG_WriteCompare1(ColorVector[0]);
        PWM_RG_WriteCompare2(ColorVector[1]);
        PWM_B_WriteCompare(ColorVector[2]);
    }
   
 CY_ISR (Custom_UART_ISR)
    {
    
     if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            ReceivedByte= UART_ReadRxData();
                
            switch ( CurrentByte )
            {
                case IDLE :
                            if (ReceivedByte == 'v')
                                UART_PutString('RGB LED Program $$$\r\n');
                                
                            if (ReceivedByte == 0xA0)
                                {   
                                    ClearVector();   
                                    Timer_1_Start();
                                    CurrentByte ++;
                                }

                case TAIL :
                          if (ReceivedByte == 0xC0)
                            {     
                                Timer_1_Stop();
                                UpdateColor();
                                CurrentByte = IDLE;
                            }
               
                default:
                        if (ReceivedByte < 0 && ReceivedByte > 255 )
                            Error();
                        else  
                            { 
                              ColorVector[CurrentByte-1] = ReceivedByte;
                              CurrentByte ++;
                            }
            }
        }
    }
    
 CY_ISR (Custom_TIMER_ISR)
    {
        UART_PutString("** TIME OUT **\r\n");
        CurrentByte = IDLE;
        Timer_1_Stop();
    }

/* [] END OF FILE */
