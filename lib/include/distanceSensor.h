#pragma once
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "usart.h"
#include "sound.h"
#include "motricity.h"
#include "led.h"

constexpr uint8_t BITSHIFT_VALUE_ADC = 2; 
constexpr uint8_t DISTANCE_SENSOR_PORT = PA0;
constexpr uint8_t OBSTACLE_READ_SHIFT_BITS = 2;
constexpr uint8_t OBSTACLE_SAMPLE_COUNT = 5;
constexpr uint8_t OBSTACLE_MIN_DISTANCE = 90;
constexpr uint8_t OBSTACLE_MAX_DISTANCE = 160;
constexpr uint8_t HOUSE_MIN_DISTANCE = 65;
constexpr uint8_t HOUSE_MAX_DISTANCE = 200;
constexpr uint8_t HOUSE_CONFIRMATION_COUNT = 2;

class DistanceSensor {
public:
    DistanceSensor() = default;

    void init();
    uint8_t detectObstacle();

    bool isObstacle();

    uint8_t getDistance();
    void enableReadValues();
    void disableReadValues();
    bool detectHouse();

private:
    bool readValuesEnable = false;
    uint8_t distance = 0;
    uint8_t detectHouseCount = 0;
    Usart usart;
    adc sensor;

    uint8_t readAveragedValue(uint8_t shiftBits, uint8_t samples);
};
