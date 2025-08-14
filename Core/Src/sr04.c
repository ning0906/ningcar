#include "sr04.h"
#include "pid.h"
extern TIM_HandleTypeDef htim3;
uint16_t count;
float distance;
void delay_us(uint16_t cnt)
{
    uint8_t i;
    while(cnt)
    {
        for (i = 0; i < 10; i++)
        {
 
        }
        cnt--;
    }
}
void get_distance()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET);
	delay_us(12);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);




}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*if(GPIO_Pin==GPIO_PIN_2)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_SET)
		{
		__HAL_TIM_SetCounter(&htim3,0);
			HAL_TIM_Base_Start(&htim3);//打开定时器开始计时
			
		}
			else 
			{
				HAL_TIM_Base_Stop(&htim3);
				count=__HAL_TIM_GetCounter(&htim3);
				//distance=count/1000000*340*100/2;
				distance=count*0.017;
		
		}
	}*/
	if(GPIO_Pin==GPIO_PIN_5)
	{if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)==GPIO_PIN_SET)
     Control();
	}


}