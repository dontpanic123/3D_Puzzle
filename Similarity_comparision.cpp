// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API




int main(int argc, char * argv[]) try
{
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;   

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    using namespace cv;
    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);
    rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
    rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
    // Query frame size (width and height)
    const int w = depth.as<rs2::video_frame>().get_width();
    const int h = depth.as<rs2::video_frame>().get_height();

    const int data_size = depth.as<rs2::video_frame>().get_data_size();
    std::cout << "data size : " << data_size << std::endl;
    std::cout << "w size : " << w << std::endl;
    std::cout << "h size : " << h << std::endl;

    int a = w/2;
    int b = h/2;
    int photo_count = 0;
    double alpha=1.0; /*< Simple contrast control 1.0-3.0 */
    int beta=80;  /*< Simple brightness control 0-100 */
    while ( getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0)
    {
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        rs2::frame depth = data.get_depth_frame().apply_filter(color_map);                // color map!!
        Mat image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);    //color map!!
       
        
        // Create OpenCV matrix of size (w,h) from the depth data
       /* rs2::depth_frame depth = data.get_depth_frame();
        Mat image(Size(w, h), CV_16UC1, (void*)depth.get_data(), Mat::AUTO_STEP);*/

        // Update the window with new data
        imshow(window_name, image);
        
        // if (waitKey(1) == 115){// if "s" was drucked, one frame form each window will be saved.
        

            //// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$          save photo
            // std::string photo_name ="depth_image" + std::to_string(photo_count) + ".png";
            //imwrite(photo_name, image);
            //photo_count += 1;
            //std::cout << photo_name<<" -Saved " << std::endl;
            //// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$       
            destroyWindow("block0");
            destroyWindow("block1");
            destroyWindow("block2");
            destroyWindow("block3");

            Rect rect(0, 0, a, b);
            Mat image_ori = image(rect);
            namedWindow("block_0", 1);
            imshow("block_0", image_ori);
            //waitKey(0);

            Rect rect1(a, 0, a, b);
            Mat image_ori1 = image(rect1);
            namedWindow("block_1", 1);
            imshow("block_1", image_ori1);

            Rect rect2(0, b, a, b);
            Mat image_ori2 = image(rect2);
            namedWindow("block_2", 1);
            imshow("block_2", image_ori2);

            Rect rect3(a, b, a, b);
            Mat image_ori3 = image(rect3);
            namedWindow("block_3", 1);
            imshow("block_3", image_ori3);
            //***********************************    brightness comparsion ********************************



            //***********************************    Histogram comparsion ********************************
            cvtColor(image_ori, image_ori, COLOR_BGR2HSV);
            cvtColor(image_ori2, image_ori2, COLOR_BGR2HSV);
            
            int hBins = 256, sBins = 256, vBins=256;
            int histSize[] = { hBins,sBins ,vBins};
            
            float hRanges[] = { 0,256 };
            
            float sRanges[] = { 0,255 };
            float vRanges[] = { 0,255 };
            const float* ranges[] = { hRanges,sRanges,vRanges };
            int channels[] = { 0,1,2 };
            MatND hist1, hist2;
            calcHist(&image_ori, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false);
            normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
            calcHist(&image_ori2, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
            normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
            double similarityValue = compareHist(hist1, hist2, 4);
            std::cout << "Similarity: " << similarityValue << std::endl;
            if (similarityValue<1.0)
            {
                image_ori.convertTo(image_ori, -1, alpha, beta);
                imshow("block_0", image_ori);
                image_ori2.convertTo(image_ori2, -1, alpha, beta);
                imshow("block_2", image_ori2);
            }

            //*********************************   ORB        ***************************************

            //double t;
            //t = getTickCount();
            //Mat img_1 = image_ori;
            //Mat img_2 = image_ori1;
            //if (!img_1.data || !img_2.data)
            //{
            //    std::cout << "error reading images " << std::endl;      
            //}
            //ORB orb;
            //vector<KeyPoint> keyPoints_1, keyPoints_2;
            //Mat descriptors_1, descriptors_2;
            //orb(img_1, Mat(), keyPoints_1, descriptors_1);
            //orb(img_2, Mat(), keyPoints_2, descriptors_2);
            //BruteForceMatcher<HammingLUT> matcher;
            //vector<DMatch> matches;
            //matcher.match(descriptors_1, descriptors_2, matches);
            //double max_dist = 0; double min_dist = 100;
            //for (int i = 0; i < descriptors_1.rows; i++)
            //{
            //    double dist = matches[i].distance;
            //    if (dist < min_dist) min_dist = dist;
            //    if (dist > max_dist) max_dist = dist;
            //}
            //printf("-- Max dist : %f \n", max_dist);
            //printf("-- Min dist : %f \n", min_dist);
            //std::vector< DMatch > good_matches;
            //for (int i = 0; i < descriptors_1.rows; i++)
            //{
            //    if (matches[i].distance < 0.6 * max_dist)
            //    {
            //        good_matches.push_back(matches[i]);
            //    }
            //}
            //t = getTickCount() - t;
            //t = t * 1000 / getTickFrequency();
            //Mat img_matches;
            //drawMatches(img_1, keyPoints_1, img_2, keyPoints_2, good_matches, img_matches,
            //    Scalar::all(-1), Scalar::all(-1), vector<char>(),
            //    DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            //imshow("Match", img_matches);
            //printf("%f ms\n", t);
            //


            waitKey(2);
            
          
       // }
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}




















