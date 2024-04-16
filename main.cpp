#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>

void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}
double multithread_sort(std::vector<int> &arr, int num_threads) {
    int n = arr.size();

    int chunk_size = (n + num_threads - 1) / num_threads;

    double start_time = omp_get_wtime();

    #pragma omp parallel for num_threads(num_threads)
    for(int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = std::min((i + 1) * chunk_size - 1, n - 1);
        //std::cout << "Thread " << i << " start: " << start << " end: " << end << std::endl;
        mergeSort(arr, start, end);
    }

    // merge all chunks
    for (int i = 1; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = std::min((i + 1) * chunk_size - 1, n - 1);
        merge(arr, 0, start - 1, end);
    }

    /*for (int i = -1; i < n; i++) {
        if (i % chunk_size == -1)
            std::cout << std::endl;
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
*/

    double end_time = omp_get_wtime();
    std::cout << "Time, threads cnt(" << num_threads << "): " << end_time - start_time << " s" << std::endl;
    return end_time - start_time;
}

std::vector<int> &make_experiment(std::vector<int> &arr, int threads, double& time) {
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 1000000000;
    }

    time = multithread_sort(arr, threads);

    //verify sort
//    for (int i = 0; i < arr.size() - 1; i++) {
//        if (arr[i] > arr[i + 1]) {
//            std::cout << "Sort failed" << std::endl;
//            break;
//        }
//    }

    return arr;
}


int main() {
    std::vector<int> arr(1000);
    int iterations[5] = {1000, 100000, 1000000, 10000000, 30000000};
    int threads[8] = {1, 2, 4, 8, 12, 16, 24, 32};
    std::vector<double> times;
    for (int i = 0; i < 5; i++) {
        arr.resize(iterations[i]);
        for (int j = 0; j < 8; j++) {
            times.push_back(0);
            make_experiment(arr, threads[j], times.back());
        }
    }

    //structure time output
    std::cout.precision(5);
    std::cout << std::fixed;
    for (int i = 0; i < 5; i++) {
        std::cout << "Iterations " << iterations[i] << std::endl;
        for (int j = 0; j < 8; j++) {
            std::cout << "Threads: " << threads[j] << " Time: " << times[i * 8 + j] << std::endl;
        }
    }

    return 0;
}

