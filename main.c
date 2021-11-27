
#include <inttypes.h>
#include <tm4c123gh6pm.h>
#include <stdio.h>





#define PORTA0 0x01
#define PORTA1 0x02
#define PORTA5 0x20
#define PORTA6 0x40
#define PORTA7 0x80

#define PORTB0 0x01
#define PORTB1 0x02
#define PORTB2 0x04
#define PORTB3 0x08
#define PORTB4 0x10
#define PORTB5 0x20
#define PORTB6 0x40
#define PORTB7 0x80

#define PORTC4 0x10
#define PORTC5 0x20
#define PORTC6 0x40
#define PORTC7 0x80

#define PORTD0 0x01
#define PORTD1 0x02
#define PORTD2 0x04
#define PORTD3 0x08

#define PORTE1 0x02
#define PORTE2 0x04
#define PORTE3 0x08
#define PORTE4 0x10
#define PORTE5 0x20

#define PORTF1 0x01
#define PORTF4 0x10



#define ENABLE GPIO_PORTA_DATA_R^=0x40
uint32_t i;


void delay()
{
    uint32_t delay;
    delay=2000000;
    volatile uint32_t elapsedTime;
    uint32_t startTime = NVIC_ST_CURRENT_R;
    do{
        elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
    }
    while(elapsedTime <= delay);
}





void config(void);
void ADC_Read(void);
void UART_Send(unsigned char c, int aux);
void inicializa_LCD(void);
void cmd_LCD(unsigned char c, int count);
void escreve_LCD(char *c);
void pulso_enable(void);



void main() {

    config();
    inicializa_LCD();

    while(1)
    {
        ADC_Read();
        delay();
    }
}




void config(void)
{


    SYSCTL_RCGCGPIO_R = 0x3F;


    NVIC_ST_RELOAD_R = 16000000;
    NVIC_ST_CTRL_R = 0x01;


    GPIO_PORTC_DIR_R &= ~(PORTC4 | PORTC5 | PORTC6 | PORTC7);
    GPIO_PORTD_DIR_R &= ~(PORTD2 | PORTD3);
    GPIO_PORTE_DIR_R &= ~(PORTE1 | PORTE2 | PORTE3);
    GPIO_PORTF_DIR_R &= ~(PORTF1 | PORTF4);

    GPIO_PORTC_DATA_R = 0;
    GPIO_PORTD_DATA_R = 0;
    GPIO_PORTE_DATA_R = 0;
    GPIO_PORTF_DATA_R = 0;

    GPIO_PORTC_PUR_R = (PORTC4 | PORTC5 | PORTC6 | PORTC7);
    GPIO_PORTD_PUR_R = (PORTD2 | PORTD3);
    GPIO_PORTE_PUR_R = (PORTE1 | PORTE2 | PORTE3 | PORTE4);
    GPIO_PORTF_PUR_R = (PORTF1 | PORTF4);

    GPIO_PORTC_DEN_R = (PORTC4 | PORTC5 | PORTC6 | PORTC7);
    GPIO_PORTD_DEN_R = (PORTD2 | PORTD3);
    GPIO_PORTE_DEN_R = (PORTE1 | PORTE2 | PORTE3 | PORTE4 | PORTE5);
    GPIO_PORTF_DEN_R = (PORTF1 | PORTF4);





    NVIC_EN0_R = 0x01<<2 | 0x01<<3 | 0x01<<4 | 0x01<<30;



    GPIO_PORTC_IS_R = 0x00;
    GPIO_PORTD_IS_R = 0x00;
    GPIO_PORTE_IS_R = 0x00;
    GPIO_PORTF_IS_R = 0x00;

    GPIO_PORTC_IEV_R = 0x00;
    GPIO_PORTD_IEV_R = 0x00;
    GPIO_PORTE_IEV_R = 0x00;
    GPIO_PORTF_IEV_R = 0x00;

    GPIO_PORTC_IBE_R = 0x00;
    GPIO_PORTD_IBE_R = 0x00;
    GPIO_PORTE_IBE_R = 0x00;
    GPIO_PORTF_IBE_R = 0x00;

    GPIO_PORTC_IM_R = (PORTC4 | PORTC5 | PORTC6 | PORTC7);
    GPIO_PORTD_IM_R = (PORTD2 | PORTD3);
    GPIO_PORTE_IM_R = (PORTE1 | PORTE2 | PORTE3 | PORTE4 | PORTE5);
    GPIO_PORTF_IM_R = (PORTF1 | PORTF4);




    SYSCTL_RCGCADC_R = 0x03;

    GPIO_PORTD_AFSEL_R = (PORTD0 | PORTD1);

    GPIO_PORTD_AMSEL_R = (PORTD0 | PORTD1);

    ADC0_ACTSS_R = 0x00;
    ADC0_EMUX_R = 0x0F;
    ADC0_SSMUX0_R = 0x07;
    ADC0_SSCTL0_R = 0x02;
    ADC0_ACTSS_R = 0x01;
    ADC0_PSSI_R = 0x01;

    ADC1_ACTSS_R = 0x00;
    ADC1_EMUX_R = 0x0F;
    ADC1_SSMUX0_R = 0x06;
    ADC1_SSCTL0_R = 0x02;
    ADC1_ACTSS_R = 0x01;
    ADC1_PSSI_R = 0x01;

    SYSCTL_RCGCUART_R = 0x01;

    GPIO_PORTA_AFSEL_R = (PORTA0 | PORTA1);

    GPIO_PORTA_PCTL_R = 0x11;

    UART0_CTL_R &= 0x00;
    UART0_IBRD_R = 8;
    UART0_FBRD_R = 44;
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CC_R = 0x00;
    UART0_CTL_R |= 0x101;

    GPIO_PORTA_DEN_R = (PORTA0 | PORTA1);
}




void Trata_PortC(void)
{
    if(GPIO_PORTC_RIS_R&PORTC4)
    {
        GPIO_PORTC_ICR_R = PORTC4;
        UART_Send('a', 2);
        escreve_LCD("a");
        delay();
    }
    else if (GPIO_PORTC_RIS_R&PORTC5)
    {
        GPIO_PORTC_ICR_R = PORTC5;
        UART_Send('b', 2);
        escreve_LCD("b");
        delay();
    }
    else if (GPIO_PORTC_RIS_R&PORTC6)
    {
        GPIO_PORTC_ICR_R = PORTC6;
        UART_Send('c', 2);
        escreve_LCD("c");
        delay();
    }
    else if (GPIO_PORTC_RIS_R&PORTC7)
    {
        GPIO_PORTC_ICR_R = PORTC7;
        UART_Send('d', 2);
        escreve_LCD("d");
        delay();
    }
}


void Trata_PortD(void)
{
    if(GPIO_PORTD_RIS_R&PORTD3)
    {
        delay();
        GPIO_PORTD_ICR_R = PORTD3;
        UART_Send('0', 2);
    }
    else if (GPIO_PORTD_RIS_R&PORTD2)
    {
        delay();
        GPIO_PORTD_ICR_R = PORTD2;
        UART_Send('1', 2);
    }
}


void Trata_PortE(void)
{
    if(GPIO_PORTE_RIS_R&PORTE1)
    {
        GPIO_PORTE_ICR_R = PORTE1;
        UART_Send('e', 2);
        escreve_LCD("e");
        delay();
    }
    else if (GPIO_PORTE_RIS_R&PORTE2)
    {
        GPIO_PORTE_ICR_R = PORTE2;
        UART_Send('f', 2);;
        escreve_LCD("f");
        delay();
    }
    else if (GPIO_PORTE_RIS_R&PORTE3)
    {
        GPIO_PORTE_ICR_R = PORTE3;
        UART_Send('g', 2);
        escreve_LCD("g");
        delay();
    }
}
void Trata_PortF(void)
{
    if(GPIO_PORTF_RIS_R&PORTF4)
    {
        GPIO_PORTE_ICR_R = PORTF4;
        UART0_DR_R = 'J';
        escreve_LCD("J");
        delay();
    }
    else if (GPIO_PORTE_RIS_R&PORTF1)
    {
        GPIO_PORTF_ICR_R = PORTF1;
        UART0_DR_R = 'r';
        escreve_LCD("r");
        delay();
    }
}






void ADC_Read(void)
{
    uint32_t x = 0x00, y = 0x00;
    int x2 = 0x00, y2 = 0x00;

    x = ADC0_SSFIFO0_R;
    y = ADC1_SSFIFO0_R;


    if ((x < 2000 || x > 2300) || (y < 2000 || y > 2300))
    {

        x2 = x >> 4;
        y2 = y >> 4;
        UART_Send(x2, 2);
        UART_Send(y2, 2);
    }
}




void UART_Send(unsigned char c, int aux)
{

    UART0_DR_R = c;


}



void inicializa_LCD()
{
    GPIO_PORTB_DATA_R = 0x00;
    GPIO_PORTA_DATA_R = 0x00;
    GPIO_PORTA_DIR_R |= (PORTA5 | PORTA6 | PORTA7);
    GPIO_PORTB_DIR_R |= (PORTB0 | PORTB1 | PORTB2 | PORTB3 | PORTB4 | PORTB5 | PORTB6 | PORTB7);
    GPIO_PORTB_DEN_R |= (PORTB0 | PORTB1 | PORTB2 | PORTB3 | PORTB4 | PORTB5 | PORTB6 | PORTB7);
    GPIO_PORTA_DEN_R |= (PORTA5 | PORTA6 | PORTA7);

    for(i = 0; i < 605010; i ++){}
    //delay_us(60500);
    GPIO_PORTB_DATA_R = (PORTB4 | PORTB5);

    for(i = 0; i < 1000000; i ++){}
    //delay_us(100000);
    pulso_enable();

    for(i = 0; i < 200000; i ++){}
    //delay_us(20000);
    pulso_enable();

    for(i = 0; i < 200000; i ++){}
    //delay_us(20000);
    pulso_enable();

    GPIO_PORTB_DATA_R = (PORTB2 | PORTB3 | PORTB4 | PORTB5);
    pulso_enable();

    GPIO_PORTB_DATA_R = PORTB3;
    pulso_enable();

    GPIO_PORTB_DATA_R = PORTB0;
    pulso_enable();

    GPIO_PORTB_DATA_R = (PORTB0 | PORTB1 | PORTB2);
    pulso_enable();

    GPIO_PORTB_DATA_R = (PORTB2 | PORTB3);
    pulso_enable();

    GPIO_PORTB_DATA_R = (PORTB1 | PORTB2);
    pulso_enable();

    GPIO_PORTB_DATA_R = PORTB0;
    pulso_enable();





}

  void cmd_LCD(unsigned char c, int count)
{
    if (count == 1) //limpar linha
    {
        GPIO_PORTA_DATA_R = 0x00;
        for(i = 0; i < 40000; i ++){}
        //delay_us(4000);
        GPIO_PORTB_DATA_R = 0x01;
        pulso_enable();
        GPIO_PORTB_DATA_R = 0x02;
        pulso_enable();
    }
    if (count == 16)
    {
        GPIO_PORTA_DATA_R = 0x00;
        for(i = 0; i < 40000; i ++){}
        //delay_us(4000);
        GPIO_PORTB_DATA_R = (PORTB7 | PORTB6);
        pulso_enable();
    }
    GPIO_PORTA_DATA_R = 0x20;
    for(i = 0; i < 40000; i ++){}
    //delay_aus(4000);
    GPIO_PORTB_DATA_R = c;
    pulso_enable();
}

  void escreve_LCD(char *c)
{
    int count = 0;
    for (; *c != 0; c++)
    {
        count++;
        cmd_LCD(*c, count);//escreve um char por vez

    }
}

  void pulso_enable()
{
    ENABLE;
    for(i = 0; i < 100000; i ++){}
    //delay_us(10000);
    ENABLE;
}

