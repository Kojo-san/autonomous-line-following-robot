#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

class Sound {
public:
    Sound();
    void init();  
    void reset(); 
    void playSound(uint8_t note);  


private:
    static const uint8_t notes[37]; 
    static constexpr uint8_t MIDI_NOTE_MIN = 18;  
    static constexpr uint8_t MIDI_NOTE_MAX = 142;  
};