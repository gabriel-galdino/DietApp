# DietApp

DietApp is a native Linux desktop application for calculating meal calories and
nutrition from the Brazilian TACO food composition spreadsheet. It uses
wxWidgets/GTK for the UI, wxSQLite3/SQLite for local data, OpenXLSX for reading
the spreadsheet, and Catch2 for presentation tests.

## Platform

The project is currently built and tested on Linux. The CI workflow uses
Ubuntu 24.04.

## Dependencies

Required system packages on Ubuntu 24.04:

```bash
sudo apt update
sudo apt install -y \
  libwxgtk3.2-dev \
  libwxgtk3.2-1t64 \
  libwxbase3.2-1t64 \
  libwxsqlite3-3.2-0t64 \
  libwxsqlite3-3.2-dev \
  libgtk-3-dev \
  cmake \
  catch2 \
  build-essential \
  pkg-config \
  xvfb
```

Version requirements:

- wxWidgets 3.2.4 or newer
- CMake 3.5 or newer
- A C++17 compiler
- wxSQLite3 3.2
- Catch2 with `catch2-with-main` pkg-config metadata

Check the detected wxWidgets version with:

```bash
wx-config --version-full
```

## Clone

OpenXLSX is a git submodule, so clone with submodules or initialize them after
checkout:

```bash
git clone --recurse-submodules <repo-url>
cd DietApp
```

For an existing checkout:

```bash
git submodule update --init --recursive
```

## Build

Configure and build out of tree:

```bash
cmake -B build -DBUILD_TESTING=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

The application binary is generated at:

```bash
./build/bin/dietapp
```

## Tests

The presentation tests use wxWidgets/GTK UI simulation and require a display.
In a headless environment, run them with Xvfb:

```bash
xvfb-run --auto-servernum -- ./build/test/presentation_test
```

On a machine with a working desktop session, the binary can also be run
directly:

```bash
./build/test/presentation_test
```
