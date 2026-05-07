#include "motion.h"

#include <util/delay.h>

#include "timer1.h"

namespace {
constexpr int16_t MIN_PWM_SPEED = 0;
constexpr int16_t MAX_PWM_SPEED = 255;
constexpr int16_t LINE_FOLLOWING_GAIN = 8;
constexpr int16_t LINE_FOLLOWING_BASE_SPEED = 75;
constexpr uint8_t FULL_LINE_SENSOR_COUNT = 5;
constexpr uint8_t LINE_LOST_SENSOR_COUNT = 0;
constexpr uint16_t SENSOR_POLL_DELAY_MS = 10;
constexpr uint16_t LINE_STOP_DELAY_MS = 500;
constexpr uint16_t SHORT_STOP_DELAY_MS = 200;

constexpr uint8_t HOUSE_BASE_SPEED = 85;
constexpr uint8_t HOUSE_SPEED_COMPENSATION = 13;
constexpr uint8_t HOUSE_SOFT_CORRECTION = 25;
constexpr uint8_t HOUSE_HARD_CORRECTION_SPEED = 40;
}

Motion::Motion(Motricity& motricity, LineSensor& lineSensor)
    : motricity(motricity), lineSensor(lineSensor) {}

int16_t Motion::clampSpeed(int16_t value) {
    if (value < MIN_PWM_SPEED) {
        return MIN_PWM_SPEED;
    }

    if (value > MAX_PWM_SPEED) {
        return MAX_PWM_SPEED;
    }

    return value;
}

void Motion::followLine() {
    lineSensor.updateState();

    int16_t error = 0;
    if (lineSensor.sensorsDetected < FULL_LINE_SENSOR_COUNT) {
        if (lineSensor.S1) error -= 2;
        if (lineSensor.S2) error -= 1;
        if (lineSensor.S4) error += 1;
        if (lineSensor.S5) error += 2;
    } else {
        motricity.moveForward(110);
        Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
        motricity.stop();
    }

    const int16_t correction = error * LINE_FOLLOWING_GAIN;
    const int16_t leftSpeed = clampSpeed(LINE_FOLLOWING_BASE_SPEED - correction);
    const int16_t rightSpeed = clampSpeed(LINE_FOLLOWING_BASE_SPEED + correction);

    motricity.adjustSpeed(leftSpeed, rightSpeed);
}

void Motion::followLineUntilSensorCount(uint8_t targetSensorCount, bool useHouseLineFollowing) {
    bool started = false;

    while (true) {
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
        lineSensor.updateState();

        if (useHouseLineFollowing) {
            followLineHouse();
        } else {
            followLine();
        }

        if (lineSensor.sensorsDetected > 0) {
            started = true;
        }

        if (targetSensorCount == LINE_LOST_SENSOR_COUNT && started && lineSensor.sensorsDetected == LINE_LOST_SENSOR_COUNT) {
            motricity.stop();
            Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
            break;
        }

        if (targetSensorCount > LINE_LOST_SENSOR_COUNT && lineSensor.sensorsDetected >= targetSensorCount) {
            motricity.stop();
            Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
            break;
        }
    }
}

void Motion::followLineUntil(uint8_t targetSensorCount) {
    followLineUntilSensorCount(targetSensorCount, false);
}

void Motion::followLineHouse() {
    lineSensor.updateState();

    const uint8_t baseSpeed = HOUSE_BASE_SPEED;
    const uint8_t compensation = HOUSE_SPEED_COMPENSATION;

    if (lineSensor.S3) {
        if (!lineSensor.S2 && !lineSensor.S4) {
            motricity.adjustSpeed(baseSpeed - compensation, baseSpeed);
        } else if (lineSensor.S2) {
            motricity.adjustSpeed(baseSpeed - compensation, baseSpeed - 15);
        } else if (lineSensor.S4) {
            motricity.adjustSpeed(baseSpeed - compensation - 15, baseSpeed);
        }
        return;
    }

    if (lineSensor.S2 && !lineSensor.S4) {
        motricity.adjustSpeed(baseSpeed - compensation, baseSpeed - HOUSE_SOFT_CORRECTION);
    } else if (lineSensor.S4 && !lineSensor.S2) {
        motricity.adjustSpeed(baseSpeed - compensation - HOUSE_SOFT_CORRECTION, baseSpeed);
    } else if (lineSensor.S1) {
        motricity.adjustSpeed(baseSpeed - compensation, HOUSE_HARD_CORRECTION_SPEED);
    } else if (lineSensor.S5) {
        motricity.adjustSpeed(HOUSE_HARD_CORRECTION_SPEED - compensation, baseSpeed);
    } else {
        motricity.stop();
    }
}

void Motion::followLineUntilHouse(uint8_t targetSensorCount) {
    followLineUntilSensorCount(targetSensorCount, true);
}

bool Motion::followLineUntilPoteau(DistanceSensor& distanceSensor) {
    while (true) {
        lineSensor.updateState();
        followLine();

        if (distanceSensor.isObstacle()) {
            Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
            motricity.stop();
            Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
            return true;
        }

        if (lineSensor.sensorsDetected == LINE_LOST_SENSOR_COUNT) {
            motricity.stop();
            Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
            return false;
        }

        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }
}

bool Motion::detect0() {
    lineSensor.updateState();
    return lineSensor.sensorsDetected == LINE_LOST_SENSOR_COUNT;
}

bool Motion::detect5() {
    lineSensor.updateState();
    return lineSensor.sensorsDetected == FULL_LINE_SENSOR_COUNT;
}

void Motion::followLineUntilZero() {
    followLineUntil(LINE_LOST_SENSOR_COUNT);
}

void Motion::turnRightUntil() {
    while (!(lineSensor.S3 || (lineSensor.S2 && lineSensor.S3) || (lineSensor.S3 && lineSensor.S4))) {
        motricity.adjustSpeed(0, 120);
        lineSensor.updateState();
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
}

void Motion::turnLeftUntil() {
    while (!(lineSensor.S2 && lineSensor.S3) && !(lineSensor.S3 && lineSensor.S4)) {
        lineSensor.updateState();
        motricity.adjustSpeed(120, 0);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
}

void Motion::turn90RightCaptor() {
    motricity.moveForward(140);
    Timer1::waitDelay_ms(1100);

    PORTD |= (1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);

    while (!lineSensor.S5) {
        lineSensor.updateState();
        motricity.adjustSpeed(110, 70);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(300);

    while (!(lineSensor.S4 && lineSensor.S3)) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, 130);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(300);
}

void Motion::turn90RightCaptorHouse() {
    PORTD |= (1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);

    while (!lineSensor.S5) {
        lineSensor.updateState();
        motricity.adjustSpeed(90, 50);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(300);

    while (!(lineSensor.S4 && lineSensor.S3)) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, 130);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(300);
}

void Motion::turn90LeftCaptor() {
    motricity.moveForward(130);
    Timer1::waitDelay_ms(1150);

    PORTD &= ~(1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!lineSensor.S1) {
        lineSensor.updateState();
        motricity.adjustSpeed(50, 90);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);

    while (!(lineSensor.S2 && lineSensor.S3)) {
        lineSensor.updateState();
        motricity.adjustSpeed(130, 0);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(300);
}

void Motion::turnRightUntilNewLine() {
    while (lineSensor.S1 || lineSensor.S2 || lineSensor.S3 || lineSensor.S4 || lineSensor.S5) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, 120);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    while (!(lineSensor.S1 || lineSensor.S2 || lineSensor.S3 || lineSensor.S4 || lineSensor.S5)) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, 120);
        if (lineSensor.S1 || lineSensor.S2 || lineSensor.S3 || lineSensor.S4 || lineSensor.S5) {
            motricity.stop();
            break;
        }
    }
}

void Motion::turn90Right() {
    motricity.moveForward(130);
    Timer1::waitDelay_ms(1150);

    PORTD |= (1 << DIRECTION_R);
    PORTD &= ~(1 << DIRECTION_L);

    lineSensor.updateState();
    motricity.adjustSpeed(100, 60);
    Timer1::waitDelay_ms(2300);

    motricity.stop();
    Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
}

void Motion::turn90Left() {
    motricity.moveForward(130);
    Timer1::waitDelay_ms(1150);

    PORTD &= ~(1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!(lineSensor.S2 && lineSensor.S3)) {
        lineSensor.updateState();
        motricity.adjustSpeed(60, 100);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(SHORT_STOP_DELAY_MS);
}

void Motion::moveBackwardLeft(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!lineSensor.S5) {
        lineSensor.updateState();
        motricity.adjustSpeed(speed, 0);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
}

void Motion::moveBackwardRight(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!lineSensor.S1) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, speed);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
}

void Motion::moveBackwardRightGrid(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!(lineSensor.S1 && lineSensor.S2)) {
        lineSensor.updateState();
        motricity.adjustSpeed(0, speed);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
}

void Motion::moveBackwardLeftGrid(uint8_t speed) {
    PORTD |= (1 << DIRECTION_R);
    PORTD |= (1 << DIRECTION_L);

    while (!(lineSensor.S5 && lineSensor.S4)) {
        lineSensor.updateState();
        motricity.adjustSpeed(speed, 0);
        Timer1::waitDelay_ms(SENSOR_POLL_DELAY_MS);
    }

    motricity.stop();
    Timer1::waitDelay_ms(LINE_STOP_DELAY_MS);
}
