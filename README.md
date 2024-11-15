# fzf-folder
The purpose of this tool is to be able to fuzzy find
for folder names using fuzzy searching from the command line.

## Building

Generate the build files for either debug/release

```bash
cmake -S . -B out -DCMAKE_TOOLCHAIN_FILE=TC-clang.cmake -G Ninja -DCMAKE_BUILD_TYPE=<Debug/Release>
```

Then build it using ninja and clang

```bash
cmake --build out
```
