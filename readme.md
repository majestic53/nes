NES
=

[![License: MIT](https://shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/majestic53/nes/blob/master/license) [![Build Status](https://github.com/majestic53/nes/workflows/Build/badge.svg)](https://github.com/majestic53/nes/actions)

A lightweight NES emulator, written in C

Features
=

```TBD```

Latest Release
=

The latest release can be found here: ```TBD```

Building from Source
=

__NOTE__: This project is tested under GNU/Linux, using the GCC C compiler. Some modifications will be required to build with a different OS/compiler.

### Preparing to build

The following libraries are required to build this project:

```
libsdl2
```

### Building

Run the following commands from the projects root directory:

```
$ export CC=<COMPILER>
$ make <BUILD>
```

|Field   |Supported values          |Description                                                 |
|:-------|:-------------------------|:-----------------------------------------------------------|
|COMPILER|```gcc```                 |Specifies the compiler to be used                           |
|BUILD   |```release```, ```debug```|Specifies the build type (defaults to relaese)              |

#### Examples

For example, to compile the NES release build using GCC, run the following commands:

```
$ export CC=gcc
$ make
```

Using the Library
=

This project is implemented in C and exposes a simple API, described in ```include/nes.h```:

|Routine    |Signature                                   |Description              |
|:----------|:-------------------------------------------|:------------------------|
|nes        |```int nes(const nes_t *)```                |Run emulator             |
|nes_error  |```const char *nes_error(void)```           |Retrieve emulator error  |
|nes_version|```const nes_version_t *nes_version(void)```|Retrieve emulator version|

For an example of how to use this interface, see the [launcher tool](https://github.com/majestic53/nes/tree/master/tool) under ```tool/```

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
