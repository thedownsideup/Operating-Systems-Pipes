#ifndef WORKER_HPP
#define WORKER_HPP

#include "libraries.hpp"
using namespace std;

class Worker{
private:
    string folder;
    vector<string> files;
    vector<string> fields;
    vector<string> results;
public:
    Worker(char* folder, char* files, char* fields);
    void search();
    void send_results();
};

#endif
