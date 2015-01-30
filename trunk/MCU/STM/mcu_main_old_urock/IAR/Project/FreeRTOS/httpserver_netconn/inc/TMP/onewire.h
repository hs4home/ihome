/*
 * onewire.h
 *
 *  Version 1.0.3
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "stm32f2xx.h"
#include "main.h"
#include "croutine.h"
#include "timers.h"
//#include "init_const.h"   

//----------- 1-WIRE -----------------------------------------------------------
/* *********************************************************************** */
// ���� ����� �������� ���� FreeRTOS, �� �����������������
#define OW_GIVE_TICK_RTOS

// -- ������ USART �������� �� ���� USART_3

// ������ �������� ������� OW_Send
#define OW_SEND_RESET		1
#define OW_NO_RESET		2

// ������ �������� �������
#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	        3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	        0xff

///* Definition for USARTx resources ********************************************/
   
#define MAX_DS18  1 // ������������ ����� �������� ����������� �� ���� UART (����� ������ ������ 3 �������,
                    // � �������� ������������ ������� ���� = 28)   

//---- ������������ USART ��� 1-wire ------------------------------------------
#define USARTx_DR_ADDRESS                ((uint32_t)USART3 + 0x04) 

//#define OW_0	0x00
//#define OW_1	0xff
//#define OW_R_1	0xff

#define USART_1WIRE                           USART3   //UASRT ������������ � ���� 1-wire
#define USART_1WIRE_CLK                       RCC_APB1Periph_USART3
#define USART_1WIRE_AF                        GPIO_AF_USART3

#define USART3_CLK_INIT                       RCC_APB1PeriphClockCmd
//#define USARTx_IRQn                           USART3_IRQn
//#define USARTx_IRQHandler                     USART3_IRQHandler

#define USART_1WIRE_TX_PIN                    GPIO_Pin_8                
#define USART_1WIRE_TX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define USART_1WIRE_TX_SOURCE                 GPIO_PinSource8
#define USART_1WIRE_TX_PORT                   GPIOD      
#define USART_1WIRE_TX_AF                     GPIO_AF_USART3
                     
#define USART_1WIRE_RX_PIN                    GPIO_Pin_9     
#define USART_1WIRE_RX_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define USART_1WIRE_RX_SOURCE                 GPIO_PinSource9
#define USART_1WIRE_RX_PORT                   GPIOD                
#define USART_1WIRE_RX_AF                     GPIO_AF_USART3

 //------------------------------------------------------------------------------
//#define USART_1WIRE_DMA_CLK              RCC_AHB1Periph_DMA1
//#define DMA_TX_STREAM_1_WIRE             DMA1_Stream3
//#define DMA_RX_STREAM_1_WIRE             DMA1_Stream1
//#define DMA_CHANNEL_1_WIRE               DMA_Channel_4

#define USART_1WIRE_DMA                       DMA1
#define USART_1WIRE_DMA_CLK                   RCC_AHB1Periph_DMA1
   
#define USART_1WIRE_TX_DMA_CHANNEL            DMA_Channel_4
#define USART_1WIRE_TX_DMA_STREAM             DMA1_Stream3
#define USART_1WIRE_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF3
#define USART_1WIRE_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF3
#define USART_1WIRE_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF3
#define USART_1WIRE_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF3
#define USART_1WIRE_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3
            
#define USART_1WIRE_RX_DMA_CHANNEL            DMA_Channel_4
#define USART_1WIRE_RX_DMA_STREAM             DMA1_Stream1
#define USART_1WIRE_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USART_1WIRE_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USART_1WIRE_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USART_1WIRE_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USART_1WIRE_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1

//#define USART_1WIRE_DMA_TX_IRQn               DMA1_Stream3_IRQn
//#define USART_1WIRE_DMA_RX_IRQn               DMA1_Stream1_IRQn
//#define USART_1WIRE_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
//#define USART_1WIRE_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler   


struct owidbuf_t{  //� ���� ��������� �������� ID ������� ����������� � ��������� �������� �����������
  uint8_t id[16];
  int temp;
};


//-------- �������� ������� ---------------------
void OW_Init(void);
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);
uint8_t OW_Scan(uint8_t *buf, uint8_t num);

#endif /* ONEWIRE_H_ */
