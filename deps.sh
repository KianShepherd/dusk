mkdir llvm-project/build
cd llvm-project/build
cmake -G Ninja -DLLVM_ENABLE_ASSERTIONS=On -DCMAKE_BUILD_TYPE=Release ../llvm
ninja
cd ../..
