/*-----------------------------------------------------------------------------
 * Name:    Touch_743I_EVAL.c
 * Purpose: Touchscreen interface for STM32H743I-EVAL Board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2017 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "stm32h743i_eval_ts.h"
#include "Board_Touch.h"


/**
  \fn          int32_t Touch_Initialize (void)
  \brief       Initialize touchscreen
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Touch_Initialize (void) {
  uint32_t ts_status = BSP_ERROR_NONE;
  TS_Init_t hTS;

#ifdef USE_MB1046
  hTS.Width = 480;
  hTS.Height = 272;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 0;
#else
  hTS.Width = 640;
  hTS.Height = 480;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 0;
#endif

  ts_status = BSP_TS_Init(0, &hTS);
  if (ts_status != BSP_ERROR_NONE) {
    return -1;
  }

  return 0;
}

/**
  \fn          int32_t Touch_Uninitialize (void)
  \brief       De-initialize touchscreen
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Touch_Uninitialize (void) {
  uint32_t ts_status = BSP_ERROR_NONE;

  ts_status = BSP_TS_DeInit(0);

  if (ts_status != BSP_ERROR_NONE) {
    return -1;
  }

  return 0;
}

/**
  \fn          int32_t Touch_GetState (TOUCH_STATE *pState)
  \brief       Get touchscreen state
  \param[out]  pState  pointer to TOUCH_STATE structure
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Touch_GetState (TOUCH_STATE *pState) {
  TS_State_t TS_State;
  uint32_t ts_status = BSP_ERROR_NONE;

  ts_status = BSP_TS_GetState(0, &TS_State);

  if (ts_status != BSP_ERROR_NONE) {
    return -1;
  }

  if (TS_State.TouchDetected) {
    pState->pressed = 1;
    pState->x = (int16_t)TS_State.TouchX;
    pState->y = (int16_t)TS_State.TouchY;
  } else {
    pState->pressed = 0;
  }

  return 0;
}
