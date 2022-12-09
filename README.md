![](assets/esma.gif)

## Abstract

The repository is dedicated to the legacy of the Belarusan ESMA company. To its MS DOS period at least.

A lot of old software in the world is lost even as released applications, and even more can never be recompiled because of lack of source code. But the backup CD had been burned here more than 20 years ago to resque floppy content, and now it goes on the github.

No code has been changed since 1990-s except some minor changes in order to provide suitable compiling and execution environment isolation.

## Prerequisites

Currently, you need an installed instance of Borland C++ 3.1 for MS DOS to compile source code. The preferrable location is c:\bc (it should also be included into the PATH variable value). Otherwise you should manually update each project file settings for include and library directories.

You might also need some MS DOS emulation environment. DOSBox looks to be good enough. At least it successfully was used on our side.

## Projects

Four utility libraries: *rtl\Graphics*, *rtl\Files*, *rtl\Dbf*, and *rtl\Servis*, are required to build main projects *City* in apps\MinskMap\1.0 and apps\MinskMap\2.0 folders, and project *RB_map* in apps\RBMAP folder.
You may complie any of them manually via opening and compiling projects in IDE, or using provided makefiles. If you prefer using IDE you might need to create some auxiliary folders manually according to compiler/linker error messages (since git doesn't store empty directories). See the section below for using Make utility.

## Using Make utility for project building

Make files provided are set up to allow "one click" project building including any required dependencies. Just type and execute regular Make tool command line: **make [target]**

Some of useful targets are:
* update -- creates auxiliary folders, and compiles the required libraries
* project -- builds the project in current folder
* release -- the same as project target, but with stripping application debug information
* run -- runs the application using the some environment set up command line
