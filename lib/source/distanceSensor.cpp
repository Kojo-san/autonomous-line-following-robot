#include "distanceSensor.h"

namespace {
constexpr uint16_t SERIAL_READ_DELAY_MS = 200;
constexpr uint8_t ADC_STABILIZATION_DELAY_MS = 5;
}

void DistanceSensor::init() {
    readValuesEnable = false;
    DDRA &= ~(1 << DISTANCE_SENSOR_PORT);
}

uint8_t DistanceSensor::detectObstacle() {
    distance = sensor.read(DISTANCE_SENSOR_PORT) >> BITSHIFT_VALUE_ADC;
    if (readValuesEnable) {
        usart.sendChar(distance);
        _delay_ms(SERIAL_READ_DELAY_MS);
    }

    return distance;
}

uint8_t DistanceSensor::getDistance() {
    return distance;
}

void DistanceSensor::enableReadValues() {
    readValuesEnable = true;
}

void DistanceSensor::disableReadValues() {
    readValuesEnable = false;
}

uint8_t DistanceSensor::readAveragedValue(uint8_t shiftBits, uint8_t samples) {
    if (samples == 0) {
        return 0;
    }

    uint16_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        _delay_ms(ADC_STABILIZATION_DELAY_MS);
        sum += sensor.read(DISTANCE_SENSOR_PORT);
    }

    const uint16_t average = sum / samples;
    return average >> shiftBits;
}

bool DistanceSensor::isObstacle() {
    const uint8_t value = readAveragedValue(OBSTACLE_READ_SHIFT_BITS, OBSTACLE_SAMPLE_COUNT);
    return value >= OBSTACLE_MIN_DISTANCE && value <= OBSTACLE_MAX_DISTANCE;
}

bool DistanceSensor::detectHouse() {
    const uint8_t currentDistance = detectObstacle();
    const bool inRange = currentDistance > HOUSE_MIN_DISTANCE && currentDistance < HOUSE_MAX_DISTANCE;

    if (inRange && detectHouseCount < HOUSE_CONFIRMATION_COUNT) {
        detectHouseCount++;
    }

    const bool houseDetected = detectHouseCount >= HOUSE_CONFIRMATION_COUNT;
    if (houseDetected) {
        detectHouseCount = 0;
    }

    return houseDetected;
}
