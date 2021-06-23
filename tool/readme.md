NES Launcher
=

A launcher for NES

Interface
=

Launch from the project root directory:

```
$ nes [options] file
```

### Options

The following options are available:

```
-d	Enter debug mode
-f	Fullscreen display
-h	Show help information
-s	Scale display
-v	Show version information
```

#### Examples

To launch nes with a file, run the following command:

```
$ ./bin/nes <PATH_TO_FILE>
```

To launch nes in debug mode, run the following command:

```
$ ./bin/nes <PATH_TO_FILE> -d
```

To launch nes with the display fullscreen, run the following command:

```
$ ./bin/nes <PATH_TO_FILE> -f
```

Fullscreen mode can be toggled via the ```F11``` key.

To launch nes with the display scaled, run the following command:

```
$ ./bin/nes <PATH_TO_FILE> -s <SCALE>
```

Valid scaling values fall between 1-4.

### Debug commands

The following commands are available in debug mode:

```
q	Exit debug mode
h	Show help information
p	Read/Write/Show processor
r	Read data from address
c	Run processor
s	Step processor
v	Show version information
w	Write data to address
```

#### Examples

To exit debug mode, run the following command:

```
[0000] $ q
```

To show help information, run the following command:

```
[0000] $ h
```

To step the processor, run the following command:

```
[0000] $ s
```

To run the processor, run the following command:

```
[0000] $ c
```

To read data from memory at some address, run the following command:

```
[0000] $ r <ADDRESS> [<OFFSET>]
```

To write data to memory at some address, run the following command:

```
[0000] $ w <ADDRESS> <DATA>
```

To show processor information, run the following command:

```
[0000] $ p
```

To read/write processor data at some register (```pc```, ```sp```, ```s```, ```a```, ```x```, ```y```), run the following command:

```
[0000] $ p <REGISTER> [<DATA>]
```

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
