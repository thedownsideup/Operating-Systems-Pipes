#include "presenter.hpp"

vector<string> split(string s){
    vector<string> splited;
    stringstream ss(s);
    string token;
    while (getline(ss, token)) {
        splited.push_back(token);
    }
    return splited;
}


vector<string> extract_from_line(string s) {
    vector<string> extracted;
    string delimiter = " - ";

    size_t  pos = 0;
    string token;
    while((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
    }
    extracted.push_back(s);
    return extracted;
}

Presenter::Presenter(char* sorting_by, char* sorting_val, char* processes){
    this->sorting_by = string(sorting_by);
    this->sorting_val = string(sorting_val);
    this->processes = atoi(processes);
}

void Presenter::present(){

    char temp[1000000];
    string result = "";
    int sorting_num;
    vector<string> splited;
    make_heap(lines.begin(), lines.end());

    int fd = open(FIFO, O_RDONLY);
    int i = 0;
    
    while(i < processes) {
       
        if (read(fd, temp, sizeof(temp))) {
            i++;
            result += string(temp);
        }
    }
    
    close(fd);

    splited = split(result);

    if(sorting_val == "NA_Sales")
        sorting_num = NA_Sales + OFFSET;
    else if(sorting_val == "EU_Sales")
        sorting_num = EU_Sales + OFFSET;
    else if(sorting_val == "JP_Sales")
        sorting_num = JP_Sales + OFFSET;
    else if(sorting_val == "Other_Sales")
        sorting_num = Other_Sales + OFFSET;
    else if(sorting_val == "Global_Sales")
        sorting_num = Global_Sales + OFFSET;

    for(int i = 0; i < splited.size(); i++){
        vector<string> extracted = extract_from_line(splited[i]);
        Line line(splited[i], sorting_num, extracted);
        lines.push_back(line);
        push_heap(lines.begin(), lines.end());
    }

    sort_heap(lines.begin(), lines.end());

    if (this->sorting_val == "descending")
        reverse(lines.begin(), lines.end());

    cout << "Results Are: \n";
    for(int i = 0; i < lines.size(); i++)
        cout << lines[i].line << endl;


}

    


int main(int argc, char *argv[]){
	Presenter presenter(argv[0], argv[1], argv[2]);
    presenter.present();
    return 0; 
}