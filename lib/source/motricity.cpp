#include "motricity.h"
#include "timer1.h"  

WheelTimer Motricity::wheelTimerInstance;
WheelTimer* Motricity::timer = &Motricity::wheelTimerInstance;


void Motricity::init() {
    timer->initPhaseCorrectPWM(8, false);  
    DDRD |= (1 << PD4) | (1 << PD5)| (1 << PD6) | (1 << PD7);
}

void Motricity::adjustSpeed(uint8_t leftSpeed, uint8_t rightSpeed) {
    timer->setDutyCycle(leftSpeed, rightSpeed);
}

void Motricity::moveForward(uint8_t speed) {
    PORTD &= ~(1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);
    adjustSpeed(speed - ADJUSTING_SPEED_FORWARD, speed); 
}

void Motricity::moveBackward(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);
    adjustSpeed(speed, speed);
}

void Motricity::rotateInPlace(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);
    adjustSpeed(speed, speed - 45);
}

void Motricity::turnRight() {
    PORTD &= ~(1 << DIRECTION_R);  
    PORTD &= ~(1 << DIRECTION_L);  
    
    adjustSpeed(0, TURN_SPEED_RIGHT);
    Timer1::waitDelay_ms(TURN_90_DURATION_MS);
    stop();
}

void Motricity::turnLeft() {
    PORTD &= ~(1 << DIRECTION_R);  
    PORTD &= ~(1 << DIRECTION_L);  

    adjustSpeed(TURN_SPEED_LEFT, ZERO_SPEED); 
    Timer1::waitDelay_ms(TURN_90_DURATION_MS);
    stop();
}

void Motricity::stop() {
    adjustSpeed(ZERO_SPEED, ZERO_SPEED);
    PORTD &= ~(1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);
}

void Motricity::dynamicTurnLeft(uint8_t leftSpeed, uint8_t rightSpeed) {
    PORTD &= ~(1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);
    adjustSpeed(leftSpeed, rightSpeed);
}

void Motricity::dynamicTurnRight(uint8_t leftSpeed, uint8_t rightSpeed) {
    PORTD &= ~(1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);
    adjustSpeed(leftSpeed, rightSpeed);
}

void Motricity::moveBackwardLeft(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);
    adjustSpeed(speed, ZERO_SPEED);
}

void Motricity::moveBackwardRight(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);
    adjustSpeed(ZERO_SPEED, speed);
}

