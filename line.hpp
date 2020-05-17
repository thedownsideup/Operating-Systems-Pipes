#ifndef LINE_HPP
#define LINE_HPP

#include "libraries.hpp"
using namespace std;

class Line{
public:
    string line;
    int sortval;
    int sortby;
    vector<string> fields;

    Line(string line, int sortby, vector<string> fields){
        this->line = line;
        this->sortby = sortby;
        this->fields = fields;     
        this->sortval = stoi(fields[sortby]);
    }
    bool operator< (const Line &other)const{
        return sortval < other.sortval; 
    }
    
};

#endif
