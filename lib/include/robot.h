#pragma once

#define F_CPU 8000000UL
 
#include <avr/io.h>
#include <util/delay.h>
#include "externalInterrupts.h"
#include "led.h"
#include "motricity.h"
#include "sound.h"
#include "lineSensor.h"
#include "timer1.h"
#include "timer0.h"
#include "motion.h"
#include "distanceSensor.h"


constexpr uint8_t BUTTON_PRESS_TARGET = 2;
constexpr uint16_t BUTTON_DEBOUNCE_MS = 85;
constexpr uint16_t START_WAIT_DELAY_MS = 2000;
constexpr uint8_t LED_BLINK_DELAY_MS = 250;
constexpr uint8_t LOW_NOTE = 18;

enum ButtonDirection  {
    LEFT = 1,
    RIGHT = 0
};
enum HouseState {
    INIT,
    DETECT_CROSSINGS,
    TURN_AFTER_CROSSINGS,
    SEARCH_LINE,
    BOOST_IF_READY,
    CHECK_OBSTACLE,
    HANDLE_OBSTACLE,
    FINISH_LOOP
};
enum class GridState {
    FOLLOW_INITIAL_LINE,
    CHECK_FIRST_OBSTACLE,
    EVALUATE_L_POSITION,
    RETREAT_AND_TURN,
    CHECK_SECOND_OBSTACLE,
    EVALUATE_K_POSITION,
    FALLBACK_TO_M,
    END
};
struct Robot {

    Robot(); 
    void init();

    uint8_t getFirstButton() const;
    uint8_t getSecondButton() const;

    void handleButtonPress(uint8_t buttonState, uint8_t buttonValue, void (Led::*ledAction)());
    void wait();
    void playSound();
    void handleButtonAction(uint8_t button);
    void endLed();
    void indicateObstacle();
    void indicateGoodPassage();
    bool detectHouse();
    bool detectFirstPoleGrid();
    bool detectSecondPoleGrid();
    void houseLoop();
    void handleBC();
    void fromK();
    void fromL();
    void fromM();
    void gridStateNavigation();
    void testDistanceSensor(); 
    void testCaptorDistance(); 




    Led led;
    Motricity motor;
    Sound sound;
    LineSensor lineSensor;
    Motion motion;
    DistanceSensor distanceSensor;
    volatile uint8_t firstButton = 0;
    volatile uint8_t secondButton = 0;
    volatile uint8_t pressCount = 0;
    HouseState currentState;
    uint8_t crossingCount;
    uint8_t lostLineCount;
    bool boostDone;
    
};
