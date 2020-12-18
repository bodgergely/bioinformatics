set -e
mkdir -p build
g++ -std=c++11 -Wall -Wno-char-subscripts -Wno-sign-compare -g -O2 -o build/hiddenmessage hiddenmessage.cpp rdtsc.cpp