#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include "libraries.hpp"
using namespace std;

class Loadbalancer{
private:
    vector<int> workers;
    map<string, string> fields;
    string dir;
    vector<string> files;
    int processes;
    string sortby; // NA_Sales or ...
    string sorting_val; //ascending or descending
    string strfields;

public:
	Loadbalancer(vector<string> query);
    void makedirs();
    void fork_workers();
	void fork_presenter();
    map<string, string> getfields();
    string getdir();


};

#endif
