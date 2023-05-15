// target_program.cpp
#include <iostream>
#include <vector>

int main() {
    int iterations = 100000;

    for (int i = 0; i < iterations; i++) {
        int a = i * 2;
        int b = a + 3;
        int c = b / 2;
        int d = c * 3;

        if (i % 10000 == 0) {
            std::cout << "Iteration: " << i << " - " << "a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << std::endl;
        }
    }

    return 0;
}

