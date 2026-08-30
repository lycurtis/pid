/*
Publish a desired position on /setpoint (std_msgs/msgs/Float64)

Building
colcon build --symlink-install --packages-select pid_ros --allow-overriding pid_core

Check/Run
ros2 run pid_ros setpoint_node
ros2 topic echo /setpoint
*/
#include <chrono>
#include <std_msgs/msg/detail/float64__struct.hpp>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

class SetPointNode : public rclcpp::Node{
    public:
        SetPointNode()
        : Node("setpoint_node"){
            publisher_ = this->create_publisher<std_msgs::msg::Float64>("setpoint", 10);
            timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SetPointNode::timer_callback, this));
        }
    private:
        void timer_callback(){
            auto message = std_msgs::msg::Float64();
            message.data = setpoint_;
            RCLCPP_INFO(this->get_logger(), "Set Point: %.1f", message.data);
            publisher_->publish(message);
        }
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;

        double setpoint_ = 1.0;
};

int main(int argc, char* argv[]){ // argc == argument count, argv is array of arguments (char** argv = char* argv[])
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SetPointNode>());
    rclcpp::shutdown();
    return 0;
}