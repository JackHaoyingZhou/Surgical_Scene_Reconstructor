#include "myproject/MyReconstr_Vision.h"

MyReconstr_Vision::MyReconstr_Vision()
{
	this->MY_FEATURE_ALGO = DEFAULT_FEATURE_ALGO;

	// set feature detection parameter values
	reset_feature_parameters();
}


MyReconstr_Vision::~MyReconstr_Vision()
{
}


void MyReconstr_Vision::reset_feature_parameters()
{
	// for FAST_ALGO
	this->FEATURE_PARAM_INTENSITY_THRES = DEFAULT_FEATURE_PARAM_INTENSITY_THRES;
	this->FEATURE_PARAM_NON_MAX_SUPPRE = DEFAULT_FEATURE_PARAM_NON_MAX_SUPPRE;	

	// for SURF_ALGO
	this->FEATURE_PARAM_HESSIAN_THRES = DEFAULT_FEATURE_PARAM_HESSIAN_THRES;
	this->FEATURE_PARAM_N_PYRAMIDS = DEFAULT_FEATURE_PARAM_N_PYRAMIDS;
	this->FEATURE_PARAM_N_PYRAMID_LAYERS = DEFAULT_FEATURE_PARAM_N_PYRAMID_LAYERS;
	this->FEATURE_PARAM_DESCRIPTOR_EXTENDED = DEFAULT_FEATURE_PARAM_DESCRIPTOR_EXTENDED;

	// for ORB_ALGO
	this->FEATURE_PARAM_N_FEATURES = DEFAULT_FEATURE_PARAM_N_FEATURES;
	this->FEATURE_PARAM_SCALE = DEFAULT_FEATURE_PARAM_SCALE;
	this->FEATURE_PARAM_N_LEVELS = DEFAULT_FEATURE_PARAM_N_LEVELS;
	this->FEATURE_PARAM_EDGE_THRES = DEFAULT_FEATURE_PARAM_EDGE_THRES;

	// initialize detectors
	create_feature_detectors();

	// clear feature points
	features_inter_cam.clear();
	features_intra_cam.clear();
}


void MyReconstr_Vision::set_feature_parameters_fast(int param1,bool param2)
{
	this->FEATURE_PARAM_INTENSITY_THRES = param1;
	this->FEATURE_PARAM_NON_MAX_SUPPRE = param2;

	create_feature_detectors();
}


void MyReconstr_Vision::set_feature_parameters_surf(double param1,int param2,int param3,bool param4)
{
	this->FEATURE_PARAM_HESSIAN_THRES = param1;
	this->FEATURE_PARAM_N_PYRAMIDS = param2;
	this->FEATURE_PARAM_N_PYRAMID_LAYERS = param3;
	this->FEATURE_PARAM_DESCRIPTOR_EXTENDED = param4;
	
	create_feature_detectors();
}


void MyReconstr_Vision::set_feature_parameters_orb(int param1,float param2,int param3,int param4)
{
	this->FEATURE_PARAM_N_FEATURES = param1;
	this->FEATURE_PARAM_SCALE = param2;
	this->FEATURE_PARAM_N_LEVELS = param3;
	this->FEATURE_PARAM_EDGE_THRES = param4;

	create_feature_detectors();
}


void MyReconstr_Vision::show_feature_parameters(FEATURE_ALGO_LIST my_algo)
{
	switch(my_algo)
	{
		case FAST_ALGO:
			CONSOLE.show_feature_detection_parameters_fast(FEATURE_PARAM_INTENSITY_THRES,FEATURE_PARAM_NON_MAX_SUPPRE);
			break;

		case SURF_ALGO:
			CONSOLE.show_feature_detection_parameters_surf(FEATURE_PARAM_HESSIAN_THRES,FEATURE_PARAM_N_PYRAMIDS,FEATURE_PARAM_N_PYRAMID_LAYERS,FEATURE_PARAM_DESCRIPTOR_EXTENDED);
			break;	

		case ORB_ALGO:
			CONSOLE.show_feature_detection_parameters_orb(FEATURE_PARAM_N_FEATURES,FEATURE_PARAM_SCALE,FEATURE_PARAM_N_LEVELS,FEATURE_PARAM_EDGE_THRES);
			break;

	}
}


void MyReconstr_Vision::show_all_feature_parameters()
{
	for(int i=0; i<NUM_OF_FEATURE_ALGO; i++)
	{
		show_feature_parameters((FEATURE_ALGO_LIST)i);
	}
}


void MyReconstr_Vision::create_feature_detectors()
{
	// ref: https://docs.opencv.org/3.2.0/df/d74/classcv_1_1FastFeatureDetector.html
	// ref: https://docs.opencv.org/3.4.1/d5/df7/classcv_1_1xfeatures2d_1_1SURF.html
	// ref: https://docs.opencv.org/3.4.0/db/d95/classcv_1_1ORB.html

	detector_fast = FastFeatureDetector::create(FEATURE_PARAM_INTENSITY_THRES,FEATURE_PARAM_NON_MAX_SUPPRE);
	detector_surf = xfeatures2d::SURF::create(FEATURE_PARAM_HESSIAN_THRES,FEATURE_PARAM_N_PYRAMIDS,FEATURE_PARAM_N_PYRAMID_LAYERS,this->FEATURE_PARAM_DESCRIPTOR_EXTENDED);
	detector_orb = ORB::create(FEATURE_PARAM_N_FEATURES,FEATURE_PARAM_SCALE,FEATURE_PARAM_N_LEVELS,FEATURE_PARAM_EDGE_THRES); 
}


void MyReconstr_Vision::set_feature_detection_algo(FEATURE_ALGO_LIST my_algo)
{
	this->MY_FEATURE_ALGO = my_algo;
	CONSOLE.set_feature_detection_algo(my_algo);
}


void MyReconstr_Vision::display_all_feature_algos(cv::Mat src)
{
	cv::Mat img;
	vector<cv::Mat> src_vec(NUM_OF_FEATURE_ALGO);
	for(int i=0; i<NUM_OF_FEATURE_ALGO; i++)
	{
		string ss = FEATURE_ALGO_TO_STRING((FEATURE_ALGO_LIST)i);
		Time t_start = ros::Time::now();
		vector<KeyPoint> kpts = feature_extraction(src,(FEATURE_ALGO_LIST)i);
		Time t_end = ros::Time::now();
		CONSOLE.show_runtime(ss,t_end-t_start);
		img = draw_feature_points(src,kpts);
		putText(img, ss, cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,0), 1, CV_AA);
		src_vec[i] = img.clone();
		
	}
	img = image_collage_maker(src_vec);
	imshow(OPENCV_IMSHOW_WINDOW_NAME,img);
	cv::waitKey(30);
}


vector<KeyPoint>  MyReconstr_Vision::feature_extraction(cv::Mat img, FEATURE_ALGO_LIST my_algo)
{
	vector<KeyPoint> kpts;

	switch(my_algo)
	{
		case FAST_ALGO:
			detector_fast->detect(img,kpts,Mat());
			break;

		case SURF_ALGO:
			detector_surf->detect(img,kpts);
			break;	

		case ORB_ALGO:
			detector_orb->detect(img,kpts,Mat());	
			break;

	}
	return kpts;
}


vector<KeyPoint>  MyReconstr_Vision::feature_extraction(cv::Mat img)
{
	vector<KeyPoint> kpts = feature_extraction(img,this->MY_FEATURE_ALGO);
	return kpts;
}


vector<vector<KeyPoint> >  MyReconstr_Vision::feature_extraction_for_images(vector<cv::Mat> img_vec)
{
	vector<vector<KeyPoint> > kpts_vec = feature_extraction_for_images(img_vec,this->MY_FEATURE_ALGO);
	return kpts_vec;
}


vector<vector<KeyPoint> >  MyReconstr_Vision::feature_extraction_for_images(vector<cv::Mat> img_vec, FEATURE_ALGO_LIST my_algo)
{
	int img_cnt = img_vec.size();
	vector<vector<KeyPoint> >  kpts_vec(img_cnt);

	switch(my_algo)
	{
		case FAST_ALGO:
			detector_fast->detect(img_vec,kpts_vec,Mat());
			break;

		case SURF_ALGO:
			detector_surf->detect(img_vec,kpts_vec);
			break;

		case ORB_ALGO:
			detector_orb->detect(img_vec,kpts_vec,Mat());
			break;
	}
	
	return kpts_vec;
}


cv::Mat MyReconstr_Vision::draw_feature_points(cv::Mat src,vector<KeyPoint> kpts)
{
	cv::Mat img;
	drawKeypoints(src, kpts, img);
	return img;
}


vector<cv::Mat> MyReconstr_Vision::draw_feature_points_for_images(vector<cv::Mat> src_vec,vector<vector<KeyPoint> > kpts_vec)
{
	vector<cv::Mat> img_vec;
	int img_cnt = src_vec.size();

	if(src_vec.size() != kpts_vec.size())
	{
		CONSOLE.visual_processing_error(0);
		exit(1);
	}
	else
	{
		for(int i=0; i<img_cnt; i++)
		{
			cv::Mat img = draw_feature_points(src_vec[i],kpts_vec[i]);
			img_vec.push_back(img.clone());
		}
	}
	
	return img_vec;
}


cv::Mat MyReconstr_Vision::image_collage_maker(vector<cv::Mat> src_vec)
{
	int img_cnt = src_vec.size();
	int rows = floor(sqrt(img_cnt));
	int cols = rows;
	cv::Mat img;

	while(cols*rows<img_cnt)
		cols = cols+1;
	vector<cv::Mat> img_vec(cols*rows);
	double img_scale = double(1.0*OPENCV_IMSHOW_WINDOW_SCALE/cols);

	for(int i=1; i<img_cnt; i++)
	{
		if(src_vec[i].rows != src_vec[0].rows || src_vec[i].cols != src_vec[0].cols)
		{
			CONSOLE.visual_processing_error(1);
			exit(1);
		}
	}

	for(int i=0; i<cols*rows; i++)
	{
		if(i<img_cnt)
			resize(src_vec[i], img_vec[i], cv::Size(), img_scale, img_scale);
		else
			img_vec[i] = cv::Mat(img_vec[i-1].rows,img_vec[i-1].cols,img_vec[i-1].type(), Scalar(0, 0, 0));
	}

	for(int r=0; r<rows; r++)
	for(int c=1; c<cols; c++)
		hconcat(img_vec[r*cols],img_vec[r*cols+c],img_vec[r*cols]);

	for(int r=1; r<rows; r++)
		vconcat(img_vec[0],img_vec[r*cols],img_vec[0]);

	img = img_vec[0].clone();

	return img;
}


cv::Mat MyReconstr_Vision::process_image2D(vector<cv::Mat> curr_imgs, vector<int> camera_group_labels) 
{
	cv::Mat image2D;
	features_inter_cam.clear();
	features_intra_cam.clear();

	if(camera_group_labels.size() != curr_imgs.size())
	{
		CONSOLE.visual_processing_error(2);
		return image2D;
		
	}

	// (0) compute camera matches combinations
	vector<vector<int> > camera_matches_comb = IMGFUNC.constrained_combination(camera_group_labels,false); // set: true for debugging

	// (1) feature point extraction
	vector<vector<KeyPoint> >  kpts_vec = feature_extraction_for_images(curr_imgs);
	
	
	// (2) feature point tracking (inter-camera: across cameras, no memory)
	features_inter_cam = feature_tracking_inter_camera(kpts_vec,curr_imgs,camera_matches_comb); //<--- ToDo: we're here

	// (3) feature point tracking (intra-camera: across time, memory dependent)
	//features_intra_cam = feature_tracking_intra_camera(kpts_vec,curr_imgs); // ToDo: think about what to result...?
	

	// (4) manage display 
	vector<cv::Mat> img_vec = draw_feature_points_for_images(curr_imgs,kpts_vec); // for (1): print out feature points
	// for (2): use color to show the feature point matches between cameras
	// for (3): use arrow to show the motion of feature points over time
	image2D = image_collage_maker(img_vec); // merging images


	// ToDo: think about addressing camera pose uncertainty ... in (2) and (3)


	// [Other ideas and notes:]
	// - Can look up DescriptorExtractor.
	// - Do the following to access each element of image:
	//   int r=10; int c=24; int b=1; // BGR
	//   Images2D.at<cv::Vec3b>(r,c)[b] = 27;

	return image2D;
}


PointCloud<PointXYZRGB> MyReconstr_Vision::process_model3D() // Next ToDo~
{
	PointCloud<PointXYZRGB> model3D;
	model3D.clear(); //

	// (5) feature point registration (associate feature points IDs) - need to design a data structure for the 2D 3D point relation
	// ToDo: what about accumulative error in camera pose? 
	// when to modify the slow drift from true cam pose? in (5)? 
	// when intra cam tracks are right but majority of points are classified as dynamic	

	// (6) feature point classification (distinguish static and dynamic points)
	 
	// (7) map building --> could be put to process_model3D() function


	// ToDo: Replace the following baby script with real stuff!
	PointXYZRGB pt1, pt2;
	pt1.x = 1;	pt1.y = 2;	pt1.z = 50;	pt1.r = 100;	pt1.g = 100;	pt1.b = 125;
	pt2.x = 1;	pt2.y = 57;	pt2.z = 5;  	pt2.r = 223;	pt2.g = 12;	pt2.b = 50;
	model3D.points.push_back(pt1);
	model3D.points.push_back(pt2);

	// [Other ideas and notes:]
	// - More info about point cloud: http://docs.pointclouds.org/trunk/classpcl_1_1_point_cloud.html
	// - what does model3D.clear() do? 
	//   Removes all points in a cloud and sets the width and height to 0.
	// - Do the following to access each element of 3D model:
	//   int pt_idx = 2;
	//   Model3D.points[pt_idx].x , Model3D.points[pt_idx].y , Model3D.points[pt_idx].z
	//   Model3D.points[pt_idx].r , Model3D.points[pt_idx].g , Model3D.points[pt_idx].b

	return model3D;
}


vector<vector<KeyPoint> > MyReconstr_Vision::feature_tracking_inter_camera(vector<vector<KeyPoint> > kpts_vec, vector<cv::Mat> curr_imgs,vector<vector<int> > cam_comb_idx)
{
	// Goal: feature point tracking (inter-camera: across cameras, no memory)
	//       use camera combo information to find feature point matches
	// ref: https://github.com/Tetragramm/opencv_contrib/blob/master/modules/mapping3d/samples/computeMapping3d.cpp

	vector<vector<KeyPoint> > kpts_inter_cam;

	return kpts_inter_cam;
	
}


