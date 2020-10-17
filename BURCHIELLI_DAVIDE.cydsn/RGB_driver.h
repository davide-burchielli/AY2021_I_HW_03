/* =========================================================================
    author: Davide Burchielli

    This file declares the function Custom_TIMER_ISR and Custom_UART_ISR.
 * =========================================================================
*/

#ifndef _RGB_DRIVER_H
    #define _RGB_DRIVER_H
    
    #include "project.h"
    
    CY_ISR_PROTO (Custom_TIMER_ISR);
   
    CY_ISR_PROTO (Custom_UART_ISR);
    

    
    typedef enum {
                    IDLE,
                    HEADER,
                    R, 
                    G,
                    B,
                    TAIL
        
                  } Byte;
    
    char ColorVector[3];
    
    void RGB_Start(); 
    void RGB_Stop();
    void Return();
    void ResetColor();   
    void UpdateColor();
    
    
    
    
#endif

/* [] END OF FILE */
