/*
In control, the plant is the thing that is being controlled (aka the physical system)
- It can be a motor, a drone, robot arm, or a car's speed, etc...

Currently I am building a simulated plant: a double integrator with mass and damping
- Input: effort u (like a force)
- Physics: x" = u/m - bx'
- Output: state (usually position x, and maybe also velocity later)

Formula Breakdown
x" = 2nd derivative of position with respect to time (AKA Acceleration)
b = Damping coefficient (resistance or friction)
x' = 1st derviative of psoition with respect to time (Velocity (speed in a given direction))
u = External force or control input applied to the system

ROS Node Explanation
- A ROS node is just a small program that talks over topics (named message streams)

The goal of plant_node
1. Liston on /effort (topic) -- "here is the force that needs to be applied"
2. Simulate physics over time (integrate acceleration -> velocity -> position)
3. Publish on /state -- "here is where I am now"

colcon build --symlink-install --packages-select pid_ros --allow-overriding pid_core

ros2 topic pub /effort std_msgs/msg/Float64 "{data: 1.0}" --rate 10
ros2 topic echo /state

Resources
https://medium.com/@zakerima/setting-up-subscribers-in-ros2-control-lambda-vs-member-function-9bf06f7caa39
*/

#include <std_msgs/msg/detail/float64__struct.hpp>
#include <string>
#include <chrono>
#include <functional>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"

// class StatePublisher : public rclcpp::Node{
//     public:
//     // Create constructor
//     StatePublisher() 
//     : Node("state_publisher"), count_(0){
//         publisher_ = this->create_publisher<std_msgs::msg::Float64>("state", 10);
//         timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&StatePublisher::timer_callback, this));
//     }

//     private:
//         void timer_callback(){
//             auto message = std_msgs::msg::Float64();
//             message.data = 0.0 + count_++;
//             RCLCPP_INFO(this->get_logger(), "Publishing: %.1f", message.data);
//             publisher_->publish(message);
//         }
//         rclcpp::TimerBase::SharedPtr timer_;
//         rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
//         size_t count_;
// };

// class EffortSubscriber : public rclcpp::Node{
//     public:
//     EffortSubscriber()
//     : Node("effort_subscriber"){
//         subscription_ = this->create_subscription<std_msgs::msg::Float64>("effort", 10, std::bind(&EffortSubscriber::topic_callback, this, std::placeholders::_1));
//     }

//     private:
//         void topic_callback(const std_msgs::msg::Float64 & msg) {
//             RCLCPP_INFO(this->get_logger(), "I hear: %.1f", msg.data);
//             if(msg.data != effort_){ // optional log when effort changes
//                 RCLCPP_INFO(this->get_logger(), "Effort Changed: %.1f", msg.data);
//             }
//             effort_ = msg.data;
//         }

//         rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;

//         double effort_;
// };

class PlantNode : public rclcpp::Node{
    public:
    // Create constructor
    PlantNode() 
    : Node("plant_node"), tick_count_(0){
        // Create publisher and its corresponding callback
        publisher_ = this->create_publisher<std_msgs::msg::Float64>("state", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&PlantNode::timer_callback, this));

        // Create subscriber and its corresponding callback
        subscription_ = this->create_subscription<std_msgs::msg::Float64>("effort", 10, std::bind(&PlantNode::topic_callback, this, std::placeholders::_1));

    }

    private:
        void timer_callback(){ // publishes state information
            /*
            Newton's 2nd Law with linear drag:
                - Newton's 2nd Law: 
                    F = ma
                - Solve for acceleration:
                    a = F/m
                - Apply linear drag
                    a = F/m - b*v
            NOTES:
                - F is the applied Force in other words it is the "effort variable"
                - The -b*v term is viscious damping 
                    -- drag scales with how fast we're moving thus scaled by velocity (v)
                    -- The negative sign (-) means that it always opposes motion which makes sense for drag
            */
            tick_count_++;
            rclcpp::Time now = this->get_clock()->now();

            auto message = std_msgs::msg::Float64();

            double a = (effort_ / m_) - b_*v_;
            // Skip integration on first timer tick (when now == last_time_)
            if(tick_count_ > 1){ 
                double dt = (now - last_time_).seconds();
                // RCLCPP_INFO(this->get_logger(), "dt: %.3f", dt);
                v_ += a*dt; // update v_ (integrate)
                x_ += v_*dt; // update x_ (integrate)
            }
            message.data = x_;

            if(tick_count_ % 10 == 0){ // throttle log message to every 10th run
                RCLCPP_INFO(this->get_logger(), "Publishing: %.3f", message.data);
            }
            publisher_->publish(message);

            last_time_ = now;
        }

        void topic_callback(const std_msgs::msg::Float64 & msg) { //  Subscribes to /effort
            if(msg.data != effort_){ // optional log when effort changes
                RCLCPP_INFO(this->get_logger(), "Updated Effort: %.3f", msg.data);
            }
            effort_ = msg.data;
        }

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        size_t tick_count_;
        rclcpp::Time last_time_ = this->get_clock()->now();
        // physical model member functions
        double x_ = 0; // position
        double v_ = 0; // velocity
        double m_ = 1.0; // mass
        double b_ = 0.1; // damping

        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
        double effort_ = 0.0;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);

    // rclcpp::spin(std::make_shared<StatePublisher>());
    // rclcpp::spin(std::make_shared<EffortSubscriber>());
    rclcpp::spin(std::make_shared<PlantNode>());

    return 0;
}