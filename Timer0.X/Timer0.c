/*
 * File:   Timer0.c
 * Author: ADMON
 *
 * Created on 18 de febrero de 2021, 03:59 PM
 */


#include <xc.h>
#include <pic18f4550.h>
#include "Configuracion.h"

#define TRSMD LATDbits.LD0  // Trasnsistor para las decenas de los minutos
#define TRSMU LATDbits.LD1  // Trasnsistor para las unidades de los minutos
#define TRSSD LATDbits.LD2  // Trasnsistor para las decenas de los segundos
#define TRSSU LATDbits.LD3  // Trasnsistor para las unidades de los segundos
#define TRSTP LATDbits.LD4  // Trasnsistor para los dos leds
#define DisplaysPort LATB   // Puerto del display

int DATTMR0 = 57725;        /* Constante que se coloca en el TRM0L y TRM0H con 
                               la cual se genera un segundo*/
char milsecons = 90;        // Constante de tiempo para el encendido de los transistores

int display [10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67}; /*
 Arreglo de los numero en los displays                   */

char Mdecenas = 5;    // Variable de las decenas de los Minutos inicializada en 5
char Munidades = 9;   // Variable de las unidades de los Minutos inicializada en 9
char Sdecenas = 4;    // Variable de las decenas de los Segundos inicializada en 4
char Sunidades = 5;   // Variable de las decenas de los Segundos inicializada en 5

void Init(void) {  // Funcion que configura los puertos como salidas y los apaga 

    ADCON1bits.PCFG = 0XFF;
    TRISD = 0x00;
    TRISB = 0x00;
    LATD = 0x1F;
    LATB = 0x00;

}

void InitTMR0(void) {  // Funci�n que inicializa el timer0 


    INTCONbits.GIE = 0; // Habilito las interrupciones globales
    T0CONbits.T0CS = 0; // Selecciono la fuente de reloj interna
    T0CONbits.PSA = 0; // Activo el asignamiento del preescaler
    T0CONbits.T0PS = 0b111; // Configuro el preescaler a 256  
    T0CONbits.T08BIT = 0; // Configuro el Timer0 a 16bits
    INTCONbits.TMR0IF = 0; // Inicializo la bandera del timer en 0
    INTCONbits.TMR0IE = 1; // Habilitamos la interrupcion por desbordamiento
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1; // Habilitamos las interrupciones globales
    T0CONbits.TMR0ON = 1; // activo el Timer0


    TMR0H = DATTMR0;
    TMR0L = (DATTMR0) >> 8;

}

void main(void) {

    Init();
    InitTMR0();

    while (1) {


        TRSTP = 1;
        __delay_ms(milsecons);
        TRSMD = 1;
        TRSSU = 0;
        TRSMU = 0;
        TRSSD = 0;
        LATB = display[Mdecenas];
        __delay_ms(milsecons);
        TRSSU = 0;
        TRSMD = 0;
        TRSMU = 1;
        TRSSD = 0;
        LATB = display[Munidades];
        __delay_ms(milsecons);
        TRSSD = 1;
        TRSSU = 0;
        TRSMD = 0;
        TRSMU = 0;
        LATB = display[Sdecenas];
        __delay_ms(milsecons);
        TRSSU = 1;
        TRSMD = 0;
        TRSMU = 0;
        TRSSD = 0;
        LATB = display[Sunidades];
        __delay_ms(milsecons);
    }

    return;
}


void __interrupt() Timer0(void) { // funcion de interrupcion que se activa cada vez que timer0 se desborda

    if (INTCONbits.T0IF) {
        TMR0H = DATTMR0;
        TMR0L = (DATTMR0) >> 8;
        
        Sunidades++;
        
        if (Sunidades >= 9) 
        {
            
            Sunidades = 0;
            
            if (Sdecenas >= 5) 
            {
                Sdecenas = 0;
                
                if (Munidades >= 9) 
                {
                    Munidades = 0;
                    
                    if (Mdecenas >= 5) 
                    {
                        Mdecenas = 0;
                    } 
                    else 
                    {
                        Mdecenas++;

                    }
                } 
                else 
                {
                    Munidades++;

                }
            }
            else 
            {
                Sdecenas++;
            }
        }
        TRSTP = 0;
        INTCONbits.T0IF = 0;

    }

}

