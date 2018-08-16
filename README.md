# R-tree
r-tree implementation in c++ core for Linux.

## Getting Started
To run the project you need a Linux OS

### Prerequisites

Install make to run MakeFile if you need

### CLONE
Clone repository
* [Rtree repository](https://github.com/wzuniga/r-tree.git) link 'https://github.com/wzuniga/r-tree.git'

Enter to folder r-tree
```
 cd r-tree
```

### COMPILE C++ API
If you use MakeFile just run the next step on folder r-tree
```
make
```
Or
```
g++ -o API_C++/apiexample2 API_C++/apiexample.cc API_C++/glovehttpserver.cpp API_C++/glove.cpp API_C++/glovewebsockets.cpp API_C++/glovecoding.cpp API_C++/glovehttpcommon.cpp API_C++/RTree/RTree.cpp API_C++/RTree/Object.cpp -lpthread -DENABLE_OPENSSL=0 -DENABLE_COMPRESSION=0 -std=c++11 -O3
```

### EXCUTE C++ API
Run comand
```
./API_C++/apiexample2
```
Or
```
make exec
```

### Open Application
Look the application on Local Server

* [Server Link](http://127.0.0.1:8080/rtree) - click to open 'http://127.0.0.1:8080/rtree'


## The Aplication is also running in Cloud

* [Server Link](http://212.8.251.77:8080/rtree) - click to open 'http://212.8.251.77:8080/rtree'