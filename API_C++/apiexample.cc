#include "glovehttpserver.hpp"
#include "json.hpp"
#include "RTree/RTree.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>

RTree MyR_tree(3);

class RTreeAPI
{
public:
    
    void get(GloveHttpRequest &request, GloveHttpResponse& response){}

    void postInsert(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        //auto res = "{'1':[[[1,2],[1,2]]]}";
        auto jsonInput = nlohmann::json::parse(request.getData());
        auto data = jsonInput["data"];
        //auto res = "[{\"level\":0,\"is_leaf\":0,\"elements\":[[100,200],[300,400]]},{\"level\":1,\"is_leaf\":0,\"elements\":[[10,20],[30,40]]},{\"level\":2,\"is_leaf\":1,\"elements\":[[100,200],[100,200],[6,7]]},{\"level\":1,\"is_leaf\":0,\"elements\":[[50,80],[90,150]]},{\"level\":0,\"is_leaf\":0,\"elements\":[[15,25],[35,45]]},{\"level\":2,\"is_leaf\":1,\"elements\":[[524,345],[413,389],[469,492],[561,477],[585,411]]}]";
        std::vector<Point> R_1;
        for(int i= 0; i < data.size(); i++){
            R_1.push_back(Point(data[i]["x"],data[i]["y"]));
            std::cout <<"---"<<std::endl;
            std::cout <<R_1[i].get_X()<<" " <<R_1[i].get_Y()<<std::endl;
            //std::cout <<data[i]["y"] <<std::endl;
        }
        Polygon * p = new Polygon(R_1,1);
        Polygon * r = new Polygon(p->get_Pmin(),p->get_Pmax());
        MyR_tree.insert_polygon(p,r);
        auto res =  MyR_tree.show_values_JSON();
        response << res;
        std::cout << "POST INSERT API x="<<data<<std::endl;
    }

    void postNearest(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        //auto res = "{'1':[[[1,2],[1,2]]]}";
        auto jsonInput = nlohmann::json::parse(request.getData());
        auto x = jsonInput["x"];
        auto y = jsonInput["y"];
        auto k = jsonInput["k"];
        std::vector<d_leaf*> ans_knn;
        MyR_tree.k_NN_DF(Point(x,y),k, ans_knn);
        std::string res="";
        MyR_tree.get_polygons_JSON( ans_knn,res);
        response << res;
        std::cout << "POST NEAREST API x="<<x<<" y="<<y<<" k="<<k<<std::endl;
    }

    void postRange(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");

        auto jsonInput = nlohmann::json::parse(request.getData());
        auto p1 = jsonInput["point1"];
        auto p2 = jsonInput["point2"];

        std::vector<data_query_return > answ;
        Polygon query(Point(p1["x"],p1["y"]),Point(p2["x"],p2["y"]));
        std::cout<<query.get_Pmin().get_X()<<"," <<query.get_Pmin().get_Y()<<std::endl;
        std::cout<<query.get_Pmax().get_X()<<"," <<query.get_Pmax().get_Y()<<std::endl;
        MyR_tree.range_search(query,answ);
        std::cout<<"....."<<std::endl;
        std::cout<<answ.size();
        for(int i = 0; i < answ.size(); i++){
            std::cout<<answ[i].Pol->corners<<" ";
        }
        std::string res = "";
        MyR_tree.get_Range_Search_JSON(answ, res);
        response << res;
        std::cout << "POST Range API"<<std::endl;
    }

};

class RTreeView
{
public:
    
    void get(GloveHttpRequest &request, GloveHttpResponse& response)
    {
        response.contentType("text/html");
        std::string line;
        std::string file = "API_C++/view/ViewTree.html";
        if(!request.special["file"].empty())
           file = "API_C++/view/js/"+request.special["file"];
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

    serv.addRest("/rtree", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeView::get, &view, ph::_1, ph::_2)
    );

    serv.addRest("/rtree/js/$file", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeView::get, &view, ph::_1, ph::_2)
    );

    serv.addRest("/rtree/insert", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postInsert, &rtree, ph::_1, ph::_2)
    );

    serv.addRest("/rtree/nearest", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postNearest, &rtree, ph::_1, ph::_2)
    );
    
    serv.addRest("/rtree/range", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postRange, &rtree, ph::_1, ph::_2)
    );

    std::cout << "READY"<<std::endl;

    while(1)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "TEST"<<std::endl;

}