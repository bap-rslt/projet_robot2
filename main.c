#include<p18f2520.h>
#include<i2c.h>
#include<stdio.h>
#include "MI2C.h"
#include "def.h"

#pragma config OSC = INTIO67
#pragma config PBADEN = OFF, WDT = OFF, LVP = OFF, DEBUG = ON


volatile unsigned char demarrage = 0;


void main(void)
{
	init_bits() ; // Initialisation des registres du PIC18
        while(1)
        {
            detection() ; // Rotation jusqu'a trouver une cible
            raffinement() ; // Prise de mesures pour trouver et s'orienter vers le centre de la cible
            approche() ; // Approche vers la cible ne ligne droite
            demarrage = 0 ;
        }
}
