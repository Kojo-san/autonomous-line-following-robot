#include "lineSensor.h"

namespace {
constexpr uint8_t LEFT_SENSOR_PIN = PA2;
constexpr uint8_t LEFT_CENTER_SENSOR_PIN = PA3;
constexpr uint8_t CENTER_SENSOR_PIN = PA4;
constexpr uint8_t RIGHT_CENTER_SENSOR_PIN = PA5;
constexpr uint8_t RIGHT_SENSOR_PIN = PA6;
constexpr uint8_t FULL_INTERSECTION_SENSOR_COUNT = 5;
constexpr uint8_t NO_LINE_SENSOR_COUNT = 0;
}

LineSensor::LineSensor()
    : sensorsDetected(0), S1(false), S2(false), S3(false), S4(false), S5(false) {
    DDRA &= ~((1 << LEFT_SENSOR_PIN) | (1 << LEFT_CENTER_SENSOR_PIN) | (1 << CENTER_SENSOR_PIN) |
              (1 << RIGHT_CENTER_SENSOR_PIN) | (1 << RIGHT_SENSOR_PIN));
}

void LineSensor::detect() {
    S1 = (PINA & (1 << LEFT_SENSOR_PIN));
    S2 = (PINA & (1 << LEFT_CENTER_SENSOR_PIN));
    S3 = (PINA & (1 << CENTER_SENSOR_PIN));
    S4 = (PINA & (1 << RIGHT_CENTER_SENSOR_PIN));
    S5 = (PINA & (1 << RIGHT_SENSOR_PIN));
}

void LineSensor::updateState() {
    detect();
    sensorsDetected = 0;
    if (S1) sensorsDetected++;
    if (S2) sensorsDetected++;
    if (S3) sensorsDetected++;
    if (S4) sensorsDetected++;
    if (S5) sensorsDetected++;
}

bool LineSensor::detectFullIntersection() {
    return sensorsDetected == FULL_INTERSECTION_SENSOR_COUNT;
}

bool LineSensor::detectRightLeftInsterset() {
    return detectFullIntersection();
}

bool LineSensor::detectNoLine() {
    return sensorsDetected == NO_LINE_SENSOR_COUNT;
}
