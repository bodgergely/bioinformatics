set -e
mkdir -p build
TEST_DIR=$PWD/src
BUILD_DIR=$PWD/build
cd $TEST_DIR
CC="g++"
FLAGS="-std=c++17 -g"
OPT="-O3"
WARN="-Wall -Wextra -Wno-sign-compare"
INCLUDE_PATH="/c/users/gergely.bod/workspace/google/googletest/googletest/include/"
LIB_PATH="/c/users/gergely.bod/Libs/googletest/"
LIBS="-l:libgtest.a"
$CC $FLAGS $OPT $WARN -o $BUILD_DIR/test-hiddenmessage.exe $TEST_DIR/test-hiddenmessage.cpp -I$INCLUDE_PATH -L$LIB_PATH $LIBS