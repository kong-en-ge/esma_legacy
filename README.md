![](assets/esma.gif)

# ESMA Legacy Software Archive

## Abstract
This repository preserves legacy software developed by the Belarusan ESMA company (primarily MS‑DOS era). Many binaries and sources from that period are at risk of being lost; this archive contains original source packages (mostly untouched since the 1990s) and supporting libraries required to build and run the preserved applications.

This project aims to:
- Preserve original sources and build artifacts.
- Provide instructions to rebuild and run these applications using period-appropriate toolchains (Borland C++ 3.1 for MS‑DOS) in an emulated environment (DOSBox).
- Document what is available and known limitations.

## Table of contents
- Abstract
- Prerequisites
- Repository layout
- Building (Make)
- Building in IDE (Borland)
- Running (DOSBox)
- Common tasks and targets
- Known issues
- Contributing
- License and ownership
- Contact / Acknowledgements

## Prerequisites
- Borland C++ 3.1 for MS‑DOS (required to compile the sources). Typical tools and binaries supplied with that package:
  - bcc.exe / bcc (compiler)
  - tlink.exe (linker)
  - tasm/tlink if assembly is required
  - The preferred install path for examples below is C:\bc (or c:\bc in DOS).
  - Note: Borland C++ 3.1 is proprietary software. Obtain it from a legitimate archive or use an installer you already own.

- DOSBox (recommended for running and building under a modern OS). Tested with DOSBox 0.74 and later.

- make utility provided with the repository (Makefiles) — available on Windows via MSYS2/Cygwin/GNU Make, or run inside DOSBox if you have a DOS make.

## Repository layout (high level)
- apps/
  - MinskMap/1.0
  - MinskMap/2.0
  - RB_map/... (other apps)
- rtl/
  - Graphics/
  - Files/
  - Dbf/
  - Servis/
- assets/ (images used in this README)

The four utility libraries under rtl/ are required by the main projects. Build them first or use the provided make targets that perform builds in the correct order.

## Building with Make (recommended)
Makefiles are configured to automate building of libraries and applications.

Basic examples (run in a shell with make available, or adapt command for DOS):
- Build dependencies and auxiliary folders:
  make update
- Build the current project:
  make project
- Build a release (strip debug info):
  make release
- Run the application with an environment setup (uses DOSBox or a wrapper):
  make run

Notes:
- Run make from the project folder you want to build (e.g., apps/MinskMap/1.0).
- If you prefer the Borland IDE, see the next section.

Example: build MinskMap 1.0
1. cd apps/MinskMap/1.0
2. make update
3. make project

## Building using the Borland IDE (manual)
1. Install Borland C++ 3.1 into c:\bc (or your chosen DOS drive).
2. Copy or mount the repository into DOSBox and set the PATH to include c:\bc\bin.
3. Start the IDE and open the provided project files (.prj, .mak, etc.).
4. Ensure auxiliary folders referenced by projects exist (the make update target will create them).

## Running in DOSBox (example)
Use DOSBox autoexec or manual mount commands. Example autoexec section (dosbox.conf):

mount c /path/to/your/repo
mount d /path/to/your/borland/cd -t iso        ; if you have an ISO
c:
set PATH=c:\bc\BIN;c:\bc\MISC;%PATH%
cd \apps\MinskMap\1.0
MINSKMAP.EXE

You can add those lines to the [autoexec] section of your dosbox.conf so the environment starts pre-mounted.

## Known issues and limitations
- Source files are largely unchanged since the 1990s; some paths and assumptions are period-specific.
- Building requires proprietary toolchain (Borland C++), which may be hard to obtain or run on modern OS without emulation.
- Some parts may require manual edits or recreation of auxiliary folders before building (the make update target handles much of this).
- Assembly files depend on assemblers/linkers from the era.

## Contributing
This repository preserves historical artifacts. If you want to contribute:
- Please open an issue describing your proposed change before submitting source edits.
- For modernization patches (porting to modern toolchains), place changes on a separate branch and mark them clearly (e.g., port/modernize-*).
- If you have missing artifacts or documentation (screenshots, build logs), please submit them as separate files or PRs.

If the intent is to keep exact historical source unchanged, place any modernization efforts on separate branches and clearly document modifications.

## License and ownership
No LICENSE file is included in this repository. The original source code and binaries are historically owned by the ESMA company (Belarus). This archive preserves those historical artifacts; no license is asserted or granted here. If you are the rights holder and want to apply a license, please add a LICENSE file or contact the maintainer.

## Contact / Acknowledgements
Maintainer: kong-en-ge
This archive originated from backup CDs collected and preserved by contributors. Thank you to everyone who helped preserve these artifacts.
