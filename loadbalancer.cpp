#include "loadbalancer.hpp"

string segregate_sortby(map<string, string> fields){}
int segregate_processes(map<string, string> fields){}
vector<string, string> segregate_files(map<string, string> fields){}
string segregate_dir(map<string, string> fields){}

Loadbalancer::Loadbalancer(vector<string> query) {

    //Fields Map
    for (int i = 0; i < query.size(); i++)
        if(query[i] == "=")
            this->fields.insert(pair<string, string>(query[i-1], query[i+1]));
    //Number of Processes
    if (this->fields.find(PROCESSES) != this->fields.end()) {
        this->processes = stoi(this->fields.find(PROCESSES)->second);
        this->fields.erase(PROCESSES);
    }
    else {
        cerr << "Number of Processes Not Provided.\n"; 
        this->processes = 1;
    }

    //Dir
    if (this->fields.find(FOLDER) != this->fields.end()) {
        this->dir = fields.find(FOLDER)->second;
        this->fields.erase(FOLDER);
    }
    else {
       cerr << "Folder Not Provided.\n";
       exit(1);
    }
    
    //Files Per Processes
    for(int i = 0; i < this->processes; i++)
        this->files.push_back("");

    DIR *folder = opendir((string(DATASETS) + "/" + this->dir).c_str());
	struct dirent *dent;
    int i = 0;
	if (folder != NULL) {
		while ((dent = readdir(folder)) != NULL) {
			if (strcmp(dent->d_name, ".") && strcmp(dent->d_name, "..") && strcmp(dent->d_name, ".DS_Store")) {
                this->files[i].append(string(dent->d_name) + ",");
                i++;
            }
            if (i == processes)
                i = 0;
        }
    }
    else cerr << "Directory Not Found.\n";

    //Sorting Value

    for (map<string, string>::iterator it = this->fields.begin(); it != this->fields.end(); ++it) {
        if (it->second == ASCENDING || it->second == DESCENDING) {
            this->sortby = it->first; 
            this->sorting_val = it->second; 
            this->fields.erase(it);
            break;
        }
    }

    //Fields String
    this->strfields = "";
    for (map<string, string>::iterator it = this->fields.begin(); it != this->fields.end(); ++it) { 
        //strfields += it->first + ","; //we only need the second ones
        strfields += it->second + ","; 
    }      
}


void Loadbalancer::fork_workers() {

	int pipes[processes][2];
    string adr = string(DATASETS) + "/" + string(dir) + "/" ;
    char new_adr[PATHSIZE];
    char file_adr[PATHSIZE];
    char arrfields[PATHSIZE];
    int len;

    adr += '\0';
    strfields += '\0';
    
	for (int i = 0; i < processes; i++ ){
        files[i].append("\0");
        if (pipe(pipes[i]) == -1)
            cerr << "Failed To Create pipe().\n";

        int pid = fork();

        if (pid > 0) { //parent
            workers.push_back(pid);
            strcpy(new_adr, adr.c_str()); 
            strcpy(file_adr, files[i].c_str());
            strcpy(arrfields, strfields.c_str());
            close(pipes[i][0]); //close the useless end
            write(pipes[i][1], new_adr, PATHSIZE);
            write(pipes[i][1], file_adr, PATHSIZE);
            write(pipes[i][1], arrfields, PATHSIZE);
            close(pipes[i][1]); //close after used
        }
        else if (pid == 0) { //child
            char path[PATHSIZE];
            char file_path[PATHSIZE];
            char fieldsarr[PATHSIZE];
            close(pipes[i][1]);
            read(pipes[i][0], path, PATHSIZE);
            read(pipes[i][0], file_path, PATHSIZE);
            read(pipes[i][0], fieldsarr, PATHSIZE);
            close(pipes[i][0]);
            

            char* argv[] = {path, file_path, fieldsarr, NULL};
            execv("./worker" , argv);
            exit(0);
        }
        else{ //fail
        	cerr << "fork() Creation Failed.\n";
        }
    }


}

void Loadbalancer::fork_presenter() {
    int pipes[2];
    int presenter_pid;
    sortby += '\0';
    sorting_val += '\0';
 
    int pid = fork();

    if (pid > 0) { //parent
        presenter_pid = pid;
        int fd = open(FIFO, O_WRONLY); // open fifo

        if (write(fd, to_string(processes).c_str(), PATHSIZE) < 0)
            cerr << "Error writing to fifo";
        if (write(fd, sortby.c_str(), PATHSIZE < 0))
            cerr << "Error writing to fifo";
        if (write(fd, sorting_val.c_str(), PATHSIZE) < 0)
            cerr << "Error writing to fifo";

        close(fd);

    }
    else if (pid == 0) { //child
        char sortval[PATHSIZE];
        char sortb[PATHSIZE];
        char process[PATHSIZE];

        int fd = open(FIFO, O_RDONLY);
        read(fd, process, PATHSIZE);
        read(fd, sortval, PATHSIZE);
        read(fd, sortb, PATHSIZE);
        close(fd);

        char* argv[] = {sortb, sortval, process, NULL};
        execv("./presenter" , argv);
    }
    else{ //fail
        cerr << "fork() Creation Failed.\n";
    }
}



int main(){
    string query, word; 
    vector<string> words;
    int num_of_workers = 0; 

    while(1) {
        getline(cin, query);
        if(query == QUIT)
            return 0;
        if (query == "")
            continue;
        
        stringstream spliter(query); 
        while (getline(spliter, word, ' ')) { 
            words.push_back(word);
        }

        Loadbalancer loadbalancer(words);
        
        mkfifo(FIFO, 0666); // create fifo

        loadbalancer.fork_presenter();
        loadbalancer.fork_workers();
    
    }
    return 0; 
}

