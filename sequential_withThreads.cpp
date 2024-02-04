#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <cstdlib>

using namespace std;

void init_rand_numbers(vector<int>&);
unsigned long long int sum_vector(vector<int>&);
unsigned long long int show_result(vector<int> v, unsigned long long int&);

int main(){
    const long SIZE = 50000000;
    vector<int> a(SIZE);
    vector<int> b(SIZE);
    vector<int> c(SIZE);
    vector<int> d(SIZE);
    vector<int> e(SIZE);
    unsigned long long int result1, result2, result3, result4, result5;

    init_rand_numbers(a);
    init_rand_numbers(b);
    init_rand_numbers(c);
    init_rand_numbers(d);
    init_rand_numbers(e);

    auto start = chrono::steady_clock::now();
    thread t1(show_result, cref(a), ref(result1));
    thread t2(show_result, cref(b), ref(result2));
    thread t3(show_result, cref(c), ref(result3));
    thread t4(show_result, cref(d), ref(result4));
    thread t5(show_result, cref(e), ref(result5));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    auto end = chrono::steady_clock::now();
    cout << "Sum of Vector A: " << result1 << endl;
    cout << "Sum of Vector B: " << result2 << endl;
    cout << "Sum of Vector C: " << result3 << endl;
    cout << "Sum of Vector D: " << result4 << endl;
    cout << "Sum of Vector E: " << result5 << endl;

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

    return 0;
}

void init_rand_numbers(vector<int>& v) {
    for(int & i : v) {
        i = rand();
    }
}

unsigned long long int sum_vector(vector<int>& array) {
    int sum = 0;
    for(auto element : array) {
        sum+=element;
    }
    return sum;
}

unsigned long long int show_result(vector<int> v, unsigned long long int& result) {
    result = sum_vector(v);
    return result;
}

