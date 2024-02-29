#include <iostream>
#include <thread>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <random>
#define MAX_BUFFER_SIZE 10000
#define MIN_PRODUCER_VALUE 1
#define MAX_PRODUCER_VALUE 10
#define MIN_SLEEP_MS 1
#define MAX_SLEEP_MS 100

std::mutex mtx;
std::condition_variable cv;

// Initialize values to 0
int producer_sum = 0;
int consumer_sum = 0;
int producer_waits = 0;
int consumer_waits = 0;

std::random_device device;
std::mt19937 generator(device());

std::uniform_int_distribution<int> value_distributor(MIN_PRODUCER_VALUE, MAX_PRODUCER_VALUE);
std::uniform_int_distribution<int> timeout_distributor(MIN_SLEEP_MS, MAX_SLEEP_MS);

void producer(std::stack<int> &buffer) {
    for (int i = 0; i < MAX_BUFFER_SIZE; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        int value = value_distributor(generator);
        cv.wait(lock, [&buffer]{
            if(buffer.size() >= MAX_BUFFER_SIZE) {
                printf("[producerThread: %d] Producer Waiting...\n", std::this_thread::get_id());
                producer_waits ++;
            }
            return buffer.size() < MAX_BUFFER_SIZE;});

        producer_sum += value;
        buffer.push(value);

        lock.unlock();
        cv.notify_all();
        printf("[producerThread: %d] Producer notifying...\n", std::this_thread::get_id());

        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_distributor(generator)));
    }
}

void consumer(std::stack<int> &buffer) {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&buffer]{
            if (buffer.empty()) {
                printf("[producerThread: %d] Consumer Waiting...\n", std::this_thread::get_id());
                consumer_waits++;
            }
            return !buffer.empty();});

        consumer_sum += buffer.top();
        buffer.pop();

        lock.unlock();
        cv.notify_all();
        printf("[producerThread: %d] Consumer notifying...\n", std::this_thread::get_id());

        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_distributor(generator)));
    }
}

int main() {
    std::stack<int> stack;

    std::thread producer_thread1(producer, std::ref(stack));
    std::thread producer_thread2(producer, std::ref(stack));

    std::thread consumer_thread1(consumer, std::ref(stack));
    std::thread consumer_thread2(consumer, std::ref(stack));

    producer_thread1.join();
    producer_thread2.join();
    consumer_thread1.join();
    consumer_thread2.join();

    std::cout << "Producer sum: " << producer_sum << std::endl;
    std::cout << "Consumer sum: " << consumer_sum << std::endl;
    std::cout << "Producer waits: " << producer_waits << std::endl;
    std::cout << "Consumer waits: " << consumer_waits << std::endl;

    return 0;
}