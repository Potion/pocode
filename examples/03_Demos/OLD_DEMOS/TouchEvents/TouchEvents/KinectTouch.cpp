//
//  KinectTouch.cpp
//  KinectMouse
//
//  Created by Ilan Schifter on 7/26/11.
//  Copyright 2011 Potion. All rights reserved.
//

#include "KinectTouch.h"
#include "poApplication.h"

#define PI 3.14159

int KinectTouch::initOpenNI(const XnChar* fname) {
	XnStatus nRetVal = XN_STATUS_OK;
    
	// initialize context
	nRetVal = xnContext.InitFromXmlFile(fname);
	CHECK_RC(nRetVal, "InitFromXmlFile");
    
	// initialize depth generator
	nRetVal = xnContext.FindExistingNode(XN_NODE_TYPE_DEPTH, xnDepthGenerator);
	CHECK_RC(nRetVal, "FindExistingNode(XN_NODE_TYPE_DEPTH)");
    
	// initialize image generator
	nRetVal = xnContext.FindExistingNode(XN_NODE_TYPE_IMAGE, xnImgeGenertor);
	CHECK_RC(nRetVal, "FindExistingNode(XN_NODE_TYPE_IMAGE)");

    xnDepthGenerator.GetAlternativeViewPointCap().SetViewPoint(xnImgeGenertor);
    
    XnFieldOfView fov;
    xnDepthGenerator.GetFieldOfView(fov);
    
    XtoZ=tan(fov.fHFOV/2)*2;
    YtoZ=tan(fov.fVFOV/2)*2;
    
    return 0;
}

void KinectTouch::average(vector<Mat1s>& frames, Mat1s& mean) {
	Mat1d acc(mean.size());
	Mat1d frame(mean.size());
    
	for (unsigned int i=0; i<frames.size(); i++) {
		frames[i].convertTo(frame, CV_64FC1);
		acc = acc + frame;
	}
    
	acc = acc / frames.size();
    
	acc.convertTo(mean, CV_16SC1);
}

void KinectTouch::on_mouse(int event, int x, int y, int flags, void* param)
{
    KinectTouch* obj = (KinectTouch*)param;
    
    Point2i point(x,y);
    if( event == CV_EVENT_LBUTTONDOWN)
    {
        for (int i=0 ; i<obj->corners.size() ; ++i) {
            if(obj->corners[i].rect.contains(point)){
                obj->corners[i].rect.x = point.x - obj->corners[i].rect.width/2;
                obj->corners[i].rect.y = point.y - obj->corners[i].rect.height/2;
                obj->corners[i].clicked = true;
                obj->corners[i].center = point;
                obj->table_corners[i] = point;
                break;
            }
        }
    }
    else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
    {
        for (int i=0 ; i<obj->corners.size() ; ++i) {
            if(obj->corners[i].clicked){
                obj->corners[i].rect.x = point.x - obj->corners[i].rect.width/2;
                obj->corners[i].rect.y = point.y - obj->corners[i].rect.height/2;                
                obj->corners[i].center = point;
                obj->table_corners[i] = point;
                break;
            }
        }
    }
    else if( event == CV_EVENT_LBUTTONUP)
    {
        cout<<"********"<<endl;
        for (int i=0 ; i<obj->corners.size() ; ++i) {
            if(obj->corners[i].clicked){
                obj->corners[i].clicked = false;
            }
            cout<<obj->corners[i].center<<endl;
        }
        Mat pts1(obj->table_corners, true), pts2(obj->screen_corners, true); 
        obj->projective_mat = findHomography(pts1, pts2);
    }
    if( event == CV_EVENT_RBUTTONDOWN)
    {
        if(!obj->cap_bg)
            obj->learnBackground();
    }
}   

void KinectTouch::Proj2RealPoint(Point3f &vec) {
    vec.x=(vec.x/640-.5)*vec.z*XtoZ;
    vec.y=(0.5-vec.y/480)*vec.z*YtoZ;
}

void KinectTouch::Proj2RealMat(Mat &proj) {
    Mat real = Mat::zeros(480, 640, CV_8S);
    float realX;
    float realY;
    for(int y=0 ; y<proj.rows ; ++y){
        for(int x=0 ; x<proj.cols ; ++x){
            realX=(x/640-.5)*proj.at<short>(y,x)*XtoZ;
            realY=(0.5-y/480)*proj.at<short>(y,x)*YtoZ;
            real.at<short>(realY,realX);
        }
    }
    proj = real;
}

KinectTouch::KinectTouch(int scr_x, int scr_y, 
                         int ulx, int uly, int urx, int ury,
                         int lrx, int lry, int llx, int lly,
                         float scl_factor,bool flip_vw,
                         int tDepMin, int tDepMax, int tMinArea,
                         int stbl_tch_mrg, string ovr_img_path){
    
    scale_factor = scl_factor;
    
    corner UL = {cv::Rect(ulx-5, uly-5,10,10),  "UL", false, Point2i(ulx, uly)};
    corner UR = {cv::Rect(urx-5, ury-5,10,10),  "UR", false, Point2i(urx, ury)};
    corner LR = {cv::Rect(lrx-5, lry-5,10,10),  "LR", false, Point2i(lrx, lry)};
    corner LL = {cv::Rect(llx-5, lly-5,10,10),  "LL", false, Point2i(llx, lly)};
    
    corners.push_back(UL);
    corners.push_back(UR);
    corners.push_back(LR);
    corners.push_back(LL);
    
    table_corners.push_back(UL.center);
    table_corners.push_back(UR.center);
    table_corners.push_back(LR.center);
    table_corners.push_back(LL.center);
    
    screen_table_x = scr_x/scale_factor;
    screen_table_y = scr_y/scale_factor;
    nBackgroundTrain = 100;

    screen_corners.push_back(Point2f(0,0));
    screen_corners.push_back(Point2f(screen_table_x,0));
    screen_corners.push_back(Point2f(screen_table_x,screen_table_y));
    screen_corners.push_back(Point2f(0,screen_table_y));

    overlay_image_path = ovr_img_path;
    flip_view = flip_vw;
    stable_touch_margin = stbl_tch_mrg;
    
    pts1 = Mat(table_corners, true);
    pts2 = Mat(screen_corners, true); 
    background  =   Mat1s(screen_table_y,screen_table_x);
    warpedDepth =   Mat1s(screen_table_y,screen_table_x);
    warpedDepth8 =  Mat1b(screen_table_y,screen_table_x);
    cap_bg = false;
    
	depth = Mat1s(480, 640);
	rgb = Mat3b(480, 640);
    warpedRgb = Mat3b(screen_table_y,screen_table_x);
	touch = Mat1b(screen_table_x, screen_table_y);
    foreground = Mat1s(screen_table_x, screen_table_y);
        
    touchDepthMin = tDepMin;
	touchDepthMax = tDepMax;
	touchMinArea =  tMinArea;

    boost::thread t(boost::bind(&KinectTouch::run,this));
}

void KinectTouch::updateTouchPoints(vector<poPoint>& tp){
    if(mutex.try_lock()){
        tp = touch_points;
        mutex.unlock();
    }
}

void KinectTouch::learnBackground()
{
    cap_bg = true;
    vector<Mat1s> buffer(nBackgroundTrain);
	Mat1s depth(480, 640);
    
    for (unsigned int i=0; i<nBackgroundTrain; i++) {
        xnContext.WaitAndUpdateAll();
        depth.data = (uchar*) xnDepthGenerator.GetDepthMap();
        medianBlur(depth,depth, 5);
        if(flip_view) flip(depth,depth,0);
        warpPerspective(depth, warpedDepth, projective_mat, warpedDepth.size());
        buffer[i] = warpedDepth;
    }
    average(buffer, background);
    
    cap_bg = false;
}

void KinectTouch::run() {
	initOpenNI("/Potion/libraries/OpenNI/SamplesConfig.xml");
    
    projective_mat = findHomography(pts1, pts2);

    Mat1s warpedDepth(screen_table_y,screen_table_x);
    Mat1b warpedDepth8(screen_table_y,screen_table_x);
    
	const unsigned int nBackgroundTrain = 100;
    
	const double debugFrameMaxDepth = 1000; // maximal distance
	const char* windowDepth = "Depth";
	const char* windowImage = "Image";
	const char* windowImageWarp = "Warped Image";    
	const char* windowTrackbars = "Trackbars";
	const Scalar colorBordo(0,0,128);
	const Scalar colorBlue(255,0,0);
	const Scalar colorWhite(255,255,255);
	const Scalar colorGreen(0,255,0);
	const Scalar colorRed(0,0,255);
	const Scalar colorYellow(0,255,255);
            
    Mat1s empty(100,640);
    // create some sliders
    namedWindow(windowTrackbars);
    createTrackbar("touchDepthMin", windowTrackbars, &touchDepthMin, 100);
    createTrackbar("touchDepthMax", windowTrackbars, &touchDepthMax, 100);
    createTrackbar("touchMinArea" , windowTrackbars, &touchMinArea , 100);    
    imshow(windowTrackbars, empty);
    
	// create a mouse callback
    namedWindow(windowImage);
    setMouseCallback(windowImage, (MouseCallback)on_mouse, this);

    if(overlay_image_path.compare("")>0){
        overlay_image = imread(overlay_image_path, 1);
        Mat1b overlay_image_grey;
        cvtColor(overlay_image, overlay_image_grey, CV_RGB2GRAY);     
        Canny(overlay_image_grey, overlay_image_grey, 0, 30, 3);
        cvtColor(overlay_image_grey, overlay_image, CV_GRAY2RGB);        
    }
    
    poWindow* win = applicationCurrentWindow();
    
	while ( waitKey(1) != 27 ) {
        if(cap_bg)
            continue;
        
		xnContext.WaitAndUpdateAll();
        
		// update 16 bit depth matrix
		depth.data = (uchar*) xnDepthGenerator.GetDepthMap();
        medianBlur(depth,depth, 5);
        if(flip_view) flip(depth,depth,0);
        warpPerspective(depth, warpedDepth, projective_mat, warpedDepth.size());
                
		// extract foreground by simple subtraction of very basic background model
        foreground = background - warpedDepth;
                
		// find touch mask by thresholding (points that are close to background = touch points)
		touch = (foreground > touchDepthMin) & (foreground < touchDepthMax);
        
		// find touch points
		vector< vector<Point2i> > contours;
		vector<Point2f> touchPoints;
		findContours(touch, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		for (unsigned int i=0; i<contours.size(); i++) {
			Mat contourMat(contours[i]);
            if ( contourArea(contourMat) > touchMinArea && contourMat.rows >= 6 ) {
                RotatedRect rrect = fitEllipse(contourMat);
                ellipse(warpedDepth,rrect,colorBlue);
                int x = sin(rrect.angle*PI/180) * rrect.size.height/2;
                int y = cos(rrect.angle*PI/180) * rrect.size.height/2;
                Point2f touchPoint((rrect.center.x - x)*scale_factor, (rrect.center.y + y)*scale_factor);
                touchPoints.push_back(touchPoint);
//                win->touchBegin( (rrect.center.x - x)*5, (rrect.center.y + y)*5 , i );
			}
		}

        int countCV = 0;
        int countPO = 0;
        
        int* stable_touch_count_tmp = new int[touchPoints.size()]();
        
        BOOST_FOREACH( poPoint& pPO, touch_points ){
            countCV = 0;
            BOOST_FOREACH( Point2f& pCV, touchPoints ){
                if( abs(pCV.x-pPO.x)<stable_touch_margin && abs(pCV.y-pPO.y)<stable_touch_margin ){
                    stable_touch_count_tmp[countCV] = stable_touch_count[countPO];
                    stable_touch_count_tmp[countCV]++;
                    if(stable_touch_count_tmp[countCV]==1){
                        win->touchBegin( touchPoints[countCV].x, touchPoints[countCV].y , countCV );
                    }
                }
                countCV++;
            }
            countPO++;
        }
        
        stable_touch_count.assign(stable_touch_count_tmp, stable_touch_count_tmp+touchPoints.size());        
        delete[] stable_touch_count_tmp;
        
        mutex.lock();
        touch_points.clear();
        BOOST_FOREACH( Point2f& point, touchPoints )
        {
            touch_points.push_back(poPoint(point.x, point.y));
        }
        mutex.unlock();
        
		// draw depth frame
		warpedDepth.convertTo(warpedDepth8, CV_8U, 255 / debugFrameMaxDepth); // render depth to debug frame
		cvtColor(warpedDepth8, warpedDepth, CV_GRAY2BGR);
		warpedDepth.setTo(colorBlue, touch);  // touch mask
		for (unsigned int i=0; i<touchPoints.size(); i++) { // touch points
			circle(warpedDepth, touchPoints[i], 5, colorRed, CV_FILLED);            
		}
		imshow(windowDepth, warpedDepth);
        
		// draw image frame
        rgb.data = (uchar*) xnImgeGenertor.GetRGB24ImageMap();
        if(flip_view) flip(rgb,rgb,0);
        cvtColor(rgb, rgb, CV_RGB2BGR);        
        for (int i=0 ; i<corners.size() ; ++i) {
			circle(rgb, corners[i].center , 7, colorRed, 1);
            if(i<corners.size()-1)
                line(rgb, corners[i].center, corners[i+1].center, colorBlue);
            else
                line(rgb, corners[i].center, corners[0].center, colorBlue);
        }
		imshow(windowImage, rgb);
        
		// draw warped image frame
        warpPerspective(rgb, warpedRgb, projective_mat, warpedDepth.size());
		imshow(windowImageWarp, warpedRgb + overlay_image);
	}
}
