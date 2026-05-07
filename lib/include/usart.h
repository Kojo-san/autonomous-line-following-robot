/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe permet la communication série via l'interface UART du microcontrôleur.  
 *              Elle initialise la transmission avec un baud rate défini et offre des fonctions pour envoyer et  
 *              recevoir des caractères ou des chaînes de caractères. Elle assure également l'attente de la  
 *              disponibilité du buffer avant chaque envoi ou réception de données.  
 *
 */

#pragma once

#define F_CPU 8000000UL
#define BAUD_RATE 2400 

#include <avr/io.h>
#include <avr/interrupt.h> 

class Usart
{
public:
    Usart();

    void initialisation();
    
    void sendChar(uint8_t data);
    
    uint8_t receiveChar();
    
    void sendString(const char* word);
};
