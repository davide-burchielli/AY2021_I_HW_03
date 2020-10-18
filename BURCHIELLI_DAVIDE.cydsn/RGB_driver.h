/* =================================================
    author: Davide Burchielli

    This file declares the functions and the ISRs.
 * =================================================
*/

#ifndef _RGB_DRIVER_H
    #define _RGB_DRIVER_H
    
    #include "project.h"
    
    CY_ISR_PROTO (Custom_TIMER_ISR);
    CY_ISR_PROTO (Custom_UART_ISR);
  
    // Enumerate the states
    typedef enum {
                    IDLE,   // 0
                    HEADER, // 1
                    RED,    // 2
                    GREEN,  // 3
                    BLUE,   // 4
                    TAIL    // 5
 
                  } StateEnum;
    
    char ColorVector[3];  // ColorVector array contains the 3 colors values:
                          // element 0 : RED
                          // element 1 : GREEN
                          // element 2 : BLUE
    
    void RGB_Start(); 
    void RGB_Stop();
    void Return();
    void ResetColor();  
    void UpdateColor();
    void ResetTimer();

#endif

/* [] END OF FILE */
