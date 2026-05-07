#include "externalInterrupts.h"

ExternalInterrupts::ExternalInterrupts(InterruptNumber interruptNumber, InterruptMode mode)
    : interruptNumber_(interruptNumber)  
{
    setPinMode();
    setInterruptMode(mode);
}

void ExternalInterrupts::setPinMode()
{
    switch (interruptNumber_)
    {
    case InterruptNumber::INTERRUPT0:
        DDRD &= ~(1 << PIND2);  
        break;
    case InterruptNumber::INTERRUPT1:
        DDRD &= ~(1 << PIND3);  
        break;
    case InterruptNumber::INTERRUPT2:
        DDRB &= ~(1 << PINB2);  
        break;
    }
}

void ExternalInterrupts::setInterruptMode(InterruptMode mode)
{
    cli();  

    uint8_t modeFlags[][3] = {
        {ISC00, ISC01}, 
        {ISC10, ISC11}, 
        {ISC20, ISC21}, 
    };

    uint8_t ISCn0 = modeFlags[(uint8_t)interruptNumber_][0];
    uint8_t ISCn1 = modeFlags[(uint8_t)interruptNumber_][1];

    switch (mode)
    {
    case InterruptMode::BOTH_EDGES:
        EICRA |= (1 << ISCn0);
        EICRA &= ~(1 << ISCn1);
        break;
    case InterruptMode::FALLING_EDGE:
        EICRA &= ~(1 << ISCn0);
        EICRA |= (1 << ISCn1);
        break;
    case InterruptMode::RISING_EDGE:
        EICRA |= (1 << ISCn0);
        EICRA |= (1 << ISCn1);
        break;
    }

    sei();  
}

void ExternalInterrupts::enableInterrupt()
{
    switch (interruptNumber_)
    {
    case InterruptNumber::INTERRUPT0:
        EIMSK |= (1 << INT0);  
        break;
    case InterruptNumber::INTERRUPT1:
        EIMSK |= (1 << INT1);  
        break;
    case InterruptNumber::INTERRUPT2:
        EIMSK |= (1 << INT2);  
        break;
    }
}

void ExternalInterrupts::disableInterrupt()
{
    switch (interruptNumber_)
    {
    case InterruptNumber::INTERRUPT0:
        EIMSK &= ~(1 << INT0);  
        break;
    case InterruptNumber::INTERRUPT1:
        EIMSK &= ~(1 << INT1);  
        break;
    case InterruptNumber::INTERRUPT2:
        EIMSK &= ~(1 << INT2);  
        break;
    }
}

void ExternalInterrupts::clearInterruptFlag()
{
    switch (interruptNumber_)
    {
    case InterruptNumber::INTERRUPT0:
        EIFR |= (1 << INTF0);  
        break;
    case InterruptNumber::INTERRUPT1:
        EIFR |= (1 << INTF1);  
        break;
    case InterruptNumber::INTERRUPT2:
        EIFR |= (1 << INTF2);  
        break;
    }
}
