#include "sound.h"

const uint8_t Sound::notes[37] = {
    142, 134, 126, 119, 112, 106, 100, 94,
    89, 84, 79, 75, 71, 67, 63, 59, 56, 53,
    50, 47, 44, 42, 39, 37, 35, 33, 31, 29,
    28, 26, 25, 23, 22, 21, 20, 19, 18
};

Sound::Sound() {
    init();
}

void Sound::init() {
    DDRB |= (1 << PORTB2) | (1 << PORTB3); 
}

void Sound::reset() {
    // Exemple : désactive le PWM
    TCCR0A = 0;
    TCCR0B = 0;
    OCR1A = 0;

    // Ou si c’est une broche en sortie simple :
    PORTB &= ~(1 << PB3);  // ou autre broche selon le buzzer
}

void Sound::playSound(uint8_t note) {
    reset();  
    if (note >= MIDI_NOTE_MIN && note <= MIDI_NOTE_MAX) {  
        OCR0A = notes[note - MIDI_NOTE_MIN]; 
        TCCR0A = (1 << WGM01) | (1 << COM0A0);  
        TCCR0B = (1 << CS02); 
    }
}
