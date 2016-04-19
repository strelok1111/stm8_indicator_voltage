#include <string.h>
#include <stdio.h>
#include "stm8s.h"
#include "main.h"
float factor = 0.59;
volatile uint16_t adc;
void uart_write_c(const char c) {
	while(!(UART1 -> SR & UART1_SR_TXE));
	UART1 -> DR = c;
}

int uart_write(const char *str) {
	char i;
	for(i = 0; i < strlen(str); i++) {
		uart_write_c(str[i]);
	}
	return(i);
}
void UART_init(void){
	UART1 -> CR2 = UART1_CR2_TEN | UART1_CR2_REN | UART1_CR2_RIEN; // Allow TX & RX
	UART1 -> CR3 &= ~(UART1_CR3_STOP); // 1 stop bit
	UART1 -> BRR2 = 0x03;
	UART1 -> BRR1 = 0x68; // 9600 baud
	GPIOD -> DDR |= _BV(5); // Put TX line on
	GPIOD -> CR1 |= _BV(5);
}
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
		//uart_write("0\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR &= ~_BV(2);
	}else if(count == 1){
		//uart_write("1\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 2){
		//uart_write("2\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6);
		GPIOC -> ODR = _BV(7);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 3){
		//uart_write("3\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 4){
		//uart_write("4\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 5){
		//uart_write("5\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR &= ~_BV(3);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 6){
		//uart_write("6\n");
		GPIOB -> ODR |= _BV(4) | _BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 7){
		//uart_write("7\n");
		GPIOB -> ODR |=  _BV(5);
		GPIOB -> ODR &= ~_BV(4);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2);
	}else if(count == 8){
		//uart_write("8\n");
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

void delay3_us(uint32_t del){
	uint32_t i;
	for(i = 0;i < del;i++){nop();};
}

int main() {
	//uint8_t i_1 =0;
	//uint16_t adc = 0;
	//char buffer[10];
	CLK_init();
	//UART_init();
	ACD1_init();
	GPIO_init();
	enableInterrupts();

	start_check();
	while(1){
		delay_ms(100);
		//i_1++;
		//enable_lights(2);
		display(ACD1_get_voltage());

		//adc = ACD1_get_value();
		//display_adc(adc);
		//sprintf(buffer,"%d",adc);
		//uart_write(buffer);
		//uart_write("\n");
	};
}
