set -e
mkdir -p build
g++ -std=c++11 -Wall -g -O2 -o build/hiddenmessage hiddenmessage.cpp rdtsc.cpp
