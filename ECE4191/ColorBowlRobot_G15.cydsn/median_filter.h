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
#ifndef MEDIAN_FILTER_H
    #define MEDIAN_FILTER_H
    #define ARRAY_SIZE 5
    #include <stdio.h>

    void swap(double *a, double *b);
    int partition(double *arr, int low, int high);
    void quickSort(double *arr, int low, int high);
    double findMedian(double arr[], int n);
    double medianFilter(double fifo[], double newValue);
#endif
/* [] END OF FILE */
