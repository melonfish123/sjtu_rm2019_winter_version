
#include "armor_finder/armor_finder.h"
#include <cvaux.h>

using namespace cv;
using std::cout;
using std::endl;

void ArmorFinder::initLightParam() {
    light_blob_param_.GRAY_THRESH = 200;
    light_blob_param_.CONTOUR_AREA_MIN = 1;
    light_blob_param_.CONTOUR_AREA_MAX = 3000;
    light_blob_param_.CONTOUR_LENGTH_MIN = 3;
    light_blob_param_.CONTOUR_HW_RATIO_MIN = 2.5;       // 2.5
    light_blob_param_.CONTOUR_HW_RATIO_MAX = 15;
    light_blob_param_.CONTOUR_ANGLE_MAX = 20.0;
}


bool ArmorFinder::findLightBlob(const cv::Mat &src, vector<LightBlob> &light_blobs) {
    static Mat src_gray;
    static Mat src_bin;
    if(src.type() == CV_8UC3){
        cvtColor(src, src_gray, COLOR_BGR2GRAY);
    }else if(src.type() == CV_8UC1){
        src_gray = src.clone();
    }

    threshold(src_gray, src_bin, light_blob_param_.GRAY_THRESH, 255, THRESH_BINARY);
    imshow("binary image", src_bin);
//    Mat element = getStructuringElement(MORPH_RECT,Size(15,15));
//    Mat src_bin_out;
//    dilate(src_bin,src_bin_out,element);
//    imshow("out binary image", src_bin_out);
    std::vector<vector<Point> > light_contours;
    findContours(src_bin, light_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //cout<<"number of contours:"<<light_contours.size()<<endl;
    for (auto &light_contour : light_contours) {
        if(!isValidLightContour(light_contour))
        {
            continue;
        }
        light_blobs.emplace_back(light_contour);
    }
    return light_blobs.size() >= 2;
}

bool ArmorFinder::isValidLightContour(const vector<Point> &light_contour) {
    double cur_contour_area = contourArea(light_contour);
    if(cur_contour_area > light_blob_param_.CONTOUR_AREA_MAX ||
       cur_contour_area < light_blob_param_.CONTOUR_AREA_MIN)
    {
        //cout<<cur_contour_area<<" "<<light_blob_param_.CONTOUR_AREA_MIN<<" "<<light_blob_param_.CONTOUR_AREA_MAX<<endl;
        //cout<<"area fail."<<endl;
        return false;
    }
    RotatedRect cur_rect = minAreaRect(light_contour);
    Size2f cur_size = cur_rect.size;
    float length = cur_size.height > cur_size.width ? cur_size.height : cur_size.width;
    float width = cur_size.height < cur_size.width ? cur_size.height : cur_size.width;
    if(length < light_blob_param_.CONTOUR_LENGTH_MIN)
    {
        //cout<<"length min fail."<<endl;
        return false;
    }
    float length_width_ratio = length / width;
    if(length_width_ratio > light_blob_param_.CONTOUR_HW_RATIO_MAX ||
       length_width_ratio < light_blob_param_.CONTOUR_HW_RATIO_MIN)
    {
        //cout<<"length width ratio fail."<<endl;
        return false;
    }
//    if(cur_contour_area / cur_size.area() < 0.7) return false;
    return true;
}