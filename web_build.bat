cmake -DCMAKE_TOOLCHAIN_FILE=".\modules\Emscripten.cmake" -G "Unix Makefiles" . -B "build"
emmake make -C ./build