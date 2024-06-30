/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

void logMetrics(const string &detectorType, const string &descriptorType, int keypoints, int matches, double detector_time, double descriptor_time)
{
    ofstream logFile("performance_metrics.csv", ios::app);
    logFile << detectorType << "," << descriptorType << "," << keypoints << "," << matches << "," << detector_time << "," << descriptor_time << "\n";
    logFile.close();
}

void clearLogFile()
{
    ofstream logFile("performance_metrics.csv");
    logFile << "Detector,Descriptor,Keypoints,Matches,Detector Time(ms),Descriptor Time(ms)\n";
    logFile.close();
}

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */
    clearLogFile();

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time - 2 for const vel model, 3 for const accel model
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

    /* MAIN LOOP OVER ALL IMAGES */
    // Detector and descriptor types
    vector<string> detectorTypes = {"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
    vector<string> descriptorTypes = {"BRIEF", "ORB", "FREAK", "AKAZE", "SIFT"};

    // Main loop over all images
    for (const string &detectorType : detectorTypes)
    {
        for (const string &descriptorType : descriptorTypes)
        {
            // Reset data buffer for each combination
            dataBuffer.clear();

            if ((descriptorType == "AKAZE" && detectorType != "AKAZE") ||
                (detectorType == "AKAZE" && descriptorType != "AKAZE") ||
                (detectorType == "SIFT" && descriptorType == "ORB"))
            {
                continue; // Skip incompatible combinations
            }

            for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
            {
                /* LOAD IMAGE INTO BUFFER */

                // assemble filenames for current index
                ostringstream imgNumber;
                imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
                string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

                // load image from file and convert to grayscale
                cv::Mat img, imgGray;
                img = cv::imread(imgFullFilename);
                cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

                //// STUDENT ASSIGNMENT
                //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize
                if (dataBuffer.size() > dataBufferSize)
                {
                    dataBuffer.erase(dataBuffer.begin());
                    cout << "Erased first in image in buffer." << endl;
                }

                // push image into data frame buffer
                DataFrame frame;
                frame.cameraImg = imgGray;
                dataBuffer.push_back(frame);

                //// EOF STUDENT ASSIGNMENT
                cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

                /* DETECT IMAGE KEYPOINTS */

                // extract 2D keypoints from current image
                vector<cv::KeyPoint> keypoints; // create empty feature list for current image
                // string detectorType = "BRISK";  //"SIFT";//"BRISK";//"HARRIS";//"SHITOMASI";

                //// STUDENT ASSIGNMENT
                //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
                //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT
                // auto detector_start = high_resolution
                double detector_time = (double)cv::getTickCount();

                if (detectorType.compare("SHITOMASI") == 0)
                {
                    detKeypointsShiTomasi(keypoints, imgGray, false);
                }
                else if (detectorType.compare("HARRIS") == 0)
                {
                    detKeypointsHarris(keypoints, imgGray, false);
                }
                else
                {
                    detKeypointsModern(keypoints, imgGray, detectorType, false);
                }

                detector_time = 1000.0 * ((double)cv::getTickCount() - detector_time) / cv::getTickFrequency();
                //// EOF STUDENT ASSIGNMENT

                img.release();
                imgGray.release();
                //// STUDENT ASSIGNMENT
                //// TASK MP.3 -> only keep keypoints on the preceding vehicle

                // only keep keypoints on the preceding vehicle
                bool bFocusOnVehicle = true;
                cv::Rect vehicleRect(535, 180, 180, 150);
                if (bFocusOnVehicle)
                {
                    filterKeypoints(keypoints, vehicleRect); // disable for final project
                }

                //// EOF STUDENT ASSIGNMENT

                // optional : limit number of keypoints (helpful for debugging and learning)
                bool bLimitKpts = false;
                if (bLimitKpts)
                {
                    int maxKeypoints = 50;

                    if (detectorType.compare("SHITOMASI") == 0)
                    { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                        keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
                    }
                    cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
                    cout << " NOTE: Keypoints have been limited!" << endl;
                }

                // push keypoints and descriptor for current frame to end of data buffer
                (dataBuffer.end() - 1)->keypoints = keypoints;
                cout << "#2 : DETECT KEYPOINTS done" << endl;

                /* EXTRACT KEYPOINT DESCRIPTORS */

                //// STUDENT ASSIGNMENT
                //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
                //// -> BRIEF, ORB, FREAK, AKAZE, SIFT
                cv::Mat descriptors;
                double descriptor_time = (double)cv::getTickCount();
                // string descriptorType = "BRISK"; // BRIEF, ORB, FREAK, AKAZE, SIFT
                descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
                descriptor_time = 1000.0 * ((double)cv::getTickCount() - descriptor_time) / cv::getTickFrequency();
                //// EOF STUDENT ASSIGNMENT

                // push descriptors for current frame to end of data buffer
                (dataBuffer.end() - 1)->descriptors = descriptors;

                cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

                if (dataBuffer.size() > 1) // wait until at least two images have been processed
                {

                    /* MATCH KEYPOINT DESCRIPTORS */

                    vector<cv::DMatch> matches;
                    string descType = (descriptorType == "SIFT" || descriptorType == "SURF") ? "DES_HOG" : "DES_BINARY";
                    string matcherType = (descType == "DES_HOG") ? "MAT_FLANN" : "MAT_BF";

                    // string matcherType = "MAT_BF";   // MAT_BF, MAT_FLANN
                    // string descType = "DES_BINARY";  // DES_BINARY, DES_HOG
                    string selectorType = "SEL_KNN"; // SEL_NN, SEL_KNN

                    //// STUDENT ASSIGNMENT
                    //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
                    //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

                    matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                                     (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                                     matches, descType, matcherType, selectorType);

                    //// EOF STUDENT ASSIGNMENT

                    // store matches in current data frame
                    (dataBuffer.end() - 1)->kptMatches = matches;

                    cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

                    // Log metrics
                    logMetrics(detectorType, descriptorType, keypoints.size(), matches.size(), detector_time, descriptor_time);

                    // visualize matches between current and previous image
                    bVis = false;
                    if (bVis)
                    {
                        cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                        cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                        (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                        matches, matchImg,
                                        cv::Scalar::all(-1), cv::Scalar::all(-1),
                                        vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                        string windowName = "Matching keypoints between two camera images";
                        cv::namedWindow(windowName, 7);
                        cv::imshow(windowName, matchImg);
                        cout << "Press key to continue to next image" << endl;
                        cv::waitKey(0); // wait for key to be pressed

                        matchImg.release();
                    }
                    bVis = false;
                }

            } // eof loop over all images
        }
    }
    return 0;
}