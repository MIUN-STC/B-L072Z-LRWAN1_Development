#pragma once


// Network EUI
#define LORAWAN_APPLICATION_EUI {0xe5,0xe0,0x55,0x68,0x56,0x80,0x43,0x24}

// App Key
#define LORAWAN_APPLICATION_KEY {0xa3,0x51,0x5e,0x56,0x2a,0x69,0x1e,0xea,0x50,0xf5,0x4e,0x18,0xd0,0x6c,0x54,0xa4}

#define LWAN_MHDR_MAJOR                        0b00000011
#define LWAN_MHDR_RFU                          0b00011100
#define LWAN_MHDR_MTYPE                        0b11100000

#define LWAN_MDHR_MTYPE_JOIN_REQUEST           0b00000000
#define LWAN_MDHR_MTYPE_JOIN_ACCEPT            0b00100000
#define LWAN_MDHR_MTYPE_UNCONFIREM_DATA_UP     0b01000000
#define LWAN_MDHR_MTYPE_UNCONFIREM_DATA_DOWN   0b01100000



struct LoRa_PHYPayload
{
  uint8_t MHDR;
  uint8_t MACPayload [6];
  uint8_t MIC [4];
};

void LoRa_Join (struct LoRa_PHYPayload * Item)
{
  Item->MDHR = LWAN_MDHR_MTYPE_JOIN_REQUEST;
  Item->MAX
}