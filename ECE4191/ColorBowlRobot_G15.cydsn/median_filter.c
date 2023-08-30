/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdio.h>
#include "median_filter.h"

// Function to swap two elements
void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for Quick Sort
int partition(double *arr, int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(double *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

double findMedian(double arr[], int n) {
    quickSort(arr, 0, n-1);

    if (n % 2 == 0) {
        return (arr[n / 2 - 1] + arr[n / 2]) / 2;
    } else {
        return arr[n / 2];
    }
}

double medianFilter(double fifo[], double newValue) {
    static int currentIndex = 0;
    static int isInitialized = 0;
    double tempArray[ARRAY_SIZE];

    if (!isInitialized) {
        for (int i = 0; i < ARRAY_SIZE; i++) {
            fifo[i] = newValue;
        }
        isInitialized = 1;
    }

    fifo[currentIndex] = newValue;
    currentIndex = (currentIndex + 1) % ARRAY_SIZE;

    // Make a copy of the FIFO array and calculate the median
    for (int i = 0; i < ARRAY_SIZE; i++) {
        tempArray[i] = fifo[i];
    }

    double median = findMedian(tempArray, ARRAY_SIZE);
    return median;
}

/* [] END OF FILE */
