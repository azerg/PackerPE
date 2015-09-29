REM rd /s /q build
cmake . -B"build" -G"Visual Studio 14" -Wno-dev --debug-output -DBOOST_ROOT:STRING="F:\Work_Installs\boost_1_58_0"