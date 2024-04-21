# DbgCensus

A self-contained C++ library for the Daybreak Game Company Census API.

> [!IMPORTANT]
> This library is a work in progress (aka. hot mess) and is not really ready for anything. It is primarily replacing some Qt-infected jank from [leonhard-s/ps2-rich-presence](https://github.com/leonhard-s/ps2-rich-presence) with leaner, more testable variants that don't require a half-dozen Qt modules to build.

## Overview

- REST and WebSocket clients for the Daybreak Game Company Census API
- Uses IXWebSocket for HTTP, with optional OpenSSL support
- All requests are synchronous, choose your own poison (`std::thread`, `std::async`, `QThread`, etc.)
- \*No JSON parsing, bring your own (nlohmann/json, rapidjson, `QJsonDocument` etc.)

*\*The dependency on nlohmann/json is only used for tests and examples, the library output is `std::string`-based.*

## Dependencies

- C++ 20
- IXWebSocket: <https://github.com/machinezone/IXWebSocket>
- OpenSSL (optional): <https://github.com/openssl/openssl>
- gtest (optional, for tests): <https://github.com/google/googletest>

IXWebSocket is currently always brought in via FetchContent, everything else is provided through [vcpkg](https://vcpkg.io/). Note that vcpkg is entirely optional, this project just calls CMake's ´find_package()` to locate its dependencies.

## Building

This project uses CMake and can be consumed via `find_package()` after installation. When including in another project via `FetchContent`, it is recommended to set `DbgCensus_INSTALL=OFF` to avoid reinstalling its headers into its install tree.

```bash
mkdir build/
cmake -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/

# Optionally install
cmake --build build/ --target install
```

## Testing

This project uses Google Test driven by CMake's CTest. Tests can be run with the following command:

```bash
# After building
cd build/
ctest . -C Release # replace with the desired configuration
```
