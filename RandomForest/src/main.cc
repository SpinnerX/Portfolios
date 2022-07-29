#include <iostream>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include "../include/Vector/Vec2.h"
using namespace std;

int main(){
    Coords<double> coord1 = Coords<double>(2., 2.);
    Vec2<double> vec = Vec2<double>(coord1);

    cout << vec.length() << endl;
    

    
}