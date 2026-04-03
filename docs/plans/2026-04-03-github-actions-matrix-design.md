# GitHub Actions Matrix CI Design

**Date:** 2026-04-03

**Goal:** Replace the current single-platform GitHub Actions workflow and the proliferating per-case workflow files with one matrix-based workflow that runs `LottieExpressions` across Ubuntu, macOS, Windows MSVC, and Windows MSYS2 for both ThorVG subprojects and both thread modes.

## Current Context

The repository currently has multiple workflow files for `thorvg-main` and `thorvg-nors` with `threads=true` and `threads=false`, plus an older single Ubuntu workflow. This creates duplication and makes it easy for package installation, Meson flags, or run commands to drift between files.

The build also depends on platform-specific package setup:

- Ubuntu needs Meson, Ninja, SDL2 development headers, and OpenGL development headers.
- macOS needs Meson, Ninja, and SDL2 via Homebrew.
- Windows MSVC needs a Visual Studio build environment plus Meson, Ninja, and SDL2 from a dependency provider such as vcpkg.
- Windows MSYS2 needs the UCRT64 toolchain plus Meson, Ninja, and SDL2 from pacman.

## Chosen Approach

Use one workflow file with a matrix over these axes:

- `platform`: `ubuntu`, `macos`, `windows-msvc`, `windows-msys2`
- `thorvg_subproject`: `thorvg-main`, `thorvg-nors`
- `threads`: `true`, `false`

The workflow will use a base matrix and an `include` section to attach platform-specific details such as:

- `runs-on`
- shell
- dependency installation command
- build command differences
- executable path differences

This keeps the workflow declarative while still allowing each platform to use the right package manager and environment setup.

## Why This Approach

This is the smallest structure that removes duplication without hiding platform-specific behavior behind too much indirection.

Alternatives considered:

1. Separate workflow files per platform.
This keeps each file simple but repeats the same Meson flags and trigger definitions in several places.

2. A fully generic matrix without `include`.
This works for Linux and macOS but becomes awkward once Windows MSVC and Windows MSYS2 need different shells, dependency installers, and executable paths.

3. Reusable workflow plus wrapper workflows.
This is structurally clean but too heavy for the current repository size and would add indirection before the single workflow is stable.

## Workflow Structure

The consolidated workflow should:

- keep the existing `push` and `pull_request` triggers for `main`
- use `fail-fast: false` so all matrix combinations report results
- check out submodules recursively because ThorVG is stored in `subprojects`
- install dependencies per platform
- run Meson with `-Dthorvg_subproject=<value>` and `-Dthreads=<value>`
- build with Ninja or `meson compile`
- run `LottieExpressions`

## Platform Notes

### Ubuntu

- `runs-on: ubuntu-latest`
- install with `apt-get`
- packages: `meson`, `ninja-build`, `libsdl2-dev`, `libgl1-mesa-dev`

### macOS

- `runs-on: macos-latest`
- install with `brew`
- packages: `meson`, `ninja`, `sdl2`
- ensure Homebrew-installed pkg-config data is visible if Meson cannot find SDL2 automatically

### Windows MSVC

- `runs-on: windows-latest`
- initialize the MSVC developer command environment
- install Meson and Ninja
- install SDL2 with vcpkg and expose it to Meson
- run the Windows executable path with `.exe`

### Windows MSYS2

- `runs-on: windows-latest`
- use `msys2/setup-msys2`
- install UCRT64 packages with `pacman`
- build and run in the MSYS2 shell

## Risk Handling

- Set `fail-fast: false` so one platform failure does not hide the rest.
- Keep platform install logic explicit instead of trying to normalize all package managers.
- If one Windows path is initially unstable, add a temporary `exclude` entry rather than cloning the workflow into separate files again.

## Verification

Success means:

- there is one consolidated workflow file for this CI path
- the old duplicated workflow files are removed
- the matrix clearly shows platform, ThorVG subproject, and thread mode in job names
- the workflow checks out submodules recursively
- each platform has explicit package setup

