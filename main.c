#include "Clock.h"
#include "ADC.h"
#include "Oled.h" 
#include "GPIO.h"
#include "stm32f103c8t6.h"
#include "timer.h"

#define VCC									(float)	3.3
#define FIXED_RESISTOR 			(float) 10000
#define RFlexMin 						(float)	40000
#define RFlexMax 						(float)	100000
#define AngleMin 						(float)	0
#define AngleMax 						(float)	90

static uint32_t value = 0;
static float 		angle;
static uint8_t update_flag = 0; // Flag to signal OLED update

void ADC1_2_IRQHandler(ADC_TypeDef* ADC);
void  TIM2_IRQHandler(void);

void Convert_ADC(uint32_t adc);
float map(float x, float in_min, float in_max, float out_min, float out_max);
float GetAngle(float adcValue);


float map( float x, float in_min, float in_max,float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float GetAngle(float adcValue)
{
		float voltage;
	  float temp;

    voltage = (float)((adcValue * VCC) / ADC_RESOLUTION);

    float Rflex = (float)(((VCC * FIXED_RESISTOR) / voltage) - FIXED_RESISTOR);

    temp = map(Rflex, RFlexMin, RFlexMax, AngleMin, AngleMax);

    return temp;
}
void Convert_ADC(uint32_t adc)
{
	if (adc > 650 )
	{
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_4, 0);
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_5, 0);
		oled_print(I2C1, 4, 50, "DO NOTHING", 's');
	}
	else if ((adc > 400) && (adc < 650 ))
	{
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_4, 1);
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_5, 0);
		//oled_print(I2C1, 4, 50, "I'M HUNGRY", 's');
	}
	else if ((adc > 200) && (adc < 400 ))
	{
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_4, 0);
		GPIO_WriteOutputPin(GPIOA, GPIO_PIN_5, 1);
		oled_print(I2C1, 4, 50, "I'M DEATH", 's');
	}

}
void delay(volatile int time);

void delay(volatile int time)
{
		while (time--);
}

int main()
{
	GPIO_Init(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_MODE_ANALOG);
	GPIO_WriteOutputPin(GPIOA, GPIO_PIN_4, 1);
	
	ADC_Config(ADC1);
	ADC_Start(ADC1);
	
	TIM_Config(TIMER2);
	TIM_Base_Start_IT(TIMER2);
	
	oled_init_64(I2C1);
	oled_clear(I2C1);	
		
	while (1) 
	{
		GPIO_TogglePin(GPIOA,GPIO_PIN_4);
		if (update_flag) 
		{

			update_flag = 0;
		}
			value = ADC_GetValue(ADC1);	
			Convert_ADC(value);
			oled_clear(I2C1);
	}
}

void  TIM2_IRQHandler(void)
{
	while ((TIMER2->TIMx_SR) & (1 << 0)) {
		update_flag = 1;
		TIMER2->TIMx_SR &= ~(1u << 0);
	}
}	

