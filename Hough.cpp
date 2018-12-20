//
//  Hough.cpp
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/19.
//  Copyright © 2018 Kluni. All rights reserved.
//

#include "Hough.hpp"
const unsigned char yellow[] = {255, 255, 0};


bool comp(_Point a, _Point b){
    return get<2>(a) > get<2>(b);
}
CImg<unsigned char>Hough:: detect_line(const CImg<unsigned char> &img, double sigma){
    int width = img.width();
    int height = img.height();
    // 膨胀2次
    CImg<unsigned char> t(img);
    CImg<unsigned char> Dilation(width, height, 1, 1, 0);
    for (int i = 0; i<2; i++) {
        cimg_forXY(t, x, y){
            if (x == t.width() -1 || y == t.height() -1) continue;
            if (t(x, y) == 255 || t(x+1, y) == 255 || t(x+1, y+1) == 255 )Dilation(x,y) = 255;
        }
        t = Dilation;
    }
    
    
    CImg<unsigned char> Erosion(width, height, 1, 1, 0);
    cimg_forXY(Dilation, x, y){
        if (x == Dilation.width() -1 || y == Dilation.height() -1) continue;
        if (Dilation(x, y) == 255 && Dilation(x+1, y) == 255 &&Dilation(x+1, y+1) == 255 ) Erosion(x,y) = 255;
    }
    
    // 描边
    int dx [] = {-1, 0, 1, 1, 1, 0, -1, -1};
    int dy [] = {-1, -1, -1, 0, 1, 1, 1, 0};
    CImg<unsigned char> temp(width, height, 1, 1, 0);
    
    cimg_forXY(Erosion, x, y){
        for (int i = 0; i<8; i++) {
            int x0 = x+dx[i], y0 = y+dy[i];
            if (x0 >= 0 && x0 < Erosion.width() && y0 >= 0 && y0 < Erosion.height()) {
                if (Erosion(x,y) != Erosion(x0, y0)){
                    temp(x, y) = 255;
                }
            }
        }
    }
    
    CImg<unsigned char> blurred_img = temp.get_blur(sigma);
    
    // hough 过程
    int max =sqrt(blurred_img._width*blurred_img._width+ blurred_img._height*blurred_img._height);
    CImg<float> hough_space(360, max, 1, 1, 0);

    
    cimg_forXY(blurred_img, i, j) {
        if (blurred_img(i, j) > gradient_threshold) {
            cimg_forX(hough_space, alpha) {
                double theta = ((double)alpha*cimg::PI) / 180;
                int p = (int)(i*cos(theta) + j*sin(theta));
                if (p >= 0 && p < max) {
                    hough_space(alpha, p)++;
                }
            }
        }
    }
    
    //vote
    auto points = vote(hough_space);
    
    // draw
    draw_Dot(blurred_img, points);
    return blurred_img;
}

vector<_Point> Hough::vote(CImg<float> houghspace) {
    vector<_Point> pointSet;

    cimg_forXY(houghspace, x, y) {  // x -> theta ; y -> p
        if (houghspace(x, y) > vote_threshold) {
            bool abandon = true;
            for (auto &i : pointSet) {  // 遍历已经保存的点
                if (_distance(i,x,y) < peak_dis) { // 存在过于相近的点
                    abandon = false;
                    if (get<2>(i) < houghspace(x, y)) {
                        i = _Point(x, y, houghspace(x, y));
                    }
                }
            }
            if(abandon){
                pointSet.push_back(_Point(x, y, houghspace(x, y)));
            }
        }
    }
    return pointSet;
}

void Hough::draw_Dot(CImg<unsigned char> &result,
                        vector<_Point> &pointSet){
    for (auto i = 0 ; i < pointSet.size() ; ++i){
        double theta = get<0>(pointSet[i]) * cimg::PI / 180
        ,k1 = -cos(theta) / sin(theta)
        ,b1 = double(get<1>(pointSet[i])) / sin(theta);
        for (auto j = i + 1; j < pointSet.size(); ++j) {
            double theta2 = get<0>(pointSet[j]) * cimg::PI / 180
            ,k2 = -cos(theta2) / sin(theta2)
            ,b2 = double(get<1>(pointSet[j])) / sin(theta2);
            int x0 = (double)(b2-b1) / (double)(k1-k2)
            ,y0 = -(double)(k1*b2-k2*b1)/(double)(k2-k1);
            if (x0 > 0 && x0 < result.width() && y0>0 && y0<result.height()) {
                result.draw_circle(x0, y0, 15, yellow);
                points.push_back(_Point(x0, y0, 0));
                printf("Dot - x: %d, y: %d\n", x0, y0);
            }
        }
    }
}
vector<_Point> Hough:: get_Points(){
    return points;
}
