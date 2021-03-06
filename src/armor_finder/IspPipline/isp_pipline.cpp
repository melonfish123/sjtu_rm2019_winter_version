#include "armor_finder/armor_finder.h"

using namespace cv;

void ArmorFinder::ispPipline(cv::Mat &src) {


}

void ArmorFinder::imagePreprocess(cv::Mat &src_left, cv::Mat &src_right) {
    if(src_left.type() == CV_8UC1)
    {
        splitBayerBG(src_left, src_blue0, src_red0);
        splitBayerBG(src_right, src_blue1, src_red1);

    }else if(src_left.type() == CV_8UC3)
    {
        std::vector<Mat> channels_left, channels_right;
        split(src_left, channels_left);
        split(src_right, channels_right);
        resize(channels_left.at(0), src_blue0, Size(SRC_WIDTH, SRC_HEIGHT));
        resize(channels_left.at(2), src_red0, Size(SRC_WIDTH, SRC_HEIGHT));
        resize(channels_right.at(0), src_blue1, Size(SRC_WIDTH, SRC_HEIGHT));
        resize(channels_right.at(2), src_red1, Size(SRC_WIDTH, SRC_HEIGHT));
    }
    if(enemy_color_ == ENEMY_RED)
    {
        src_left_ = src_red0 - src_blue0;
        src_right_ = src_red1 - src_blue1;
    }else if(enemy_color_ == ENEMY_BLUE){
        src_left_ = src_blue0 - src_red0;
        src_right_ = src_blue1 - src_red1;
    }
}