set -e
mkdir -p build
SRC_DIR=$PWD/src
BUILD_DIR=$PWD/build
cd $SRC_DIR
CC="g++"
FLAGS="-std=c++2a -g"
OPT="-O3"
WARN="-Wall -Wextra -Werror -Wpedantic -Wno-sign-compare"
$CC $FLAGS $OPT $WARN -o $BUILD_DIR/hiddenmessage.exe hiddenmessage.cpp
