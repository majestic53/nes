NES
=

[![License: MIT](https://shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/majestic53/nes/blob/master/license) [![Build Status](https://github.com/majestic53/nes/workflows/Build/badge.svg)](https://github.com/majestic53/nes/actions)

A lightweight NES emulator, written in C

Features
=

This project is a work-in-progress and is missing a lot of functionality. See the table below for the current implementation status:

|Subsystem|Complete          |Description                          |
|:--------|:-----------------|:------------------------------------|
|Audio    |                  |Still to be started                  |
|Input    |                  |Still to be started                  |
|Processor|:heavy_check_mark:|Support for all official 6502 opcodes|
|Video    |                  |Still to be started                  |

Latest Release
=

The latest release can be found here: ```TBD```

Building from Source
=

This project is tested under GNU/Linux, using the GCC C compiler. Some modifications will be required to build with a different OS/compiler.

### Preparing to build

The following libraries are required to build this project:

```
libreadline
libsdl2
```

### Building

Run the following commands from the projects root directory:

```
$ export CC=<COMPILER>
$ make [<BUILD>] [LEVLE=<LEVEL>]
```

|Field   |Supported values                           |Description                                                                                                  |
|:-------|:------------------------------------------|:------------------------------------------------------------------------------------------------------------|
|COMPILER|```gcc```                                  |Specifies the compiler to be used                                                                            |
|BUILD   |```debug```, ```release```                 |Optionally specifies the build type (defaults to release)                                          |
|LEVEL   |```0```, ```1```, ```2```, ```3```, ```4```|Optionally specifies the debug tracing level (0=None, 1=Error, 2=Warning, 3=Information, 4=Verbose)|

If the build succeeds, the binary files can be found under ```bin/```

#### Examples

To compile the NES release build using GCC, run the following commands:

```
$ export CC=gcc
$ make release
```

To compile the NES debug build using GCC, with verbose tracing enabled, run the following commands:

```
$ export CC=gcc
$ make debug LEVEL=4
```

Using the Library
=

This project is implemented in C and exposes a simple API, described in ```include/nes.h```:

### Routines

|Name       |Signature                                                 |Description               |
|:----------|:---------------------------------------------------------|:-------------------------|
|nes_load   |```int nes_load(const nes_t *)```                         |Load emulator             |
|nes_unload |```int nes_unload(void)```                                |Unload emulator           |
|nes_action |```int nes_action(const nes_action_t *, nes_action_t *)```|Run NES action in emulator|
|nes_error  |```const char *nes_error(void)```                         |Retrieve emulator error   |
|nes_version|```const nes_version_t *nes_version(void)```              |Retrieve emulator version |

### Actions

|Name                       |Input/Output    |Description|
|:--------------------------|:---------------|:-----------------------|
|NES_ACTION_RUN             |Request         |Run emulator            |
|NES_ACTION_STEP            |Request         |Step emulator           |
|NES_ACTION_BUS_READ        |Request/Response|Read byte from bus      |
|NES_ACTION_BUS_WRITE       |Request         |Write byte to bus       |
|NES_ACTION_PROCESSOR_READ  |Request         |Read processor register |
|NES_ACTION_PROCESSOR_WRITE |Request/Response|Write processor register|
|NES_ACTION_CARTRIDGE_HEADER|Request/Response|Read cartridge header   |

For an example of how to use this interface, see the [launcher](https://github.com/majestic53/nes/tree/master/tool) under ```tool/```

Trademark
=

Nintendo name/logo are trademarks of Nintendo Co., Ltd.

License
=

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
