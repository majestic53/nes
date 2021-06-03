NES
=

[![License: MIT](https://shields.io/badge/license-MIT-blue.svg?style=flat)]() [![Language](https://shields.io/github/languages/top/majestic53/nes?style=flat)]() [![Code Size](https://shields.io/github/languages/code-size/majestic53/nes?style=flat)]()
[![Build Status](https://shields.io/github/workflow/status/majestic53/nes/Build?style=flat)]() [![Release](https://shields.io/github/v/release/majestic53/nes?style=flat)]()

Features
=

```TBD```

Latest Release
=

The latest release can be found here: ```TBD```

Building from Source
=

__NOTE__: This project was tested under GNU/Linux, using the GCC C compiler. Some modifications to the makefiles will be required to build with a different OS/compiler.

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

|Field   |Supported values          |Note                                                        |
|:-------|:-------------------------|:-----------------------------------------------------------|
|COMPILER|```gcc```                 |                                                            |
|BUILD   |```release```, ```debug```|If no argument is specified, a release build will be created|

#### Examples

The following example shows how to compile the NES release build using GCC:

```
$ export CC=gcc
$ make
```

Using the Library
=

This project is implemented in C and exposes a simple API, described in ```include/nes.h```:

For an example of how to use this interface, see the [launcher tool](https://github.com/majestic53/nes/tree/master/tool) under ```tool/```

#### Routines

|Name       |Description              |Signature                                   |
|:----------|:------------------------|:-------------------------------------------|
|nes        |Run emulator             |```int nes(const nes_t *)```                |
|nes_error  |Retrieve emulator error  |```const char *nes_error(void)```           |
|nes_version|Retrieve emulator version|```const nes_version_t *nes_version(void)```|

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
