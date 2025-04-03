#include <iostream>
#include <tuple>
#include "gameAlgo.h"

std::tuple<int, int, int> gameAlgo(int a, int b, int c) {
    if (a >= b && a >= c) {
        a = std::max(0, a - 1);
    } else if (b >= a && b >= c) {
        b = std::max(0, b - 1);
    } else {
        c = std::max(0, c - 1);
    }

    std::cout << "New integers: " << a << " " << b << " " << c << "\n";
    return std::make_tuple(a, b, c);
}

int sum(int a, int b, int c) {
    return a + b + c;
}
