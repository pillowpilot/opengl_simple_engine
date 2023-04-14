Requirements
============

- pkg-config: Required by xorg/system recipe. Run: `sudo apt install pkg-config`.
- Add the following into your conan profile (~/.conan2/profiles/default):

```
[conf]
tools.system.package_manager:mode = install
tools.system.package_manager:sudo = True
 ```

 Building
 ========

 1. Run: `conan install . --output-folder=build --build=missing`, at the root directory.
 2. At `build` (`cd build/`) run: `cmake --preset conan-default` (CMake >= 3.23) or `cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release`.
 3. Immediately start the actual building process with: `cmake --build . --config Release`. The `config` value must be equal to `build_type` in the default profile.

Development
===========

- See [this](https://stackoverflow.com/questions/58077908/linking-conan-include-to-vs-code) SO thread to understand how to feed VSCode Intellisense with conan/CMake output.
- To add a library (like spdlog):
  1. First, add it to the `conanfile.py` file (`self.requires('spdlog/[>=1.4.1]')` inside the `requirements` method).
  2. Then, add it into the `CMakeLists.txt` file (add `find_package(spdlog REQUIRED)`).
  3. Finally, link it to the executable (`target_link_libraries(${target} spdlog::spdlog)`).
- If using VSCode, set the configure preset to `conan-default` to include the dependencies paths.

 Throubleshooting
 ================

- You may need to manually install `libgl-dev` and `libgl1-mesa-dev`. This solved the error: Package opengl was not found in the pkg-config path.