/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    BSD_Client.c
 * Purpose: BSD socket client example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "RTE_Components.h" 

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32H743I-EVAL::Board Support:Graphic LCD


extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <o>Socket type <0=>Stream <1=>Datagram
//   <i>Stream (TCP) or Datagram (UDP) socket type.
#define SOCK_TYPE       0

//   <o>Remote Port <1-65535>
//   <i>Remote port number.
//   <i> Default: 1001
#define SOCK_PORT       1001

//   <e>Use IPv4
//   <i>Use IPv4 Protocol to connect.
#define IP4_ENABLE      1

//     <s.15>Remote IP Address
//     <i>Remote IPv4 Address in text representation.
//     <i>Default: "192.168.0.100"
#define IP4_ADDR        "192.168.0.100"

//   </e>
//   <e>Use IPv6
//   <i>Use IPv6 Protocol to connect
#define IP6_ENABLE      0

//     <s.40>Remote IP Address
//     <i>Remote IPv6 Address in text representation.
//     <i>Default: "fe80::1c30:6cff:fea2:455e"
#define IP6_ADDR        "fe80::1c30:6cff:fea2:455e"

//   </e>

//------------- <<< end of configuration section >>> -----------------------

#if (!(IP4_ENABLE ^ IP6_ENABLE))
  #error Select only IPv4 or only IPv6!
#endif

#define BLINKLED        0x01
#define SOCKTYPE  ((SOCK_TYPE == 0) ? SOCK_STREAM : SOCK_DGRAM)

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

#if   (GLCD_SIZE_X == 480)
  static uint32_t x =  5;
#elif (GLCD_SIZE_X == 640)
  static uint32_t x = 10;
#endif

static void Client (void *arg);
static void print_address (void);

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
    print_address ();
  }
}

/* Initialize display */
static void init_display () {
  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
  GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
}

/* Print local IP addresses */
static void print_address (void) {
  char    buf[24];

#if (IP4_ENABLE)
  uint8_t ip4_addr[NET_ADDR_IP4_LEN];
  char    ip4_ascii[16];

  GLCD_DrawString         (x*16U, 0U*24U, "     MW Network     ");
  GLCD_DrawString         (x*16U, 1U*24U, "   BSD Client IPv4  ");

  if (netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP4_Address,
                                         ip4_addr, sizeof(ip4_addr)) == netOK) {
  /* Print IPv4 network address */
    netIP_ntoa (NET_ADDR_IP4, ip4_addr, ip4_ascii, sizeof(ip4_ascii));
    sprintf (buf, "IP4=%-15s", ip4_ascii);
    GLCD_DrawString (x*16U, 4U*24U, buf);
  }
#endif

#if (IP6_ENABLE)
  uint8_t ip6_addr[NET_ADDR_IP6_LEN];
  char    ip6_ascii[40];

  GLCD_DrawString         (x*16U, 0U*24U, "     MW Network     ");
  GLCD_DrawString         (x*16U, 1U*24U, "   BSD Client IPv6  ");

  if (netIF_GetOption (NET_IF_CLASS_ETH, netIF_OptionIP6_LinkLocalAddress,
                                         ip6_addr, sizeof(ip6_addr)) == netOK) {
    /* Print IPv6 network address (MDK-Pro only) */
    netIP_ntoa(NET_ADDR_IP6, ip6_addr, ip6_ascii, sizeof(ip6_ascii));
    sprintf (buf, "IP6=%.16s", ip6_ascii);
  GLCD_DrawString (x*16U, 4U*24U, buf);
    sprintf (buf, "%s",ip6_ascii+16);
    GLCD_DrawString ((x+10U)*16U, 5U*24U, buf);
  }
#endif
}

/*----------------------------------------------------------------------------
  Thread 'Client': BSD Client socket process
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Client (void *arg) {
  int32_t  sock, retv;
  uint32_t led_num, led;
  char dbuf[4];

  (void)arg;

  led_num = LED_GetCount();

  /* Connect and send LED state */

  while (1) {
    osDelay (2000U);

    print_address();

#if (IP4_ENABLE)
    /* Create IPv4 socket address */
    SOCKADDR_IN  addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(SOCK_PORT);
    if (!inet_pton (AF_INET, IP4_ADDR, &addr.sin_addr)) {
      /* Address configuration error */
      for (;;);
    }

    sock = socket (AF_INET, SOCKTYPE, 0);
    retv = connect (sock, (SOCKADDR *)&addr, sizeof(addr));
#endif

#if (IP6_ENABLE)
    /* Create IPv6 socket address (MDK-Pro only) */
    SOCKADDR_IN6 addr6;
    addr6.sin6_family   = AF_INET6;
    addr6.sin6_port     = htons(SOCK_PORT);
    addr6.sin6_flowinfo = 0;
    if (!inet_pton (AF_INET6, IP6_ADDR, &addr6.sin6_addr)) {
      /* Address configuration error or IPv6 not supported */
      for (;;);
    }

    sock = socket (AF_INET6, SOCKTYPE, 0);
    retv = connect (sock, (SOCKADDR *)&addr6, sizeof(addr6));
#endif

    if (retv < 0) {
      closesocket (sock);
      continue;
    }

    led = 0;
    while (1) {
      // Send data to BSD Server
      dbuf[0] = BLINKLED;
      dbuf[1] = 0x01 << led;
      retv = send (sock, dbuf, 2, 0);
      if (retv < 0) {
        break;
      }
      if (++led >= led_num) {
        led = 0;
      }
      // Blink LED 0
      LED_On (0);
      osDelay (100);
      LED_Off (0);
      osDelay (900);
    }
    closesocket (sock);
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

  LED_Initialize ();
  init_display   ();
  netInitialize  ();
  print_address  ();

  // Create networking task
  osThreadNew (Client, NULL, NULL);

  osThreadExit();
}
