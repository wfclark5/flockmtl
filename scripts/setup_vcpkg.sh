#!/bin/bash

# Step 1: Check if the vcpkg repository is already cloned
if [ -d "vcpkg" ]; then
    echo "vcpkg repository already cloned. Skipping the cloning phase..."
else
    echo "Cloning the vcpkg repository..."
    git clone https://github.com/Microsoft/vcpkg.git

    # Check if cloning was successful
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone the vcpkg repository."
        exit 1
    fi
    echo "vcpkg repository cloned successfully."
fi

# Step 2: Bootstrap vcpkg
echo "Bootstrapping vcpkg..."
./vcpkg/bootstrap-vcpkg.sh

# Check if bootstrapping was successful
if [ $? -ne 0 ]; then
    echo "Error: Failed to bootstrap vcpkg."
    exit 1
fi
echo "vcpkg bootstrapped successfully."

# Step 3: Export the VCPKG toolchain path environment variable
export VCPKG_TOOLCHAIN_PATH=`pwd`/vcpkg/scripts/buildsystems/vcpkg.cmake
echo "VCPKG_TOOLCHAIN_PATH is set to $VCPKG_TOOLCHAIN_PATH"

echo "vcpkg setup complete."


cmake -B build/release \
  -S "/home/situ/tests/flockmtl/duckdb" \
  -DCMAKE_BUILD_TYPE=Release \
  -DDUCKDB_EXTENSION_CONFIGS="$(pwd)/extension_config.cmake" \
  -DCMAKE_TOOLCHAIN_FILE="$(pwd)/vcpkg/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_MANIFEST_DIR="$(pwd)" \
  -DEXTENSION_STATIC_BUILD=1 \
  -DVCPKG_FEATURE_FLAGS=manifest \
  -DVCPKG_MANIFEST_MODE=ON 


cmake -G "Ninja" -DFORCE_COLORED_OUTPUT=1 -DEXTENSION_STATIC_BUILD=1 -DDUCKDB_EXTENSION_CONFIGS='/home/situ/tests/flockmtl/extension_config.cmake'   -DOSX_BUILD_ARCH=   -DVCPKG_MANIFEST_DIR='/home/situ/tests/flockmtl/' -DVCPKG_BUILD=1 -DCMAKE_TOOLCHAIN_FILE='/home/situ/tests/flockmtl/vcpkg/scripts/buildsystems/vcpkg.cmake' -DDUCKDB_EXPLICIT_PLATFORM='' -DCUSTOM_LINKER= -DOVERRIDE_GIT_DESCRIBE=""  -DCMAKE_BUILD_TYPE=Debug -S "./duckdb/" -B build/debug
