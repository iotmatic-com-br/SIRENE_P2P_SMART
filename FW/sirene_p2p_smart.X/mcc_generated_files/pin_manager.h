/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC16F1827
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.30 and above
        MPLAB 	          :  MPLAB X 5.40	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SIRENE aliases
#define SIRENE_TRIS                 TRISAbits.TRISA0
#define SIRENE_LAT                  LATAbits.LATA0
#define SIRENE_PORT                 PORTAbits.RA0
#define SIRENE_ANS                  ANSELAbits.ANSA0
#define SIRENE_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define SIRENE_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define SIRENE_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define SIRENE_GetValue()           PORTAbits.RA0
#define SIRENE_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define SIRENE_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define SIRENE_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define SIRENE_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set JP1 aliases
#define JP1_TRIS                 TRISAbits.TRISA1
#define JP1_LAT                  LATAbits.LATA1
#define JP1_PORT                 PORTAbits.RA1
#define JP1_ANS                  ANSELAbits.ANSA1
#define JP1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define JP1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define JP1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define JP1_GetValue()           PORTAbits.RA1
#define JP1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define JP1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define JP1_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define JP1_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set JP2 aliases
#define JP2_TRIS                 TRISAbits.TRISA2
#define JP2_LAT                  LATAbits.LATA2
#define JP2_PORT                 PORTAbits.RA2
#define JP2_ANS                  ANSELAbits.ANSA2
#define JP2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define JP2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define JP2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define JP2_GetValue()           PORTAbits.RA2
#define JP2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define JP2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define JP2_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define JP2_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set JP3 aliases
#define JP3_TRIS                 TRISAbits.TRISA3
#define JP3_LAT                  LATAbits.LATA3
#define JP3_PORT                 PORTAbits.RA3
#define JP3_ANS                  ANSELAbits.ANSA3
#define JP3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define JP3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define JP3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define JP3_GetValue()           PORTAbits.RA3
#define JP3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define JP3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define JP3_SetAnalogMode()      do { ANSELAbits.ANSA3 = 1; } while(0)
#define JP3_SetDigitalMode()     do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set JP4 aliases
#define JP4_TRIS                 TRISAbits.TRISA4
#define JP4_LAT                  LATAbits.LATA4
#define JP4_PORT                 PORTAbits.RA4
#define JP4_ANS                  ANSELAbits.ANSA4
#define JP4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define JP4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define JP4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define JP4_GetValue()           PORTAbits.RA4
#define JP4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define JP4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define JP4_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define JP4_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set VD aliases
#define VD_TRIS                 TRISAbits.TRISA6
#define VD_LAT                  LATAbits.LATA6
#define VD_PORT                 PORTAbits.RA6
#define VD_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define VD_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define VD_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define VD_GetValue()           PORTAbits.RA6
#define VD_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define VD_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set VM aliases
#define VM_TRIS                 TRISAbits.TRISA7
#define VM_LAT                  LATAbits.LATA7
#define VM_PORT                 PORTAbits.RA7
#define VM_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define VM_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define VM_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define VM_GetValue()           PORTAbits.RA7
#define VM_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define VM_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set BOTAO aliases
#define BOTAO_TRIS                 TRISBbits.TRISB0
#define BOTAO_LAT                  LATBbits.LATB0
#define BOTAO_PORT                 PORTBbits.RB0
#define BOTAO_WPU                  WPUBbits.WPUB0
#define BOTAO_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define BOTAO_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define BOTAO_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define BOTAO_GetValue()           PORTBbits.RB0
#define BOTAO_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define BOTAO_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define BOTAO_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define BOTAO_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)

// get/set RST aliases
#define RST_TRIS                 TRISBbits.TRISB1
#define RST_LAT                  LATBbits.LATB1
#define RST_PORT                 PORTBbits.RB1
#define RST_WPU                  WPUBbits.WPUB1
#define RST_ANS                  ANSELBbits.ANSB1
#define RST_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RST_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RST_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RST_GetValue()           PORTBbits.RB1
#define RST_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RST_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RST_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define RST_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define RST_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define RST_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()              PORTBbits.RB2
#define RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()             do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()           do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode()         do { ANSELBbits.ANSB2 = 1; } while(0)
#define RB2_SetDigitalMode()        do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

#define SireneON    SIRENE_SetHigh()
#define SireneOFF   SIRENE_SetLow()
#define VDon        VD_SetHigh()
#define VDoff       VD_SetLow()
#define VMon        VM_SetHigh()
#define VMoff       VM_SetLow()
#define rstlora     RST_SetLow()
#define runlora     RST_SetHigh()
#define Sensor1     BOTAO_GetValue()==1
#define Sensor0     BOTAO_GetValue()==0
#define jp1AB       JP1_GetValue()==1
#define jp1FEC      JP1_GetValue()==0
#define jp2AB       JP2_GetValue()==1
#define jp2FEC      JP2_GetValue()==0
#define jp3AB       JP3_GetValue()==1
#define jp3FEC      JP3_GetValue()==0
#define jp4AB       JP4_GetValue()==1
#define jp4FEC      JP4_GetValue()==0

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/