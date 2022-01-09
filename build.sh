set -e
mkdir -p build
CC="g++"
FLAGS="-std=c++2a -g"
OPT="-O3"
WARN="-Wall -Wextra -Wno-sign-compare"
$CC $FLAGS $OPT $WARN -o build/hiddenmessage.exe hiddenmessage.cpp
