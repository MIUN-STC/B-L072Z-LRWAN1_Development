#pragma once

#include "aes.h"

//https://hackmd.io/s/S1kg6Ymo-#

struct LRWAN_Frame_Join_Request
{
  uint8_t MDHR;
  uint8_t AppEUI [8];
  uint8_t DevEUI [8];
  uint8_t DevNonce [2];
  uint8_t MIC [4];
};


struct LRWAN_Frame_Join_Accept
{
  uint8_t MDHR;
  uint8_t DevNonce [3];
  uint8_t NetID [3];
  uint8_t DevAddr [4];
  uint8_t DLSettings [1];
  uint8_t RXDelay [1];
  //0 or 16 bytes.
  uint8_t CFList [16];
  uint8_t MIC [4];
};

void LRWAN_Join (struct LoRa_Frame_Join_Request * Frame, uint8_t Key [16])
{
  uint8_t const Size = 19;
  uint8_t const * Data = (uint8_t *) Frame;
  AES_ECB_encrypt (Data, Key, Frame->MIC, Size);
}