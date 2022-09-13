set -e
BUILD_DIR=$PWD/build
./build-test.sh && $BUILD_DIR/test-hiddenmessage

if [[ $? == 0 ]]; then
    echo "########### Tests Passed. ##################"
fi