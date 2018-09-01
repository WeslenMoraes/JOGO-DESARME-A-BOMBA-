/**
**===========================================================================
**
**  Displays:
**  PB0 = seg a, PB1 = seg b, PB2 = seg c, PB3 = seg d, PB4 = seg e, PB5 = seg f, PB6 = seg g
**
**  Transistores:
**  PA0 = T0, PA1 = T1
**
**  LEDs:
**  PA4 = Led_Verde, PA5 = Led_Amarelo
**
**  Buzzer:
**  PA6 = BZ0
**
**  Fios:
**  PC0 = F0, PC1 = F1, PC2 = F2, PC3 = F3, PC4 = F4, PC5 = F5, PC6 = F6, PC7 = F7
**
**  Botão:
**  PC8 = BTN0
**
**===========================================================================
*/
/* Includes */
#include "stm32f4xx.h"
#include "stdlib.h"
#include "time.h"

/* Private macro */
#define cathode_0                    ((uint32_t)0x03f)
#define cathode_1                    ((uint32_t)0X006)
#define cathode_2                    ((uint32_t)0X05b)
#define cathode_3                    ((uint32_t)0X04f)
#define cathode_4                    ((uint32_t)0X066)
#define cathode_5                    ((uint32_t)0X06d)
#define cathode_6                    ((uint32_t)0X07d)
#define cathode_7                    ((uint32_t)0X007)
#define cathode_8                    ((uint32_t)0X07f)
#define cathode_9                    ((uint32_t)0X06f)

#define us 0
#define ds 1
/* Private variables */
uint16_t cathode[]={cathode_0,cathode_1,cathode_2,cathode_3,cathode_4,cathode_5,cathode_6,cathode_7,cathode_8,cathode_9};

uint16_t seletor[]={GPIO_ODR_ODR_0,GPIO_ODR_ODR_1};
/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

/*Sorteia valores aleatórios entre [0,7] para as posições [0,7] de vetor[]
 *{Explode a posição que tiver o valor 0}
 *{Desarma a posição que tiver o valor 1}
 * Faz varredura para ver se a posição que está com a bomba ou desarmar está com seu fio ligado ou não
 * {para isso, será um laço for}
 *
 */
int main(void)
{
	srand(time(NULL));
	uint16_t TEMPO_TROCA=1000;
	uint16_t wire[8];
	uint16_t vetor[8];
	uint16_t display[2],valor[2]={9,9};
	uint16_t FtimerS=0, timerMs=0,Buzzer=0;
	uint16_t button=0x0;
	uint16_t i=0,start=0,teste[]={0,0,0,0,0,0,0,0};
	float ARR_boss=1599;

	//Enabling GPIOS clock
		RCC->AHB1ENR|=0x87;

	//Enabling TIM10 clock
		RCC->APB2ENR|=RCC_APB2ENR_TIM10EN;

	//Setting UIF overflow time
		TIM10->PSC = 9;
		TIM10->ARR = ARR_boss;
		TIM10->CR1|=TIM_CR1_CEN;//starts

	//habilita os pinos do GPIOA como saídas digitais
		GPIOA->MODER &=~ (GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6);
		GPIOA->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0);
	//habilita os pinos do GPIOB como saídas digitais
		GPIOB->MODER &=~ (GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6);
	    GPIOB->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0);//controla os segmentos dos catodos
    //habilita os pinos do GPIOC como entradas digitais
	    GPIOC->MODER &=~ (GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5|GPIO_MODER_MODER6|GPIO_MODER_MODER7|GPIO_MODER_MODER8);
	//habilitando pull down do GPIOC
	    GPIOC->PUPDR|=(GPIO_MODER_MODER0_1|GPIO_MODER_MODER1_1|GPIO_MODER_MODER2_1|GPIO_MODER_MODER3_1|GPIO_MODER_MODER4_1|GPIO_MODER_MODER5_1|GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1);

  /* Infinite loop */
  while (1)
  {
	  display[i]=cathode[valor[i]];

	  GPIOB->ODR &=~ (GPIO_ODR_ODR_0|GPIO_ODR_ODR_1|GPIO_ODR_ODR_2|GPIO_ODR_ODR_3|GPIO_ODR_ODR_4|GPIO_ODR_ODR_5|GPIO_ODR_ODR_6);
	  GPIOA->ODR &=~ (GPIO_ODR_ODR_0|GPIO_ODR_ODR_1);
	  GPIOA->ODR |= seletor[i];
	  GPIOB->ODR |= display[i];

	  wire[0]=GPIOC->IDR&=0x1;
	  wire[1]=GPIOC->IDR&=0x2;
	  wire[2]=GPIOC->IDR&=0x4;
	  wire[3]=GPIOC->IDR&=0x8;
	  wire[4]=GPIOC->IDR&=0x10;
	  wire[5]=GPIOC->IDR&=0x20;
	  wire[6]=GPIOC->IDR&=0x40;
	  wire[7]=GPIOC->IDR&=0x80;

	  button=GPIOC->IDR&=0x100;

	  TIM10->ARR=ARR_boss;

	  if(button == 0x100){//START
		  start=1;
		  ARR_boss=1599;
		  valor[us] = 9;
	  	  valor[ds] = 5;
	  	  GPIOA->ODR&=~0x70;
	  	  for(int a=0;a<8;a++){//lógica de reset dos testes
	  		  //vetor[a]=a;
	  		  teste[a]=0;
	  	  }
	  	  for(int c=0;c<8;c++){//gerador de valores aleatórios para vetor[]
	  		  vetor[c]=rand()%8;
	  	  	  for(int d=0;d<c;d++){//verificador de repetição
	  	  		if(vetor[d] == vetor[c])
	  	  		{
	  	  			vetor[c] = rand()%8;
	  	  			d=0;
	  	  		}
	  	  	  }
	  	  }
	  }

	  if(TIM10->SR&TIM_SR_UIF){						//verifica o overflow
		  TIM10->SR &=~ TIM_SR_UIF;				 	//zera o contador de tempo
		  timerMs++;
		  if(timerMs == TEMPO_TROCA){		    	//avisa overflow do timerMs
			  timerMs = 0;
		 	  FtimerS = 1;
			  if(valor[us] == 0&&valor[ds] == 0){
				  if(Buzzer==0){
					  GPIOA->ODR|=0x40;
					  Buzzer=1;
				  }
				  else{
					  GPIOA->ODR&=~0x40;
					  Buzzer=0;
				  }
			  }
		  }
		  if(i==0)i=1;
		  else i=0;
		  //lógica dos fios
		  for(int b=0;b<8;b++){
			  if(wire[b]==0x0&&(vetor[b+1]==0||vetor[b-1]==0))
				  GPIOA->ODR|=0x10;
			  if(wire[b]==0x0&&(vetor[b+1]==1||vetor[b-1]==1))
			  	  GPIOA->ODR|=0x20;
			  if(wire[7]==0x0&&vetor[0]==0)
				  GPIOA->ODR|=0x10;
			  if(wire[7]==0x0&&vetor[0]==1)
				  GPIOA->ODR|=0x20;
			  if(wire[0]==0x0&&vetor[7]==0)
				  GPIOA->ODR|=0x10;
			  if(wire[0]==0x0&&vetor[7]==1)
				  GPIOA->ODR|=0x20;
			  if(wire[b]==0x0&&vetor[b]==0){
				  valor[us] = 0;
				  valor[ds] = 0;
				  start=0;
			  }
			  if(wire[b]==0x0&&vetor[b]==1){
				  valor[us] = 0;
				  valor[ds] = 6;
				  start=0;
			  }
			  if(wire[b]==0&&teste[0]==0&&vetor[b]==2){
				  teste[0]=1;
			  	  ARR_boss/=1.5;
			  }
			  if(wire[b]==0&&teste[1]==0&&vetor[b]==3){
				  teste[1]=1;
			  	  ARR_boss/=1.5;
			  }
			  if(wire[b]==0&&teste[2]==0&&vetor[b]==4){
				  teste[2]=1;
			  	  ARR_boss/=1.5;
			  }
			  if(wire[b]==0&&teste[3]==0&&vetor[b]==5){
				  teste[3]=1;
			  	  ARR_boss/=1.5;
			  }
			  if(wire[b]==0&&teste[4]==0&&vetor[b]==6){
				  teste[4]=1;
			  	  ARR_boss/=1.5;
			  }
			  if(wire[b]==0&&teste[5]==0&&vetor[b]==7){
				  teste[5]=1;
			  	  ARR_boss/=1.5;
			  }
		  }
	  }

	  if(start==1){
		  if(FtimerS){
			  FtimerS = 0;
			  if(valor[us] > 0)
				  valor[us]--;
			  else if(valor[us] == 0){
				  if (valor[ds] > 0){
					  valor[us] = 9;
					  valor[ds]--;
				  }
			  }
		  }//end of FtimerS
	  }//end of start
  }//end of while
}//end of main
