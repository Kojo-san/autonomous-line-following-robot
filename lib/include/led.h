/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe permet de contrôler une DEL bicolore branchée sur les ports PA0 et PA1.  
 *              Elle fournit des méthodes pour allumer la DEL en rouge, en vert, en mode ambre (alternance rapide)  
 *              ou l'éteindre complètement.  
 *
 */

#pragma once

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

constexpr uint8_t AMBER_DELAY_MS = 10;
constexpr uint8_t GREEN_DEL_PIN = (1 << PC0);
constexpr uint8_t RED_DEL_PIN = (1 << PC1);

class Led {
public:
    Led();
    
    void red();

    void green();

    void amber();
    
    void off();
};