#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <random>

using namespace std;

struct kWithTime
{
    int value; //struct to hold k and time, used to check for best k
    float time;
};

void runWithTests();

void setIncrementAndSendToTests(ofstream& output, int incrementBy, kWithTime& bestK);

float testWithIncrements(ofstream& output, int insertionLimit);

void runOrderedAndRandomTests();

bool checkList(int currentList[]);

int* getRandomList();

void runWithoutTests(int argc, char** argv);

void merge(int* currentList, int lowerBound, int midpoint, int upperBound);

void mergeSort(int* currentList, int insertionLimit, int lowerBound, int upperBound);

void insertionSort(int* currentList, int lowerBound, int upperBound);