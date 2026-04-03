# GitHub Actions Matrix CI Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Consolidate the current LottieExpressions CI into one matrix-based GitHub Actions workflow that runs across Ubuntu, macOS, Windows MSVC, and Windows MSYS2 for both ThorVG subprojects and both thread modes.

**Architecture:** Replace the existing per-case workflow files with a single workflow that defines a shared matrix for `platform`, `thorvg_subproject`, and `threads`, then uses matrix `include` metadata to attach platform-specific runner, shell, package install, and executable path details. Keep all platform-specific setup explicit inside the workflow so CI behavior remains easy to inspect and adjust.

**Tech Stack:** GitHub Actions, YAML, Meson, Ninja, Homebrew, apt, MSVC, MSYS2, vcpkg

---

### Task 1: Inspect the existing workflow files and preserve their intent

**Files:**
- Modify: `.github/workflows/build_test.yml`
- Modify: `.github/workflows/LottieExpressions-main-thread-false.yml`
- Modify: `.github/workflows/LottieExpressions-main-thread-true.yml`
- Modify: `.github/workflows/LottieExpressions-nors-thread-false.yml`
- Modify: `.github/workflows/LottieExpressions-nors-thread-true .yml`

**Step 1: Review the current workflow set**

Run: `Get-ChildItem '.github/workflows' | Select-Object Name`
Expected: a short list showing the current duplicated workflow files.

**Step 2: Capture the current Meson invocation pattern**

Run: `rg -n "meson setup build|LottieExpressions" .github/workflows`
Expected: matching lines showing the existing `thorvg_subproject` and `threads` combinations.

**Step 3: Define the canonical matrix axes**

Write down this exact set for the new workflow:

```text
platform: ubuntu, macos, windows-msvc, windows-msys2
thorvg_subproject: thorvg-main, thorvg-nors
threads: true, false
```

**Step 4: Commit**

```bash
git add docs/plans/2026-04-03-github-actions-matrix-design.md docs/plans/2026-04-03-github-actions-matrix.md
git commit -m "docs: add GitHub Actions matrix CI design and plan"
```

### Task 2: Replace duplicated workflows with a single matrix workflow

**Files:**
- Create: `.github/workflows/lottie-expressions-matrix.yml`
- Delete: `.github/workflows/build_test.yml`
- Delete: `.github/workflows/LottieExpressions-main-thread-false.yml`
- Delete: `.github/workflows/LottieExpressions-main-thread-true.yml`
- Delete: `.github/workflows/LottieExpressions-nors-thread-false.yml`
- Delete: `.github/workflows/LottieExpressions-nors-thread-true .yml`

**Step 1: Write the new workflow skeleton**

Create a workflow with:

```yaml
name: LottieExpressions Matrix

on:
  pull_request:
    branches: [main]
  push:
    branches: [main]
```

**Step 2: Add matrix strategy**

Define a matrix with:

```yaml
strategy:
  fail-fast: false
  matrix:
    platform: [ubuntu, macos, windows-msvc, windows-msys2]
    thorvg_subproject: [thorvg-main, thorvg-nors]
    threads: [true, false]
```

**Step 3: Add platform metadata through `include`**

Attach at least these values per platform:

```yaml
runs_on
shell
artifact_os_name
binary_path
```

Also attach platform-specific install steps or command fragments where needed.

**Step 4: Remove the duplicated workflows**

Delete the old workflow files once the new file contains all supported combinations.

**Step 5: Commit**

```bash
git add .github/workflows
git commit -m "ci: consolidate LottieExpressions workflows into matrix"
```

### Task 3: Add recursive submodule checkout and platform package setup

**Files:**
- Modify: `.github/workflows/lottie-expressions-matrix.yml`

**Step 1: Add recursive checkout**

Include:

```yaml
- uses: actions/checkout@v4
  with:
    submodules: recursive
```

**Step 2: Add Ubuntu package installation**

Use:

```yaml
sudo apt-get update
sudo apt-get install -y meson ninja-build libsdl2-dev libgl1-mesa-dev
```

**Step 3: Add macOS package installation**

Use:

```yaml
brew install meson ninja sdl2
```

If needed, add a `PKG_CONFIG_PATH` export for Homebrew.

**Step 4: Add Windows MSVC environment and dependencies**

Use an MSVC developer command action, install Meson and Ninja, install SDL2 with vcpkg, and make the dependency visible to Meson.

Expected result: the workflow contains an explicit MSVC path instead of trying to reuse Linux package commands.

**Step 5: Add Windows MSYS2 environment and dependencies**

Use `msys2/setup-msys2@v2` and install:

```text
mingw-w64-ucrt-x86_64-gcc
mingw-w64-ucrt-x86_64-meson
mingw-w64-ucrt-x86_64-ninja
mingw-w64-ucrt-x86_64-SDL2
```

**Step 6: Commit**

```bash
git add .github/workflows/lottie-expressions-matrix.yml
git commit -m "ci: add platform-specific package setup to matrix workflow"
```

### Task 4: Add build and run commands for every matrix combination

**Files:**
- Modify: `.github/workflows/lottie-expressions-matrix.yml`

**Step 1: Add the Meson setup command**

Use the matrix variables directly:

```yaml
meson setup build -Dthorvg_subproject=${{ matrix.thorvg_subproject }} -Dthreads=${{ matrix.threads }}
```

Adjust shell syntax for PowerShell or MSYS2 where required.

**Step 2: Add the compile step**

Use one of:

```yaml
meson compile -C build
```

or:

```yaml
ninja -C build
```

**Step 3: Add the executable run step**

Use the correct binary path per platform:

- POSIX: `./build/src/LottieExpressions sw 0`
- Windows native: `.\build\src\LottieExpressions.exe sw 0`
- MSYS2: a valid MSYS2 path to the generated executable

**Step 4: Make the matrix job name readable**

Set the job name to show platform, subproject, and thread mode.

Expected: GitHub Actions UI clearly identifies each of the 16 combinations.

**Step 5: Commit**

```bash
git add .github/workflows/lottie-expressions-matrix.yml
git commit -m "ci: add build and run steps for all matrix variants"
```

### Task 5: Verify the workflow definition before pushing

**Files:**
- Test: `.github/workflows/lottie-expressions-matrix.yml`

**Step 1: Review the final YAML**

Run: `Get-Content -Raw '.github/workflows/lottie-expressions-matrix.yml'`
Expected: one consolidated workflow containing the matrix, platform setup, build, and run steps.

**Step 2: Confirm no duplicate workflow files remain**

Run: `Get-ChildItem '.github/workflows' | Select-Object Name`
Expected: only the consolidated workflow remains for this CI path.

**Step 3: Inspect the working tree**

Run: `git status --short`
Expected: the old workflows are deleted, the new workflow is added, and the docs are present.

**Step 4: Commit**

```bash
git add .github/workflows docs/plans
git commit -m "ci: finalize matrix workflow migration"
```
