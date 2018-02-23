REM rd /s /q build
cmake . -B"build" -G"Visual Studio 14" -Wno-dev --debug-output -DBOOST_ROOT:STRING="E:\common_libs\boost_1_60_0"