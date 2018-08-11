#include "glovehttpserver.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>

class RTreeAPI
{
public:
    
    void get(GloveHttpRequest &request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        auto res = "{'1':[[[x,y],[x,y]]]}";
        response << res;
        std::cout << "GET INSERT API "<< res <<std::endl;
    }

    void post(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        //auto res = "{'1':[[[1,2],[1,2]]]}";
        auto res = "{\"item\":3}";
        response << res;
        std::cout << "POST INSERT API "<< res <<std::endl;
    }
};

class RTreeView
{
public:
    
    void get(GloveHttpRequest &request, GloveHttpResponse& response)
    {
        response.contentType("text/html");
        std::string line;
        std::string file = "view/ViewTree.html";
        if(!request.special["file"].empty())
           file = "view/js/"+request.special["file"];
        std::cout << "SEND "<< file <<std::endl;
        std::ifstream myfile (file);
        if (myfile.is_open()){
            while ( getline (myfile,line) )
                response << line << '\n';
            myfile.close();
        }
        else response << "Unable to open file"; 
    }
};

int main(int argc, char *argv[])
{
    RTreeAPI rtree;
    RTreeView view;

    GloveHttpServer serv(8080, "", 2048);
    namespace ph = std::placeholders;
    /* serv.addRoute("/films/$filmId", restFilm, 2, 1, { "GET", "POST", "PUT", "PATCH", "DELETE" }); */
    serv.addRest("/rtree", 1,
                             GloveHttpServer::jsonApiErrorCall,
                             std::bind(&RTreeView::get, &view, ph::_1, ph::_2)
    );
    serv.addRest("/rtree/insert", 1,
                             GloveHttpServer::jsonApiErrorCall,
                             std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
                             std::bind(&RTreeAPI::post, &rtree, ph::_1, ph::_2)
    );
    serv.addRest("/rtree/js/$file", 1,
                             GloveHttpServer::jsonApiErrorCall,
                             std::bind(&RTreeView::get, &view, ph::_1, ph::_2)
    );
    
    std::cout << "READY"<<std::endl;
    while(1)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "TEST"<<std::endl;

}