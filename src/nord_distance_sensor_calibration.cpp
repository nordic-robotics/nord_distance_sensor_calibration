#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <sstream>
#include "ras_arduino_msgs/ADConverter.h"
#include "nord_messages/DistanceCalibration.h"

/**
 * 
 * 
 * Author: Tobias Lundin
 */

class DistanceSensorCalibration
{
	public:
		ros::NodeHandle n;
		ros::Publisher distance_pub;
		ros::Subscriber adc_sub;
		nord_messages::DistanceCalibration output;

	DistanceSensorCalibration()
	{
		distance_pub = n.advertise<nord_messages::DistanceCalibration>("/distance_sensor_test_node/distance", 1);
		adc_sub = n.subscribe("/arduino/adc", 1,&DistanceSensorCalibration::adcMsgCallback, this);
		distance = 0; counter = 0;

		adc1 = 0; adc2 = 0; adc3 = 0; adc4 = 0;
		adc5 = 0; adc6 = 0; adc7 = 0; adc8 = 0;
	}

	void adcMsgCallback(const ras_arduino_msgs::ADConverter adc_values)
	{
  		adc1 = adc_values.ch1; 
		adc2 = adc_values.ch2; 
  		adc3 = adc_values.ch3; 
		adc4 = adc_values.ch4;
  		adc5 = adc_values.ch5; 
		adc6 = adc_values.ch6;
  		adc7 = adc_values.ch7; 
		adc8 = adc_values.ch8;
		print_info();

	}
	void changeDistance(){

		output.meter = distance/100.0;
		ROS_INFO("Current distance is: %f cm", output.meter*100);
		output.adc_1 = adc1; output.adc_2 = adc2; 
		output.adc_3 = adc3; output.adc_4 = adc4;
		output.adc_5 = adc5; output.adc_6 = adc6;
		output.adc_7 = adc7; output.adc_8 = adc8;
		print_info();
		counter +=1;

		if(counter>30){
			distance += 5;
			counter   = 0;
			ros::Duration(6).sleep();
		}

		distance_pub.publish(output);
		if(distance > 80){
			distance = 0;
		}
	}

	void print_info(){
		ROS_INFO("Distance: %d ", distance);
		ROS_INFO("Rfront: [%d]  Rback: [%d]", adc8,adc7);
		ROS_INFO("Lfront: [%d]  Lback: [%d]", adc1,adc3);
		ROS_INFO(" Front: [%d]   Back: [%d]", adc6,adc5);
	}

	private:
		int distance;    int counter;
		 unsigned int short adc1;	 unsigned int short adc2;
		 unsigned int short adc3;	 unsigned int short adc4;
		 unsigned int short adc5;  	 unsigned int short adc6;
		 unsigned int short adc7;	 unsigned int short adc8;
		
};



 int main(int argc, char **argv)
{

	ros::init(argc, argv, "nord_distance_sensor_calibration");
	
	
	DistanceSensorCalibration calib_obj;
	ros::Rate loop_rate(20);


	// ~ while everything is running as it should
	while(ros::ok()){
		ros::spinOnce(); //added but not really needed
		calib_obj.changeDistance();
		loop_rate.sleep(); // go to sleep

	}

	return 0;
}	
