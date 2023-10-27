#include<p18f2520.h>
#include<i2c.h>
#include<stdio.h>
#include "MI2C.h"
#include "def.h"


void init_bits(void)
{
    // Horloge interne cadencée à 8 MHz
    OSCCONbits.IRCF = 7 ;

    // Init I2C avec bibliothèque
    MI2CInit() ;
    
    // Init UART
    TXSTAbits.SYNC = 0 ;
    TXSTAbits.BRGH = 1 ;
    BAUDCONbits.BRG16 = 0 ; // configuration 8 bits
    SPBRGH = 0 ;
    SPBRG = 51 ; // baudrate = env. 9615 b/s
    RCSTAbits.SPEN = 1 ;
    PIE1bits.TXIE = 0 ;
    TXSTAbits.TXEN = 1 ; // transmission
    RCSTAbits.CREN = 1 ; // reception

    // Init ADC
    TRISAbits.RA2 = 1 ; // tension batterie en entrée
    ADCON1bits.PCFG = 12 ; // entrée mode analogique
    ADCON1bits.VCFG = 0 ;
    ADCON2bits.ADCS = 1 ; //réglage de Tad = 1ms et Tacq = 6 Tad
    ADCON2bits.ACQT = 3 ;
    ADCON0bits.CHS = 2 ; // selection du port AN2 pour mesure tension batterie
    ADCON0bits.ADON = 1 ;
    ADCON2bits.ADFM = 0 ;

    // Init timer0 : sonar et surveillance batterie
    T0CONbits.T08BIT = 0 ; // compteur mode 16 bits
    T0CONbits.T0CS = 0 ; // clock interne
    T0CONbits.PSA = 0 ; // activer prescaler
    T0CONbits.T0PS = 1 ; // prescaler à 4
    INTCONbits.TMR0IE = 1 ;
    TMR0H = 0x3C ;  //rechargement réglé pour 0.1 sec
    TMR0L = 0xB0 ;
    T0CONbits.TMR0ON = 1 ;
    TRISBbits.RB5 = 0 ; // LED surevillance batterie
    LATBbits.LATB5 = 0 ;

    // Init timer2 : pwm
    TRISCbits.RC1 = 0 ; //PWM D
    TRISCbits.RC2 = 0 ; //PWM G
    TRISAbits.RA6 = 0 ; //DIR D
    TRISAbits.RA7 = 0 ; //DIR G
    T2CONbits.T2CKPS = 1 ; // prescaler = 4
    PIE1bits.TMR2IE = 0 ;
    PR2 = 249 ; // période du pwm : 2 kHz
    CCPR1L = 0 ; // vitesse des moteurs à 0 au départ
    CCPR2L = 0 ;
    CCP1CONbits.DC1B = 0 ; // LSB du duty cycle
    CCP2CONbits.DC2B = 0 ;
    CCP1CONbits.CCP1M = 15 ;
    CCP2CONbits.CCP2M = 15 ;
    T2CONbits.TMR2ON = 1 ;

    // Init interuptions
    INTCONbits.PEIE = 1 ;
    INTCONbits.GIE = 1 ;
        // Init int0
    TRISBbits.RB0 = 1 ;
    INTCONbits.INT0IE = 1 ;
    INTCONbits.INT0IF = 0 ;
    INTCON2bits.INTEDG0 = 0 ; // front descendant pour l'int
        // Init int ADC
    PIR1bits.ADIF = 0 ;
    PIE1bits.ADIE = 1 ;
        // timer0
    INTCONbits.TMR0IF = 0 ;
        // timer2
    PIR1bits.TMR2IF = 0 ;

    printf("Initialisation terminee\n\r") ;

}



