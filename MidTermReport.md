# Mid-Term Report

## Overview

This report outlines how each criterion for the mid-term project was addressed. The project involves implementing and evaluating various keypoint detection and descriptor techniques for image processing.

## Submission Requirements and Solutions


| **Criteria** | **Submission Requirements** | **Filename** | **Line Numbers** | **Solution** |
|--------------|-----------------------------|-------------|------------------|-------------|
| **MP.0 Mid-Term Report** | Provide a Writeup / README that includes all the rubric points and how you addressed each one. | `MIDTERM_REPORT.md` | - | The report is provided in Markdown format and includes sections addressing each criterion. |
| **MP.1 Data Buffer Optimization** | Implement a vector for dataBuffer objects whose size does not exceed a limit (e.g. 2 elements). This can be achieved by pushing in new elements on one end and removing elements on the other end. | `MidTermProject_Camera_Student.cpp` | 95-99 | Implemented a circular buffer with a maximum size of 2 for the `dataBuffer` objects. |
| **MP.2 Keypoint Detection** | Implement detectors HARRIS, FAST, BRISK, ORB, AKAZE, and SIFT and make them selectable by setting a string accordingly. | `MidTermProject_Camera_Student.cpp`<br> <br>`matching2D_Student.cpp` | 61-77 121-132 <br> 139-225 | Developed functions for Harris detector and modern OpenCV detectors along with a mechanism to loop through the detectors using a vector of strings. |
| **MP.3 Keypoint Removal** | Remove all keypoints outside of a pre-defined rectangle and only use the keypoints within the rectangle for further processing. | `MidTermProject_Camera_Student.cpp` <br> `matching2D_Student.cpp` | 142-148, 227-238 | Added functionality to filter keypoints based on their position relative to a predefined rectangle. |
| **MP.4 Keypoint Descriptors** | Implement descriptors BRIEF, ORB, FREAK, AKAZE, and SIFT and make them selectable by setting a string accordingly. | `MidTermProject_Camera_Student.cpp` | 178 | Implemented descriptor methods and provided a way to call them via a string parameter. |
| **MP.5 Descriptor Matching** | Implement FLANN matching as well as k-nearest neighbor selection. Both methods must be selectable using the respective strings in the main function. | `MidTermProject_Camera_Student.cpp` | 192-206 | Added support for FLANN-based matching and k-nearest neighbor selection, selectable through string input. |
| **MP.6 Descriptor Distance Ratio** | Use the K-Nearest-Neighbor matching to implement the descriptor distance ratio test, which looks at the ratio of best vs. second-best match to decide whether to keep an associated pair of keypoints. | `matching2D_Student.cpp` | 42-56 | Implemented the ratio test in the K-Nearest-Neighbor matching process to filter keypoint matches. |
| **MP.7 Performance Evaluation 1** | Count the number of keypoints on the preceding vehicle for all 10 images and take note of the distribution of their neighborhood size. Do this for all the detectors you have implemented. | `sorted_performance_metrics.csv` |   | Analyzed 10 images, counted keypoints for each detector. Note, to obtain the sorted file run `sort_results.sh` |
| **MP.8 Performance Evaluation 2** | Count the number of matched keypoints for all 10 images using all possible combinations of detectors and descriptors. In the matching step, the BF approach is used with the descriptor distance ratio set to 0.8. | `sorted_performance_metrics.cpp` |  | Evaluated keypoint matching for all detector-descriptor combinations and used the BF matcher with a distance ratio of 0.8. Note, to obtain the sorted file run `sort_results.sh` |
| **MP.9 Performance Evaluation 3** | Log the time it takes for keypoint detection and descriptor extraction. The results must be entered into a spreadsheet and based on this data, the TOP3 detector / descriptor combinations must be recommended as the best choice for our purpose of detecting keypoints on vehicles. | `sorted_performance_metrics.csv` |  | Measured and logged the time for detection and extraction processes. Analyzed results to recommend the top 3 detector-descriptor combinations as follows: 1. FAST/BRIEF. 2. FAST/ORB 3. ORB/BRIEF . |


