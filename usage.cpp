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
  * @file usage.cpp
  * @brief sample test code
  * @author Shunmuga (ssundaramp@outlook.com)
  *
  */

#include <stdio.h>

#include "Tracer.hpp"

using namespace std;

using namespace AKKU;

static void _PrintSomething()
{
    Log::Tracer tracer(TRACER_ARGS);

    tracer.Log("Printing as Log scope");
    tracer.Error("Printing as Error scope");
    tracer.Warn("Printing as Warn scope");
    tracer.NotImplemented("Printing as NotImplemented scope");
}

static void _HexDump(unsigned char columns)
{
    Log::Tracer tracer(TRACER_ARGS);
    unsigned char addr[128];

    for (unsigned char it = 0; it < sizeof(addr); ++it) {
        addr[it] = it;
    }

    tracer.HexDump("HexDumper", addr, sizeof(addr), columns);
}

/// @brief Main entry
/// @param[in] argc - argument count
/// @param[in] argv - argument values
/// @return error value
int main(int argc, char** argv)
{
    // Logging in Console

    Log::Tracer::SetLevel(Log::Tracer::LOG_LEVEL_ALL);
    cout << "\n*** Calling _PrintSomething() after enabling all levels\n";
    _PrintSomething();

    Log::Tracer::SetLevel(static_cast<Log::Tracer::LogLevelEnum_t>(Log::Tracer::LOG_LEVEL_ERROR | Log::Tracer::LOG_LEVEL_WARNING));
    cout << "\n*** Calling _PrintSomething() after enabling error and warning levels\n";
    _PrintSomething();

    Log::Tracer::SetLevel(static_cast<Log::Tracer::LogLevelEnum_t>(Log::Tracer::LOG_LEVEL_NONE));
    cout << "\n*** Calling _PrintSomething() after disabling all levels\n";
    _PrintSomething();

    cout << "\n*** Dumping HexaDecimal values as 10 columns\n";
    _HexDump(10);

    cout << "\n*** Dumping HexaDecimal values as 32 columns\n";
    _HexDump(32);



    // Logging in File
    Log::Tracer::SetMedium(Log::Tracer::MEDIUM_FILE);

    Log::Tracer::SetLevel(Log::Tracer::LOG_LEVEL_ALL);
    cout << "\n*** Calling _PrintSomething() after enabling all levels\n";
    _PrintSomething();

    Log::Tracer::SetLevel(static_cast<Log::Tracer::LogLevelEnum_t>(Log::Tracer::LOG_LEVEL_ERROR | Log::Tracer::LOG_LEVEL_WARNING));
    cout << "\n*** Calling _PrintSomething() after enabling error and warning levels\n";
    _PrintSomething();

    Log::Tracer::SetLevel(static_cast<Log::Tracer::LogLevelEnum_t>(Log::Tracer::LOG_LEVEL_NONE));
    cout << "\n*** Calling _PrintSomething() after disabling all levels\n";
    _PrintSomething();

    cout << "\n*** Dumping HexaDecimal values as 10 columns\n";
    _HexDump(10);

    cout << "\n*** Dumping HexaDecimal values as 32 columns\n";
    _HexDump(32);

    return 0;
}

