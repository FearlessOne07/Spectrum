# Spectrum

## Cloning

- Run `git clone https://github.com/FearlessOne07/Spectrum.git --recursive`
- Run `git submodule update --remote` to get latest commits of submodules

## Building

- Make sure cmake and your desired generator are installed
- Run `cmake -S . -B build` to generate build files in the `build` directory
- Run `cmake --build build` to build the executable and library.
- Executable will be in the `bin` directory

## Credits

Made using [raylib](https://github.com/raysan5/raylib).
