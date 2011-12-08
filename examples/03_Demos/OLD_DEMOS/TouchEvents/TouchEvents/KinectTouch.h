//
//  Header.h
//  KinectMouse
//
//  Created by Ilan Schifter on 7/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "ApplicationServices/ApplicationServices.h"
#include "poPoint.h"

// openCV
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include <opencv2/gpu/gpu.hpp>
using namespace cv;

#include <XnOpenNI.h>
#include <XnCppWrapper.h>
using namespace xn;
#define CHECK_RC(rc, what)                                      \
if (rc != XN_STATUS_OK)                                     \
{                                                           \
printf("%s failed: %s\n", what, xnGetStatusString(rc));	\
return rc;                                              \
}

class KinectTouch {
    // OpenNI
    xn::Context xnContext;
    xn::DepthGenerator xnDepthGenerator;
    xn::ImageGenerator xnImgeGenertor;
    
public:
    KinectTouch(int scr_x = 640, int scr_y = 480, 
                int ulx = 0, int uly = 0, int urx = 640, int ury = 0,
                int lrx = 640, int lry = 480, int llx = 0, int lly = 480,
                float scl_factor = 1 ,bool flip_vw = false,
                int tDepMin = 12, int tDepMax = 22, int tMinArea = 20,
                int stbl_tch_mrg = 5, string ovr_img_path = "");
    
    ~KinectTouch();
    
    int initOpenNI(const XnChar* fname);
    void average(vector<Mat1s>& frames, Mat1s& mean);
    static void on_mouse(int event, int x, int y, int flags, void* param);
    void Proj2RealPoint(Point3f &vec);
    void Proj2RealMat(Mat &proj);
    void run();
    void updateTouchPoints(vector<poPoint>& tp);
    void learnBackground();
    
    bool flip_view;
    int scale_factor;
    
    struct corner{
        cv::Rect rect;
        string name;
        bool clicked;
        Point2i center;
    };
    
    vector<corner> corners;
    vector<Point2f> table_corners;
    
    int screen_table_x;
    int screen_table_y;
    vector<Point2f> screen_corners;
    Mat         projective_mat;
    float XtoZ, YtoZ;
    
	int touchDepthMin;
	int touchDepthMax;
	int touchMinArea;

    boost::mutex mutex;
    vector<poPoint> touch_points;
    vector<int> stable_touch_count;
    int stable_touch_margin;
        
    bool cap_bg;
	unsigned int nBackgroundTrain;
	Mat1s background;
    Mat1s warpedDepth;
    Mat1b warpedDepth8;
    Mat pts1;
    Mat pts2; 
	Mat1s depth;
	Mat3b rgb;
	Mat3b warpedRgb;

	Mat1b touch;
    Mat1s foreground;

    Mat3b overlay_image;
    string overlay_image_path;
};