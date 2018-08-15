# R-tree
r-tree implementation in python and c++ core.

## Getting Started
To run the project you need a Linux OS

### Prerequisites

Install make to run MakeFile if you need
```

```

### RUN C++ API
If you use MakeFile just run the next step
```
make
```
Or
```
g++ -o API_C++/apiexample2 API_C++/apiexample.cc API_C++/glovehttpserver.cpp API_C++/glove.cpp API_C++/glovewebsockets.cpp API_C++/glovecoding.cpp API_C++/glovehttpcommon.cpp API_C++/RTree/RTree.cpp API_C++/RTree/Object.cpp -lpthread -DENABLE_OPENSSL=0 -DENABLE_COMPRESSION=0 -std=c++11 -O3
```
and execute
```
./API_C++/apiexample2
```

### Open Application
Look the application on

* [Server Link](http://127.0.0.1:8080/rtree) - click to open

