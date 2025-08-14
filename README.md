# DietApp
Desktop application to calculate calories on given diet from Taco (Diet Composition Table)

## Dependencies

### Essentials
- **wxWidgets** 3.2+ (GTK3 support)
- **CMake** 3.1+
- **C++ Compiler** (C++ 11 standard)

### System dependencies (Linux)
Install via apt:
```bash
sudo apt-get install \
    libwxgtk3.2-dev \
    cmake \
    build-essential \
    pkg-config \
    libgtk-3-dev
```

## Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
