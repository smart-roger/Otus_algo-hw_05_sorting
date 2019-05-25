#include <iostream>
#include <cassert>
using namespace std;
using idx_type = uint32_t;

uint64_t generate_increase(const idx_type idx, const idx_type size) {
    return idx;
}

uint64_t generate_random(const idx_type idx, const idx_type size) {
    return std::rand();
}

template <typename T, typename gen_func>
T* generate_array(const idx_type len, gen_func func) {
   T* result = new T[len];
   for (idx_type idx = 0; idx < len; ++idx) {
        result[idx] = func(idx, len);
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
void Swap(T& left, T& right) {
    T tmp = left;
    left = right;
    right = tmp;
}

template<typename T>
void insertion_sort(T* sorting_array, idx_type size) {
    assert(size<idx_max);
    for (idx_type idx = 1; idx < size; ++idx) {
        for (idx_type sorted_idx = idx; sorted_idx > 0; --sorted_idx) {
            if (sorting_array[sorted_idx] < sorting_array[sorted_idx-1])
                Swap(sorting_array[sorted_idx], sorting_array[sorted_idx-1]);
            else
                break;
        }
        print_array(sorting_array, size);
        std::cout << std::endl;
    }
}

int main()
{
    idx_type length = 10;
    uint64_t* arr = generate_array<uint64_t>(length, generate_random);

    std::cout << "Init array: " << std::endl;
    print_array(arr, length);
    std::cout << std::endl;

    insertion_sort(arr, length);

    std::cout << "Sorted array: " << std::endl;
    print_array(arr, length);
    std::cout << std::endl;

    return 0;
}
