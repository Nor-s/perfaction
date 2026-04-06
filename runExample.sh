#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

usage() {
    echo "Usage: $0 <executable_args> [feature|main|main-shared|feature-shared]"
    echo ""
    echo "Examples:"
    echo "  $0 \"LottieExample sw 0 10000\" feature"
    echo "  $0 \"LinearGradient sw 0 10000\" main-shared"
    echo ""
    echo "Build dirs:"
    echo "  feature        -> builddir-feature"
    echo "  feature-shared -> builddir-feature-shared"
    echo "  main           -> builddir-main"
    echo "  main-shared    -> builddir-main-shared"
    echo ""
    echo "Default: feature"
    exit 1
}

if [ -z "$1" ]; then
    usage
fi

ARGS=($1)
EXE_NAME="${ARGS[0]}"
EXE_ARGS="${ARGS[@]:1}"

BUILD_TYPE="${2:-feature}"

case "$BUILD_TYPE" in
    feature)        BUILD_DIR="builddir-feature" ;;
    feature-shared) BUILD_DIR="builddir-feature-shared" ;;
    main)           BUILD_DIR="builddir-main" ;;
    main-shared)    BUILD_DIR="builddir-main-shared" ;;
    *)
        echo "Error: Unknown build type '$BUILD_TYPE'"
        echo "Valid options: feature, feature-shared, main, main-shared"
        exit 1
        ;;
esac

EXE_PATH="$SCRIPT_DIR/$BUILD_DIR/src/$EXE_NAME"

if [ ! -f "$EXE_PATH" ]; then
    echo "Error: Executable not found: $EXE_PATH"
    exit 1
fi

echo "Running: $EXE_PATH $EXE_ARGS"
"$EXE_PATH" $EXE_ARGS
