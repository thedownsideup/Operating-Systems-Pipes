#ifndef LIBRARIES_HPP
#define LIBRARIES_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <stdlib.h> 
#include <errno.h>
#include <sstream>
#include <map>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<bits/stdc++.h>  

#define PROCESSES "processes"
#define DATASETS "Datasets"
#define BACKSLASH "/"
#define DATASET "dataset"
#define FOLDER "dir"
#define DESCENDING "descending"
#define ASCENDING "ascending"
#define FIFO "presenterfifo"
#define PATHSIZE 512
#define MODE 0666

enum Sort {NA_Sales, EU_Sales, JP_Sales, Other_Sales, Global_Sales};
#define OFFSET 6

#define QUIT "quit"

#endif
