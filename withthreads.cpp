#include <chrono>
#include <thread>
#include <iostream>

void print_char(const char& c, const int& number_of_times) {
    for (int i = 0; i < number_of_times; i++)
        std::cout << c;
}

int main() {
    const int NUMBER = 5000;
    const char a = 'A';
    const char b = 'B';
    const char c = 'C';

    auto start = std::chrono::steady_clock::now();
    std::thread t1(print_char, a, NUMBER);
    t1.join();

    std::thread t2(print_char, b, NUMBER);
    t2.join();

    std::thread t3(print_char, c, NUMBER);
    t3.join();

    std::cout << "END OF PROGRAM" << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    return 0;
}
