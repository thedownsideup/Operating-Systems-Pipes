#include "worker.hpp"

vector<string> split(string s){
    vector<string> splited;
    stringstream ss(s);
    string token;
    while (getline(ss, token, ',')) {
        splited.push_back(token);
    }
    return splited;
}

Worker::Worker(char* cfolder, char* cfiles, char* cfields){
    string strfiles, strfields;
    strfiles = string(cfiles);
    strfields = string(cfields);

    this->folder = string(cfolder);
    this->fields = split(strfields);

    this->files = split(strfiles);
}

void Worker::search(){
    size_t pos;
    string line;
    int found;


    for (int i = 0; i < files.size(); i++) {

        ifstream input;
        input.open(folder + files[i]);
    
        if (input.is_open()) {
            
            while (getline(input, line)) {


                for (int j = 0; j < fields.size(); j++) {

                
                    pos = line.find(fields[j]);

                    if (pos == string::npos) { //string::npos is returned if string is not found 
                        found = 0;
                        break;
                    }
                    found = 1;
                }
                if (found || fields.size() == 0) {
                    results.push_back(line);
                }
            }
        }
        else cout << "file not found\n";
    }

    
}

void Worker::send_results(){
    string result = "";
    for(int i = 0; i < results.size(); i++){
        result += results[i] + "\n";
    }
    
	int fd = open(FIFO, O_WRONLY);
    write(fd, result.c_str(), 1000000);
    close(fd);
}

int main(int argc, char *argv[]){

	Worker worker(argv[0], argv[1], argv[2]); //folder path, file names, array of fields
	worker.search();
	worker.send_results();

    return 0; 
}