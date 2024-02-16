/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::CAN
 * Copyright (c) 2004-2017 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    UI.h
 * Purpose: CAN FD Example - User Interface Header File
 *----------------------------------------------------------------------------*/

#ifndef __UI_H
#define __UI_H

#include <stdbool.h>

// Global functions exported by UI.c module
extern bool UI_Initialize (void);
extern void UI_Thread     (void *arg);

#endif /* __UI_H */
