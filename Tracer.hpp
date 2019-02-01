/*
Copyright [2016] [ssundaramp@outlook.com]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
  *
  * @file Tracer.hpp
  * @brief Implementation of Tracer Functionality
  * @author Shunmuga (ssundaramp@outlook.com)
  *
  */

#ifndef _AKKU_LogTracer_hpp_
#define _AKKU_LogTracer_hpp_

#pragma once

// System headers
#include <iostream>
#include <map>
#include <stdarg.h>
#include <stdint.h>
#include <mutex>

// User headers

using namespace std;

#define TRACER_ARGS __FILE__, __FUNCTION__, __LINE__

namespace AKKU {
namespace Log {

#define DELIMITER "*"

/**
 *  A Tracer class. It is used to control the debug prints
 */
class Tracer {
   public:
      /// @brief Log levels
      typedef enum {
         LOG_LEVEL_NONE              = 0x00000000,          ///< None of the log will be printed
         LOG_LEVEL_ERROR             = 0x00000001,          ///< Error print only comes
         LOG_LEVEL_WARNING           = 0x00000002,          ///< Warning print only comes
         LOG_LEVEL_LOG               = 0x00000004,          ///< Log print only comes
         LOG_LEVEL_CALL_TRACE        = 0x00000008,          ///< Calltrace print only comes
         LOG_LEVEL_NOT_IMPLEMENTED   = 0x00000010,          ///< Not implemented print only comes
         LOG_LEVEL_DUMP              = 0x00000020,          ///< Dump print only comes
         LOG_LEVEL_ALL               = ~0,                  ///< All print comes
      } LogLevelEnum_t;

      /// @brief Medium to print the log
      typedef enum {
         MEDIUM_CONSOLE              = 0x00000000,          ///< Print the log in console
         MEDIUM_FILE                 = 0x00000001,          ///< Print the log in file
         MEDIUM_NETWORK              = 0x00000002,          ///< Print the log in network
      } MediumTypeEnum_t;

   private:
      const char * m_func;
      const char * m_file;
      bool         m_enableCalltrace;

      /// @brief Constructor
      Tracer();

      /// @brief Rendering the border
      /// @return none
      inline void border() {
         int it = 0;
         while(it < 60 && ++it) {
            cout << DELIMITER;
         }
         cout << endl;
      }

      /// @brief API to print call trace
      /// @param[in] str - format specifier 
      /// @param[in] ... - variable arguments
      /// @return none
      void calltrace(const char * str,...);

   public:
      /// @brief Construct a new Tracer object
      /// @param[in] File - Name of the file
      /// @param[in] Func - Name of the function
      /// @param[in] Line - Line number
      /// @param[in] needEntryExit - Need log trace print in both entry & exit
      Tracer(const char * File, const char *Func, int Line, bool needEntryExit = true);

      /// @brief Destroy the Tracer object
      ~Tracer();

      /// @brief API to print log trace
      /// @param[in] str - format specifier 
      /// @param[in] ... - variable arguments
      /// @return none
      void Log(const char * str,...);

      /// @brief API to print error trace
      /// @param[in] str - format specifier 
      /// @param[in] ... - variable arguments
      /// @return none
      void Error(const char * str,...);

      /// @brief API to print warn trace
      /// @param[in] str - format specifier 
      /// @param[in] ... - variable arguments
      /// @return none
      void Warn(const char * str,...);

      /// @brief API to print not implemented trace
      /// @param[in] args - format specifier 
      /// @param[in] ... - variable arguments
      /// @return none
      void NotImplemented(const char * args,...);

      /// @brief API to print not implemented trace
      /// @return none
      void NotImplemented();

      /// @brief To enable/configure the log level
      /// @param[in] level - config level
      /// @return none
      static void SetLevel(LogLevelEnum_t level = LOG_LEVEL_NONE);

      /// @brief To enable/configure the log medium
      /// @param[in] medium - log medium
      /// @return none
      static void SetMedium(MediumTypeEnum_t medium = MEDIUM_CONSOLE);

      /// @brief Hex dump of the given raw buffer
      /// @param[in] title - Title to print above the Dump
      /// @param[in] addr - Address to dump
      /// @param[in] len - How much size to dump from the given address
      /// @param[in] column - Column size
      /// @return none
      static void HexDump(const char* title, const uint8_t* addr, uint32_t len, uint8_t column = 32);

      static LogLevelEnum_t     m_logLevel;     ///< Log level
      static MediumTypeEnum_t   m_medium;       ///< Log medium
};

/**
 *  A TracerMedium class. It is used to control the medium where the log prints
 */
class TracerMedium
{
   protected:
      /// @brief Construct a new Tracer Medium object
      TracerMedium();

      /// @brief Destroy the Tracer Medium object
      virtual ~TracerMedium();

      /// @brief To prepare the log data
      /// @param[in] type - Log level
      /// @param[in] file - File name
      /// @param[in] func - Function name
      /// @param[in] buf - Buffer to print
      /// @param[in] vaargs - Argument list
      /// @param[out] len - Size of the prepared buffer
      /// @return Output buffer
      const char* Prepare(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs, uint32_t &len);

   public:
      /// @brief Get the the singleton object
      /// @return Object/Instance of TracerMedium
      static TracerMedium* Instance();

      /// @brief Destroy the object
      static void Destroy();

      /// @brief To print the data which should be implemented in the Medium classes
      /// @param[in] type - Log level
      /// @param[in] file - File name
      /// @param[in] func - Function name
      /// @param[in] buf - Buffer to print
      /// @param[in] vaargs - Argument list
      /// @return none
      virtual void Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs) = 0;

      /// @brief To get the location where the log is dumped
      /// @return location string
      virtual string Location() const = 0;

      /// @brief To lock the buffer
      void Lock() {m_guard.lock();}

      /// @brief To unlock the buffer
      void Unlock() {m_guard.unlock();}

   private:
      mutex              m_guard;  ///< Instance for Guard
};

/**
 *  A TracerMediumConsole class. It is used to print the log in console
 */
class TracerMediumConsole : public TracerMedium
{
   public:
      /// @brief Construct a new Tracer Medium Console object
      TracerMediumConsole();

      /// @brief Destroy the Tracer Medium Console object
      ~TracerMediumConsole();

      /// @brief To print the data
      /// @param[in] type - Log level
      /// @param[in] file - File name
      /// @param[in] func - Function name
      /// @param[in] buf - Buffer to print
      /// @param[in] vaargs - Argument list
      /// @return none
      void Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs);

      /// @brief To get the location where the log is dumped
      /// @return location string
      string Location() const {return m_fileName;}

   private:
      string    m_fileName;
};

/**
 *  A TracerMediumFile class. It is used to print the log in physical file
 */
class TracerMediumFile : public TracerMedium
{
   public:
      /// @brief Construct a new Tracer Medium File object
      TracerMediumFile();

      /// @brief Destroy the Tracer Medium File object
      ~TracerMediumFile();

      /// @brief To print the data
      /// @param[in] type - Log level
      /// @param[in] file - File name
      /// @param[in] func - Function name
      /// @param[in] buf - Buffer to print
      /// @param[in] vaargs - Argument list
      /// @return none
      void Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs);

      /// @brief To get the location where the log is dumped
      /// @return location string
      string Location() const {return m_fileName;}

   private:
      FILE*     m_handle;
      string    m_fileName;
};

/**
 *  A TracerMediumNetwork class. It is used to print the log in network
 */
class TracerMediumNetwork : public TracerMedium
{
   public:
      /// @brief Construct a new Tracer Medium Network object
      TracerMediumNetwork();

      /// @brief Destroy the Tracer Medium Network object
      ~TracerMediumNetwork();

      /// @brief To print the data
      /// @param[in] type - Log level
      /// @param[in] file - File name
      /// @param[in] func - Function name
      /// @param[in] buf - Buffer to print
      /// @param[in] vaargs - Argument list
      /// @return none
      void Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs);

      /// @brief To get the location where the log is dumped
      /// @return location string
      string Location() const {return m_fileName;}

   private:
      int32_t   m_handle;
      string    m_fileName;
};

}; // namespace Log
}; // namespace AKKU

#endif //_AKKU_LogTracer_hpp_

