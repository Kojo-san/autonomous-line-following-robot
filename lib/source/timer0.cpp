#include "timer0.h"

uint16_t Timer0::currentPrescaler = Timer0::DEFAULT_PRESCALER_MS;
uint16_t Timer0::savedPrescaler = 0;
Timer0::Mode Timer0::currentMode = Timer0::Mode::NONE;

void Timer0::resetMode() {
    cli(); 
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;
    TIFR0 = 0xFF;
    TCNT0 = 0;
    sei();  
    currentMode = Mode::NONE;
}

void Timer0::initCTC(uint8_t topValue_ticks) {
    resetMode();
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << WGM02);
    setTopValue(topValue_ticks);
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::CTC;
}

void Timer0::initNormal() {
    resetMode();
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::NORMAL;
}

void Timer0::initPhaseCorrectPWM(uint16_t prescaler, bool inverting) {
    resetMode();
    cli();
    
    TCCR0A = (1 << WGM00);
    TCCR0B = 0;            

    uint8_t compareMode = (1 << COM0A1) | (1 << COM0B1);
    if (inverting) {
        compareMode |= (1 << COM0A0) | (1 << COM0B0);
    }
    TCCR0A |= compareMode;

    setPrescaler(prescaler);

    DDRB |= (1 << PB3) | (1 << PB4);

    currentMode = Mode::PHASE_CORRECT_PWM;
    sei();
}


void Timer0::initFastPWM(uint8_t prescaler, bool inverting) {
    resetMode();
    cli();
    TCCR0A = (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << WGM02);
    
    uint8_t compareMode = (1 << COM0A1) | (1 << COM0B1);
    if (inverting) {
        compareMode |= (1 << COM0A0) | (1 << COM0B0);
    }
    TCCR0A |= compareMode;

    setPrescaler(prescaler);
    DDRB |= (1 << PB3) | (1 << PB4);
    currentMode = Mode::FAST_PWM;
    sei();
}

// void Timer0::setDelayCTC(uint16_t delay_ms) {
//     uint32_t ticks = (F_CPU / (DEFAULT_PRESCALER_MS * 1000UL)) * delay_ms;
//     if (ticks > 255) {
//         ticks = 255;
//     }
//     initCTC(static_cast<uint8_t>(ticks));
// }

void Timer0::enableCTCInterruptA() { TIMSK0 |= (1 << OCIE0A); }
void Timer0::enableCTCInterruptB() { TIMSK0 |= (1 << OCIE0B); }
void Timer0::enableOverflowInterrupt() { TIMSK0 |= (1 << TOIE0); }
void Timer0::disableCTCInterruptA() { TIMSK0 &= ~(1 << OCIE0A); }
void Timer0::disableCTCInterruptB() { TIMSK0 &= ~(1 << OCIE0B); }
void Timer0::disableOverflowInterrupt() { TIMSK0 &= ~(1 << TOIE0); }

void Timer0::setPrescaler(uint16_t prescaler) {
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
    switch (prescaler) {
        case 1:    TCCR0B |= (1 << CS00); break;
        case 8:    TCCR0B |= (1 << CS01); break;
        case 64:   TCCR0B |= (1 << CS01) | (1 << CS00); break;
        case 256:  TCCR0B |= (1 << CS02); break;
        case 1024: TCCR0B |= (1 << CS02) | (1 << CS00); break;
        default:   TCCR0B |= (1 << CS01); prescaler = 8; break;  // fallback to 8
    }
    currentPrescaler = prescaler;
}


uint16_t Timer0::getPrescaler() { return currentPrescaler; }
void Timer0::setTopValue(uint8_t value_ticks) { OCR0A = value_ticks; }
uint8_t Timer0::getTopValue() { return OCR0A; }
void Timer0::setOCR0A(uint8_t value_ticks) { OCR0A = value_ticks; }
uint8_t Timer0::getOCR0A() { return OCR0A; }
void Timer0::setOCR0B(uint8_t value_ticks) { OCR0B = value_ticks; }
uint8_t Timer0::getOCR0B() { return OCR0B; }
uint8_t Timer0::getCurrentCount() { return TCNT0; }
void Timer0::stop() { savedPrescaler = currentPrescaler; TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); }
void Timer0::resume() { setPrescaler(savedPrescaler); }
void Timer0::resetToZero() { TCNT0 = 0; }

void Timer0::setDutyCycle(uint8_t dutyA, uint8_t dutyB) {
    OCR0A = dutyA;
    OCR0B = dutyB;
}

void Timer0::waitDelay_ms(uint16_t delay_ms) {
    setDelayCTC(delay_ms); 
    resetToZero();          

    while (!(TIFR0 & (1 << OCF0A)));  
    TIFR0 |= (1 << OCF0A);            
}

void Timer0::setDelayCTC(uint16_t delay_ms) {
    uint32_t ticks = (F_CPU / DEFAULT_PRESCALER_MS) * delay_ms / 1000;

    if (ticks > 255) { 
        ticks = 255;
    }

    initCTC(static_cast<uint8_t>(ticks));
}
