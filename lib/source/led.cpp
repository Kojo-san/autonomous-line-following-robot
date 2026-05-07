#include "led.h"
#include "timer1.h"
constexpr uint8_t AMBER_DELAY = 50;
constexpr uint8_t TIME_BETWEEN_DELS = 5;
Led::Led() {
    DDRC |= GREEN_DEL_PIN | RED_DEL_PIN;
}

void Led::off() {
    PORTC &= ~(GREEN_DEL_PIN | (RED_DEL_PIN));
}


void Led::red() {
    PORTC &= ~GREEN_DEL_PIN;
    PORTC |= RED_DEL_PIN;
}

void Led::green() {
    PORTC &= ~RED_DEL_PIN;
    PORTC |= GREEN_DEL_PIN;
}

void Led::amber() {
    for (uint8_t i = 0; i < AMBER_DELAY; i++) {
        red();
        Timer1::waitDelay_ms(TIME_BETWEEN_DELS);
        green();
        Timer1::waitDelay_ms(TIME_BETWEEN_DELS);
    }
}