/*
 * debug_mng.h
 *
 *  Created on: 07-Jun-2025
 *      Author: xpress_embedo
 */

#ifndef _DEBUG_MNG_H_
#define _DEBUG_MNG_H_

#include "SEGGER_RTT.h"

#define DEBUGGING_ENABLED                   (0u)
#define DEBUGGING_DISABLED                  (1u)

// NOTE: Enable or Disable debugging
#define IS_DEBUG_ENABLED                    (DEBUGGING_ENABLED)
// #define IS_DEBUG_ENABLED                    (DEBUGGING_DISABLED)

#define PROTECT(x)                          do {          \
                                                  x       \
                                            } while(0);

#if (IS_DEBUG_ENABLED == DEBUGGING_ENABLED)
// Public Macros
#define DBG_INIT()
// Simple Print in White Color
#define DBG_WRITE_STR(x)                    PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);    \
                                                      SEGGER_RTT_WriteString(0, x);                 \
                                                      )
#define DBG_PRINTF(...)                     PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);    \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);            \
                                                      )

// Print in Red Color (here I am using bright color)
#define DBG_WRITE_STR_RED(x)                PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_RED);  \
                                                      SEGGER_RTT_WriteString(0, x);                         \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);            \
                                                      )
#define DBG_PRINTF_RED(...)                 PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_RED);  \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                    \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);            \
                                                      )

// Print in Green Color (here I am using bright color)
#define DBG_WRITE_STR_GREEN(x)              PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_GREEN);  \
                                                      SEGGER_RTT_WriteString(0, x);                           \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )
#define DBG_PRINTF_GREEN(...)               PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_GREEN);  \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                      \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )
// Print in Yellow Color (here I am using bright color)
#define DBG_WRITE_STR_YELLOW(x)             PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_YELLOW);  \
                                                      SEGGER_RTT_WriteString(0, x);                     \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);        \
                                                      )
#define DBG_PRINTF_YELLOW(...)              PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_YELLOW);  \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);        \
                                                      )

// Print in Blue Color (here I am using bright color)
#define DBG_WRITE_STR_BLUE(x)               PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_BLUE);   \
                                                      SEGGER_RTT_WriteString(0, x);                           \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )
#define DBG_PRINTF_BLUE(...)                PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_BLUE);   \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                      \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )

// Print in Magenta Color (here I am using bright color)
#define DBG_WRITE_STR_MAGENTA(x)            PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_MAGENTA); \
                                                      SEGGER_RTT_WriteString(0, x);                           \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )
#define DBG_PRINTF_MAGENTA(...)             PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_MAGENTA); \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                      \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )

// Print in Cyan Color (here I am using bright color)
#define DBG_WRITE_STR_CYAN(x)               PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_CYAN);   \
                                                      SEGGER_RTT_WriteString(0, x);                           \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )
#define DBG_PRINTF_CYAN(...)                PROTECT(  SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_CYAN);   \
                                                      SEGGER_RTT_printf(0, __VA_ARGS__);                      \
                                                      SEGGER_RTT_WriteString(0, RTT_CTRL_RESET);              \
                                                      )

#define DBG_COLOR_RESET()                   DBG_WRITE_STR(RTT_CTRL_RESET)
#define DBG_WRITE_IN_RED()                  DBG_WRITE_STR(RTT_CTRL_TEXT_RED)
#define DBG_WRITE_IN_BLACK()                DBG_WRITE_STR(RTT_CTRL_TEXT_BLACK)
#define DBG_WRITE_IN_GREEN()                DBG_WRITE_STR(RTT_CTRL_TEXT_GREEN)
#define DBG_WRITE_IN_YELLOW()               DBG_WRITE_STR(RTT_CTRL_TEXT_YELLOW)
#define DBG_WRITE_IN_CYAN()                 DBG_WRITE_STR(RTT_CTRL_TEXT_CYAN)
#define DBG_WRITE_IN_WHITE()                DBG_WRITE_STR(RTT_CTRL_TEXT_WHITE)
#define DBG_WRITE_IN_BRIGHT_BLACK()         DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_BLACK)
#define DBG_WRITE_IN_BRIGHT_RED()           DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_RED)
#define DBG_WRITE_IN_BRIGHT_GREEN()         DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_GREEN)
#define DBG_WRITE_IN_BRIGHT_YELLOW()        DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_YELLOW)
#define DBG_WRITE_IN_BRIGHT_BLUE()          DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_BLUE)
#define DBG_WRITE_IN_BRIGHT_MAGENTA()       DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_MAGENTA)
#define DBG_WRITE_IN_BRIGHT_CYAN()          DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_CYAN)
#define DBG_WRITE_IN_BRIGHT_WHITE()         DBG_WRITE_STR(RTT_CTRL_TEXT_BRIGHT_WHITE)
#else
// dummy macros
#define DBG_INIT()
#define DBG_WRITE_STR(x)
#define DBG_PRINTF(...)
#define DBG_WRITE_STR_RED(x)
#define DBG_PRINTF_RED(...)
#define DBG_WRITE_STR_GREEN(x)
#define DBG_PRINTF_GREEN(...)
#define DBG_WRITE_STR_YELLOW(x)
#define DBG_PRINTF_YELLOW(...)
#define DBG_WRITE_STR_BLUE(x)
#define DBG_PRINTF_BLUE(...)
#define DBG_WRITE_STR_MAGENTA(x)
#define DBG_PRINTF_MAGENTA(...)
#define DBG_WRITE_STR_CYAN(x)
#define DBG_PRINTF_CYAN(...)
#define DBG_COLOR_RESET()
#define DBG_WRITE_IN_RED()
#define DBG_WRITE_IN_BLACK()
#define DBG_WRITE_IN_GREEN()
#define DBG_WRITE_IN_YELLOW()
#define DBG_WRITE_IN_CYAN()
#define DBG_WRITE_IN_WHITE()
#define DBG_WRITE_IN_BRIGHT_BLACK()
#define DBG_WRITE_IN_BRIGHT_RED()
#define DBG_WRITE_IN_BRIGHT_GREEN()
#define DBG_WRITE_IN_BRIGHT_YELLOW()
#define DBG_WRITE_IN_BRIGHT_BLUE()
#define DBG_WRITE_IN_BRIGHT_MAGENTA()
#define DBG_WRITE_IN_BRIGHT_CYAN()
#define DBG_WRITE_IN_BRIGHT_WHITE()
#endif

// Function Prototypes

#endif // _DEBUG_MNG_H_
