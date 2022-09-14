set -e
mkdir -p build
SRC_DIR=$PWD/src
BUILD_DIR=$PWD/build
cd $SRC_DIR
CC="g++"
FLAGS="-std=c++17 -g"
OPT="-O3"
WARN="-Wall -Wextra -Wno-sign-compare"
$CC $FLAGS $OPT $WARN -o $BUILD_DIR/hiddenmessage.exe hiddenmessage.cpp
