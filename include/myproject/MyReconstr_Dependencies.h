#ifndef MYRECONSTR_DEPENDENCIES_H
#define MYRECONSTR_DEPENDENCIES_H

// dependencies on c++ libraries
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <time.h> 
#include <stdio.h>
#include <iomanip>
#include <pthread.h>
#include <termios.h>
#include <signal.h>
#include <queue>
#include <unistd.h>

// dependencies on ros packages
#include <ros/ros.h>
#include <ros/transport_hints.h>
#include <tf/transform_datatypes.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>

//PCL
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/ply_io.h>
#include <pcl_conversions/pcl_conversions.h>

// eliminate the deprecated headers warning
#define VTK_EXCLUDE_STRSTREAM_HEADERS
#include <pcl/io/vtk_lib_io.h>

// dependencies on opencv libraries
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv/cv.h>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/utility.hpp"

#define ROS_PARAMETER_NAME0 "/Surgical_Scene_Reconstructor/CAMERA_COUNT"
#define ROS_PARAMETER_NAME1 "/Surgical_Scene_Reconstructor/IMG_FOLDER"
#define ROS_PARAMETER_NAME2 "/Surgical_Scene_Reconstructor/TME_STAMP"
#define ROS_PARAMETER_NAME3 "/Surgical_Scene_Reconstructor/CAM_POSE"
#define ROS_PARAMETER_NAME4 "/Surgical_Scene_Reconstructor/CAM_CALI"

#define ROS_TOPIC_PUBLISH_NAME1 "/Surgical_Scene_Reconstructor/2D_images"
#define ROS_TOPIC_PUBLISH_NAME2 "/Surgical_Scene_Reconstructor/3D_model"

#define CONSOLE_LOOP_RATE   10     // in Hz
#define IO_LOOP_RATE        30     // in Hz


enum IMAGE_STATUS_LIST{
	IMAGE_EMPTY,       // 0
	IMAGE_START_LOADING,     // 1
	IMAGE_DONE_LOADING,      // 2
	IMAGE_START_PROCESSING,  // 3
	IMAGE_DONE_PROCESSING,   // 4
	IMAGE_START_PUBLISHING,  // 5
	IMAGE_DONE_PUBLISHING    // 6
};

enum MODEL_STATUS_LIST{
	MODEL_EMPTY,       // 0
	MODEL_START_PROCESSING,  // 1
	MODEL_DONE_PROCESSING,   // 2
	MODEL_START_PUBLISHING,  // 3
	MODEL_DONE_PUBLISHING    // 4
};

enum SYSTEM_STATUS_LIST{
	SYSTEM_JUST_STARTING,                   // 0
	SYSTEM_SHOW_MENU,                       // 1
	SYSTEM_PENDING_USER_SELECTION,          // 2
	SYSTEM_CAMERA_CALIBRATE_MODE,           // 3
	SYSTEM_CAMERA_CALIBRATE_PNP_READY_MODE, // 4
	SYSTEM_EXIT_ALL,                        // 5
};

// namespace declaration
using namespace std;
using namespace cv;
using namespace ros;
using namespace pcl;
using namespace pcl::io;
using namespace pcl_conversions;

#endif
