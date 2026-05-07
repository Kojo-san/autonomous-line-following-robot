/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description: Cette classe permet de configurer et de contrôler le Timer0 d'un microcontrôleur AVR.  
 *              Elle supporte plusieurs modes (CTC, PWM, etc.), la gestion des interruptions, 
 *              ainsi que la configuration du prescaler, de la valeur TOP et des registres OCR0A et OCR0B.
 * 
 */

#pragma once

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

class Timer0 {
public:
    static constexpr uint16_t DEFAULT_PRESCALER_MS = 255;
    static constexpr uint8_t DEFAULT_TOP_VALUE_TICKS = 0xFF;
    
    enum class Mode { NONE, CTC, FAST_PWM, PHASE_CORRECT_PWM, NORMAL };

    static void resetMode();
    
    static void initCTC(uint8_t topValue);
    static void initNormal();
    static void initPhaseCorrectPWM(uint16_t prescaler, bool inverting = false);
    static void initFastPWM(uint8_t prescaler, bool inverting);

    

    static void enableCTCInterruptA();
    static void enableCTCInterruptB();
    static void enableOverflowInterrupt();
    static void disableCTCInterruptA();
    static void disableCTCInterruptB();
    static void disableOverflowInterrupt();

    static void waitDelay_ms(uint16_t delay_ms);
    static void setDelayCTC(uint16_t delay_ms);

    static void setPrescaler(uint16_t prescaler);
    static uint16_t getPrescaler();

    static void setTopValue(uint8_t value_ticks);
    static uint8_t getTopValue();

    static void stop();
    static void resume();
    static void resetToZero();

    static void setOCR0A(uint8_t value_ticks);
    static uint8_t getOCR0A();
    static void setOCR0B(uint8_t value_ticks);
    static uint8_t getOCR0B();

    static uint8_t getCurrentCount();
    static void setDutyCycle(uint8_t dutyA, uint8_t dutyB);
    
private:
    static uint16_t currentPrescaler;
    static uint16_t savedPrescaler;
    static Mode currentMode;
};
