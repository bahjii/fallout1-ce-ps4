# Fallout 1 CE PS4 build

This tree uses the OpenOrbis GitHub Actions toolchain. The PS4 package is built by `.github/workflows/ps4.yml`.

Game data is not bundled. Install your legally obtained Fallout 1 data under `/data/fallout1/` on the PS4.

The PS4 CMake layer also expects the OpenOrbis SDL2/zlib portlibs provided by the toolchain environment.
