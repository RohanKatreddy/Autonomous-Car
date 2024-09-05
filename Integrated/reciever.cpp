#include "reciever.h"

Reciever::Reciever(){

}

void Reciever::begin(){
  #ifdef CUSTOM_SERIAL_RECIEVER
  Serial1.setRX(RECIEVER_RX_PIN);
  Serial1.setTX(RECIEVER_TX_PIN);
  Serial1.begin(115200);
  #endif
}


void Reciever::formatRx(){
  // Can be used to map channel values.
  for (int i=0; i< IBUS_MAXCHANNELS; i++)
     {
        channelValues[i] = ibusValue[i];
     }
}

void Reciever::read(){
  Serial1.flush();
  rxFrameDone = false;
  
  if (Serial1.available())
  {
    uint8_t val = Serial1.read();
    if (ibusIndex == 0 && val != 0x20)
    {
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == 1 && val != 0x40) 
    {
      ibusIndex = 0;
      return;
    }

    if (ibusIndex == IBUS_BUFFSIZE)
    {
      ibusIndex = 0;
      int high=3;
      int low=2;
      for(int i=0;i<IBUS_MAXCHANNELS; i++)
      {
        ibusValue[i] = (ibus[high] << 8) + ibus[low];
        high += 2;
        low += 2;
      }
      rxFrameDone = true;
      formatRx();
      return;
    }
    else
    {
      ibus[ibusIndex] = val;
      ibusIndex++;
    }
  }
}
