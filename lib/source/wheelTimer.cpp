#include "wheelTimer.h"

uint16_t WheelTimer::currentPrescaler = WheelTimer::DEFAULT_PRESCALER_MS;
uint16_t WheelTimer::savedPrescaler = 0;
WheelTimer::Mode WheelTimer::currentMode = WheelTimer::Mode::NONE;

void WheelTimer::resetMode() {
    cli(); 
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    TIFR2 = 0xFF;
    TCNT2 = 0;
    sei();  
    currentMode = Mode::NONE;
}

void WheelTimer::initCTC(uint8_t topValue_ticks) {
    resetMode();
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << WGM22);
    setTopValue(topValue_ticks);
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::CTC;
}

void WheelTimer::initNormal() {
    resetMode();
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::NORMAL;
}

void WheelTimer::initFastPWM(uint16_t prescaler, bool inverting) {
    resetMode();
    cli();
    TCCR2A = (1 << WGM20) | (1 << WGM21);
    TCCR2B = (1 << WGM22);
    
    uint8_t compareMode = (1 << COM2A1) | (1 << COM2B1);
    if (inverting) {
        compareMode |= (1 << COM2A0) | (1 << COM2B0);
    }
    TCCR2A |= compareMode;

    setPrescaler(prescaler);
    DDRD |= (1 << PD6) | (1 << PD7);
    currentMode = Mode::FAST_PWM;
    sei();
}

void WheelTimer::initPhaseCorrectPWM(uint16_t prescaler, bool inverting) {
    resetMode();
    cli ();

    TCCR2A = (1 << WGM20); 
    TCCR2B = 0;            

    uint8_t compareMode = (1 << COM2A1) | (1 << COM2B1);
    if (inverting) {
        compareMode |= (1 << COM2A0) | (1 << COM2B0);
    }
    TCCR2A |= compareMode;

    setPrescaler(prescaler);

    DDRD |= (1 << PD6) | (1 << PD7);

    currentMode = Mode::PHASE_CORRECT_PWM;
    sei();
}


void WheelTimer::setDelayCTC(uint16_t delay_ms) {
    uint32_t ticks = (F_CPU / (DEFAULT_PRESCALER_MS * 1000UL)) * delay_ms;
    if (ticks > 255) {
        ticks = 255;
    }
    initCTC(static_cast<uint8_t>(ticks));
}

void WheelTimer::enableCTCInterruptA() { TIMSK2 |= (1 << OCIE2A); }
void WheelTimer::enableCTCInterruptB() { TIMSK2 |= (1 << OCIE2B); }
void WheelTimer::enableOverflowInterrupt() { TIMSK2 |= (1 << TOIE2); }
void WheelTimer::disableCTCInterruptA() { TIMSK2 &= ~(1 << OCIE2A); }
void WheelTimer::disableCTCInterruptB() { TIMSK2 &= ~(1 << OCIE2B); }
void WheelTimer::disableOverflowInterrupt() { TIMSK2 &= ~(1 << TOIE2); }

void WheelTimer::setPrescaler(uint16_t prescaler) {
    TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
    switch (prescaler) {
        case 1:    TCCR2B |= (1 << CS20); break;
        case 8:    TCCR2B |= (1 << CS21); break;
        case 32:   TCCR2B |= (1 << CS21) | (1 << CS20); break;
        case 64:   TCCR2B |= (1 << CS22); break;
        case 128:  TCCR2B |= (1 << CS22) | (1 << CS20); break;
        case 256:  TCCR2B |= (1 << CS22) | (1 << CS21); break;
        case 1024: TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); break;
        default:   TCCR2B |= (1 << CS21); prescaler = 8; break;
    }
    currentPrescaler = prescaler;
}


uint16_t WheelTimer::getPrescaler() { return currentPrescaler; }
void WheelTimer::setTopValue(uint8_t value_ticks) { OCR2A = value_ticks; }
uint8_t WheelTimer::getTopValue() { return OCR2A; }
void WheelTimer::setOCR2A(uint8_t value_ticks) { OCR2A = value_ticks; }
uint8_t WheelTimer::getOCR2A() { return OCR2A; }
void WheelTimer::setOCR2B(uint8_t value_ticks) { OCR2B = value_ticks; }
uint8_t WheelTimer::getOCR2B() { return OCR2B; }
uint8_t WheelTimer::getCurrentCount() { return TCNT2; }
void WheelTimer::stop() { savedPrescaler = currentPrescaler; TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); }
void WheelTimer::resume() { setPrescaler(savedPrescaler); }
void WheelTimer::resetToZero() { TCNT2 = 0; }

void WheelTimer::setDutyCycle(uint8_t dutyA, uint8_t dutyB) {
    OCR2A = dutyA;
    OCR2B = dutyB;
}
