@echo off
pushd ..
cmake -S . -B build -G "Ninja" ^
-Ddev_WARNINGS_AS_ERRORS=OFF -DCMAKE_BUILD_TYPE=Release
popd
