#include <future>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

bool make_thread = true;

void merge(int* arr, int l, int m, int r)
{
    int i, j, k;
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    int* left = new int[nl] {};
    int* right = new int[nr]{};

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j])
        {
            arr[k] = left[i++];
        }
        else
        {
            arr[k] = right[j++];
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl)
    {
        arr[k++] = left[i++];
    }
    // записываем оставшиеся элементы правой части
    while (j < nr)
    {
        arr[k++] = right[j++];
    }
    delete[] left;
    delete[] right;
}

void mergeSortThreads(int* arr, int l, int r )
{ 
    int m;
    if (l < r)
    {
        m = l + (r - l) / 2;
        future<void> f1 = std::async(std::launch::deferred, [&]() 
            {
                mergeSortThreads(arr, l, m);
            });
        future<void> f2 = std::async(std::launch::deferred, [&]() 
            {
                mergeSortThreads(arr, m + 1, r);
            });

        f1.get();
        f2.get();
        merge(arr, l, m, r);
    }
}

void mergeSort(int* arr, int l, int r)
{
    if (l >= r)
        return;
    {
        int m = (l + r - 1) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main()
{
    const int threads = 2;
    srand(0);
    long arr_size = 1000000;
    int* array = new int[arr_size];
    for (long i = 0; i < arr_size; i++)
    {
        array[i] = rand() % 50000;
    }
    double seconds;
    time_t start, end;
    // многопоточный запуск
    time(&start);
    mergeSortThreads(array, 0, arr_size-1);
    time(&end);
    seconds = difftime(end, start);
    cout<<"multi threads the time:  "<< seconds<<" seconds"<<endl;
    // проверка
    for (long i = 0; i < arr_size - 2; i++)
    {
       if (array[i] > array[i + 1]) 
        {
            cout << "Unsorted" << endl;
            break;
        }
    }
    // однопоточный запуск
    for (long i = 0; i < arr_size; i++)
    {
        array[i] = rand() % 50000;
    }
    time(&start);
    mergeSort(array, 0, arr_size - 1);
    time(&end);
    seconds = difftime(end, start);
    cout << "One thread the time:  " << seconds << " seconds" << endl;
    // проверка
    for (long i = 0; i < arr_size - 1; i++)
    {
        if (array[i] > array[i + 1])
        {
            cout << "Unsorted" << endl;
            break;
        }
    }

    delete[] array;
}
