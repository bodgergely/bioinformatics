set -e
mkdir -p build
CC="g++"
FLAGS="-std=c++17 -g"
OPT="-O3"
WARN="-Wall -Wextra -Wno-sign-compare"
INCLUDE_PATH="/c/users/gergely.bod/workspace/google/googletest/googletest/include/"
LIB_PATH="/c/users/gergely.bod/Libs/googletest/"
LIBS="-l:libgtest_main.a -l:libgtest.a"
$CC $FLAGS $OPT $WARN -o build/test-hiddenmessage.exe test-hiddenmessage.cpp -I$INCLUDE_PATH -L$LIB_PATH $LIBS

