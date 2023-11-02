#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

struct result
{
    int lastLine;
    int cost;
};

struct mailCollection
{
    vector<vector<int>> costAtLocation, costToNextStation;
    vector<vector<vector<int>>> costToOtherLine;
    vector<vector<result>> results;
};

void findMailRoute(int x, mailCollection& collection);