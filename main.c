#include "stm8s.h"
#include "main.h"
float factor = 0.59;
volatile uint16_t adc;
void CLK_init(void){
	CLK -> CKDIVR = 0x00; // Set the frequency to 16 MHz
	CLK -> PCKENR1 = 0xFF; // Enable peripherals
}
void ACD1_init(void){
	ADC1 -> CR2 |= ADC1_CR2_ALIGN; //Выравнивание по левому краю
	ADC1 -> TDRH |= _BV(ADC_CHANNEL);
	ADC1 -> CSR |= ADC_CHANNEL | ADC1_CSR_EOCIE; //Выбираем 3 канал
	ADC1 -> CR1 |= _BV(4) | _BV(5) | _BV(6) | ADC1_CR1_ADON | ADC1_CR1_CONT;
	ADC1 -> CR1 |= ADC1_CR1_ADON;
}
uint16_t ACD1_get_value(void){
	return adc;
}
float ACD1_get_voltage(void){
	return ACD1_get_value() * factor;
}
void enable_lights(uint8_t count){
	if(count == 0){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR &= ~_BV(2);
	}else if(count == 1){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 2){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6);
		GPIOC -> ODR = _BV(7);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 3){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 4){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 5){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 6){
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 7){
		GPIOB -> ODR |=  _BV(5);
		GPIOB -> ODR &= ~_BV(4);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 8){
		GPIOB -> ODR &= ~_BV(4) & ~_BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2);
	}
}

void GPIO_init(void){
	GPIOB -> DDR |= _BV(4) | _BV(5);
	GPIOB -> CR1 |= _BV(4) | _BV(5);
	GPIOB -> ODR |= _BV(4) | _BV(5);

	GPIOC -> DDR |= _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7);
	GPIOC -> CR1 |= _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7);

	GPIOD -> DDR |= _BV(2);
	GPIOD -> CR1 |= _BV(2);
}
void display(float volts){
	if (volts <= 10.0){
		enable_lights(0);
	}else if(volts > 10.0 && volts <= 150.0){
		enable_lights(1);
	}else if(volts > 150.0 && volts <= 200.0){
		enable_lights(2);
	}else if(volts > 200.0 && volts <= 250.0){
		enable_lights(3);
	}else if(volts > 250.0 && volts <= 300.0){
		enable_lights(4);
	}else if(volts > 300.0 && volts <= 350.0){
		enable_lights(5);
	}else if(volts > 350.0 && volts <= 400.0){
		enable_lights(6);
	}else if(volts > 400.0 && volts <= 450.0){
		enable_lights(7);
	}else if(volts > 450.0){
		enable_lights(8);
	}
}
void start_check(void){
	uint32_t i;
	uint32_t delay = 50000;
	int count = 0;
	for(count = 0 ;count <= 8;count ++){
		enable_lights(count);
		for(i = 0;i < delay ;i++){nop();};
	}
	for(count = 8 ;count >= 0;count --){
		enable_lights(count);
		for(i = 0;i < delay ;i++){nop();};
	}
}

void ADC1_Interrupt(void) __interrupt(22) {
	adc = ADC1 -> DRH << 8;
	adc |= ADC1 -> DRL;
	ADC1 -> CSR &= ~ADC1_CSR_EOC;
}
void delay_ms(uint32_t del){
	uint32_t i;
	uint32_t ddd = 540 * del;
	for(i = 0;i < ddd;i++){nop();};
}
int main() {
	CLK_init();
	ACD1_init();
	GPIO_init();
	enableInterrupts();
	start_check();
	while(1){
		delay_ms(100);
		display(ACD1_get_voltage());
	};
}
