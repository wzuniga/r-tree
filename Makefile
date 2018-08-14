main:
	g++ -o API_C++/apiexample2 API_C++/apiexample.cc API_C++/glovehttpserver.cpp API_C++/glove.cpp API_C++/glovewebsockets.cpp API_C++/glovecoding.cpp API_C++/glovehttpcommon.cpp API_C++/RTree/RTree.cpp API_C++/RTree/Object.cpp -lpthread -DENABLE_OPENSSL=0 -DENABLE_COMPRESSION=0 -std=c++11
	./API_C++/apiexample2
exec:
	./API_C++/apiexample2