/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe permet de configurer et manipuler le timer 1 du microcontrôleur AVR.  
 *              Elle offre des fonctionnalités pour le mode CTC, Fast PWM et Normal, ainsi que des interruptions.  
 *              Elle permet aussi de gérer la fréquence et la durée des événements temporisés.  
 * 
 */

#pragma once

#define F_CPU 8000000UL
 
#include <avr/io.h>
#include <util/delay.h>
#include "wheelTimer.h"

     

constexpr uint8_t DIRECTION_R = PD5;
constexpr uint8_t DIRECTION_L = PD4;
constexpr uint8_t TURN_SPEED_LEFT = 115;
constexpr uint8_t TURN_SPEED_RIGHT = 140;
constexpr uint8_t ZERO_SPEED = 0 ; 
constexpr uint8_t ADJUSTING_SPEED_FORWARD = 10; 
constexpr uint8_t ADJUSTING_SPEED_BACKWARD = 30; 

constexpr uint16_t TURN_90_DURATION_MS = 1500; 

 
class Motricity { 
public:
    static void init();
    static void adjustSpeed(uint8_t leftSpeed, uint8_t rightSpeed);
    static void moveForward(uint8_t speed);
    static void moveBackward(uint8_t speed);
    static void rotateInPlace(uint8_t speed);
    static void turnRight();
    static void turnLeft();
    static void stop();
    static void moveBackwardLeft(uint8_t speed);
    static void moveBackwardRight(uint8_t speed);
    static void dynamicTurnLeft(uint8_t leftSpeed, uint8_t rightSpeed);
    static void dynamicTurnRight(uint8_t leftSpeed, uint8_t rightSpeed);
    
private:
    static WheelTimer wheelTimerInstance; 
    static WheelTimer* timer;  
 };
