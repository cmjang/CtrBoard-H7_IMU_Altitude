/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    BSD_Server.c
 * Purpose: BSD socket server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "RTE_Components.h" 

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.STM32H743I-EVAL::Board Support:Graphic LCD
#include "Board_LED.h"                  // ::Board Support:LED

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

//   <o>Local Port <1-65535>
//   <i>Local port number.
//   <i> Default: 1001
#define SOCK_PORT      1001

//   <q>Use IPv4
//   <i>Use IPv4 Protocol to accept connections.
#define IP4_ENABLE      1

//   <q>Use IPv6
//   <i>Use IPv6 Protocol to accept connections.
#define IP6_ENABLE      0

//------------- <<< end of configuration section >>> -----------------------

#if (!(IP4_ENABLE ^ IP6_ENABLE))
  #error Select only IPv4 or only IPv6!
#endif

#define BLINKLED        0x01

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

#if   (GLCD_SIZE_X == 480)
  static uint32_t x =  5;
#elif (GLCD_SIZE_X == 640)
  static uint32_t x = 10;
#endif

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

static void Stream_Server (void *arg);
static void Dgram_Server (void *arg);

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

  GLCD_DrawString (x*16U, 0*24, "     MW Network     ");
  GLCD_DrawString (x*16U, 1*24, "  BSD Server IPv4   ");

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

  GLCD_DrawString (x*16U, 0U*24U, "     MW Network     ");
  GLCD_DrawString (x*16U, 1U*24U, "   BSD Client IPv6  ");

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

/// Stream socket server thread
static void Stream_Server (void *arg) {
  int32_t sock, sd, retv;
  char dbuf[4];

  while (1) {

#if (IP4_ENABLE)
    /* Create IPv4 socket address */
    SOCKADDR_IN  addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons (SOCK_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    sock = socket (AF_INET, SOCK_STREAM, 0);
    retv = bind (sock, (SOCKADDR *)&addr, sizeof (addr));
#endif

#if (IP6_ENABLE)
    /* Create IPv6 socket address (MDK-Pro only) */
    SOCKADDR_IN6 addr6;
    addr6.sin6_family    = AF_INET6;
    addr6.sin6_port      = htons (SOCK_PORT);
    addr6.sin6_flowinfo  = 0;
    if (!inet_pton (AF_INET6, "::", &addr6.sin6_addr)) {
      /* IPv6 not supported */
      for (;;);
    }

    sock = socket (AF_INET6, SOCK_STREAM, 0);
    retv = bind (sock, (SOCKADDR *)&addr6, sizeof (addr6));
#endif

    if (retv < 0) {
      closesocket (sock);
      continue;
    }
    retv = listen (sock, 1);
    if (retv < 0) {
      closesocket (sock);
      continue;
    }
      sd = accept (sock, NULL, NULL);
      closesocket (sock);

    while (1) {
      retv = recv (sd, dbuf, sizeof (dbuf), 0);
      if (retv <= 0) {
        break;
      }
      if (dbuf[0] == BLINKLED) {
        LED_SetOut (dbuf[1]);
      }
    }
    closesocket (sd);
  }
}

/// Datagram socket server thread
static void Dgram_Server (void *arg) {
  int32_t sock, retv;
  char dbuf[4];

  while (1) {

#if (IP4_ENABLE)
    /* Create IPv4 socket address */
    SOCKADDR_IN  addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons (SOCK_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    sock = socket (AF_INET, SOCK_DGRAM, 0);
    retv = bind (sock, (SOCKADDR *)&addr, sizeof (addr));
#endif

#if (IP6_ENABLE)
    /* Create IPv6 socket address (MDK-Pro only) */
    SOCKADDR_IN6 addr6;
    addr6.sin6_family    = AF_INET6;
    addr6.sin6_port      = htons (SOCK_PORT);
    addr6.sin6_flowinfo  = 0;
    if (!inet_pton (AF_INET6, "::", &addr6.sin6_addr)) {
      /* IPv6 not supported */
      for (;;);
    }

    sock = socket (AF_INET6, SOCK_DGRAM, 0);
    retv = bind (sock, (SOCKADDR *)&addr6, sizeof (addr6));
#endif

    if (retv < 0) {
      closesocket (sock);
      continue;
    }

    while (1) {
      retv = recv (sock, dbuf, sizeof (dbuf), 0);
      if (retv == BSD_ETIMEDOUT) {
        continue;
      }
      if (retv <= 0) {
        break;
      }
      if (dbuf[0] == BLINKLED) {
        LED_SetOut (dbuf[1]);
      }
    }
    closesocket (sock);
  }
}

/*----------------------------------------------------------------------------
  Main Thread 'app_main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;

  LED_Initialize ();
  init_display ();

  netInitialize ();
  print_address ();

  /* Create server threads */
  osThreadNew (Stream_Server, NULL, NULL);
  osThreadNew (Dgram_Server, NULL, NULL);

  osThreadExit();
}
