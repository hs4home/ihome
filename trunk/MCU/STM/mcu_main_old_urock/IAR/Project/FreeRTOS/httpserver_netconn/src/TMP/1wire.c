#include "1wire.h"
//#include "stm32f2xx_tim.h"

#define FALSE 0
#define TRUE  1

uint8_t ROM_NO[8];
uint16_t LastDiscrepancy;
uint16_t LastFamilyDiscrepancy;
uint16_t LastDeviceFlag;
uint8_t crc8;


void RCC_Configuration (void)
{
    RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC, ENABLE);
        
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
}

void TIM3_Configuration (void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        
    TIM_TimeBaseStructure.TIM_Period = UINT16_MAX;
    TIM_TimeBaseStructure.TIM_Prescaler = 59;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_Cmd(TIM3, ENABLE);
}

void Delay_US (uint16_t useconds)
{
    TIM3->CNT = 0;
    while((TIM3->CNT) <= useconds);
}

void Delay_MS (uint16_t mseconds)
{
    uint16_t i;
    for(i=0; i<=(mseconds - 1); ++i)
    Delay_US (1000);  
}

void Set_Port_input (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);      
}

void Set_Port_output (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);      
}

void Set_Port_Up (void)
{
    GPIOC->BSRRL = GPIO_Pin_7;
}

void Set_Port_Down (void)
{
    GPIOC->BSRRH = GPIO_Pin_7;
}

uint8_t ow_reset (void)
{
    uint8_t ret = 10;

    Set_Port_output ();
    Set_Port_Down ();
    Delay_US(600);
    Set_Port_input ();
    Delay_US(80);
    if(GPIOC->IDR & GPIO_Pin_7)
        {
            ret = 0;
        }
    else
        {
            ret = 1;
        }
    Delay_US (400);
    return ret;
}

void ow_write_bit (uint8_t bit)
{
    if (bit)
        {
            Set_Port_output ();
            Set_Port_Down ();
            Delay_US (10);
            Set_Port_Up ();
            Delay_US (49);
        }
    else
        {
            Set_Port_output ();
            Set_Port_Down ();
            Delay_US (59);
            Set_Port_Up ();
        } 
}

uint8_t ow_read_bit (void)
{
    uint8_t ret = 10;

    Set_Port_output ();
    Set_Port_Down ();
    Delay_US (5);
    Set_Port_input ();
    Delay_US (10);
    if (GPIOC->IDR & GPIO_Pin_7)
        {
            ret = 1;
        }
    else
        {
            ret = 0;
        }
    Delay_US (44);
    return ret; 
}

uint8_t ow_read_byte (void)
{
    uint8_t i;
    uint8_t byte=0;

    for (i=0;i<8;i++)
        {
            if(ow_read_bit ()) byte |= (0x01<<i);
            Delay_US (1);
        }
    return(byte);
}

void ow_write_byte (uint8_t byte)
{
    uint8_t a;
    uint8_t temp;

    for(a=0;a<8;a++)
        {
            temp = byte >> a;
            temp &=0x01;
            ow_write_bit (temp);
            Delay_US (1);
        }
}

uint8_t docrc8(uint8_t value)
{
    static uint8_t dscrc_table[] = {
    0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
    35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
    70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
    17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
    50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
    87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};
                        
   crc8 = dscrc_table[crc8 ^ value];
   return crc8;
}


uint16_t ow_search (void)
{
    uint16_t id_bit_number;
    uint16_t last_zero, rom_byte_number, search_result;
    uint16_t id_bit, cmp_id_bit;
    uint8_t rom_byte_mask, search_direction;

  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;
  crc8 = 0;

   if (!LastDeviceFlag)
   {
      if (!ow_reset ())
      {
         LastDiscrepancy = 0;
         LastDeviceFlag = FALSE;
         LastFamilyDiscrepancy = 0;
         return FALSE;
      }
 
      ow_write_byte(0xF0);  

      do
      {
         id_bit = ow_read_bit();
         cmp_id_bit = ow_read_bit();

         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            if (id_bit != cmp_id_bit)
               search_direction = id_bit; 
            else
            {
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  search_direction = (id_bit_number == LastDiscrepancy);

               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            ow_write_bit(search_direction);

            id_bit_number++;
            rom_byte_mask <<= 1;

            if (rom_byte_mask == 0)
            {
                docrc8(ROM_NO[rom_byte_number]); 
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8); 

      if (!((id_bit_number < 65) || (crc8 != 0)))
      {
         LastDiscrepancy = last_zero;

         if (LastDiscrepancy == 0)
            LastDeviceFlag = TRUE;
         
         search_result = TRUE;
      }
   }

   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = FALSE;
      LastFamilyDiscrepancy = 0;
      search_result = FALSE;
   }
   return search_result;
}

int OWFirst()
{
    LastDiscrepancy = 0;
    LastDeviceFlag = FALSE;
    LastFamilyDiscrepancy = 0;

    return ow_search();
}

int OWNext()
{
    return ow_search();
}

//**********************************************************************************
//#define PORT  GPIOC   //������� ����, � �������� ���������� �������
//#define TIMER TIM3    //������ ������, ������������ ��� ������������ ��������
// 
////*********************************************************************************************
///**
//* @function  ������� ������                                                                   
//* @param  ����� �����                                                                      
//* @return    0 - ���������� ���������, 1 - �� ����������, 2 - �.�. �� �����                   
//*/
////*********************************************************************************************
//uint8_t ds_reset_pulse(uint16_t PinMask)
//{
//   uint16_t result;   
// 
//   if((PORT->IDR & PinMask)==0)  return 2;         //��������� ����� �� ���������� ���������
//   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
//   TIMER->CNT=0;
//   while(TIMER->CNT<480){};                        //����� 480 �����������
//   PORT->ODR |=  PinMask;                          //��������� ����
//   while(TIMER->CNT<550){};                        //����� 70 �����������
//   result     =  PORT->IDR & PinMask;              //��������� ���� 
//   while(TIMER->CNT<960){};                        //��������� ��������� �������������
//   if(result) return 1;                            //������ �� ���������
//   return 0;                                       //������ ���������      
//}
//
////*********************************************************************************************
///**
//* @function  �������� ����                                                                    
//* @param  �������� ������������� ����,����� �����                                         
//* @return     none                                                                        
//*/
////*********************************************************************************************
//void ds_write_bit(uint8_t bit,uint16_t PinMask)
//{
//   TIMER->CNT=0;
//   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
//   while(TIMER->CNT<2){};                          //����� 1 ������������
//   if(bit) PORT->ODR |=  PinMask;                  //���� �������� 1, �� ��������� ����
//   while(TIMER->CNT<60){};                         //�������� 60 ����������� 
//   PORT->ODR |=  PinMask;                          //��������� ���� 
//}
//
////*********************************************************************************************
///**
//* @function  ������ ����                                                                      
//* @param  ����� �����                                                                     
//* @return    ����������� ���                                                              
//*/
////*********************************************************************************************
//uint16_t ds_read_bit(uint16_t PinMask)
//{
//   uint16_t result;
// 
//   TIMER->CNT=0;
//   PORT->ODR &= ~PinMask;                          //�������� ���� � �����
//   while(TIMER->CNT<2){};
//   PORT->ODR |=  PinMask;                          //��������� ����  
//   while(TIMER->CNT<15){};                         //�������� 15 �����������
//   result     =  PORT->IDR & PinMask;              //��������� ����
//   while(TIMER->CNT<60){};                         //���������� ����� 
//   return result;                                  //���������� ���������
//}
//
////*********************************************************************************************
///**
//* @function  ������ �����                                                                     
//* @param  ������������ ����,����� �����                                                   
//* @return    none                                                                         
//*/
////*********************************************************************************************
//
//void ds_write_byte(uint8_t byte, uint16_t PinMask)
//{
//   uint8_t i;
//   for(i=0;i<8;i++) ds_write_bit(byte&(1<<i), PinMask);
//}
////*********************************************************************************************
///**
//* @function  ������ �����                                                                     
//* @param  ����� �����                                                                     
//* @return    ����������� ����                                                             
//*/
////*********************************************************************************************
//uint8_t ds_read_byte(uint16_t PinMask)
//{
//   uint8_t i,result = 0;
//   for(i=0;i<8;i++) 
//   if(ds_read_bit(PinMask)) result |= 1<<i; 
//   return result;
//}
//
////*********************************************************************************************
///**
//* @function  ���� ��������� �����������                                                       
//* @param  ����� ���� ����� 0..15                                                          
//* @return    0 - ���� ��������, 1 - ������ �� ���������, 2 - �.�. �� �����                
//*/
////*********************************************************************************************
//uint8_t ds_start_convert_single(uint8_t PinNumb)
//{
//  uint8_t result;
//  result = ds_reset_pulse(1<<PinNumb);       //������� ������� ������
//  if(result) return result;                  //���� ������ - ���������� �� ���
//  ds_write_byte(0xCC,1<<PinNumb);            //��������� ������ � ������� �� ��������� �����  
//  ds_write_byte(0x44,1<<PinNumb);            //��������� ��������������
//  return 0;
//}
//
//
////*********************************************************************************************
///**
//* @function  ������ ������ �������                                                            
//* @param  ��������� �� ������-�������� ������, ����� ���� ����� 0..15                      
//* @return    0 - ������ ���������, 1 - ������ �� ���������, 2 - �.�. 3 - ������ CRC        
//*/
////*********************************************************************************************
//uint8_t ds_read_data_single(uint8_t *buff, uint8_t PinNumb)
//{
//  uint8_t crc = 0;
//  uint8_t data;
//  uint8_t i,j;
//  uint8_t tmp;
// 
//  tmp = ds_reset_pulse(1<<PinNumb);          //������� ������� ������
//  if(tmp) return tmp;                        //���� ������ - ���������� �� ���
//  ds_write_byte(0xCC,1<<PinNumb);            //��������� ������ � ������� �� ��������� �����
// 
//  ds_write_byte(0xBE,1<<PinNumb);            //������ 9 ���� ������
// 
//  //��������� 8 ���� � ��������� CRC
//  for( i=0; i<8; i++) 
//  {
//    data = ds_read_byte(1<<PinNumb);         //��������� ��������� ����      
//    buff[i] = data;                          //��������� ��� � �������
// 
//    //���������� CRC - ������������ ������ ��� ��������� �����
//    for( j=0; j<8; j++)         
//    {
//      tmp = (crc ^ data) & 0x01;
//      if (tmp==0x01) crc = crc ^ 0x18;
//      crc = (crc >> 1) & 0x7F;
//      if (tmp==0x01) crc = crc | 0x80;
//      data = data >> 1;
//    }		
//  }
// 
//  data = ds_read_byte(1<<PinNumb);          //��������� CRC �������
//  if(crc==data) return 0;                   //���� CRC ������� - ������ ������ ���
//  return 3;                                 //CRC �� �������, ������ �������� ������
//}
//
////*********************************************************************************************
///**
//* @function  ������ ����������� �������                                                       
//* @param   ����� ���� ����� 0..15                                                         
//* @return    1280 - ������, ����� - ��������� ���������                                   
//*/
////*********************************************************************************************
//signed int ds_read_temperature(uint8_t PinNumb)
//{
//  signed char integer = 0;     
//  signed char frac;
//  signed int  result;
//  uint8_t     buff[8];
// 
//  //��������� ������ �� ������� 
//  if(ds_read_data_single(&buff[0],PinNumb))  return 1280;
// 
//  frac    = buff[0] & 0x0f;                            //�������� ������� �����      
//  integer = (buff[0]>>4) | ((buff[1] & 0x0f)<<4);      //�������� ����� ����� 
// 
//  //���� ����������� �������������
//  if(integer<0)
//  {
//    integer = 0 - integer - 1;
//    result  = integer *10;                            //��������� ����� �����
//    frac = frac | 0xf0;
//    frac = 0 - frac ;                                 
//  }
//  //���� ����������� �������������
//  else     result  = integer *10;                    //��������� ����� �����
// 
//  result = result + ((frac*10)/16);                  //��������� ������� �����
// 
//  return result;
//}
////******************************************************************************
//void ds_init(void)
//{
//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   //������ ������������ �� TIM3                           /
//TIM3->PSC     = 25 - 1;                  //��������� �������� ��� ������������ �����������
//TIM3->ARR     = 1000; 
//TIM3->CR1     = TIM_CR1_CEN;
//
//RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;  //��������� ������������ �����                          /
//GPIOC->CRL   &= ~GPIO_CRL_MODE7;      //�������� ���� MODE
//GPIOC->CRL   &= ~GPIO_CRL_CNF7;       //�������� ���� CNF
//GPIOC->CRL   |=  GPIO_CRL_MODE7_1 ;   //�����,������� 2MHz
//GPIOC->CRL   |=  GPIO_CRL_CNF7_0;     //�������� ���� ������ ����������
//GPIOC->ODR   |=  1<<7 ;               //��������� ����  
//}
////******************************************************************************
//
///*
//http://mycontroller.ru/stm32-ds18b20-izmerenie-temperaturyi/
//� ������ 1280 (128.0 �������), �.�. ������������ ����������� ��� ���� �������� 125 ��������.
//���������, ��� ������ �������� ������� ��������� �����������:
//
//signed int tmp;
// 
//ds_start_convert_single(7);     //��������� ��������� �����������                              /
//delay_ms(1000);                 //����� ��� ��������� ��������������
//tmp = ds_read_temperature(7);   //��������� ��������� ���������
//*/
//
//
///*
//������ ��� ������������ ����������� ���� �������, ���������� ���������������� ������ � ����.
//������ �������������� ���:
//RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   //������ ������������ �� TIM3                           /
//TIM3->PSC     = 8-1;                  //��������� �������� ��� ������������ �����������
//TIM3->ARR     = 1000; 
//TIM3->CR1     = TIM_CR1_CEN;
//������ �2 ������������ ������ �������������� ��������� �� ���� �������� ��������.
//� ���� ������� ������������ ������� �������������� �� 8MHz, ������� � TIM3->PSC ����������� 8-1. ��� ������ ����� ���������� ��� ���� ������������.
//��������� �����. � ��������� ������ � ������ 7 ����� �. ��������� �������� ���:
//
//RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;  //��������� ������������ �����                          /
//GPIOC->CRL   &= ~GPIO_CRL_MODE7;      //�������� ���� MODE
//GPIOC->CRL   &= ~GPIO_CRL_CNF7;       //�������� ���� CNF
//GPIOC->CRL   |=  GPIO_CRL_MODE7_1 ;   //�����,������� 2MHz
//GPIOC->CRL   |=  GPIO_CRL_CNF7_0;     //�������� ���� ������ ����������
//GPIOC->ODR   |=  1<<7 ;               //��������� ����
//*/
