set -e
./build-test.sh && ./build/test-hiddenmessage

if [[ $? == 0 ]]; then
    echo "########### Tests Passed. ##################"
fi