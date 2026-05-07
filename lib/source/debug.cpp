#include "debug.h"

void Debug::printMessage(const char *message) {
    if (message == nullptr) return;
    uint16_t i = 0;
    while (message[i] != '\0') {
        usart.sendChar(message[i++]); 
    }
}

void Debug::printNumber(uint16_t value) {
    char buffer[10];  
    itoa(value, buffer, 10);  
    printMessage(buffer);     
}