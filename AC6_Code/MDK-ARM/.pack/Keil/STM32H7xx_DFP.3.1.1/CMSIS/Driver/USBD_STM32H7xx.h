/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2023 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        15. February 2023
 * $Revision:    V1.3
 *
 * Project:      USBD Driver Header File for ST STM32H7xx
 * -------------------------------------------------------------------------- */

#ifndef __USBD_STM32H7XX_H
#define __USBD_STM32H7XX_H

#include <stdint.h>
#include "Driver_USBD.h"

#include "stm32h7xx_hal.h"

#define USBDx_EXPORT_DRIVER(x, fs_hs)                                                                                                                                                                                     \
static  ARM_USBD_CAPABILITIES USBD##x##_GetCapabilities           (void)                                              {return USBD_GetCapabilities (&USBD_##fs_hs##_Resources); }                                         \
static  int32_t               USBD##x##_Initialize                (ARM_USBD_SignalDeviceEvent_t   cb_device_event,                                                                                                        \
                                                                   ARM_USBD_SignalEndpointEvent_t cb_endpoint_event)  {return USBD_Initialize (cb_device_event, cb_endpoint_event, &USBD_##fs_hs##_Resources); }          \
static  int32_t               USBD##x##_Uninitialize              (void)                                              {return USBD_Uninitialize (&USBD_##fs_hs##_Resources); }                                            \
static  int32_t               USBD##x##_PowerControl              (ARM_POWER_STATE state)                             {return USBD_PowerControl (state, &USBD_##fs_hs##_Resources); }                                     \
static  int32_t               USBD##x##_DeviceConnect             (void)                                              {return USBD_DeviceConnect (&USBD_##fs_hs##_Resources); }                                           \
static  int32_t               USBD##x##_DeviceDisconnect          (void)                                              {return USBD_DeviceDisconnect (&USBD_##fs_hs##_Resources); }                                        \
static  ARM_USBD_STATE        USBD##x##_DeviceGetState            (void)                                              {return USBD_DeviceGetState (&USBD_##fs_hs##_Resources); }                                          \
static  int32_t               USBD##x##_DeviceRemoteWakeup        (void)                                              {return USBD_DeviceRemoteWakeup (&USBD_##fs_hs##_Resources); }                                      \
static  int32_t               USBD##x##_DeviceSetAddress          (uint8_t dev_addr)                                  {return USBD_DeviceSetAddress (dev_addr, &USBD_##fs_hs##_Resources); }                              \
static  int32_t               USBD##x##_ReadSetupPacket           (uint8_t *setup)                                    {return USBD_ReadSetupPacket ( setup, &USBD_##fs_hs##_Resources); }                                 \
static  int32_t               USBD##x##_EndpointConfigure         (uint8_t ep_addr,                                                                                                                                       \
                                                                   uint8_t ep_type,                                                                                                                                       \
                                                                   uint16_t ep_max_packet_size)                       {return USBD_EndpointConfigure (ep_addr, ep_type, ep_max_packet_size, &USBD_##fs_hs##_Resources); } \
static  int32_t               USBD##x##_EndpointUnconfigure       (uint8_t ep_addr)                                   {return USBD_EndpointUnconfigure (ep_addr, &USBD_##fs_hs##_Resources); }                            \
static  int32_t               USBD##x##_EndpointStall             (uint8_t ep_addr, bool stall)                       {return USBD_EndpointStall (ep_addr, stall, &USBD_##fs_hs##_Resources); }                           \
static  int32_t               USBD##x##_EndpointTransfer          (uint8_t ep_addr, uint8_t *data, uint32_t num)      {return USBD_EndpointTransfer (ep_addr, data, num, &USBD_##fs_hs##_Resources); }                    \
static  uint32_t              USBD##x##_EndpointTransferGetResult (uint8_t ep_addr)                                   {return USBD_EndpointTransferGetResult (ep_addr, &USBD_##fs_hs##_Resources); }                      \
static  int32_t               USBD##x##_EndpointTransferAbort     (uint8_t ep_addr)                                   {return USBD_EndpointTransferAbort (ep_addr, &USBD_##fs_hs##_Resources); }                          \
static  uint16_t              USBD##x##_GetFrameNumber            (void)                                              {return USBD_GetFrameNumber (&USBD_##fs_hs##_Resources); }                                          \
                                                                                                                                                                                                                          \
ARM_DRIVER_USBD Driver_USBD##x = {                                                                                                                                                                                        \
  USBD_GetVersion,                                                                                                                                                                                                        \
  USBD##x##_GetCapabilities,                                                                                                                                                                                              \
  USBD##x##_Initialize,                                                                                                                                                                                                   \
  USBD##x##_Uninitialize,                                                                                                                                                                                                 \
  USBD##x##_PowerControl,                                                                                                                                                                                                 \
  USBD##x##_DeviceConnect,                                                                                                                                                                                                \
  USBD##x##_DeviceDisconnect,                                                                                                                                                                                             \
  USBD##x##_DeviceGetState,                                                                                                                                                                                               \
  USBD##x##_DeviceRemoteWakeup,                                                                                                                                                                                           \
  USBD##x##_DeviceSetAddress,                                                                                                                                                                                             \
  USBD##x##_ReadSetupPacket,                                                                                                                                                                                              \
  USBD##x##_EndpointConfigure,                                                                                                                                                                                            \
  USBD##x##_EndpointUnconfigure,                                                                                                                                                                                          \
  USBD##x##_EndpointStall,                                                                                                                                                                                                \
  USBD##x##_EndpointTransfer,                                                                                                                                                                                             \
  USBD##x##_EndpointTransferGetResult,                                                                                                                                                                                    \
  USBD##x##_EndpointTransferAbort,                                                                                                                                                                                        \
  USBD##x##_GetFrameNumber                                                                                                                                                                                                \
}

// Endpoint structure definition
typedef struct {
           uint8_t  *data;
           uint32_t  num;
  volatile uint32_t  num_transferred_total;
  volatile uint32_t  num_transferring;
           uint16_t  max_packet_size;
  volatile uint16_t  active;
} ENDPOINT_t;

// USBD Info
typedef struct {
  ARM_USBD_SignalDeviceEvent_t   cb_device_event;
  ARM_USBD_SignalEndpointEvent_t cb_endpoint_event;
  volatile ARM_USBD_STATE        state;
  uint8_t                        hw_powered;
  uint8_t                        hw_initialized;
  volatile uint8_t               setup_received;
  uint8_t                        reserved;
} USBD_INFO_t;

// USBD Resources definition
typedef struct {
  PCD_HandleTypeDef  *p_hpcd;
  USBD_INFO_t        *info;
  ENDPOINT_t         *ep;
  uint32_t           *p_setup_packet;
  uint8_t            *p_otg_role;
  int16_t             irqn;
  uint8_t             max_ep_num;
  uint8_t             reserved;
  uint16_t            rx_fifo_size;
  uint16_t            tx_fifo_size[9];
} USBD_RESOURCES_t;

// Global functions and variables exported by driver .c module
#ifdef OTG_FS_DEVICE_USED
extern ARM_DRIVER_USBD Driver_USBD0;
#endif

#ifdef OTG_HS_DEVICE_USED
extern ARM_DRIVER_USBD Driver_USBD1;
#endif

#endif /* __USBD_STM32H7XX_H */
