#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#define ARRAY_SIZE 5

// Function to sort the array using insertion sort
void insertionSort(double arr[], int n);

// Function to find the median of an array
double findMedian(double arr[], int n);

// Function to update the FIFO array and calculate the median
double medianFilter(double fifo[], double newValue);

#endif

