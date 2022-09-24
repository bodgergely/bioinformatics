#include <cstdio>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

vector<string> generateAll(int word_length, const vector<char>& alphabet)
{
    string word;
    word.reserve(word_length);
    vector<string> result;
    auto generateNext = [&]() -> void {
        auto genHelper = [&](auto& gen_ref) -> void {
            if (word.length() == word_length) {
                result.push_back(word);
                return;
            }
            for (int i = 0; i < alphabet.size(); i++) {
                word.push_back(alphabet[i]);
                gen_ref(gen_ref);
                word.pop_back();
            }
        };
        genHelper(genHelper);
    };
    generateNext();
    return result;
}

int howManyTimesIncluded(string_view pattern, string_view text)
{
    int count = 0;
    int pos = 0;
    while (true) {
        pos = text.find(pattern, pos);
        if (pos == string::npos) {
            break;
        }
        pos = pos + 1;
        count++;
    }
    return count;
}

int main(int argc, char** argv)
{
    vector<char> alphabet{'0', '1', '2'};
    int word_length = 7;
    auto words = generateAll(word_length, alphabet);
    int total = words.size();
    cout << std::setprecision(10);
    cout << "Total generated: " << total << endl;
    const string target{"01"};
    int min_count = 2;
    int counter = 0;
    auto count =
        accumulate(words.begin(), words.end(), 0, [&](int t, const auto& str) {
            if (howManyTimesIncluded(target, str) >= min_count)
                return t + 1;
            else
                return t;
        });
    double ratio = count / (double)total;
    cout << count << "/" << total << " = " << ratio << endl;
    printf("%0.10f\n", ratio);
    return 0;
}