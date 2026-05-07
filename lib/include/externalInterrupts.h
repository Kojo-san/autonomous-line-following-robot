/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe permet de configurer et gérer les interruptions externes  
 *              sur un microcontrôleur AVR. Elle offre des méthodes pour activer, désactiver et configurer  
 *              le mode de déclenchement (front montant, descendant ou les deux).  
 * 
 */

#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

enum class InterruptMode
{
    RISING_EDGE,
    FALLING_EDGE,
    BOTH_EDGES
};

enum class InterruptNumber
{
    INTERRUPT0 = 0,
    INTERRUPT1 = 1,
    INTERRUPT2 = 2
};

class ExternalInterrupts
{
public:
    
    ExternalInterrupts(InterruptNumber interruptNumber, InterruptMode mode);

    void setInterruptMode(InterruptMode mode);
    void enableInterrupt();
    void disableInterrupt();
    void clearInterruptFlag();

    void setPinMode();

    InterruptNumber interruptNumber_;  
};
