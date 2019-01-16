#include "armor_finder/armor_finder.h"

using std::cout;
using namespace cv;

bool ArmorFinder::stateSearchingTarget(cv::Mat &src_left, cv::Mat &src_right) {
    showTwoImages("raw images", src_left, src_right);

    sendTargetByUart(0, 0, 0);

    /************************** find light blobs **********************************************/
    light_blobs_left_.clear(); light_blobs_right_.clear();
    bool state_left, state_right;
    state_left = findLightBlob(src_left, light_blobs_left_);
    state_right = findLightBlob(src_right, light_blobs_right_);

//    clock_t end = clock();
//    cout << "find time:" << end-start << '\n';

    if(!(state_left && state_right)) {return false;}
    showContours("light contours", src_left, light_blobs_left_, src_right, light_blobs_right_);

//    start = clock();


    /*************************** match light blobs***********************************/
    state_left = matchLightBlob(light_blobs_left_, armor_box_left_);
    state_right = matchLightBlob(light_blobs_right_, armor_box_right_);

//    end = clock();
//    cout << "match time:" << end-start << '\n';

    if(!(state_left && state_right)) {return false;}
    showArmorBox("armor boxes", src_left, armor_box_left_, src_right, armor_box_right_);

//    cout << "match success!" << '\n';

//    std::cout<<"running till here"<<std::endl;

    /********************** convert to 3d coordinate *********************************/
    //convertToStereoscopicCoordinate(armor_box_left_, armor_box_right_, armor_space_position_);


    /********************** convert 3d coordinate back to two camera vision ***************/
    //showSpacePositionBackToStereoVision(src_left, src_right, armor_space_position_);


    /******************** predict the armor moving path *******************************/
    //predictArmorPosition(armor_space_position_, armor_predicted_position_);



    /*********************** send position by uart **************************************/
//    //cout<<armor_space_position_<<endl;
//    armor_space_position_.x += 5;
//    armor_space_position_.y = armor_space_position_.y * 1.63 + 7.7;
//    armor_space_position_.z = armor_space_position_.z * 1.24 - 42.4;


//    sendTargetByUart(
//            static_cast<float>(armor_space_position_.x),
//            static_cast<float>(armor_space_position_.y),
//            static_cast<float>(armor_space_position_.z));

    return true;

}