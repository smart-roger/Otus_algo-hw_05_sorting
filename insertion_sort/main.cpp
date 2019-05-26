#include <iostream>
#include <cassert>
#include <ctime>
#include <chrono>
#include <random>
#include <algorithm> // for shuffle

using namespace std;
using idx_type = uint32_t;
using sort_function = void (uint64_t*, idx_type);

template<typename T>
void Swap(T& left, T& right) {
    T tmp = left;
    left = right;
    right = tmp;
}

uint64_t* generate_increase(const idx_type size) {
    uint64_t* result = new uint64_t[size];
    for(uint64_t idx = 0; idx < size; ++idx)
        result[idx] = idx;
    return result;
}

uint64_t* generate_random(const idx_type size) {
    std::mt19937 generator;
    generator.seed(time(0));
    uint64_t* result = new uint64_t[size];
    for(uint64_t idx = 0; idx < size; ++idx)
        result[idx] = generator();
    return result;
}

uint64_t* generate_shuffle_n(const idx_type size, const idx_type n) {
    uint64_t* result = generate_increase(size);
    for (idx_type counter = n-1; counter > 0; --counter) {
        idx_type shuffle_to = std::rand() % size;
        Swap(result[0], result[shuffle_to]);
    }
    return result;
}

template<typename T>
void print_array(T* arr, idx_type size) {
    for (idx_type idx = 0; idx < size; ++idx) {
        std::cout << arr[idx] << "\t";
    }
}

template<typename T>
void destroy_array(T* arr) {
    delete [] arr;
}

template<typename T>
void insertion_sort(T* sorting_array, idx_type size) {
    for (idx_type idx = 1; idx < size; ++idx) {
        for (idx_type sorted_idx = idx; sorted_idx > 0; --sorted_idx) {
            if (sorting_array[sorted_idx] < sorting_array[sorted_idx-1])
                Swap(sorting_array[sorted_idx], sorting_array[sorted_idx-1]);
            else
                break;
        }
        //print_array(sorting_array, size);
        //std::cout << std::endl;
    }
}

template <typename T, typename func>
double perf_sort(T* arr, idx_type length, func sort_func) {
    auto clock_begin = std::chrono::high_resolution_clock::now();
    sort_func(arr, length);
    auto clock_end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>  (clock_end-clock_begin);

    for (idx_type idx = 1; idx < length; ++idx) {
        if (arr[idx-1] > arr[idx]) {
            std::cout << "ERROR!" << std::endl;
        }
    }

    return elapsed.count() / 1000.0;
}

template<typename T>
void shell_sort_step(T* sorting_array, idx_type length, idx_type step) {
    for (idx_type first = 0; first < step; ++first) {
        for (idx_type sorted = first + step; sorted < length; sorted+=step) {
            for (idx_type test = sorted; test >= step; test -= step) {
                if (sorting_array[test] < sorting_array[test-step])
                    Swap(sorting_array[test], sorting_array[test-step]);
                else
                    break;
            }
        }
        //print_array(sorting_array, length);
        //std::cout << std::endl;
    }
}

template<typename T>
void shell_sort_default (T* sorting_array, idx_type length) {
    idx_type step = length;
    while (step != 1) {
        step = step/2;
        //auto clock_begin = std::chrono::high_resolution_clock::now();
        //std::cout << "Step: " << step << std::endl;
        shell_sort_step (sorting_array, length, step);
        //auto clock_end = std::chrono::high_resolution_clock::now();

        //auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>  (clock_end-clock_begin);
        //std::cout << elapsed.count()/1000.0 <<std::endl;
    }
}

template<typename T>
void shell_sort_A036562 (T* sorting_array, idx_type length) {
    idx_type shell_steps [] = {1, 3, 8, 23, 77, 281, 1073, 4193, 16577, 65921, 262913};
    int k=0;
    idx_type step = 1;
    for (k = 0; step < length; ++k) {
        step = shell_steps[k];
    }

    while (step != 1) {
        --k;
        step = shell_steps[k];
        shell_sort_step (sorting_array, length, step);
    }
}

int main()
{
    idx_type length [14] = {20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480, 40960, 81920, 163840};

    std::cout << "array_size"<<";";
    for (auto i = 0; i < 14; ++i) {
        std::cout << length[i] << ";" << std::flush;
    }
    std::cout << std::endl;

    std::cout << "Insertion sort" << std::endl;
    std::cout << "random_array" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_random(length[i]);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], insertion_sort) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_10%" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], length[i]/10);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], insertion_sort) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_5" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], 5);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], insertion_sort) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "shell sort (default)" << std::endl;
    std::cout << "random_array" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_random(length[i]);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_default) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_10%" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], length[i]/10);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_default) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_5" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], 5);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_default) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "shell sort (A036562)" << std::endl;
    std::cout << "random_array" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_random(length[i]);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_A036562) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_10%" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], length[i]/10);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_A036562) << ";" << std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    std::cout << "array_shuffle_5" << ";" << std::flush;
    for (auto i = 0; i < 14; ++i) {
        uint64_t* arr = generate_shuffle_n(length[i], 5);
        std::cout << perf_sort<uint64_t, sort_function>(arr, length[i], shell_sort_A036562) << ";"<<std::flush;
        destroy_array(arr);
    }
    std::cout << std::endl;

    return 0;
}
