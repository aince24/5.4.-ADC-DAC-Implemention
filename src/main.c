
// Pot ile led parlaklýðýný ayarlama

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

uint8_t AdcData;
uint16_t DacData;

GPIO_InitTypeDef LED;
ADC_InitTypeDef  Adcstruct;
ADC_CommonInitTypeDef AdcCommonstruct;
DAC_InitTypeDef Dacstruct;

//Giriþ çýkýþ konfigrason fonksiyonu
void config(){
 //DataBus
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

 //LED
 LED.GPIO_Mode=GPIO_Mode_OUT;
 LED.GPIO_OType=GPIO_OType_PP;
 LED.GPIO_Pin=GPIO_Pin_4;
 LED.GPIO_PuPd=GPIO_PuPd_NOPULL;
 LED.GPIO_Speed=0x03;

 GPIO_Init(GPIOA,&LED);

 //POT
 LED.GPIO_Mode=GPIO_Mode_AN;
 LED.GPIO_OType=GPIO_OType_PP;
 LED.GPIO_Pin=GPIO_Pin_0;
 LED.GPIO_PuPd=GPIO_PuPd_NOPULL;
 LED.GPIO_Speed=0x03;

 //ADC
 Adcstruct.ADC_Resolution=ADC_Resolution_8b;//0-255 arasý çözünürlük

 ADC_Init(ADC1,&Adcstruct);

 ADC_Cmd(ADC1,ENABLE);// adc1 modunu aktif ettik

 //ADC Common
 AdcCommonstruct.ADC_Mode=ADC_Mode_Independent;//adc baðýmsýz modda çalýþacak(týmer yok)
 AdcCommonstruct.ADC_Prescaler=ADC_Prescaler_Div4;// ön bölücü 1/4 oranýnda

 ADC_CommonInit(&AdcCommonstruct);

 //DAC
 Dacstruct.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
 Dacstruct.DAC_Trigger=DAC_Trigger_None;
 Dacstruct.DAC_WaveGeneration=DAC_WaveGeneration_None;

 DAC_Init(DAC_Channel_1,&Dacstruct);

 DAC_Cmd(DAC_Channel_1,ENABLE);

}

//ADC data okuma fonksiyonu
uint8_t Read_ADC(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_56Cycles);

	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}

//map fonksiyonu
uint32_t map(long A,long B,long C,long D,long E){

	return (A*E)/C;
}

//Bekleme fonksiyonu
void delay(uint32_t time){
	while(time--){
	}
}

int main(void)
{
 config();

  while (1)
  {
	  AdcData=Read_ADC();// analog data okundu
	  DacData=map(AdcData,0,255,0,4095);// 8 bit data 12 bite çevrildi

	  DAC_SetChannel1Data(DAC_Align_12b_R,DacData);

  }
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
