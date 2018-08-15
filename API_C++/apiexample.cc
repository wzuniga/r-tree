#include "glovehttpserver.hpp"
#include "json.hpp"
#include "RTree/RTree.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>

RTree * MyR_tree = new RTree(4);

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
        }
        Polygon * p = new Polygon(R_1);
        Polygon * r = new Polygon(p->get_Pmin(),p->get_Pmax());
        MyR_tree->insert_polygon(p,r);
        auto res =  MyR_tree->show_values_JSON();
        response << res;
        std::cout << "POST INSERT API x="<<data<<std::endl;
    }

    void postNearest(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        auto jsonInput = nlohmann::json::parse(request.getData());
        auto x = jsonInput["x"];
        auto y = jsonInput["y"];
        auto k = jsonInput["k"];
        std::vector<d_leaf*> ans_knn;
        MyR_tree->k_NN_DF(Point(x,y),k, ans_knn);
        std::string res="";
        MyR_tree->get_polygons_JSON( ans_knn,res);
        response << res;
        std::cout << "POST NEAREST API x="<<x<<" y="<<y<<" k="<<k<<std::endl;
    }

    void postRange(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");

        auto jsonInput = nlohmann::json::parse(request.getData());
        auto p1 = jsonInput["point1"];
        auto p2 = jsonInput["point2"];

        std::vector<Polygon * > answ;
        Polygon query(Point(p1["x"],p1["y"]),Point(p2["x"],p2["y"]));
        MyR_tree->range_search(query,answ);
        std::string res = "";
        MyR_tree->get_Range_Search_JSON(answ, res);
        response << res;
        std::cout << "POST RANGE API"<<std::endl;
    }

    void postClear(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        delete MyR_tree;
        MyR_tree = new RTree(4);
        std::cout << "POST Clear API"<<std::endl;
    }

    void postLoad(GloveHttpRequest& request, GloveHttpResponse& response)
    {
        response.contentType("text/json");
        auto res =  MyR_tree->show_values_JSON();
        response << res;
        std::cout << "POST LOAD API"<<std::endl;
    }

    void postInsertTest(GloveHttpRequest& request, GloveHttpResponse& response){
	    response.contentType("text/json");
    
	    Polygon * P_2 = new Polygon(Point(632,396));
	    Polygon * P_4 = new Polygon(Point(368,382));
	    Polygon * P_6 = new Polygon(Point(436,492));
	    Polygon * P_7 = new Polygon(Point(622,514));
	    Polygon * P_8 = new Polygon(Point(673,337));
	    Polygon * P_10 = new Polygon(Point(730,354));
	    Polygon * P_17 = new Polygon(Point(616,346));
	    Polygon * P_18 = new Polygon(Point(560,432));
	    Polygon * P_19 = new Polygon(Point(560,459));
	    Polygon * P_20 = new Polygon(Point(618,228));
	    Polygon * P_22 = new Polygon(Point(226,405));

	    Polygon * P_2_reg = new Polygon(P_2->get_Pmin(),P_2->get_Pmax());
	    Polygon * P_4_reg = new Polygon(P_4->get_Pmin(),P_4->get_Pmax());
	    Polygon * P_6_reg = new Polygon(P_6->get_Pmin(),P_6->get_Pmax());
	    Polygon * P_7_reg = new Polygon(P_7->get_Pmin(),P_7->get_Pmax());
	    Polygon * P_8_reg = new Polygon(P_8->get_Pmin(),P_8->get_Pmax());
	    Polygon * P_10_reg = new Polygon(P_10->get_Pmin(),P_10->get_Pmax());
	    Polygon * P_17_reg = new Polygon(P_17->get_Pmin(),P_17->get_Pmax());
	    Polygon * P_18_reg = new Polygon(P_18->get_Pmin(),P_18->get_Pmax());
	    Polygon * P_19_reg = new Polygon(P_19->get_Pmin(),P_19->get_Pmax());
	    Polygon * P_20_reg = new Polygon(P_20->get_Pmin(),P_20->get_Pmax());
	    Polygon * P_22_reg = new Polygon(P_22->get_Pmin(),P_22->get_Pmax());

	    MyR_tree->insert_polygon(P_2,P_2_reg);
	    MyR_tree->insert_polygon(P_4,P_4_reg);
	    MyR_tree->insert_polygon(P_6,P_6_reg);
	    MyR_tree->insert_polygon(P_7,P_7_reg);
	    MyR_tree->insert_polygon(P_8,P_8_reg);
	    MyR_tree->insert_polygon(P_10,P_10_reg);
	    MyR_tree->insert_polygon(P_17,P_17_reg);
	    MyR_tree->insert_polygon(P_18,P_18_reg);
	    MyR_tree->insert_polygon(P_19,P_19_reg);
	    MyR_tree->insert_polygon(P_20,P_20_reg);
	    MyR_tree->insert_polygon(P_22,P_22_reg);

	    std::vector<Point> R_1{Point(528,355),Point(555,355),Point(555,402),Point(529,402)};
	    std::vector<Point> R_3{Point(360,462),Point(448,462),Point(448,412),Point(358,414)};
	    std::vector<Point> R_5{Point(320,174),Point(319,137),Point(374,135),Point(376,175)};
	    std::vector<Point> R_11{Point(661,184),Point(653,184),Point(654,236),Point(662,236)};
	    std::vector<Point> R_12{Point(726,184),Point(750,184),Point(750,236),Point(725,237)};
	    std::vector<Point> R_13{Point(45,471),Point(46,478),Point(29,483),Point(30,492),Point(8,500),Point(8,517),Point(56,505),Point(108,518),Point(166,513),Point(166,460)};
	    std::vector<Point> R_14{Point(2,448),Point(42,446),Point(54,448),Point(100,423),Point(110,418),Point(106,404),Point(98,407),Point(96,395),Point(122,387),Point(147,389),Point(155,324),Point(147,302),Point(111,301),Point(84,280),Point(75,254),Point(64,238),Point(40,229),Point(22,229),Point(1,239)};
	    std::vector<Point> R_15{Point(564,396),Point(599,397),Point(600,361),Point(566,361)};
	    std::vector<Point> R_16{Point(2,545),Point(26,529),Point(49,521),Point(90,528),Point(142,534),Point(192,540),Point(228,560),Point(240,598),Point(2,599)};
	    std::vector<Point> R_21{Point(612,338),Point(661,339),Point(661,323),Point(612,320)};

	    Polygon * myReal_R_1 = new Polygon(R_1);
	    Polygon * myReal_R_3 = new Polygon(R_3);
	    Polygon * myReal_R_5 = new Polygon(R_5);
	    Polygon * myReal_R_11 = new Polygon(R_11);
	    Polygon * myReal_R_12 = new Polygon(R_12);
	    Polygon * myReal_R_13 = new Polygon(R_13);
	    Polygon * myReal_R_14 = new Polygon(R_14);
	    Polygon * myReal_R_15 = new Polygon(R_15);
	    Polygon * myReal_R_16 = new Polygon(R_16);
	    Polygon * myReal_R_21 = new Polygon(R_21);

	    Polygon * reg_real_R_1 = new Polygon(myReal_R_1->get_Pmin(),myReal_R_1->get_Pmax());
	    Polygon * reg_real_R_3 = new Polygon(myReal_R_3->get_Pmin(),myReal_R_3->get_Pmax());
	    Polygon * reg_real_R_5 = new Polygon(myReal_R_5->get_Pmin(),myReal_R_5->get_Pmax());
	    Polygon * reg_real_R_11 = new Polygon(myReal_R_11->get_Pmin(),myReal_R_11->get_Pmax());
	    Polygon * reg_real_R_12 = new Polygon(myReal_R_12->get_Pmin(),myReal_R_12->get_Pmax());
	    Polygon * reg_real_R_13 = new Polygon(myReal_R_13->get_Pmin(),myReal_R_13->get_Pmax());
	    Polygon * reg_real_R_14 = new Polygon(myReal_R_14->get_Pmin(),myReal_R_14->get_Pmax());
	    Polygon * reg_real_R_15 = new Polygon(myReal_R_15->get_Pmin(),myReal_R_15->get_Pmax());
	    Polygon * reg_real_R_16 = new Polygon(myReal_R_16->get_Pmin(),myReal_R_16->get_Pmax());
	    Polygon * reg_real_R_21 = new Polygon(myReal_R_21->get_Pmin(),myReal_R_21->get_Pmax());

	    MyR_tree->insert_polygon(myReal_R_1, reg_real_R_1);
	    MyR_tree->insert_polygon(myReal_R_3, reg_real_R_3);
	    MyR_tree->insert_polygon(myReal_R_5, reg_real_R_5);
	    MyR_tree->insert_polygon(myReal_R_11,reg_real_R_11);
	    MyR_tree->insert_polygon(myReal_R_12,reg_real_R_12);
	    MyR_tree->insert_polygon(myReal_R_13,reg_real_R_13);
	    MyR_tree->insert_polygon(myReal_R_14,reg_real_R_14);
	    MyR_tree->insert_polygon(myReal_R_15,reg_real_R_15);
	    MyR_tree->insert_polygon(myReal_R_16,reg_real_R_16);
	    MyR_tree->insert_polygon(myReal_R_21,reg_real_R_21);
        auto res =  MyR_tree->show_values_JSON();
        response << res;
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

    serv.addRest("/rtree/test", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postInsertTest, &rtree, ph::_1, ph::_2)
    );

    serv.addRest("/rtree/clear", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postClear, &rtree, ph::_1, ph::_2)
    );

    serv.addRest("/rtree/load", 1,
        GloveHttpServer::jsonApiErrorCall,
        std::bind(&RTreeAPI::get, &rtree, ph::_1, ph::_2),
        std::bind(&RTreeAPI::postLoad, &rtree, ph::_1, ph::_2)
    );

    std::cout <<std::endl;
    std::cout << "READY WEB SERVER"<<std::endl;

    while(1)
        std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "TEST"<<std::endl;

}