/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.0.0
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE

#if      defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma  clang diagnostic push
#pragma  clang diagnostic ignored "-Wformat-nonliteral"
#endif

// Local variables.
static char label[12] = "SD_CARD";

// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// Local functions.
static void dot_format (uint64_t val, char *sp);


// Process query string received by GET request.
void netCGI_ProcessQuery (const char *qstr) {
  // Method not used in this example
  (void)qstr;
}

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void netCGI_ProcessData (uint8_t code, const char *data, uint32_t len) {
  static FILE *file = NULL;
  static char var[40];

  switch (code) {
    case 0:
      // Url encoded form data received
      if (len > 0) {
        bool do_format = false;
        do {
          // Parse all parameters
          data = netCGI_GetEnvVar (data, var, sizeof (var));
          if (strncmp (var, "label=", 6) == 0) {
            strcpy (&label[0], &var[6]);
          }
          else if (strcmp (var, "format=yes") == 0) {
            do_format = true;
          }
        } while (data);
        // Format SD card on request
        if (do_format) {
          sprintf (&var[0], "/L %s", label);
          if (finit ("M0:") == fsOK) {
            fmount ("M0:");
            fformat ("M0:", var);
          }
        }
      }
      break;

    case 1:
      // Filename for file upload received
      if (data[0] != 0) {
        const char *p;
        // Skip path information
        for (p = data; *p; p++) {
          if (*p == '\\' || *p == '/') {
            data = p + 1;
          }
        }
        // Initialize and mount SD card
        if ((finit ("M0:") == fsOK) &&
            (fmount ("M0:") == fsOK)) {
          // Open a file for writing
          file = fopen (data, "w");
        }
      }
      break;

    case 2:
      // File content data received
      if (file != NULL) {
        // Write data to a file
        fwrite (data, 1, len, file);
      }
      break;

    case 3:
      // File upload finished
      if (file != NULL) {
        // Close a file
        fclose (file);
      }
      break;

    default:
      // Ignore all other codes
      break;
  }
}

// Generate dynamic web data from a script line.
uint32_t netCGI_Script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  static fsFileInfo info;
  static char temp[16];
  uint32_t len = 0;

  (void)buflen;

  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'f':
      // Format SD card in 'format.cgi'
      switch (env[2]) {
        case '1':
          // Format label
          len += (uint32_t)sprintf (buf, &env[4], label);
          break;
      }
      break;

    case 'd':
      // Directory of SD card in 'dir.cgi'
      if (MYBUF(pcgi)->xcnt == 0) {
        // Initialize environment on first call
        info.fileID = 0;
        if (!(finit ("M0:") == fsOK) ||
            !(fmount ("M0:") == fsOK)) {
          // No card or failed to initialize
          break;
        }
      }
      // Repeat for all files, ignore folders
      MYBUF(pcgi)->xcnt++;
      if (ffind ("*.*", &info) == fsOK) {
        dot_format (info.size, temp);
        len += (uint32_t)sprintf (buf, "<tr align=center><td>%d.</td>"
                                       "<td align=left><a href=\"/%s\">%s</a></td>"
                                       "<td align=right>%s</td>"
                                       "<td>%02d.%02d.%04d - %02d:%02d</td></tr>\r\n",
                                       MYBUF(pcgi)->xcnt,info.name,info.name,temp,
                                       info.time.day, info.time.mon, info.time.year,
                                       info.time.hr, info.time.min);
        // Hi bit is a repeat flag
        len |= (1u << 31);
      }
  }
  return (len);
}

// Print size in dotted decimal format.
static void dot_format (uint64_t val, char *sp) {

  if (val >= (uint64_t)1e9) {
    sp += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e9));
    val %= (uint64_t)1e9;
    sp += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e6));
    val %= (uint64_t)1e6;
    sprintf (sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  if (val >= (uint64_t)1e6) {
    sp += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e6));
    val %= (uint64_t)1e6;
    sprintf (sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  if (val >= 1000) {
    sprintf (sp,"%d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
    return;
  }
  sprintf (sp,"%d",(uint32_t)(val));
}

#if      defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma  clang diagnostic pop
#endif
