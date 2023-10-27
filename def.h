#ifndef DEF_H
#define	DEF_H


// Variable telecommande
extern volatile unsigned char demarrage ;
// Variables sonar
extern volatile unsigned char sonar ;
extern unsigned int sonarOn ;
// Variable compteur
extern volatile unsigned int compteStep ;


void HighISR(void) ;

void init_bits(void) ;

void afficheBat(void) ;
void detection(void) ;
void raffinement(void) ;
void approche(void) ;

#endif
