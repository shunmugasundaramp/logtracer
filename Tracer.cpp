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

// System headers
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#if defined(_WIN32) || defined(_WIN64)
#include <tchar.h>
#include <atlstr.h>
#elif defined(__GNUC__)
#include <sys/stat.h>
#else
#err PlatformMismatches
#endif

// User headers
#include "Tracer.hpp"

namespace AKKU {
namespace Log {

#define VERSION_PRINT(_BUF_, _DATA_) do { \
                                        sprintf(_BUF_, "%s %-56s %s", DELIMITER, _DATA_, DELIMITER); \
                                        cout << _BUF_ << endl; \
                                     } while(0);

Tracer::LogLevelEnum_t   Tracer::m_logLevel = static_cast<Tracer::LogLevelEnum_t>(
                                              Tracer::LOG_LEVEL_ALL
                                              //Tracer::LOG_LEVEL_LOG | Tracer::LOG_LEVEL_ERROR | Tracer::LOG_LEVEL_WARNING
                                              );

Tracer::MediumTypeEnum_t Tracer::m_medium   =
                                              Tracer::MEDIUM_CONSOLE
                                              //Tracer::MEDIUM_FILE
                                              //Tracer::MEDIUM_NETWORK
                                              ;

static TracerMedium* k_tracerMedium         = NULL;

/// TracerMedium ////////////////////////////////////
TracerMedium::TracerMedium()
{

}

TracerMedium::~TracerMedium()
{

}

TracerMedium* TracerMedium::Instance()
{
    if (!k_tracerMedium) {
        switch (Tracer::m_medium) {
            case Tracer::MEDIUM_CONSOLE: k_tracerMedium = new TracerMediumConsole(); break;
            case Tracer::MEDIUM_FILE:    k_tracerMedium = new TracerMediumFile(); break;
            case Tracer::MEDIUM_NETWORK: k_tracerMedium = new TracerMediumNetwork(); break;
        }
    }
    return k_tracerMedium;
}

void TracerMedium::Destroy()
{
    delete(k_tracerMedium);
    k_tracerMedium = NULL;
}

const char* TracerMedium::Prepare(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs, uint32_t &len)
{
    static char strBuffer [6144 /*6K*/] = {'\0'};
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    string caption("");
    switch(type) {
        case Tracer::LOG_LEVEL_LOG:              caption = "LOG ";       break;
        case Tracer::LOG_LEVEL_ERROR:            caption = "ERR ";       break;
        case Tracer::LOG_LEVEL_WARNING:          caption = "WARN";       break;
        case Tracer::LOG_LEVEL_CALL_TRACE:       caption = "CALL";       break;
        case Tracer::LOG_LEVEL_NOT_IMPLEMENTED:  caption = "NIMP";       break;
        case Tracer::LOG_LEVEL_DUMP:             caption = "DUMP";       break;
        default: break;
    }

    int n = sprintf(strBuffer, "[%02d.%02d.%04d %02d:%02d:%02d] [%s] ",
        timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
        caption.c_str());

    if (Tracer::LOG_LEVEL_DUMP != type) {
        n += sprintf(strBuffer+n, "[%s] %s(): ", file, func);
    }

    n += vsnprintf (strBuffer+n, sizeof(strBuffer)-n, buf, vaargs);
    strBuffer[n] = '\0';
    ++n;
    len = n;
    return strBuffer;
}

/// TracerMediumConsole ////////////////////////////////////
TracerMediumConsole::TracerMediumConsole()
    : TracerMedium()
    , m_fileName("console::stdout")
{
}

TracerMediumConsole::~TracerMediumConsole()
{
}

void TracerMediumConsole::Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs)
{
    uint32_t len = 0;
    TracerMedium::Lock();
    fprintf(stdout, "%s\n", TracerMedium::Prepare(type, file, func, buf, vaargs, len));
    fflush(stdout);
    TracerMedium::Unlock();
}

/// TracerMediumFile ////////////////////////////////////
TracerMediumFile::TracerMediumFile()
    : TracerMedium()
    , m_fileName("")
{
    time_t rawtime;
    struct tm * timeinfo;
    char fileext[128];

    if (!m_fileName.empty()) {
#if defined(_WIN32) || defined(_WIN64)
    CString newPath(m_fileName.c_str());
    CreateDirectory(newPath, NULL);
#elif defined(__GNUC__)
    mkdir(m_fileName.c_str(), 0777);
#else
#err PlatformMismatches
#endif
    }

    m_fileName += "onn_ar_appmgr";

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(fileext, "_%04d%02d%02d_%02d%02d%02d.log",
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    m_fileName += fileext;
    m_handle = fopen(m_fileName.c_str(), "a");
    if (!m_handle) {
        cout << "Failed to open a log file: " << m_fileName.c_str() << endl;
    }
}

TracerMediumFile::~TracerMediumFile()
{
    if (m_handle) {
        fclose(m_handle);
    }
}

void TracerMediumFile::Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs)
{
    uint32_t len = 0;
    if (m_handle) {
        TracerMedium::Lock();
        fprintf(m_handle, "%s\n", TracerMedium::Prepare(type, file, func, buf, vaargs, len));
        fflush(m_handle);
        TracerMedium::Unlock();
    }
}

/// TracerMediumNetwork ////////////////////////////////////
TracerMediumNetwork::TracerMediumNetwork()
   : TracerMedium()
   , m_handle(0)
   , m_fileName("")
{
   // TODO:
   std::cout << "Network Medium not implemented. Please use either Console or File Medium" << endl;
}

TracerMediumNetwork::~TracerMediumNetwork()
{
   // TODO:
}

void TracerMediumNetwork::Print(Tracer::LogLevelEnum_t type, const char* file, const char* func, const char* buf, va_list vaargs)
{
   // TODO:
}

/// Tracer ////////////////////////////////////
Tracer::Tracer()
{
}

Tracer::~Tracer()
{
    if (!(m_logLevel&LOG_LEVEL_CALL_TRACE)) return;
    if (this->m_enableCalltrace) calltrace("exit");
}

Tracer::Tracer(const char * File, const char *Func, int Line, bool needEntryExit)
{
    this->m_func = Func;
    this->m_file = File;
    string file  = File;
    size_t len  = file.find("src");
    if (string::npos != len) {
        this->m_file += len;
    }
    this->m_enableCalltrace = needEntryExit;
    if (!(m_logLevel&LOG_LEVEL_CALL_TRACE)) return;
    if (this->m_enableCalltrace) calltrace("entry");
}

void Tracer::calltrace(const char * str,...)
{
    if (!(m_logLevel&LOG_LEVEL_CALL_TRACE)) return;

    va_list strArgList;
    va_start (strArgList, str);
    TracerMedium::Instance()->Print(LOG_LEVEL_CALL_TRACE, this->m_file, this->m_func, str, strArgList);
    va_end (strArgList);
}

void Tracer::Log(const char * str,...)
{
    if (!(m_logLevel&LOG_LEVEL_LOG)) return;

    va_list strArgList;
    va_start (strArgList, str);
    TracerMedium::Instance()->Print(LOG_LEVEL_LOG, this->m_file, this->m_func, str, strArgList);
    va_end (strArgList);
}


void Tracer::Error(const char * str,...)
{
    if (!(m_logLevel&LOG_LEVEL_ERROR)) return;

    va_list strArgList;
    va_start (strArgList, str);
    TracerMedium::Instance()->Print(LOG_LEVEL_ERROR, this->m_file, this->m_func, str, strArgList);
    va_end (strArgList);
}

void Tracer::Warn(const char * str,...)
{
    if (!(m_logLevel&LOG_LEVEL_WARNING)) return;

    va_list strArgList;
    va_start (strArgList, str);
    TracerMedium::Instance()->Print(LOG_LEVEL_WARNING, this->m_file, this->m_func, str, strArgList);
    va_end (strArgList);
}

void Tracer::NotImplemented(const char * args,...)
{
    if (m_logLevel == LOG_LEVEL_NONE) return;

    va_list strArgList;
    va_start (strArgList, args);
    TracerMedium::Instance()->Print(LOG_LEVEL_NOT_IMPLEMENTED, this->m_file, this->m_func, args, strArgList);
    va_end (strArgList);
}

void Tracer::NotImplemented()
{
    NotImplemented("");
}

void Tracer::SetLevel(LogLevelEnum_t level)
{
    m_logLevel = level;
}

void Tracer::SetMedium(MediumTypeEnum_t medium)
{
    m_medium = medium;
    TracerMedium::Destroy();
}

void Tracer::HexDump(const char* title, const uint8_t* addr, uint32_t len, uint8_t column)
{
    printf("\n");
    for (int32_t it = 0; it < column*3; ++it) {
        printf("=");
    }
    printf("\n%s {Bytes[%d]}\n", title, len);
    for (int32_t it = 0; it < column*3; ++it) {
        printf("=");
    }
    if (NULL == addr) {
        printf("==NULL==");
    } else {
        for (uint32_t it = 0; it < len; ++it) {
            if (it%column == 0) {
                printf("\n");
            }
            printf("%02X ", addr[it]);
        }
    }
    printf("\n");
    for (int32_t it = 0; it < column*3; ++it) {
        printf("=");
    }
    printf("\n");
}

}; // namespace Log
}; // namespace AKKU
