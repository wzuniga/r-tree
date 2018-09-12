#include "server_http.hpp"
#include <iostream>
#include <typeinfo>
#include <string>
#include <utility>
#include <fstream>

#include "models/RTree.hpp"
#include "models/RTree.cpp"
#include "models/Objects.hpp"
#include "models/Object.cpp"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>
#ifdef HAVE_OPENSSL
#include "crypto.hpp"
#endif

using namespace std;
using namespace boost::property_tree;

typedef int dtype;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;



int main() {

    HttpServer server;
    server.config.port = 8090;
    RTree * MyR_tree = new RTree(4);

    server.resource["^/rtree/([a-zA-Z]+.html)$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        std::string line;
        std::string file = "../web/ViewTree.html";
        std::ifstream myfile (file);
        if (myfile.is_open()){
            while ( getline (myfile,line) )
                stream << line << '\n';
            myfile.close();
        }

        response->write_html(stream, header);
    };

    server.resource["^/rtree/rtree/js/([a-zA-Z.]+.js)$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        std::string line;
        std::string route = "../web/js/";
        std::string file = request->path_match[1];
        std::ifstream myfile (route+file);
        if (myfile.is_open()){
            while ( getline (myfile,line) )
                stream << line << '\n';
            myfile.close();
        }

        response->write_html(stream, header);
    };

    server.resource["^/rtree/js/([a-zA-Z]+.jpeg)$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        std::string line;
        std::string route = "../web/js/";
        std::string file = request->path_match[1];
        std::ifstream myfile (route+file);
        if (myfile.is_open()){
            while ( getline (myfile,line) )
                stream << line << '\n';
            myfile.close();
        }

        response->write_html(stream, header);
    };

    server.resource["^/rtree/insert$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        std::vector<Point> R_1;
        try {
            ptree pt;
            read_json(request->content, pt);

            for (boost::property_tree::ptree::value_type& rowPair:pt.get_child("data")) {
                int arrTemp[2];
                int i = 0;
                for (boost::property_tree::ptree::value_type& itemPair : rowPair.second) {
                    int value = itemPair.second.get_value<int>();
                    arrTemp[i++] = value;
                }
                R_1.push_back(Point(arrTemp[0], arrTemp[1]));
            }

            Polygon * p = new Polygon(R_1);
            Polygon * r = new Polygon(p->get_Pmin(),p->get_Pmax());
            MyR_tree->insert_polygon(p,r);
            string res =  MyR_tree->show_values_JSON();
            stream << res;
            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
        response->write_get(stream,header);
    };

    server.resource["^/rtree/nearest$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;

        try {
            ptree pt;
            read_json(request->content, pt);

            int x = pt.get_child("x").get_value<int>();
            int y = pt.get_child("y").get_value<int>();
            int k = pt.get_child("k").get_value<int>();

            std::vector<d_leaf*> ans_knn;
            MyR_tree->k_NN_DF(Point(x,y),k, ans_knn);
            std::string res="";
            MyR_tree->get_polygons_JSON( ans_knn,res);
            stream << res;

            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
        response->write_get(stream,header);
    };


    server.resource["^/rtree/range$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        try {
            ptree pt;
            read_json(request->content, pt);
            int p_1_x = pt.get_child("point1").get_child("x").get_value<int>();
            int p_1_y = pt.get_child("point1").get_child("y").get_value<int>();
            int p_2_x = pt.get_child("point2").get_child("x").get_value<int>();
            int p_2_y = pt.get_child("point2").get_child("y").get_value<int>();
            
            std::vector<Polygon * > answ;
            Polygon query(Point(p_1_x, p_1_y),Point(p_2_x, p_2_y));
            MyR_tree->range_search(query,answ);
            std::string res = "";
            MyR_tree->get_Range_Search_JSON(answ, res);
            stream << res;
            response->write_get(stream,header);
        } catch (const exception &e) {
            response->write(
                SimpleWeb::StatusCode::client_error_bad_request,
                e.what()
            );
        }
        response->write_get(stream,header);
    };

    server.resource["^/rtree/clear$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        delete MyR_tree;
        MyR_tree = new RTree(4);
        response->write_get(stream,header);
    };

    server.resource["^/rtree/load$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;
        stream << MyR_tree->show_values_JSON();
        response->write_get(stream,header);
    };

    server.resource["^/rtree/test$"]["POST"] = [&MyR_tree](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream stream;
        SimpleWeb::CaseInsensitiveMultimap header;

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
        stream << MyR_tree->show_values_JSON();
        response->write_get(stream,header);
    };

    server.on_error = [](shared_ptr<HttpServer::Request>, const SimpleWeb::error_code &) {};

    cout << "WEB SERVER STARTED" << endl;
    thread server_thread([&server]() {
        server.start();
    });

    this_thread::sleep_for(chrono::seconds(1));
    server_thread.join();
}
