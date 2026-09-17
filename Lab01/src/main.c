#include <avr/io.h>
#include <util/delay.h>

#define POT_PIN PB4 //potentiometer pin (pin 3)
#define DATA_PIN PB2 //data pin (pin 7)
#define CLOCK_PIN PB3 //clock pin (pin 2)
#define LATCH_PIN PB0 //latch pin (pin 5)
#define OE_PIN PB1 //output enable pin (pin 6)

int adc_setup(){
    // datasheet - 17.13.1 ADMUX – ADC Multiplexer Selection Register
    ADMUX |= (1<<MUX1); //select ADC channel 2 (PB4) as input
    ADMUX |= (1<<ADLAR); //left adjust ADC result for 8-bit resolution
    // ADMUX = 00100010 -> Vcc reference voltage, left adjusted, ADC2 (PB4) as input

    // datasheet - 17.13.2 ADCSRA – ADC Control and Status Register A
    ADCSRA |= (1<<ADEN); //enable ADC
    // for 125 kHz ADC clock, prescaler = 8 -> ADCSRA [3:0] = 011
    ADCSRA |= (1<<ADPS1) | (1<<ADPS0); //set prescaler to 8

    return 0;
}

int adc_read(){
    //to adc read: start conversion, read ADC value, return value
    ADCSRA |= (1<<ADSC); //start ADC conversion
    while (ADCSRA & (1<<ADSC)); //wait for conversion to complete
    return ADCH; //return the ADC value 
}

int DDRB_setup(){
    DDRB |= (1<<DATA_PIN); //set data pin output
    DDRB |= (1<<LATCH_PIN); //set latch pin output
    DDRB |= (1<<CLOCK_PIN); //set clock pin output
    DDRB |= (1<<OE_PIN); //set OE pin output

    //set OE and latch low
    PORTB &= ~(1<<OE_PIN);
    PORTB &= ~(1<<LATCH_PIN);
    return 0;
}

int shift_out(uint8_t data){
    for(int i=0; i<8; i++){ 
        if (data & (0b10000000)){ //if MSB is 1 set pin high
            PORTB |= (1<<DATA_PIN);
        }
        else{
            PORTB &= ~(1<<DATA_PIN);
        }

        PORTB |=  (1 << CLOCK_PIN);   // rising edge shifts the bit in
        PORTB &= ~(1 << CLOCK_PIN);

        data <<= 1; //shift to next MSB
    }
    return 0;    
}

int main(){
    //init pins
    DDRB_setup();
    adc_setup();

    while (1) {
        uint8_t adc_val = adc_read(); //read potentiometer value (0-255)
        uint8_t level = 9*adc_val/256; //scale to 0-8 for 8 LEDs
        uint8_t out = (1 << level) - 1; //create bitmask for LEDs

        PORTB &= ~(1<<LATCH_PIN); //set latch low
        shift_out(out);
        PORTB |= (1<<LATCH_PIN); //set latch high

        _delay_ms(20);
    }
    return 0;
}