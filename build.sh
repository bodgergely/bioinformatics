set -e
mkdir -p build
CC="g++"
FLAGS="-std=c++17 -g"
OPT="-O3"
WARN="-Wall -Wextra"
$CC $FLAGS $OPT $WARN -o build/hiddenmessage hiddenmessage.cpp rdtsc.cpp