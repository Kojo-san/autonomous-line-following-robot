/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe fournit des outils pour faciliter l'affichage de messages de débogage  
 *              via l'interface UART. Elle inclut des fonctions pour envoyer des messages texte et des nombres,  
 *              et utilise une macro conditionnelle (`DEBUG_PRINT`) pour activer ou désactiver l'affichage.  
 * 
 */

#pragma once

#include "usart.h"
#include <stdlib.h>  


#ifdef DEBUG
#define DEBUG_PRINT_MESSAGE(message) debug.printMessage(message)
#define DEBUG_PRINT_NUMBER(value) debug.printNumber(value)
#else
#define DEBUG_PRINT_MESSAGE(message) do {} while (0)
#define DEBUG_PRINT_NUMBER(value) do {} while (0)
#endif

class Debug {
public:
    Usart& usart;  

    Debug(Usart& usart) : usart(usart) {
        usart.initialisation();  
    }

    void printMessage(const char *message);
    
    void printNumber(uint16_t value);
};