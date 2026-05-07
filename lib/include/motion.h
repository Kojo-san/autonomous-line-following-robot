#pragma once 

#include "motricity.h"
#include "lineSensor.h"
#include <avr/io.h> 
#include "distanceSensor.h"

class Motion {
public:
    Motion(Motricity& motricity, LineSensor& lineSensor);
    void followLine();
    void followLineUntil(uint8_t targetSensorCount);
    void turnRightUntil();
    void turnLeftUntil();
    bool detect0();
    bool detect5();
    void turn90RightCaptor();
    void turn90LeftCaptor();
    void turn90Right();
    void turn90Left();
    bool followLineUntilPoteau(DistanceSensor& distanceSensor);
    void moveBackwardLeft(uint8_t speed);
    void moveBackwardRight(uint8_t speed);
    void turnRightUntilNewLine();
    void turn90RightCaptorHouse();
    void followLineUntilZero();
    void moveBackwardLeftGrid(uint8_t speed);
    void moveBackwardRightGrid(uint8_t speed);
    void followLineUntilHouse(uint8_t targetSensorCount);
    void followLineHouse();

private:
    static int16_t clampSpeed(int16_t value);
    void followLineUntilSensorCount(uint8_t targetSensorCount, bool useHouseLineFollowing);

    Motricity& motricity;
    LineSensor& lineSensor;
};
