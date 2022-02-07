// #include <opencv2/highgui.hpp>
// #include <iostream>

// int main( int argc, char** argv ) {
  
//   cv::Mat image;
//   image = cv::imread("sample.jpeg" , CV_LOAD_IMAGE_COLOR);
  
//   if(! image.data ) {
//       std::cout <<  "Could not open or find the image" << std::endl ;
//       return -1;
//     }
  
//   cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
//   cv::imshow( "Display window", image );
  
//   cv::waitKey(0);
//   return 0;
// }






#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main()
{
    Mat frame;
    Mat rs_frame;
    namedWindow("video",cv::WINDOW_AUTOSIZE);
   
    VideoCapture cap("http://root:liangdao@169.254.14.239/mjpg/video.mjpg?camera=2");
    if(!cap.isOpened())
    {
        cout<<"Camera not found"<<endl;
        getchar();
        return -1;
    }
    while ( cap.isOpened() )
    {
        cap >> frame;
        if(frame.empty()) 
        {
            cout<<"frame is empty."<<std::endl;
            break;
        }
        //cout<<"imshow photo!"<<frame.size()<<endl;
        resize(frame, rs_frame, cv::Size(), 0.5, 0.5);
        imshow("video", rs_frame);
        waitKey(25);
       
    }   
    return 0;
}

//g++ main.cpp -o axis_live2 `pkg-config --cflags --libs opencv`
