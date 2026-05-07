#include "usart.h"

Usart::Usart()
{
    initialisation();  
}

void Usart::initialisation(void)
{
    UBRR0H = 0;
    UBRR0L = 0xCF;     

    UCSR0B |= (1 << RXEN0)  | (1 << TXEN0); 
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); 
}

void Usart::sendChar(uint8_t data)
{
    while (!(UCSR0A & (1 << UDRE0))); 
    UDR0 = data; 
}

uint8_t Usart::receiveChar()
{
    while (!(UCSR0A & (1 << RXC0))); 
    return UDR0;  
}

void Usart::sendString(const char* word)
{
    if (word == nullptr) return;  
    while (*word) 
    {
        sendChar(*word++);
    }
}

