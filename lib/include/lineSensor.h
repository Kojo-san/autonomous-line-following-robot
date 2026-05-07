#pragma once

#include <avr/io.h>

class LineSensor {
public:
    uint8_t sensorsDetected = 0;
    bool S1;
    bool S2;
    bool S3;
    bool S4;
    bool S5;

    LineSensor();
    void detect();
    void updateState();
    bool detectFullIntersection();
    bool detectRightLeftInsterset();
    bool detectNoLine();
};
