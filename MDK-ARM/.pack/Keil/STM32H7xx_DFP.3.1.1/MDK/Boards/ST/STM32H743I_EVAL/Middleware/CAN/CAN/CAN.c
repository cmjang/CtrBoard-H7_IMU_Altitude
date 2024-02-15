/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::CAN
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    CAN.c
 * Purpose: CAN Example - CAN Interface module
 *----------------------------------------------------------------------------*/

#include "CAN.h"

#include <string.h>

#include "main.h"
#include "UI.h"

#include "Driver_CAN.h"


// COMPILE-TIME CONFIGURATION --------------------------------------------------

#define  APP_MAIN_STK_SZ       (1024)     // Application main stack size
#define  CAN_THREAD_STK_SZ     (512)      // CAN thread stack size
#define  UI_THREAD_STK_SZ      (512)      // UI thread stack size

#define  CAN_CONTROLLER         1         // CAN Controller number
#define  CAN_LOOPBACK          (1)        // 0 = no loopback, 1 = external loopback
#define  CAN_DATA_LEN          (8)        // Number of data bytes in message
#define  CAN_BITRATE_NOMINAL   (125000)   // Nominal bitrate (125 kbit/s)

//------------------------------------------------------------------------------

#define _CAN_Driver_(n)         Driver_CAN##n
#define  CAN_Driver_(n)        _CAN_Driver_(n)
extern   ARM_DRIVER_CAN         CAN_Driver_(CAN_CONTROLLER);
#define  ptrCAN               (&CAN_Driver_(CAN_CONTROLLER))

// Global variables
             uint64_t           app_main_stk[(APP_MAIN_STK_SZ+7)/8];
       const osThreadAttr_t     app_main_attr = {
  .stack_mem  =        app_main_stk,
  .stack_size = sizeof(app_main_stk)
};
       uint8_t                  rx_data[CAN_DATA_LEN];
       uint8_t                  tx_data[CAN_DATA_LEN];

// Local variables
static       uint64_t           can_thread_stk[(CAN_THREAD_STK_SZ+7)/8];
static const osThreadAttr_t     can_thread_attr = {
  .stack_mem  =        can_thread_stk,
  .stack_size = sizeof(can_thread_stk)
};
static       uint64_t           ui_thread_stk[(UI_THREAD_STK_SZ+7)/8];
static const osThreadAttr_t     ui_thread_attr = {
  .stack_mem  =        ui_thread_stk,
  .stack_size = sizeof(ui_thread_stk)
};

static osThreadId_t             ui_thread_id;
static uint32_t                 rx_obj_idx;
static ARM_CAN_MSG_INFO         rx_msg_info;
static uint32_t                 tx_obj_idx;
static ARM_CAN_MSG_INFO         tx_msg_info;

// Global functions
       void app_main             (void *arg);

// Local functions
static bool CAN_Initialize       (void);
static void CAN_SignalUnitEvent  (uint32_t event);
static void CAN_SignalObjectEvent(uint32_t obj_idx, uint32_t event);
static void CAN_Thread           (void *arg);


/*------------------------------------------------------------------------------
 *  Error Handler
 *----------------------------------------------------------------------------*/
__NO_RETURN static void ErrorHandler (void) {
  for (;;);
}

/*------------------------------------------------------------------------------
 *  Application
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {

  (void)arg;

  if (!UI_Initialize ()) {              // User Interface Initialization
    ErrorHandler();
  }
  if (!CAN_Initialize()) {              // CAN  Interface Initialization
    ErrorHandler();
  }

  // Start Threads
  ui_thread_id = osThreadNew(UI_Thread, NULL, &ui_thread_attr);
  if (ui_thread_id == 0U) {
    ErrorHandler();
  }
  if (osThreadNew(CAN_Thread, NULL, &can_thread_attr) == 0U) {
    ErrorHandler();
  }

  osThreadExit();
}

//------------------------------------------------------------------------------
//  CAN Interface Initialization
//------------------------------------------------------------------------------
static bool CAN_Initialize (void) {
  ARM_CAN_CAPABILITIES     can_cap;
  ARM_CAN_OBJ_CAPABILITIES can_obj_cap;
  int32_t                  status;
  uint32_t                 i, num_objects, clock;

  can_cap = ptrCAN->GetCapabilities ();                                         // Get CAN driver capabilities
  num_objects = can_cap.num_objects;                                            // Number of receive/transmit objects

  status = ptrCAN->Initialize    (CAN_SignalUnitEvent, CAN_SignalObjectEvent);  // Initialize CAN driver
  if (status != ARM_DRIVER_OK) { return false; }

  status = ptrCAN->PowerControl  (ARM_POWER_FULL);                              // Power-up CAN controller
  if (status != ARM_DRIVER_OK) { return false; }

  status = ptrCAN->SetMode       (ARM_CAN_MODE_INITIALIZATION);                 // Activate initialization mode
  if (status != ARM_DRIVER_OK) { return false; }

  clock = ptrCAN->GetClock();                                                   // Get CAN bas clock
  if ((clock % (8U*CAN_BITRATE_NOMINAL)) == 0U) {                               // If CAN base clock is divisible by 8 * nominal bitrate without remainder
    status = ptrCAN->SetBitrate    (ARM_CAN_BITRATE_NOMINAL,                    // Set nominal bitrate
                                    CAN_BITRATE_NOMINAL,                        // Set nominal bitrate to configured constant value
                                    ARM_CAN_BIT_PROP_SEG  (5U) |                // Set propagation segment to 5 time quanta
                                    ARM_CAN_BIT_PHASE_SEG1(1U) |                // Set phase segment 1 to 1 time quantum (sample point at 87.5% of bit time)
                                    ARM_CAN_BIT_PHASE_SEG2(1U) |                // Set phase segment 2 to 1 time quantum (total bit is 8 time quanta long)
                                    ARM_CAN_BIT_SJW       (1U));                // Resynchronization jump width is same as phase segment 2
  } else if ((clock % (10U*CAN_BITRATE_NOMINAL)) == 0U) {                       // If CAN base clock is divisible by 10 * nominal bitrate without remainder
    status = ptrCAN->SetBitrate    (ARM_CAN_BITRATE_NOMINAL,                    // Set nominal bitrate
                                    CAN_BITRATE_NOMINAL,                        // Set nominal bitrate to configured constant value
                                    ARM_CAN_BIT_PROP_SEG  (7U) |                // Set propagation segment to 7 time quanta
                                    ARM_CAN_BIT_PHASE_SEG1(1U) |                // Set phase segment 1 to 1 time quantum (sample point at 90% of bit time)
                                    ARM_CAN_BIT_PHASE_SEG2(1U) |                // Set phase segment 2 to 1 time quantum (total bit is 10 time quanta long)
                                    ARM_CAN_BIT_SJW       (1U));                // Resynchronization jump width is same as phase segment 2
  } else if ((clock % (12U*CAN_BITRATE_NOMINAL)) == 0U) {                       // If CAN base clock is divisible by 12 * nominal bitrate without remainder
    status = ptrCAN->SetBitrate    (ARM_CAN_BITRATE_NOMINAL,                    // Set nominal bitrate
                                    CAN_BITRATE_NOMINAL,                        // Set nominal bitrate to configured constant value
                                    ARM_CAN_BIT_PROP_SEG  (7U) |                // Set propagation segment to 7 time quanta
                                    ARM_CAN_BIT_PHASE_SEG1(2U) |                // Set phase segment 1 to 2 time quantum (sample point at 83.3% of bit time)
                                    ARM_CAN_BIT_PHASE_SEG2(2U) |                // Set phase segment 2 to 2 time quantum (total bit is 12 time quanta long)
                                    ARM_CAN_BIT_SJW       (2U));                // Resynchronization jump width is same as phase segment 2
  } else {
    return false;
  }
  if (status != ARM_DRIVER_OK) { return false; }

  rx_obj_idx = 0xFFFFFFFFU;
  tx_obj_idx = 0xFFFFFFFFU;
  for (i = 0U; i < num_objects; i++) {                                          // Find first available object for receive and transmit
    can_obj_cap = ptrCAN->ObjectGetCapabilities (i);                            // Get object capabilities
    if      ((rx_obj_idx == 0xFFFFFFFFU) && (can_obj_cap.rx == 1U)) { rx_obj_idx = i;        }
    else if ((tx_obj_idx == 0xFFFFFFFFU) && (can_obj_cap.tx == 1U)) { tx_obj_idx = i; break; }
  }
  if ((rx_obj_idx == 0xFFFFFFFFU) || (tx_obj_idx == 0xFFFFFFFFU)) { return false; }

  // Set filter to receive messages with extended ID 0x12345678
  status = ptrCAN->ObjectSetFilter(rx_obj_idx, ARM_CAN_FILTER_ID_EXACT_ADD, ARM_CAN_EXTENDED_ID(0x12345678U), 0U);
  if (status != ARM_DRIVER_OK) { return false; }

  status = ptrCAN->ObjectConfigure(tx_obj_idx, ARM_CAN_OBJ_TX);                 // Configure transmit object
  if (status != ARM_DRIVER_OK) { return false; }

  status = ptrCAN->ObjectConfigure(rx_obj_idx, ARM_CAN_OBJ_RX);                 // Configure receive object
  if (status != ARM_DRIVER_OK) { return false; }

#if (CAN_LOOPBACK == 1)
  if (can_cap.external_loopback != 1U) { return false; }
  status = ptrCAN->SetMode (ARM_CAN_MODE_LOOPBACK_EXTERNAL);                    // Activate loopback external mode
  if (status != ARM_DRIVER_OK) { return false; }
#else
  status = ptrCAN->SetMode (ARM_CAN_MODE_NORMAL);                               // Activate normal operation mode
  if (status != ARM_DRIVER_OK) { return false; }
#endif

  return true;
}

//------------------------------------------------------------------------------
//  CAN Interface Signal Unit Event Callback
//------------------------------------------------------------------------------
static void CAN_SignalUnitEvent (uint32_t event) {

  switch (event) {
    case ARM_CAN_EVENT_UNIT_ACTIVE:
      break;
    case ARM_CAN_EVENT_UNIT_WARNING:
      break;
    case ARM_CAN_EVENT_UNIT_PASSIVE:
      break;
    case ARM_CAN_EVENT_UNIT_BUS_OFF:
      break;
  }
}

//------------------------------------------------------------------------------
//  CAN Interface Signal Object Event Callback
//------------------------------------------------------------------------------
static void CAN_SignalObjectEvent (uint32_t obj_idx, uint32_t event) {

  if (event == ARM_CAN_EVENT_RECEIVE) {         // If receive event
    if (obj_idx == rx_obj_idx) {                // If receive object event
      ptrCAN->MessageRead(rx_obj_idx, &rx_msg_info, rx_data, CAN_DATA_LEN);
      osThreadFlagsSet(ui_thread_id, 1U);       // Send signal to user interface thread
    }
  }
  if (event == ARM_CAN_EVENT_SEND_COMPLETE) {   // If send completed event
    if (obj_idx == tx_obj_idx) {                // If transmit object event
      osThreadFlagsSet(ui_thread_id, 2U);       // Send signal to user interface thread
    }
  }
}

//------------------------------------------------------------------------------
//  CAN Interface Thread
//------------------------------------------------------------------------------
__NO_RETURN static void CAN_Thread (void *arg) {
  int i;

  (void)arg;

  memset(&tx_msg_info, 0U, sizeof(ARM_CAN_MSG_INFO));   // Clear message info structure
  tx_msg_info.id = ARM_CAN_EXTENDED_ID(0x12345678U);    // Set extended ID for transmit message
  i = 0;
  while (1) {
    memset (tx_data, i, CAN_DATA_LEN);                  // Fill transmit buffer
    i++;

    // Send data message
    ptrCAN->MessageSend(tx_obj_idx, &tx_msg_info, tx_data, CAN_DATA_LEN);
    osDelay(1000U);
  }
}
