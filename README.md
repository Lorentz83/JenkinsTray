JenkinsTray
===========

This is a system tray icon to monitor the
[jenkins](https://jenkins-ci.org/) continuous integration system.

It is written using QT5 and tested on Linux, but should work on Mac
too.

**How it works**

Simply run `JenkinsTray`. A new system tray will appear on your
desktop. You can right click on it and configure the polling time and
the Jenkins URL.

You can specify on the command line the number of seconds to wait for
a system tray. It is useful if you run JenkinsTray in an autostart
script that executes before your desktop system tray is available.

```
JenkinsTray [ -w [secs] ]
```
if you do not specify the seconds, or specify 0, the program will wait
indefinitely.

**Download the binaries**

Binaries are released through the 
[project release page](https://github.com/Lorentz83/JenkinsTray/releases).

**Compile**

In a terminal type:
```
cd src
qmake
make
```
The executable will be placed into the build directory inside the project home.

To compile and execute the tests
```
cd test
qmake
make
../build/test/test
```

Optionally a Makefile is provided in the project home directory.
The targets are:
- JenkinsTray: compiles the project
- test: compiles the project and executes the tests
- install: compiles the project and copies the executable in $HOME/bin/

The default target is test.

