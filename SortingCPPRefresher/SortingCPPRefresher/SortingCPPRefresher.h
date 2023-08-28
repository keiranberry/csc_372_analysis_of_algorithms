#pragma once
#include <iostream>
#include <string>

using namespace std;

void merge(int* currentList, int lowerBound, int midpoint, int upperBound);

void mergeSort(int* currentList, int insertionLimit, int lowerBound, int upperBound);

void insertionSort(int* currentList, int lowerBound, int upperBound);