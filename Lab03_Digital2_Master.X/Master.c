//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include <stdio.h>
//#include <stdlib.h>
//#include <pic16f887.h>
#include "LCD8bits.h"
//*****************************************************************************
// Definición de variables
int numero = 0;
char charesc1[15];
char charesc2 [15];
char charesc22[15];
char CVPOT[15]; //Creamos un arreglo

//---------------------Constantes---------------------------------
#define contador 0
#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//------------------Prototipos-----------------------------------
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init8(); //Inicializamos la LCD
    Lcd_Clear8(); //Limpiamos la LCD
    uint8_t a;
    uint8_t esc1 = 0;
    uint8_t esc2 = 0;
    uint8_t esc22 = 0;
    
    while(1)
    {
    //-----------------------------Esclavo 1--------------------------------------
        PORTCbits.RC2 = 0;       //Slave Select
        __delay_ms(5);

        spiWrite(1);
        esc1 = spiRead();
        //charesc1 = POT(esc1);

        __delay_ms(1);
        PORTCbits.RC2 = 1;       //Slave Deselect 

        __delay_ms(10);
        //numero++;
    //------------------------------- Esclavo 2----------------------------------

        PORTCbits.RC1 = 0;       //Slave Select
        __delay_ms(5);

        spiWrite(1);
        esc2 = spiRead();
               
        __delay_ms(5);
        PORTCbits.RC1 = 1;       //Slave Deselect

        __delay_ms(10);

     //------------------------------- Esclavo 22----------------------------------

        PORTCbits.RC1 = 0;       //Slave Select
        __delay_ms(5);
        
        spiWrite(0);
        esc22 = spiRead();
        //charesc22 = POT(esc22);

        __delay_ms(5);
        PORTCbits.RC1 = 1;       //Slave Deselect
        __delay_ms(10);
        //__delay_ms(250);
        
        sprintf(charesc1, "%d", esc1);
        sprintf(charesc2, "%d", esc2);
        sprintf(charesc22, "%d", esc22);
        
        Lcd_Clear8();
        Lcd_Set_Cursor8(1,1); //Definimos en donde queremos ver ADC1 en la LCD
        Lcd_Write_String8("ADC1");

        Lcd_Set_Cursor8(1,7); //Definimos en donde queremos ver ADC2 en la LCD
        Lcd_Write_String8("ADC2");

        Lcd_Set_Cursor8(1,12); //Definimos en donde queremos ver CONT2 en la LCD
        Lcd_Write_String8("CONT2");
        
        Lcd_Set_Cursor8(2,1); //Definimos en donde queremos ver el adc1 en la LCD
        Lcd_Write_String8(charesc1);
        
        Lcd_Set_Cursor8(2,7); //Definimos en donde queremos ver el adc2 en la LCD
        Lcd_Write_String8(charesc2);
        
        Lcd_Set_Cursor8(2,12); //Definimos en donde queremos ver el contador en la LCD
        Lcd_Write_String8(charesc22);
        __delay_ms(100);
    }
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC5 = 0;
    
    TRISE = 0b1100;
    TRISB = 0;
    TRISD = 0;
    TRISA = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    
    
    OSCCONbits.IRCF = 0b111; //8 MHz
    OSCCONbits.SCS = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}

