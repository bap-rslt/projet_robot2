#include<p18f2520.h>
#include<i2c.h>
#include<stdio.h>
#include "MI2C.h"
#include "def.h"


// Variables telecommande
volatile char bouton[3] ;
volatile unsigned char compteTel = 0;
// Variable surveillance batterie
volatile unsigned int compteADC = 0 ;


#pragma code HighVector=0x08
void IntHighVector(void)
{
	_asm goto HighISR _endasm
}
#pragma code

#pragma interrupt HighISR

void HighISR(void)
{
    if(INTCONbits.INT0IF) // Interruption telecommande
    {
            INTCONbits.INT0IF = 0 ;
            
            // Lecture code bouton
            Detecte_i2c(0xA2) ;
            Lire_i2c_Telecom(0xA2, bouton);

            if(bouton[1] == 0x33 && compteTel == 0) // Verification code bouton + fonction anti double-appui
            {
                demarrage = ~demarrage ;
                compteTel = 5 ; // Delai 0.5s min
            }
    }

    if(INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0 ;
        TMR0H = 0x3C ;
        TMR0L = 0xB0 ;

        if(sonarOn)
        {
        // Lecture valeur sonar
            Detecte_i2c(0xE0) ;
            sonar = SONAR_Read(0xE0,0x02) ;
            SONAR_Write(0xE0,0x51) ;
        }

        if(compteStep>0)
        {
            compteStep -- ; // Compteur multi-usages
        }

        compteADC ++ ;
        if (compteADC == 10)
        {
            compteADC = 0 ;
            ADCON0bits.GO = 1 ;//lancer l'ADC
        }

        if (compteTel > 0)
        {
            compteTel -- ;
        }
    }

    if(PIR1bits.ADIF)
    {
        PIR1bits.ADIF = 0 ;
        afficheBat() ;
    }
}