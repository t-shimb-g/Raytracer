#include "random.h"
#include <iostream>

int main() {
    int N = 10000;
    double avg = 0;
    for (int i = 0; i < N; ++i) {
        avg += random(10, 20);
    }
    std::cout << avg / N << '\n';
}