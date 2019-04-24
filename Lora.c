#include "stm8s.h"
#include "lora.h"
#include "main.h"
#include "delay.h"
#include "timer1.h"
#include "led.h"

u8 mode;//lora--1/FSK--0
u8 Freq_Sel;//
u8 Power_Sel;//
u8 Lora_Rate_Sel;//
u8 BandWide_Sel;//

u8 Fsk_Rate_Sel;//

const u8 sx1276_7_8FreqTbl[1][3] = 
{ 
  {0x6C, 0x80, 0x00}, //434MHz
};

const u8 sx1276_7_8PowerTbl[4] =
{ 
  0xFF,                   //20dbm  
  0xFC,                   //17dbm
  0xF9,                   //14dbm
  0xF6,                   //11dbm 
};

const u8 sx1276_7_8SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
};

const u8 sx1276_7_8LoRaBwTbl[10] =
{
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

const u8  sx1276_7_8Data[] = {"HR_WT Lora sx1276_7_8"};

u8 RxData[64];

void sx1276_7_8_Standby(void)
{
  SPIWrite(LR_RegOpMode,0x09);                              		//Standby//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x01);                              	 //Standby//High Frequency Mode
}

void sx1276_7_8_Sleep(void)
{
  SPIWrite(LR_RegOpMode,0x08);                              		//Sleep//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x00);                            		 //Sleep//High Frequency Mode
}

void sx1276_7_8_EntryLoRa(void)
{
  SPIWrite(LR_RegOpMode,0x88);//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x80);//High Frequency Mode
}

void sx1276_7_8_LoRaClearIrq(void)
{
  SPIWrite(LR_RegIrqFlags,0xFF);
}

void sx1276_7_8_Config(void)
{
  u8 i; 
    
  sx1276_7_8_Sleep();                                      //Change modem mode Must in Sleep mode 
  for(i=250;i!=0;i--)                                      //Delay
    NOP();
	delay_ms(1);

  //lora mode
	sx1276_7_8_EntryLoRa();  
	//SPIWrite(0x5904);   //?? Change digital regulator form 1.6V to 1.47V: see errata note
    
	BurstWrite(LR_RegFrMsb,sx1276_7_8FreqTbl[Freq_Sel],3);  //setting frequency parameter

	//setting base parameter 
	SPIWrite(LR_RegPaConfig,sx1276_7_8PowerTbl[Power_Sel]);             //Setting output power parameter  
    
	SPIWrite(LR_RegOcp,0x0B);                              //RegOcp,Close Ocp
	SPIWrite(LR_RegLna,0x23);                              //RegLNA,High & LNA Enable
    
	if(sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]==6)           //SFactor=6
	{
		u8 tmp;
		SPIWrite(LR_RegModemConfig1,((sx1276_7_8LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x01));//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
		SPIWrite(LR_RegModemConfig2,((sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]<<4)+(CRC<<2)+0x03));
      
		tmp = SPIRead(0x31);
		tmp &= 0xF8;
		tmp |= 0x05;
		SPIWrite(0x31,tmp);
		SPIWrite(0x37,0x0C);
	} 
	else
	{
		SPIWrite(LR_RegModemConfig1,((sx1276_7_8LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x00));//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
		SPIWrite(LR_RegModemConfig2,((sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]<<4)+(CRC<<2)+0x03));  //SFactor &  LNA gain set by the internal AGC loop 
	}
	SPIWrite(LR_RegSymbTimeoutLsb,0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max) 
    
	SPIWrite(LR_RegPreambleMsb,0x00);                       //RegPreambleMsb 
	SPIWrite(LR_RegPreambleLsb,12);                      //RegPreambleLsb 8+4=12byte Preamble
    
	SPIWrite(REG_LR_DIOMAPPING2,0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
	
  sx1276_7_8_Standby();                                         //Entry standby mode
}

u8 sx1276_7_8_LoRaEntryRx(void)
{
  u8 addr; 
        
  sx1276_7_8_Config();                                         //setting base parameter
  
  SPIWrite(REG_LR_PADAC,0x84);                              //Normal and Rx
  SPIWrite(LR_RegHopPeriod,0xFF);                          //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1,0x01);                       //DIO0=00, DIO1=00, DIO2=00, DIO3=01
      
  SPIWrite(LR_RegIrqFlagsMask,0x3F);                       //Open RxDone interrupt & Timeout
  sx1276_7_8_LoRaClearIrq();   
  
  SPIWrite(LR_RegPayloadLength,21);                       //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
    
  addr = SPIRead(LR_RegFifoRxBaseAddr);           				//Read RxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                        //RxBaseAddr -> FiFoAddrPtr¡¡ 
  SPIWrite(LR_RegOpMode,0x8d);                        		//Continuous Rx Mode//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x05);                        		//Continuous Rx Mode//High Frequency Mode
	SysTime = 0;
	while(1)
	{
		if((SPIRead(LR_RegModemStat)&0x04)==0x04)   //Rx-on going RegModemStat
			break;
		if(SysTime>=3)	
			return 0;                                              //over time for error
	}
}

u8 sx1276_7_8_LoRaReadRSSI(void)
{
  u16 temp=10;
  temp=SPIRead(LR_RegRssiValue);                  //Read RegRssiValue£¬Rssi value
  temp=temp+127-137;                                       //127:Max RSSI, 137:RSSI offset
  return (u8)temp;
}

u8 sx1276_7_8_LoRaRxPacket(void)
{
  u8 i; 
  u8 addr;
  u8 packet_size;
 
  if(Get_NIRQ())
  {
    for(i=0;i<32;i++) 
      RxData[i] = 0x00;
    
    addr = SPIRead(LR_RegFifoRxCurrentaddr);      //last packet addr
    SPIWrite(LR_RegFifoAddrPtr,addr);                      //RxBaseAddr -> FiFoAddrPtr    
    if(sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]==6)           //When SpreadFactor is six£¬will used Implicit Header mode(Excluding internal packet length)
      packet_size=21;
    else
      packet_size = SPIRead(LR_RegRxNbBytes);     //Number for received bytes    
    SPIBurstRead(0x00, RxData, packet_size);
    
    sx1276_7_8_LoRaClearIrq();
    for(i=0;i<17;i++)
    {
      if(RxData[i]!=sx1276_7_8Data[i])
        break;  
    }    
    if(i>=17)                                              //Rx success
      return(1);
    else
      return(0);
  }
  else
    return(0);
}

u8 sx1276_7_8_LoRaEntryTx(void)
{
  u8 addr,temp;
    
  sx1276_7_8_Config();                                         //setting base parameter
    
  SPIWrite(REG_LR_PADAC,0x87);                                   //Tx for 20dBm
  SPIWrite(LR_RegHopPeriod,0x00);                               //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1,0x41);                       //DIO0=01, DIO1=00, DIO2=00, DIO3=01
  
  sx1276_7_8_LoRaClearIrq();
  SPIWrite(LR_RegIrqFlagsMask,0xF7);                       //Open TxDone interrupt
  SPIWrite(LR_RegPayloadLength,21);                       //RegPayloadLength  21byte
  
  addr = SPIRead(LR_RegFifoTxBaseAddr);           //RegFiFoTxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                        //RegFifoAddrPtr
	SysTime = 0;
	while(1)
	{
		temp=SPIRead(LR_RegPayloadLength);
		if(temp==21)
		{
			break; 
		}
		if(SysTime>=3)
		{			
			return 0;
		}
	}
}

u8 sx1276_7_8_LoRaTxPacket(void)
{ 
  u8 TxFlag=0;
  u8 addr;
  
	BurstWrite(0x00, (u8 *)sx1276_7_8Data, 21);
	SPIWrite(LR_RegOpMode,0x8b);                    //Tx Mode           
	while(1)
	{
		if(Get_NIRQ())                      //Packet send over
		{   
			RED_LED_H();
			SPIRead(LR_RegIrqFlags);
			sx1276_7_8_LoRaClearIrq();                                //Clear irq
				
			sx1276_7_8_Standby();                                     //Entry Standby mode      
			break;
		}
	} 
}




















































































