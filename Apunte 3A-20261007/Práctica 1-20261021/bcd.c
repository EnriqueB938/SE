#include <stdint.h>

// mirad el manual de usuario, PINSEL4 en el LPC2378. Página: 162 de 706
// emplea 2 bits de configuración 00 01 10 11
#define PINSEL4     (*(volatile uint32_t *)0xE002C010u)

// mirad el manual de usuario, página: 173 de 706
// Direccionamos las GPIO de forma indirecta
// Se pueden incluso emplear máscaras
#define FIO2DIR     (*(volatile uint32_t *)0x3FFFC040u)
#define FIO2SET     (*(volatile uint32_t *)0x3FFFC058u)
#define FIO2CLR     (*(volatile uint32_t *)0x3FFFC05Cu)

#define LED0        (1u << 0)

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 50000u; ++i) {
        __asm__ volatile ("nop");
    }
}

/* Required by startup.S */
void irq_handler_c(void)
{
    for (;;) {
    }
}

/* Required by startup.S */
void fiq_handler_c(void)
{
    for (;;) {
    }
}

int main(void)
{
    PINSEL4 &= ~(3u << 0);
    FIO2DIR |= 255u;

    uint8_t counter = 0;
    FIO2CLR = ~counter;    
    for (;;) {
        FIO2SET = counter;
        FIO2CLR = ~counter; 
        delay();
        counter++;
    }
}