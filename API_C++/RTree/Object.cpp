#include "Objects.hpp"
#include <stdlib.h>
#include <iostream>

static float PI = 3.141522;

Polygon::Polygon(std::vector<Point> p):vertices(p), key(-1),corners(p.size()){
    Polygon mbb = this->get_mbb();
    this->Pmin = mbb.Pmin;
    this->Pmax = mbb.Pmax;   
}
Polygon::Polygon(Point P):corners(1), key(-1) {
    this->Pmin = P;
    this->Pmax = P;
    this->vertices.push_back(P);
}
void Polygon::area_added(Polygon & reg, Point & pmin, Point & pmax){
    int x_max = this->Pmax.get_X();
    if(reg.Pmax.get_X() > x_max){
        x_max = reg.Pmax.get_X();
    }
    int x_min = this->Pmin.get_X();
    if(reg.Pmin.get_X() < x_min){
        x_min = reg.Pmin.get_X();
    }

    int y_max = this->Pmax.get_Y();
    if(reg.Pmax.get_Y() > y_max){
        y_max = reg.Pmax.get_Y();
    }
    int y_min = this->Pmin.get_Y();
    if(reg.Pmin.get_Y() < y_min){
        y_min = reg.Pmin.get_Y();
    }
        //This points max and minimung will welp to calculate the region of parents
    pmin = Point(x_min,y_min);
    pmax = Point(x_max,y_max);
}
int Polygon::cost_two_poligons(Polygon & reg){
    Point pmin, pmax;
    area_added(reg,pmin,pmax);
    int d = (pmax.get_X()-pmin.get_X())*(pmax.get_Y()-pmin.get_Y());
    d -= (reg.Pmax.get_X() - reg.Pmin.get_X())*(reg.Pmax.get_Y() - reg.Pmin.get_Y());
    d -= (this->Pmax.get_X() - this->Pmin.get_X())*(this->Pmax.get_Y() - this->Pmin.get_Y());
    return d;
}

bool Polygon::intersect_with_BB(Polygon & pol){
    if(pol.traberse_with(*this))
        return true;
    if(pol.is_Within_of(*this))
        return true;
    if(this->Pmin <= pol.Pmax && this->Pmin >= pol.Pmin){
            return true;
    }
    if(this->Pmax <= pol.Pmax && this-> Pmax >= pol.Pmin){
        return true;
    }
    Point myPoint_leftUP(this->Pmin.get_X(),this->Pmax.get_Y());
    Point myPoint_right_L(this->Pmax.get_X(), this->Pmin.get_Y());
    Point BB_point_leftUP(Pmin.get_X(),Pmax.get_Y());
    Point BB_point_right_L(Pmax.get_X(), Pmin.get_Y());
    if(myPoint_leftUP <= BB_point_leftUP && myPoint_leftUP >= BB_point_right_L){
        return true;
    }
    if(myPoint_right_L <= BB_point_leftUP && myPoint_right_L >= BB_point_right_L){
        return true;
    }
    //if(this->traberse_with(pol))
    //    return true;
    
    return false;
}

bool Polygon::is_Within_of(const Polygon & pol){
    if(this->Pmin >= pol.Pmin && this->Pmax <= pol.Pmax)
            return true;
    else
        return false;
}
bool Polygon::traberse_with(Polygon & other){
    if(this->Pmin.get_X() <= other.Pmax.get_X() && this->Pmax.get_Y() <= other.Pmax.get_Y())
        return true;
    if(this->Pmin.get_Y() <= other.Pmax.get_Y() && this->Pmax.get_X() <= other.Pmax.get_X())
        return true;
}

Polygon Polygon::get_mbb(){      
    int x_min= std::numeric_limits<int>::max(); 
    int y_min = x_min;
    int x_max = std::numeric_limits<int>::min();
    int y_max = x_max;
    for(int i = 0; i < this->corners; i++){
        if(this->vertices[i].get_X() < x_min)
            x_min = this->vertices[i].get_X();
        if(this->vertices[i].get_Y() < y_min)
            y_min = this->vertices[i].get_Y();
        if(this->vertices[i].get_X() > x_max)
            x_max = this->vertices[i].get_X();
        if(this->vertices[i].get_Y() > y_max)
            y_max = this->vertices[i].get_Y();
    }
    return Polygon(Point(x_min,y_min),Point(x_max,y_max));
}

//template <class T>
//T Polygon::distance_geometric(Point q){return 0.0;}
float Polygon::distance_geometric(Point q){
    float d_X_min = std::numeric_limits<float>::max();
    if(q.get_X() >= Pmin.get_X() && q.get_X() <= Pmax.get_X()){
        d_X_min    = 0.0;
    }
    else{
        if(abs(q.get_X() - Pmin.get_X()) < d_X_min){
            d_X_min = abs(q.get_X() - Pmin.get_X());
        }
        if(abs(q.get_X() - Pmax.get_X()) < d_X_min){
            d_X_min = abs(q.get_X() - Pmax.get_X());
        }
    }
    float d_Y_min = std::numeric_limits<float>::max();
    if(q.get_Y() >= Pmin.get_Y() && q.get_Y() <= Pmax.get_Y()){
        d_Y_min = 0.0;
    }
    else{
        if(abs(q.get_Y() - Pmin.get_Y()) < d_Y_min){
            d_Y_min = abs(q.get_Y() - Pmin.get_Y());
        }
        if(abs(q.get_Y() - Pmax.get_Y()) < d_Y_min){
            d_Y_min = abs(q.get_Y() - Pmax.get_Y());
        }
    }
    float d = sqrt(d_X_min*d_X_min + d_Y_min*d_Y_min);
    return d;
}
float Polygon::distance_to_polygon(Point q){
    //int minIndex;
    float minDistance = std::numeric_limits<float>::max();
    float distanceLine = std::numeric_limits<float>::max();
    float previousDistance = std::numeric_limits<float>::max();
    for(int i = 0; i < corners; i++){
		float distancePoint = this->vertices[i].distance_points(q);

        if(i!=0)
        {
            float distanceBetweenPoints = this->vertices[i].distance_points(this->vertices[i-1]);
            float angle1 = (pow(distancePoint,2)-pow(distanceBetweenPoints,2)-pow(previousDistance,2))/(-2*distanceBetweenPoints*previousDistance);
            float angle2 = (pow(previousDistance,2)-pow(distanceBetweenPoints,2)-pow(distancePoint,2))/(-2*distanceBetweenPoints*distancePoint);

            if(angle1 > 0 && angle2 >0)
            {
                float A = this->vertices[i].get_Y()-this->vertices[i-1].get_Y();
                float B = -(this->vertices[i].get_X()-this->vertices[i-1].get_X());
                float C = this->vertices[i-1].get_Y()*((this->vertices[i].get_X()-this->vertices[i-1].get_X()))-this->vertices[i-1].get_X()*(this->vertices[i].get_Y()-this->vertices[i-1].get_Y());
                distanceLine = abs(A*q.get_X()+B*q.get_Y()+C)/sqrt(pow(A,2)+pow(B,2));

            }            
        }
        else
        {
            previousDistance = this->vertices[corners-1].distance_points(q);
            float distanceBetweenPoints = this->vertices[i].distance_points(this->vertices[corners-1]);
            float angle1 = (pow(distancePoint,2)-pow(distanceBetweenPoints,2)-pow(previousDistance,2))/(-2*distanceBetweenPoints*previousDistance);
            float angle2 = (pow(previousDistance,2)-pow(distanceBetweenPoints,2)-pow(distancePoint,2))/(-2*distanceBetweenPoints*distancePoint);

            if(angle1 > 0 && angle2 >0)
            {
                float A = this->vertices[i].get_Y()-this->vertices[corners-1].get_Y();
                float B = -(this->vertices[i].get_X()-this->vertices[corners-1].get_X());
                float C = this->vertices[corners-1].get_Y()*((this->vertices[i].get_X()-this->vertices[corners-1].get_X()))-this->vertices[corners-1].get_X()*(this->vertices[i].get_Y()-this->vertices[corners-1].get_Y());
                distanceLine = abs(A*q.get_X()+B*q.get_Y()+C)/sqrt(pow(A,2)+pow(B,2));

            }    
        }

        previousDistance = distancePoint;

        if(distanceLine < distancePoint)
        {
    		if(distanceLine < minDistance){
                //minIndex = i;
                minDistance = distanceLine;
            }
        }
        else
        {
            if(distancePoint < minDistance){
                //minIndex = i;
                minDistance = distancePoint;
            }
        }

    }
    return minDistance;
}

float Polygon::max_distance_geometric(Point q){
    float d_X_max = 0.0;
    if(abs(q.get_X() - this->get_Pmin().get_X()) > d_X_max){
        d_X_max = abs(q.get_X() - this->get_Pmin().get_X());
    }
    if(abs(q.get_X() - get_Pmax().get_X()) > d_X_max){
        d_X_max = abs(q.get_X() - this->get_Pmax().get_X());
    }
    float d_Y_max = 0.0;
    if(abs(q.get_Y() - this->get_Pmin().get_Y()) > d_Y_max){
        d_Y_max = abs(q.get_Y() - this->get_Pmin().get_Y());
    }
    if(abs(q.get_Y() - this->get_Pmax().get_Y()) > d_Y_max){
        d_Y_max = abs(q.get_Y() - this->get_Pmax().get_Y());
    }
    float d = sqrt(d_X_max*d_X_max+d_Y_max*d_Y_max);
    return d;
}

std::vector<Point> Polygon::get_vertices(){
    return this->vertices;
}

bool Polygon::set_key(int k){
    if(this->key==-1){
        this->key = k;
        return true;
    }
    return false;
}

Polygon::~Polygon(){    
}
