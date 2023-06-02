set -e
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Wno-char-subscripts -Wno-sign-compare -g -O3 -o build/hiddenmessage hiddenmessage.cpp rdtsc.cpp