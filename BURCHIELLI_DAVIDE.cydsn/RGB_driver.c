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
  
 void Error ()
    {
    CurrentByte = IDLE; 
    Timer_1_Stop();
    UART_PutString(" !! Error: out of range [00 ; FF]!! \r\n");
    }
    
 void ResetColor()
    {
        for(uint8 i=0 ; i < 3; i++)
            ColorVector[i] = 0;
        PWM_RG_WriteCompare1(ColorVector[0]);
        PWM_RG_WriteCompare2(ColorVector[1]);
        PWM_B_WriteCompare(ColorVector[2]);
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
            sprintf(message, "BYTE: %x\r\n", ReceivedByte);
            UART_PutString(message);

            switch ( CurrentByte )
            {
                
                case IDLE :           
                            UART_PutString("---DENTRO IDLE\r\n");
                            switch ( ReceivedByte )
                            {
                                case 'v':
                                        UART_PutString("RGB LED Program $$$\r\n");
                                        break;
                                case 0xA0:
                                            UART_PutString("HEADER RICEVUTO\r\n");
                                            ResetColor(); 
                                            ResetTimer();
                                            Timer_1_Start();
                                            CurrentByte++;  
                                            break;
                                default:
                                        UART_PutString("!! Header non valido !!\r\n");
                                        break;
                            }
                            break;
                default:
                        //UART_PutString("---DENTRO default\r\n");
                        ResetTimer();
                        if (ReceivedByte < 0 || ReceivedByte > 255 )
                            Error();
                        else  
                              {
                               ColorVector[CurrentByte-1] = ReceivedByte;
                                sprintf(message, "VECT: %d\r\n", ColorVector[CurrentByte-1]);
                                UART_PutString(message);
                               CurrentByte++; 
                               if (CurrentByte == B)
                                    Timer_1_Stop();
                              }
                        break;
                case B :
                          UART_PutString("---DENTRO BLUE\r\n");
                          if (ReceivedByte == 0xC0)
                               CurrentByte++;
                          else 
                           { 
                            UART_PutString(" WRONG TAIL\r\n");
                            CurrentByte = IDLE; 
                            }
                          // break;
                case TAIL:
                          UART_PutString("aggiorno i colori \r\n");
                          UpdateColor();
                          CurrentByte = IDLE;
                            break;
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
