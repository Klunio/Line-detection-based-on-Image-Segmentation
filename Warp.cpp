//
//  Warp.cpp
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/19.
//  Copyright © 2018 Kluni. All rights reserved.
//

#include "Warp.hpp"
CImg<float> mapping_tri(const vector<_Point>& left ,const vector<_Point>& right){
    CImg<float> src(3, 3, 1, 1);
    CImg<float> des(3, 3, 1, 1);
    
    src(0, 0) = get<0>(left[0]);
    src(1, 0) = get<0>(left[1]);
    src(2, 0) = get<0>(left[2]);
    
    src(0, 1) = get<1>(left[0]);
    src(1, 1) = get<1>(left[1]);
    src(2, 1) = get<1>(left[2]);
    
    src(0, 2) = src(1, 2) = src(2, 2) = 1;
    
    des(0, 0) = get<0>(right[0]);
    des(1, 0) = get<0>(right[1]);
    des(2, 0) = get<0>(right[2]);

    des(0, 1) = get<1>(right[0]);
    des(1, 1) = get<1>(right[1]);
    des(2, 1) = get<1>(right[2]);

    des(0, 2) = des(1, 2) = des(2, 2) = 1;
    
    return src *=des.invert();
}
bool reg(_Point a, _Point b){
    return (get<0>(a) + get<1>(a)) < (get<0>(b) + get<1>(b));
}
bool isInthisTri(int x, int y, vector<_Point> triangles){
    int x1 = get<0>(triangles[0]),
        x2 = get<0>(triangles[1]),
        x3 = get<0>(triangles[2]),
        y1 = get<1>(triangles[0]),
        y2 = get<1>(triangles[1]),
        y3 = get<1>(triangles[2]);

    double v = (double)((x -x1)*(y2-y1) - (y -y1)*(x2-x1))/
    (double)((x3-x1)*(y2-y1) - (y3-y1)*(x2-x1));
    double u = (double)((x -x1)*(y3-y1) - (y -y1)*(x3-x1))/
    (double)((x2-x1)*(y3-y1) - (y2-y1)*(x3-x1));
    
    return ( u >= -0.001 && v >= -0.001 && (u+v-1.) <= 0.001);
    
}
_Point mapping_point(CImg<float> matrix, int x, int y){
    return _Point(
                 matrix(0,0) * x + matrix(1, 0) * y + matrix(2, 0),
                 matrix(0,1) * x + matrix(1, 1) * y + matrix(2, 1),
                 0
                 );
}
CImg<unsigned char> Morph:: morphing(CImg<unsigned char> &img, vector<_Point> &pointSet){
    CImg<unsigned char> res(img.width(), img.height(), 1, 3, 0);
    sort(pointSet.begin(), pointSet.end(), reg);
    
    vector<_Point> src = {pointSet[0], pointSet[1], pointSet[2]};
    vector<_Point> des = {
        _Point(0, 0, 0),
        _Point(img.width(), 0, 0),
        _Point(0, img.height(), 0)
    };
    auto mat = mapping_tri(src, des);
    for (int x = 0; x <= img.width(); x++) {
        for (int y = 0; y <= img.height(); y++) {
            if (isInthisTri(x, y, des)) {
                _Point s = mapping_point(mat, x, y);
                res(x, y, 0) = img(get<0>(s), get<1>(s), 0);
                res(x, y, 1) = img(get<0>(s), get<1>(s), 1);
                res(x, y, 2) = img(get<0>(s), get<1>(s), 2);
            }
        }
    }
    src = {pointSet[1], pointSet[2], pointSet[3]};
    des = {
        _Point(img.width(), 0, 0),
        _Point(0, img.height(), 0),
        _Point(img.width(), img.height(), 0)
    };
    mat = mapping_tri(src, des);
    for (int x = 0; x <= img.width(); x++) {
        for (int y = 0; y <= img.height(); y++) {
            if (isInthisTri(x, y, des)) {
                _Point s = mapping_point(mat, x, y);
                res(x, y, 0) = img(get<0>(s), get<1>(s), 0);
                res(x, y, 1) = img(get<0>(s), get<1>(s), 1);
                res(x, y, 2) = img(get<0>(s), get<1>(s), 2);
            }
        }
    }
    return res;
}


