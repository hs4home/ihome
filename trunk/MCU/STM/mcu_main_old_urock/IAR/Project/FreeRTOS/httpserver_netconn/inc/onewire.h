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
#include "gpio_low_level.h"
#include "stm32f2xx_dma.h"

//----------- 1-WIRE -----------------------------------------------------------
/* *********************************************************************** */
// ���� ����� �������� ���� FreeRTOS, �� �����������������
#define OW_GIVE_TICK_RTOS

//// -- ������ USART �������� �� ���� USART_6

//// ������ �������� ������� OW_Send
#define OW_SEND_RESET		1
#define OW_NO_RESET		2

//// ������ �������� �������
#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	        3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	        0xff

#define OW_0	0x00
#define OW_1	0xff
#define OW_R_1	0xff

///* Definition for USARTx resources ********************************************/
   
#define MAX_DS18  2 // ������������ ����� �������� ����������� �� ���� UART (����� ������ ������ 3 �������,
                    // � �������� ������������ ������� ���� = 28)   

//---- ������������ USART ��� 1-wire ------------------------------------------

#define USARTx_DR_ADDRESS                ((uint32_t)USART6 + 0x04) 

#define USART_1WIRE                           USART6   //UASRT ������������ � ���� 1-wire
#define USART_1WIRE_CLK                       RCC_APB2Periph_USART6
#define USART_1WIRE_AF                        GPIO_AF_USART6

#define USART6_CLK_INIT                       RCC_APB2PeriphClockCmd
//#define USARTx_IRQn                           USART3_IRQn
//#define USARTx_IRQHandler                     USART3_IRQHandler

#define USART_1WIRE_TX_PIN                    GPIO_Pin_6                
#define USART_1WIRE_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USART_1WIRE_TX_SOURCE                 GPIO_PinSource6
#define USART_1WIRE_TX_PORT                   GPIOC      
#define USART_1WIRE_TX_AF                     GPIO_AF_USART6
                     
#define USART_1WIRE_RX_PIN                    GPIO_Pin_7     
#define USART_1WIRE_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USART_1WIRE_RX_SOURCE                 GPIO_PinSource7
#define USART_1WIRE_RX_PORT                   GPIOC                
#define USART_1WIRE_RX_AF                     GPIO_AF_USART6

                 
 //------------------------------------------------------------------------------

#define USART_1WIRE_DMA                       DMA2
#define USART_1WIRE_DMA_CLK                   RCC_AHB1Periph_DMA2
   
//#define USART_1WIRE_TX_DMA_CHANNEL            DMA_Channel_5
//#define USART_1WIRE_TX_DMA_STREAM             DMA2_Stream7
//#define USART_1WIRE_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF7
//#define USART_1WIRE_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF7
//#define USART_1WIRE_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF7
//#define USART_1WIRE_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF7
//#define USART_1WIRE_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF7
//            
//#define USART_1WIRE_RX_DMA_CHANNEL            DMA_Channel_5
//#define USART_1WIRE_RX_DMA_STREAM             DMA2_Stream2
//#define USART_1WIRE_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF2
//#define USART_1WIRE_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF2
//#define USART_1WIRE_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF2
//#define USART_1WIRE_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF2
//#define USART_1WIRE_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF2

#define USART_1WIRE_TX_DMA_CHANNEL            DMA_Channel_5
#define USART_1WIRE_TX_DMA_STREAM             DMA2_Stream6
#define USART_1WIRE_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF6
#define USART_1WIRE_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF6
#define USART_1WIRE_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF6
#define USART_1WIRE_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF6
#define USART_1WIRE_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF6
            
#define USART_1WIRE_RX_DMA_CHANNEL            DMA_Channel_5
#define USART_1WIRE_RX_DMA_STREAM             DMA2_Stream1
#define USART_1WIRE_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USART_1WIRE_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USART_1WIRE_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USART_1WIRE_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USART_1WIRE_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1


struct owidbuf_t{  //� ���� ��������� �������� ID ������� ����������� � ��������� �������� �����������
  uint8_t id[16];
  int temp;
};


//-------- �������� ������� ---------------------
void OW_Init(void);
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);
uint8_t OW_Scan(uint8_t *buf, uint8_t num);

#endif /* ONEWIRE_H_ */
