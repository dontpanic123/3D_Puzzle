// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

#include < vector> 
using namespace cv;


int main(int argc, char * argv[]) try
{
    //// Declare depth colorizer for pretty visualization of depth data
    //rs2::colorizer color_map;   

    //// Declare RealSense pipeline, encapsulating the actual device and sensors
    //rs2::pipeline pipe;
    //// Start streaming with default recommended configuration
    //pipe.start();

    //
    //const auto window_name = "Display Image";
    //namedWindow(window_name, WINDOW_AUTOSIZE);
    //rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
    //rs2::frame depth = data.get_depth_frame().apply_filter(color_map);
    //// Query frame size (width and height)
    //const int w = depth.as<rs2::video_frame>().get_width();
    //const int h = depth.as<rs2::video_frame>().get_height();

    //const int data_size = depth.as<rs2::video_frame>().get_data_size();
    //std::cout << "data size : " << data_size << std::endl;
    //std::cout << "w size : " << w << std::endl;
    //std::cout << "h size : " << h << std::endl;

    //int a = w/2;
    //int b = h/2;
    //int photo_count = 0;
    //double alpha = 1.0; /*< Simple contrast control 1.0-3.0 */
    //int beta = 80;  /*< Simple brightness control 0-100 */
    int w = 848;
    int h = 480;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    bool compare = false;
    rs2::colorizer color_map;
    rs2::context                          ctx;        // Create librealsense context for managing devices
    std::map<std::string, rs2::colorizer> colorizers; // Declare map from device serial number to colorizer (utility cl  ass to convert depth data RGB colorspace)
    std::vector<rs2::pipeline>            pipelines;
    std::vector<Mat>            first_blocks;               // Declare blocks to save blocks of each image
    std::vector<Mat>            second_blocks;               // Declare blocks to save blocks of each image
    // Start a streaming pipe per each connected device
    for (auto&& dev : ctx.query_devices())
    {
        rs2::pipeline pipe(ctx);
        rs2::config cfg;
        cfg.enable_device(dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
        cfg.enable_stream(RS2_STREAM_DEPTH, w, h, RS2_FORMAT_Z16, 30);
        pipe.start(cfg);
        pipelines.emplace_back(pipe);

        // Map from each device's serial number to a different colorizer
       // colorizers[dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER)] = rs2::colorizer();
    }


    //Detect the number of connected cameras and generate a sufficient number of windows
    const auto window_name = "camera0";
    for (int w = 0; w != pipelines.size(); w++)
    {
        std::string j = std::to_string(w);
        const auto window_name = "camera" + j;
        namedWindow(window_name, WINDOW_AUTOSIZE);

    }
    int photo_count = 0;
    double alpha=1.0; /*< Simple contrast control 1.0-3.0 */
    int beta=80;  /*< Simple brightness control 0-100 */
    Mat compare0;
    Mat compare1;

    while ( getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0)
    {

        int camera_count = 0;
        first_blocks.clear();
        second_blocks.clear();
        
        for (auto&& pipe : pipelines)//This for loop refresh only one frame for each window
        {
            switch (camera_count){
                case 0: {

                    // Make sure the frames are spatially aligned
                    rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
                    rs2::frame depth = data.get_depth_frame().apply_filter(color_map);                // color map !!
                    Mat image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);    //color map!!
                    // Update the window with new data
                    Mat image_resize;
                    std::string j = std::to_string(camera_count);
                    const auto window_name = "camera" + j;
                    resize(image, image_resize, Size(image.cols / 2, image.rows / 2), 0, 0, INTER_LINEAR);
                    imshow(window_name, image_resize);


                    for (int i = 0; i < 4; i++)
                    {
                        switch (i) { // define the cut region a,b are start point; c,d are length.

                        case 0: {
                            a = 0;
                            b = 0;
                            c = w / 2;
                            d = h / 2;
                            break; }
                        case 1: {
                            a = w / 2;
                            b = 0;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }
                        case 2: {
                            a = 0;
                            b = h / 2;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }
                        case 3: {
                            a = w / 2;
                            b = h / 2;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }

                        }

                        std::string block_counter_string = std::to_string(i);
                        const auto block_name = j + "block" + block_counter_string;
                        Rect rect(a, b, c, d);
                        Mat image_ori = image(rect);
                       
                        /*std::cout << "this is first block" << std::endl;
                        std::cout << "block_name:" << block_name << std::endl;*/
                        first_blocks.push_back(image_ori); //save the image blocks as a vector.

                      /*  namedWindow(block_name, 1);
                        imshow(block_name, image_ori);*/
                    }
                    
                    cvtColor(first_blocks[1], compare0, COLOR_BGR2HSV);
                    imshow("compare_block_0", compare0);
                    
                    break;
                }
                case 1: {
                    // Make sure the frames are spatially aligned
                    rs2::frameset data1 = pipe.wait_for_frames(); // Wait for next set of frames from the camera
                    rs2::frame depth1 = data1.get_depth_frame().apply_filter(color_map);                // color map !!
                    Mat image1(Size(w, h), CV_8UC3, (void*)depth1.get_data(), Mat::AUTO_STEP);    //color map!!
                    // Update the window with new data
                    Mat image_resize;
                    std::string j = std::to_string(camera_count);
                    const auto window_name = "camera" + j;
                    resize(image1, image_resize, Size(image1.cols / 2, image1.rows / 2), 0, 0, INTER_LINEAR);
                    imshow(window_name, image_resize);

                    for (int i = 0; i < 4; i++)
                    {
                        switch (i) { // define the cut region a,b are start point; c,d are length.

                        case 0: {
                            a = 0;
                            b = 0;
                            c = w / 2;
                            d = h / 2;
                            break; }
                        case 1: {
                            a = w / 2;
                            b = 0;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }
                        case 2: {
                            a = 0;
                            b = h / 2;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }
                        case 3: {
                            a = w / 2;
                            b = h / 2;
                            c = w / 2;
                            d = h / 2;
                            break;
                        }

                        }

                        std::string block_counter_string = std::to_string(i);
                        const auto block_name = j + "block" + block_counter_string;
                        Rect rect(a, b, c, d);
                        Mat image_ori1 = image1(rect);

                       /* std::cout << "this is second block" << std::endl;
                        std::cout << "block_name:" << block_name << std::endl;*/
                        second_blocks.push_back(image_ori1); //save the image blocks as a vector.

                       /* namedWindow(block_name, 1);
                        imshow(block_name, image_ori1);*/
                    }
                    
                    cvtColor(second_blocks[1], compare1, COLOR_BGR2HSV);
                    imshow("compare_block_1", compare1);
                    break;
                }
            }
        
            camera_count = 1;
           

        }
       

        //std::cout << "pipelines size: " << pipelines.size() << std::endl;
        //std::cout <<"first_blocks size: "<<first_blocks.size()<<std::endl;
        //std::cout << "second_blocks size: " << second_blocks.size() << std::endl;

         

        

        int hBins = 256, sBins = 256, vBins = 256;
        int histSize[] = { hBins,sBins ,vBins };

        float hRanges[] = { 0,256 };
        float sRanges[] = { 0,255 };
        float vRanges[] = { 0,255 };
        const float* ranges[] = { hRanges,sRanges,vRanges };
        int channels[] = { 0,1,2 };
        MatND hist1, hist2;
        calcHist(&compare0, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false);
        normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
        calcHist(&compare1, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
        normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
        double similarityValue = compareHist(hist1, hist2, 4);
        std::cout << "Similarity: " << similarityValue << std::endl;
        if (similarityValue >= 1.0)
        {
            imshow("compare_block_0", compare0);
            imshow("compare_block_1", compare1);
        }
        else if (similarityValue < 1.0)
        {
            compare0.convertTo(compare0, -1, alpha, beta);
            imshow("compare_block_0", compare0);
            compare1.convertTo(compare1, -1, alpha, beta);
            imshow("compare_block_1", compare1);
        }


        compare = false;
        if (waitKey(1) == 115) {
            
            compare = true;
        }
    
        

            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$          save photo
             /*std::string photo_name ="depth_image" + std::to_string(photo_count) + ".png";
            imwrite(photo_name, image);
            photo_count += 1;
            std::cout << photo_name<<" -Saved " << std::endl;*/
            // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$   

           /* destroyWindow("block0");
            destroyWindow("block1");
            destroyWindow("block2");
            destroyWindow("block3");

            Rect rect(0, 0, a, b);
            Mat image_ori = image(rect);
            namedWindow("block_0", 1);
            imshow("block_0", image_ori);*/
           // waitKey(0);

            /*Rect rect1(a, 0, a, b);
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
            imshow("block_3", image_ori3);*/
            ////***********************************    brightness comparsion ********************************



            ////***********************************    Histogram comparsion ********************************
            //cvtColor(image_ori, image_ori, COLOR_BGR2HSV);
            //cvtColor(image_ori2, image_ori2, COLOR_BGR2HSV);
            //
            //int hBins = 256, sBins = 256, vBins=256;
            //int histSize[] = { hBins,sBins ,vBins};
            //
            //float hRanges[] = { 0,256 };
            //
            //float sRanges[] = { 0,255 };
            //float vRanges[] = { 0,255 };
            //const float* ranges[] = { hRanges,sRanges,vRanges };
            //int channels[] = { 0,1,2 };
            //MatND hist1, hist2;
            //calcHist(&image_ori, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false);
            //normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
            //calcHist(&image_ori2, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
            //normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
            //double similarityValue = compareHist(hist1, hist2, 4);
            //std::cout << "Similarity: " << similarityValue << std::endl;
            //if (similarityValue<1.0)
            //{
            //    image_ori.convertTo(image_ori, -1, alpha, beta);
            //    imshow("block_0", image_ori);
            //    image_ori2.convertTo(image_ori2, -1, alpha, beta);
            //    imshow("block_2", image_ori2);
            //}

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


            //waitKey(2);
            
          
        
        
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




















