#include <iostream>
#include <chrono>
#include <vector>
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

    //Note: Start measuring time after assigning random values.
    auto start = chrono::steady_clock::now();
    show_result(a, result1);
    show_result(b, result2);
    show_result(c, result3);
    show_result(d, result4);
    show_result(e, result5);
    auto end = chrono::steady_clock::now();

    cout << "Sum of Vector A: " << result1 << endl;
    cout << "Sum of Vector B: " << result2 << endl;
    cout << "Sum of Vector C: " << result3 << endl;
    cout << "Sum of Vector D: " << result4 << endl;
    cout << "Sum of Vector E: " << result5 << endl;

    // note: make sure to wait joining the threads before getting the end time
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