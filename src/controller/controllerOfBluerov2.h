//
// Created by tim-linux on 22.12.21.
//
#include <ros/ros.h>
#include "commonbluerovmsg/desiredStateForRobot.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/TwistWithCovarianceStamped.h"
#include "mavros_msgs/AttitudeTarget.h"
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <Eigen/Geometry>
#include <Eigen/StdVector>
#include <bluerov2common/controllerConfig.h>
#include <dynamic_reconfigure/server.h>

#ifndef BLUEROV2COMMON_CONTROLLEROFBLUEROV2_H
#define BLUEROV2COMMON_CONTROLLEROFBLUEROV2_H


class controllerOfBluerov2 {
public:
    controllerOfBluerov2(ros::NodeHandle n_) {
        subscriberDesiredState = n_.subscribe("desiredStateOfBluerov2", 1000,
                                              &controllerOfBluerov2::desiredStateCallback, this);
        subscriberCurrentPose = n_.subscribe("publisherPoseEkf", 1000, &controllerOfBluerov2::currentPoseCallback,
                                             this);
        subscriberCurrentTwist = n_.subscribe("publisherTwistEkf", 1000, &controllerOfBluerov2::currentTwistCallback,
                                              this);

        publisherMavros = n_.advertise<mavros_msgs::AttitudeTarget>("mavros/setpoint_raw/attitude", 10);
        this->integratorHeight = 0;
        this->integratorX = 0;
        this->integratorY = 0;

        this->desiredXThrustBody = 0;
        this->desiredDepth = 0;
        this->desiredYaw = 0;
        this->desiredXThrustBody = 0;
        this->desiredYThrustBody = 0;
        this->desiredRoll = 0;
        this->desiredPitch = 0;

        //startControlLogic with outside Thread
    }

    Eigen::Vector3d controllLogic();

    static Eigen::Vector3d getRollPitchYaw(Eigen::Quaterniond quat);

    static Eigen::Quaterniond getQuaternionFromRPY(double roll, double pitch, double yaw);

    static Eigen::Quaterniond getQuaternionForMavrosFromRPY(double roll, double pitch, double yaw);

    Eigen::Vector3d getThrustForMavros(double thrust_1, double thrust_2, double thrust_3);

    double calculateDepthThrust(double desiredDepthTMP);

    void setControllerValues(double height_i_tmp, double height_d_tmp, double height_p_tmp, double hold_position_p_tmp,
                             double hold_position_i_tmp, double hold_position_d_tmp);

    void callbackReconfiguration(bluerov2common::controllerConfig &config, uint32_t level);

    void getPoseRobot( Eigen::Vector3d &position, Eigen::Quaterniond &rotation);

    void getPoseTarget(Eigen::Vector3d &position, Eigen::Quaterniond &rotation);

private:
    ros::Subscriber subscriberDesiredState, subscriberCurrentPose, subscriberCurrentTwist;
    ros::Publisher publisherMavros;
    //Pose
    std::atomic<double> currentXPosition, currentYPosition, currentDepth, currentRoll, currentPitch, currentYaw;
    //Twist
    std::atomic<double> currentXVel, currentYVel, currentDepthVel, currentRollVel, currentPitchVel, currentYawVel;
    //Desired
    std::atomic<double> desiredXThrustBody, desiredYThrustBody, desiredDepth, desiredRoll, desiredPitch, desiredYaw;//

    std::atomic<bool> holdPosition;

    //StateToHold:
    std::atomic<double> holdXPosition, holdYPosition, holdDepth, holdRoll, holdPitch, holdYaw;
    //for Control:
    std::atomic<double> integratorHeight,integratorX,integratorY;
    // control parameter
    std::atomic<double> height_i, height_d, height_p, hold_position_p, hold_position_i, hold_position_d;


    //functions
    void desiredStateCallback(const commonbluerovmsg::desiredStateForRobot::ConstPtr &msg);

    void currentTwistCallback(const geometry_msgs::TwistWithCovarianceStamped::ConstPtr &msg);

    void currentPoseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg);


};


#endif //BLUEROV2COMMON_CONTROLLEROFBLUEROV2_H
