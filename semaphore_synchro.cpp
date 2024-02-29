#include <iostream>
#include <thread>
#include <stack>
#include <mutex>
#include <semaphore>
#include <random>
#define MAX_BUFFER_SIZE 10000
#define MIN_PRODUCER_VALUE 1
#define MAX_PRODUCER_VALUE 10
#define MIN_SLEEP_MS 1
#define MAX_SLEEP_MS 100

std::mutex mtx;
std::counting_semaphore<MAX_BUFFER_SIZE> producer_semaphore(MAX_BUFFER_SIZE);
std::counting_semaphore<MAX_BUFFER_SIZE> consumer_semaphore(0);

int producer_sum = 0;
int consumer_sum = 0;
int producer_waits = 0;
int consumer_waits = 0;

int producer_counts = 0;

std::random_device device;
std::mt19937 generator(device());
std::uniform_int_distribution<int> value_distributor(MIN_PRODUCER_VALUE, MAX_PRODUCER_VALUE);
std::uniform_int_distribution<int> timeout_distributor(MIN_SLEEP_MS, MAX_SLEEP_MS);

void producer(std::stack<int> &buffer) {
    for (int i = 0; i < MAX_BUFFER_SIZE; ++i) {
        producer_semaphore.acquire();
        printf("[producerThread %d] Producer receiving signal...\n", std::this_thread::get_id());

        std::unique_lock<std::mutex> lock(mtx);

        int value = value_distributor(generator);
        producer_sum += value;
        buffer.push(value);

        if(buffer.size() == MAX_BUFFER_SIZE) {
            printf("[producerThread: %d] Producer Waiting...\n", std::this_thread::get_id());
            producer_waits++;
        }

        lock.unlock();
        printf("[producerThread %d] Producer sending signal...\n", std::this_thread::get_id());
        consumer_semaphore.release();
//        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_distributor(generator)));
    }
    std::lock_guard<std::mutex>lock(mtx);
    printf("[producerThread %d] Producer task completed\n", std::this_thread::get_id());
    ++producer_counts;
    consumer_semaphore.release();
}

void consumer(std::stack<int> &buffer) {
    while (true) {
        consumer_semaphore.acquire();
        printf("[consumerThread %d] Consumer receiving signal...\n", std::this_thread::get_id());

        std::unique_lock<std::mutex> lock(mtx);

        if(!buffer.empty()) {
            consumer_sum += buffer.top();
            buffer.pop();
        } else if (producer_counts == MAX_BUFFER_SIZE) {
            break;
        } else {
            printf("[producerThread: %d] Consumer Waiting...\n", std::this_thread::get_id());
            consumer_waits++;
        }

        lock.unlock();
        printf("[consumerThread %d] Consumer receiving signal...\n", std::this_thread::get_id());
        producer_semaphore.release();
//        std::this_thread::sleep_for(std::chrono::milliseconds(timeout_distributor(generator)));
    }
}

int main() {
    std::stack<int> stack;
    std::thread producers[2];
    std::thread consumers[2];

    for (auto& i : producers) {
        i = std::thread(producer, std::ref(stack));
    }

    for (auto& i : consumers) {
        i = std::thread(consumer, std::ref(stack));
    }

    for (auto& producer : producers) {
        producer.join();
    }

    for (auto& consumer : consumers) {
        consumer.join();
    }

    std::cout << "Producer sum: " << producer_sum << std::endl;
    std::cout << "Consumer sum: " << consumer_sum << std::endl;
    std::cout << "Producer waits: " << producer_waits << std::endl;
    std::cout << "Consumer waits: " << consumer_waits << std::endl;

    return 0;
}