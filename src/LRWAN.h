#pragma once

#include <stdint.h>
#include "samtech/LoraMacCrypto.h"

//https://hackmd.io/s/S1kg6Ymo-#

#define LWAN_MHDR_MAJOR                         0b00000011
#define LWAN_MHDR_RFU                           0b00011100
#define LWAN_MHDR_MTYPE                         0b11100000

#define LWAN_MDHR_MTYPE_JOIN_REQUEST            0b00000000
#define LWAN_MDHR_MTYPE_JOIN_ACCEPT             0b00100000
#define LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_UP     0b01000000
#define LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_DOWN   0b01100000
#define LWAN_MDHR_MTYPE_CONFIRMED_DATA_UP       0b10000000
#define LWAN_MDHR_MTYPE_CONFIRMED_DATA_DOWN     0b10100000
#define LWAN_MDHR_MTYPE_RFU                     0b11000000
#define LWAN_MDHR_MTYPE_PROPRIETARY             0b11100000

#define LWAN_MDHR_MAJOR_R1                     0b00000000
#define LWAN_MDHR_MAJOR_RFU01                  0b00000001
#define LWAN_MDHR_MAJOR_RFU10                  0b00000010
#define LWAN_MDHR_MAJOR_RFU11                  0b00000011

#define LRWAN_FHDR_ADR            0b10000000
#define LRWAN_FHDR_ADRACKREQ      0b01000000
#define LRWAN_FHDR_ACK            0b00100000
#define LRWAN_FHDR_RFU            0b00010000
#define LRWAN_FHDR_FPENDING       0b00010000
#define LRWAN_FHDR_FOPTSLEN       0b00001111

// Network EUI
#define LORAWAN_APPLICATION_EUI {0xe5,0xe0,0x55,0x68,0x56,0x80,0x43,0x24}
//#define LORAWAN_APPLICATION_EUI {0x0b, 0x04, 0x1e, 0xf8, 0xea, 0x37, 0x75, 0x47}

// App Key
#define LORAWAN_APPLICATION_KEY {0xa3,0x51,0x5e,0x56,0x2a,0x69,0x1e,0xea,0x50,0xf5,0x4e,0x18,0xd0,0x6c,0x54,0xa4}

 
#define LORAWAN_MAC {0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00} 

struct __attribute__((__packed__)) 
LRWAN_Frame_Join_Request
{
  uint8_t MDHR;
  uint8_t AppEUI [8];
  uint8_t DevEUI [8];
  uint8_t DevNonce [2];
  //4
  uint8_t MIC [16];
};


struct __attribute__((__packed__)) 
LRWAN_Frame_Join_Accept
{
  uint8_t MDHR;
  uint8_t AppNonce [3];
  uint8_t NetID [3];
  uint8_t DevAddr [4];
  uint8_t DLSettings;
  uint8_t RXDelay;
  //0 or 16 bytes.
  //uint8_t CFList [16];
  //4
  //uint8_t MIC [16];
};


struct __attribute__((__packed__)) 
LRWAN_Message_Data
{
  uint8_t MDHR;
  uint8_t DevAddr [4];
  uint8_t FCtrl;
  uint8_t FCnt [2];
  //0..15B
  //uint8_t FOpts [15];
  //uint8_t MIC [16];
};

union 
LRWAN_Message
{
  uint8_t MDHR;
  struct LRWAN_Frame_Join_Request Request;
  struct LRWAN_Frame_Join_Accept Accept;
  struct LRWAN_Message_Data Data;
};


char * LRWAN_MTYPE_Get_String (int Value)
{
  switch (Value)
  {
    case LWAN_MDHR_MTYPE_JOIN_REQUEST:
    return "JOIN_REQUEST";
    case LWAN_MDHR_MTYPE_JOIN_ACCEPT:
    return "JOIN_ACCEPT";
    case LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_UP:
    return "UNCONFIRMED_DATA_UP";
    case LWAN_MDHR_MTYPE_UNCONFIRMED_DATA_DOWN:
    return "UNCONFIRMED_DATA_DOWN";
    case LWAN_MDHR_MTYPE_CONFIRMED_DATA_UP:
    return "CONFIRMED_DATA_UP";
    case LWAN_MDHR_MTYPE_CONFIRMED_DATA_DOWN:
    return "CONFIRMED_DATA_DOWN";
    case LWAN_MDHR_MTYPE_RFU:
    return "RFU";
    case LWAN_MDHR_MTYPE_PROPRIETARY:
    return "PROPRIETARY";
  }
  return "";
}


static void reverse (void * start, int size)
{
    unsigned char *lo = start;
    unsigned char *hi = start + size - 1;
    unsigned char swap;
    while (lo < hi) {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

void LRWAN_Join (struct LRWAN_Frame_Join_Request * Frame, uint8_t const Key [16])
{
  uint8_t const Size = 19;
  uint8_t const * Data = (uint8_t const *) Frame;
  reverse (Frame->AppEUI, 8);
  reverse (Frame->DevEUI, 8);
  //Frame->DevNonce [0] = 0;
  //Frame->DevNonce [1] = 0;
  LoRaMacJoinComputeMic (Data, Size, Key, Frame->MIC);
}





