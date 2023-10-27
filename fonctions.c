#include<p18f2520.h>
#include<i2c.h>
#include<stdio.h>
#include "MI2C.h"
#include "def.h"


// Variables surveillance batterie
unsigned int indiceV = 0 ;
unsigned int tabVbat[8] = {185, 185, 185, 185, 185, 185, 185, 185} ;
unsigned int Vmoy ;
unsigned int leds ;
// Variables sonar
volatile unsigned char sonar ;
unsigned int sonarOn = 0 ;
unsigned int seuilMin = 100 ;
unsigned int seuilMax = 150 ;
unsigned int tabSonar[3] = {250, 250, 250} ;
unsigned int tabDist[20] ;
unsigned int min = 200 ;
unsigned int idMin = 0 ;
// Variables compteurs
volatile unsigned int compteStep = 0 ;
unsigned int reste ;


void afficheBat(void)
{
    tabVbat[indiceV] = ADRESH ;

    indiceV ++ ;
    if(indiceV==8)
    {
        indiceV = 0 ;
    }

    Vmoy = tabVbat[0] + tabVbat[1] + tabVbat[2] + tabVbat[3] + tabVbat[4] + tabVbat[5] + tabVbat[6] + tabVbat[7] ;
    Vmoy = Vmoy >> 3 ;
    if(Vmoy<=153)
    {
        LATBbits.LATB5 = 1 ;
        leds = 0b11111111 ;
        demarrage = 0 ;
    }
    else if(Vmoy<=157)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11111110 ;
    }
    else if(Vmoy<=161)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11111100 ;
    }
    else if(Vmoy<=165)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11111000 ;
    }
    else if(Vmoy<=169)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11110000 ;
    }
    else if(Vmoy<=173)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11100000 ;
    }
    else if(Vmoy<=177)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b11000000 ;
    }
    else if(Vmoy<=181)
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b10000000 ;
    }
    else
    {
        LATBbits.LATB5 = 0 ;
        leds = 0b00000000 ;
    }
    Write_PCF8574(0x40, leds) ;
    printf("Niveau de batterie : %d\r\n", Vmoy) ;


}

void detection(void)
{
    while(!demarrage) ; // On attend l'appui sur la telecommande
    LATAbits.LATA6 = 1 ;
    LATAbits.LATA7 = 0 ;
    sonarOn = 1 ;
    compteStep = 10 ;
    while(compteStep>0) ;
    CCPR1L = 70 ;
    CCPR2L = 70 ;

    compteStep = 100 ;
    while(sonar>seuilMax || sonar<seuilMin) ;
    CCPR1L = 0 ;
    CCPR2L = 0 ;
    printf("Détection terminee\r\n");
}

void raffinement(void)
{
    while(!demarrage) ;
    // Pré-correction de l'angle pendant 0.3s
    LATAbits.LATA6 = 0 ;
    LATAbits.LATA7 = 1 ;
    compteStep = 3 ;
    while(compteStep>0)
    {
        CCPR1L = 70 ;
        CCPR2L = 70 ;
    }
    CCPR1L = 1 ;
    CCPR2L = 1 ;
    printf("Pre-correction terminee\r\n");

    while(!demarrage) ;
    // Phase de raffinement
    LATAbits.LATA6 = 1 ;
    LATAbits.LATA7 = 0 ;
    compteStep = 100 ;
    while(compteStep>2) // On attend que les mesures soient finies
    {
        reste = compteStep%5 ;
        if(reste == 0)
        {
            tabSonar[0] = sonar ;
        }
        else if(reste == 4)
        {
            tabSonar[1] = sonar ;
        }
        else if(reste == 3)
        {
            tabSonar[2] = sonar ;
            tabDist[(100 - (compteStep+2))/5] = (tabSonar[0] + tabSonar[1] + tabSonar[2])/3 ;
            printf("Tab %d : %d, step n %d\r\n", (100 - (compteStep+2))/5, tabDist[(100 - (compteStep+2))/5], compteStep) ; // Affichage de la valeur renvoyée par le sonar
        }

        else if(reste == 2)
        {
            CCPR1L = 70 ;
            CCPR2L = 70 ;
        }
        else if(reste == 1)
        {
            CCPR1L = 0 ;
            CCPR2L = 0 ;
        }
    }
    printf("Mesures terminees\r\n");
    sonarOn = 0 ;

    min = 200 ;
    compteStep = 0;
    while(compteStep<20) // Recherche du min dans le tableau du sonar
    {
        if(tabDist[compteStep]<=min && tabDist[compteStep]>=95)
        {
            min = tabDist[compteStep] ;
            idMin = compteStep;
        }
        compteStep ++ ;
    }


    while(!demarrage) ;
    // On doit se décaler de 19 - idMin intervalles
    LATAbits.LATA6 = 0 ;
    LATAbits.LATA7 = 1 ;
    compteStep = 5 * (19 - idMin);
    printf("Position du min : %d\r\n", idMin) ;
    printf("Valeur du min : %d\r\n", min) ;
    while(compteStep>0)
    {
        reste = compteStep%5 ;
        if(reste == 2)
        {
            CCPR1L = 70 ;
            CCPR2L = 70 ;
        }
        else if(reste == 1)
        {
            CCPR1L = 0 ;
            CCPR2L = 0 ;
        }
    }
    CCPR1L = 0 ;
    CCPR2L = 0 ;

    printf("Raffinement termine\r\n");

}

void approche(void)
{
    while(!demarrage) ;
    LATAbits.LATA6 = 0 ;
    LATAbits.LATA7 = 0 ;
    compteStep = 33 ; // 30cm/s -> 3.3s pour avancer d'1m -> 33 cycles de 0.1s
    while(compteStep>0)
    {
        CCPR1L = 120 ;
        CCPR2L = 120 ;
    }
    CCPR1L = 0 ;
    CCPR2L = 0 ;
    printf("Approche terminee\r\n");
}

