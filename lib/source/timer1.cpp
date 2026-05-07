#include "timer1.h"

uint16_t Timer1::currentPrescaler = Timer1::DEFAULT_PRESCALER_MS;
uint16_t Timer1::savedPrescaler = 0;
Timer1::Mode Timer1::currentMode = Timer1::Mode::NONE;

void Timer1::resetMode() {
    cli(); 
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;
    TIFR1 = 0xFF; 
    TCNT1 = 0;
    sei();  
    currentMode = Mode::NONE;
}


void Timer1::initCTC() {
    initCTC(0x0001);
}

void Timer1::initCTC(uint16_t topValue_ticks) {
    resetMode();
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    setTopValue(topValue_ticks);
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::CTC;
}

void Timer1::initNormal() {
    resetMode();
    TCCR1A = 0;
    TCCR1B = 0;
    setPrescaler(DEFAULT_PRESCALER_MS);
    currentMode = Mode::NORMAL;
}

void Timer1::initFastPWM(uint8_t resolution, uint16_t prescaler, bool inverting) {
    resetMode();
    cli();
    TCCR1A = 0;
    TCCR1B = 0;

    uint8_t compareMode = (1 << COM1A1) | (1 << COM1B1);
    if (inverting) {
        compareMode |= (1 << COM1A0) | (1 << COM1B0); 
    }

    switch (resolution) {
        case 8:   
            TCCR1A |= compareMode | (1 << WGM10);
            TCCR1B |= (1 << WGM12);
            setTopValue(0x00FF);
            break;
        case 9:   
            TCCR1A |= compareMode | (1 << WGM11);
            TCCR1B |= (1 << WGM12);
            setTopValue(0x01FF);
            break;
        case 10: 
            TCCR1A |= compareMode | (1 << WGM11) | (1 << WGM10);
            TCCR1B |= (1 << WGM12);
            setTopValue(0x03FF);
            break;
        default: 
            TCCR1A |= compareMode | (1 << WGM10);
            TCCR1B |= (1 << WGM12);
            setTopValue(0x00FF);
            break;
    }

    setPrescaler(prescaler);

    DDRD |= (1 << PD4) | (1 << PD5);

    currentMode = Mode::FAST_PWM;
    sei();
}

void Timer1::initPhaseCorrectPWM(uint8_t resolution, uint16_t prescaler, bool inverting) {
    resetMode();
    cli();
    TCCR1A = 0;
    TCCR1B = 0;

    uint8_t compareMode = (1 << COM1A1) | (1 << COM1B1);
    if (inverting) {
        compareMode |= (1 << COM1A0) | (1 << COM1B0);
    }

    switch (resolution) {
        case 8:
            TCCR1A |= compareMode | (1 << WGM10);
            TCCR1B |= 0; 
            break;
        case 9:
            TCCR1A |= compareMode | (1 << WGM11);
            TCCR1B |= 0; 
            break;
        case 10:
            TCCR1A |= compareMode | (1 << WGM11) | (1 << WGM10);
            TCCR1B |= 0; 
            break;
        default:
            TCCR1A |= compareMode | (1 << WGM10);
            TCCR1B |= 0;
            break;
    }

    setPrescaler(prescaler);

    
    DDRD |= (1 << PD4) | (1 << PD5);

    currentMode = Mode::PHASE_CORRECT_PWM;
    sei();
}



void Timer1::setDelayCTC(uint16_t delay_ms) {
    uint32_t ticks = (F_CPU / DEFAULT_PRESCALER_MS) * delay_ms / 1000;

    if (ticks > DEFAULT_TOP_VALUE_TICKS){ 
        ticks = DEFAULT_TOP_VALUE_TICKS;
    }

    initCTC(static_cast<uint16_t>(ticks));
}

void Timer1::enableCTCInterruptA() {
    TIMSK1 |= (1 << OCIE1A);
}

void Timer1::enableCTCInterruptB() {
    TIMSK1 |= (1 << OCIE1B);
}

void Timer1::enableOverflowInterrupt() {
    TIMSK1 |= (1 << TOIE1);
}

void Timer1::disableCTCInterruptA() {
    TIMSK1 &= ~(1 << OCIE1A);
}

void Timer1::disableCTCInterruptB() {
    TIMSK1 &= ~(1 << OCIE1B);
}

void Timer1::disableOverflowInterrupt() {
    TIMSK1 &= ~(1 << TOIE1);
}

void Timer1::setPrescaler(uint16_t prescaler_ms) {
    if (!isValidPrescaler(prescaler_ms)) return;
    
    currentPrescaler = prescaler_ms;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    switch (prescaler_ms) {
        case 1:    TCCR1B |= (1 << CS10); break;
        case 8:    TCCR1B |= (1 << CS11); break;
        case 64:   TCCR1B |= (1 << CS11) | (1 << CS10); break;
        case 256:  TCCR1B |= (1 << CS12); break;
        case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
    }
}

uint16_t Timer1::getPrescaler() {
    return currentPrescaler;
}

void Timer1::setTopValue(uint16_t value_ticks) {
    OCR1A = value_ticks;
}

uint16_t Timer1::getTopValue() {
    return OCR1A;
}

void Timer1::setOCR1A(uint16_t value_ticks) {
    OCR1A = value_ticks;
}

uint16_t Timer1::getOCR1A() {
    return OCR1A;
}

void Timer1::setOCR1B(uint16_t value_ticks) {
    OCR1B = value_ticks;
}

uint16_t Timer1::getOCR1B() {
    return OCR1B;
}

uint16_t Timer1::getCurrentCount() {
    return TCNT1;
}

bool Timer1::isValidPrescaler(uint16_t prescaler) {
    return (prescaler == 1 || prescaler == 8 || prescaler == 64 || 
            prescaler == 256 || prescaler == 1024);
}

void Timer1::stop() {
    savedPrescaler = currentPrescaler;
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}

void Timer1::resume() {
    setPrescaler(savedPrescaler);
}

void Timer1::resetToZero() {
    TCNT1 = 0;
}

void Timer1::waitDelay_ms(uint16_t delay_ms) {
    setDelayCTC(delay_ms);           // Configure Timer1 avec le délai en ms
    resetToZero();                   // Compteur à 0

    // Attente que la comparaison soit atteinte
    while (!(TIFR1 & (1 << OCF1A))); // OCF1A == 1 lorsque OCR1A est atteint
    TIFR1 |= (1 << OCF1A);           // Réinitialise le flag (écriture à 1)
}