#include "stm32f4xx.h"
#define LED_C13_ON() (GPIOC->BSRR = GPIO_BSRR_BR13)
#define LED_C13_OFF() (GPIOC->BSRR = GPIO_BSRR_BS13)

void set_96_hse(void);
void led_c13_init(void);
void set_peripheral_clock(void);
void uart_init(void);

int main(void)
{
    set_peripheral_clock();
	set_96_hse();
	led_c13_init();
    LED_C13_ON();
		
    while (1){
    }
}

void set_96_hse(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR &= ~PWR_CR_VOS;
    PWR->CR |= PWR_CR_VOS_0;

    FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
    FLASH->ACR |= FLASH_ACR_ICEN;
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    RCC->PLLCFGR =
        (25 << RCC_PLLCFGR_PLLM_Pos) |
        (192 << RCC_PLLCFGR_PLLN_Pos) |
        (0 << RCC_PLLCFGR_PLLP_Pos) |
        (4 << RCC_PLLCFGR_PLLQ_Pos) |
        RCC_PLLCFGR_PLLSRC_HSE;

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void set_peripheral_clock(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // led_c13
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // uart gpio
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //uart
}

void led_c13_init(void)
{
    GPIOC->MODER |= GPIO_MODER_MODER13_0;
    GPIOC->MODER &= ~GPIO_MODER_MODER13_1;
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13_0;
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13_1;
}

void uart_init(void)
{
	GPIOA->MODER &= ~GPIO_MODER_MODER9;
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER10;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD10; 
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD10_0;    
	GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos) |
    	           (7 << GPIO_AFRH_AFSEL10_Pos);
	/*
	92 000 000 / 115200 = 833,333
	833,33 / 16 = 52,0833
	mant = 52
	frac = 0,0833 * 16 = 1,33 ~ 1
	*/
	USART1->BRR = (52 << 4) | (1 << 0); 
	USART1->CR1 |= 
			(1 << USART_CR1_RE_Pos) |
			(1 << USART_CR1_TE_Pos) |
			(1 << USART_CR1_RXNEIE_Pos) |
			(1 << USART_CR1_UE_Pos);
}
