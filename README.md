
                   AKKU::Log::Tracer, Release 2.0.0

This is AKKU::Log::Tracer, a C++ library for do the logging(either 
console or file) as secured way written by Shunmuga Sundaram.  It is
a independent source which should not having any other thirdparty
software references

Currently it is having a support to compile with GCC Compiler.
Hopefully it will come for other compilers too.

AKKU Parser is free software.  You may copy, distribute, and modify
it under the terms of the License contained in the file COPYING
distributed with this package.  This license is the same as the
Apache Consortium license.


# AKKU::Log::Tracer
Simple module to do the logging in either File or Console

1. Easy to include anywhere
2. No dependencies
3. Simpler & Modular
4. Extend this logging based on our requirement easily

# Usage
## In Linux

### To Compile 

./linbuild.sh

### To Run

./objs/usage.exe

## In Windows
### To Compile

winbuild.bat

### To Run

objs/usage.exe

# Output
```
** Below Logs were written into Console
*** Calling _PrintSomething() after enabling all levels
[01.02.2019 15:40:04] [CALL] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): entry
[01.02.2019 15:40:04] [LOG ] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as Log scope
[01.02.2019 15:40:04] [ERR ] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as Error scope
[01.02.2019 15:40:04] [WARN] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as Warn scope
[01.02.2019 15:40:04] [NIMP] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as NotImplemented scope
[01.02.2019 15:40:04] [CALL] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): exit

*** Calling _PrintSomething() after enabling error and warning levels
[01.02.2019 15:40:04] [ERR ] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as Error scope
[01.02.2019 15:40:04] [WARN] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as Warn scope
[01.02.2019 15:40:04] [NIMP] [D:\proj.x\mine\logtracer\usage.cpp] _PrintSomething(): Printing as NotImplemented scope

*** Calling _PrintSomething() after disabling all levels

*** Dumping HexaDecimal values as 10 columns

==============================
HexDumper {Bytes[128]}
==============================
00 01 02 03 04 05 06 07 08 09
0A 0B 0C 0D 0E 0F 10 11 12 13
14 15 16 17 18 19 1A 1B 1C 1D
1E 1F 20 21 22 23 24 25 26 27
28 29 2A 2B 2C 2D 2E 2F 30 31
32 33 34 35 36 37 38 39 3A 3B
3C 3D 3E 3F 40 41 42 43 44 45
46 47 48 49 4A 4B 4C 4D 4E 4F
50 51 52 53 54 55 56 57 58 59
5A 5B 5C 5D 5E 5F 60 61 62 63
64 65 66 67 68 69 6A 6B 6C 6D
6E 6F 70 71 72 73 74 75 76 77
78 79 7A 7B 7C 7D 7E 7F
==============================

*** Dumping HexaDecimal values as 32 columns

================================================================================================
HexDumper {Bytes[128]}
================================================================================================
00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F
60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70 71 72 73 74 75 76 77 78 79 7A 7B 7C 7D 7E 7F
================================================================================================

** Below Logs were written into File (Filename will be like onn_ar_appmgr_20190201_154004.log in the same executable path)

*** Calling _PrintSomething() after enabling all levels

*** Calling _PrintSomething() after enabling error and warning levels

*** Calling _PrintSomething() after disabling all levels

```