#include <iostream>

int main() {
    const int NUMBER = 5000;
    const std::string letter1 = "A";
    const std::string letter2 = "B";
    const std::string letter3 = "C";

    for (int i = 0; i < NUMBER; i++)
        std::cout << letter1;

    for (int i = 0; i < NUMBER; i++)
        std::cout << letter2;

    for (int i = 0; i < NUMBER; i++)
        std::cout << letter3;

    return 0;
}
