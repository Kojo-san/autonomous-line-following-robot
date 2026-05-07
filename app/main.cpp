#define F_CPU 8000000UL

#include <avr/interrupt.h>
#include <avr/io.h>

#include "led.h"
#include "robot.h"

Robot robot;

ISR(INT0_vect) {
    const uint8_t buttonState = PIND & (1 << PIND2);
    robot.handleButtonPress(buttonState, 0, &Led::green);
}

ISR(INT1_vect) {
    const uint8_t buttonState = PIND & (1 << PIND3);
    robot.handleButtonPress(buttonState, 1, &Led::red);
}

int main() {
    robot.init();
    robot.wait();
    robot.gridStateNavigation();

    return 0;
}
