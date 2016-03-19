//------------------------------------------------------------------------------
//
//    Copyright (C) Streamlet. All rights reserved.
//
//    File Name:   Log.h
//    Author:      Streamlet
//    Create Time: 2016-03-05
//    Description: 
//
//------------------------------------------------------------------------------

#ifndef __LOG_H_6AD94564_D193_4DB3_8C90_A174D4853482_INCLUDED__
#define __LOG_H_6AD94564_D193_4DB3_8C90_A174D4853482_INCLUDED__


#ifdef _DEBUG
#define XL_LOG_TARGET_DEBUGGER                          // Output to console
#define XL_LOG_LEVEL_FATAL                              // Output log level fatal
#define XL_LOG_LEVEL_ERROR                              // Output log level error
#define XL_LOG_LEVEL_WARNING                            // Output log level warning
#define XL_LOG_LEVEL_INFO                               // Output log level info
#define XL_LOG_LEVEL_VERBOSE                            // Output log level verbose
#else
#define XL_LOG_PREFIX L"[xlFlashPlayer] "               // Log prefix
#define XL_LOG_TARGET_FILE L"%Temp%\\xlFiveChess.log"   // Output to file, supporting environment variables
#define XL_LOG_LEVEL_FATAL                              // Output log level fatal
#define XL_LOG_LEVEL_ERROR                              // Output log level error
#define XL_LOG_LEVEL_WARNING                            // Output log level warning
#endif
#include <xl/AppHelper/xlLog.h>


#endif // #ifndef __LOG_H_6AD94564_D193_4DB3_8C90_A174D4853482_INCLUDED__
