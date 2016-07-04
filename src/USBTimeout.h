#ifndef USB_TIMEOUT_H
#define USB_TIMEOUT_H

// Maximum number of transmit packets to queue so we don't starve other endpoints for memory
#define TX_PACKET_LIMIT 3

// When the PC isn't listening, how long do we wait before discarding data?
#define TX_TIMEOUT_MSEC 30

#if F_CPU == 240000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1600)
#elif F_CPU == 216000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1440)
#elif F_CPU == 192000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1280)
#elif F_CPU == 180000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1200)
#elif F_CPU == 168000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 1100)
#elif F_CPU == 144000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 932)
#elif F_CPU == 120000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 764)
#elif F_CPU == 96000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 596)
#elif F_CPU == 72000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 512)
#elif F_CPU == 48000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 428)
#elif F_CPU == 24000000
  #define TX_TIMEOUT (TX_TIMEOUT_MSEC * 262)
#endif

#endif // USB_TIMEOUT_H
