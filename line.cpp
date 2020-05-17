#include "presenter.hpp"


int main(int argc, char *argv[]){

	Worker worker(argv[0], argv[1], argv[2]); //folder path, file names, array of fields
	worker.search();
	worker.send_results();

    return 0; 
}