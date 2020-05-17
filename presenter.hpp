#ifndef PRESENTER_HPP
#define PRESENTER_HPP

#include "libraries.hpp"
#include "line.hpp"

using namespace std;

class Presenter{
private:
    string sorting_val; //NA_Sales or ...
    string sorting_by; //ascending or descending
    int processes;
    vector<Line> lines;
public:
    Presenter(char* sorting_by, char* sorting_val, char* processes);
    void present();
};

#endif
