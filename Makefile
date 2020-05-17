all : loadbalancer.o worker.o presenter.o

loadbalancer.o:loadbalancer.cpp 
	g++ -std=c++11 loadbalancer.cpp -o load
worker.o:worker.cpp 
	g++ -std=c++11 worker.cpp -o worker
presenter.o:presenter.cpp 
	g++ -std=c++11 presenter.cpp -o presenter