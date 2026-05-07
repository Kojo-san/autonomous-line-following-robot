/* 
 * Auteurs: Bendriss Ilyas, Caya Alex, Daoust Alexis, Guimond Mathieu, Kalefe Gamaliel
 * Section 3
 * 
 * Description:  Cette classe permet de configurer et manipuler le timer 1 du microcontrôleur AVR.  
 *               Elle offre des fonctionnalités pour le mode CTC, Fast PWM et Normal, ainsi que des interruptions.  
 *               Elle permet aussi de gérer la fréquence et la durée des événements temporisés.  
 *
 */

#pragma once

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

class Timer1 {
public:
    static constexpr uint16_t DEFAULT_PRESCALER_MS = 1024;
    static constexpr uint16_t DEFAULT_TOP_VALUE_TICKS = 0xFFFF;
    static constexpr uint16_t DEFAULT_RESOLUTION_FAST_PWM_BITS = 8;

    enum class Mode { NONE, CTC, FAST_PWM, PHASE_CORRECT_PWM, NORMAL };

    static void resetMode();
    
    static void initCTC();                    
    static void initCTC(uint16_t topValue);
    static void initFastPWM(uint8_t resolution, uint16_t prescaler, bool inverting);
    static void initPhaseCorrectPWM(uint8_t resolution, uint16_t prescaler, bool inverting);
    static void initNormal();

    static void setDelayCTC(uint16_t delay_ms);

    static void waitDelay_ms(uint16_t delay_ms);
    static void enableCTCInterruptA();
    static void enableCTCInterruptB();
    static void enableOverflowInterrupt();
    static void disableCTCInterruptA();
    static void disableCTCInterruptB();
    static void disableOverflowInterrupt();

    static void setPrescaler(uint16_t prescaler_ms);
    static uint16_t getPrescaler();

    static void setTopValue(uint16_t value_ticks);
    static uint16_t getTopValue();

    static void stop();
    static void resume();
    static void resetToZero();


    static void setOCR1A(uint16_t value_ticks);
    static uint16_t getOCR1A();
    static void setOCR1B(uint16_t value_ticks);
    static uint16_t getOCR1B();

    static uint16_t getCurrentCount();

    static bool isValidPrescaler(uint16_t prescaler);

    

private:
    static uint16_t currentPrescaler;
    static uint16_t savedPrescaler;
    static Mode currentMode;
};
