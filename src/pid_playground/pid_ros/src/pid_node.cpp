/*
Read /setpoint and /state, compute effort with pid_core::PidController, and publish /effort
*/

#include <chrono>
#include <std_msgs/msg/detail/float64__struct.hpp>
#include <functional> // std::bind

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

#include "pid_core/pid_controller.hpp"

class PidNode : public rclcpp::Node{
    public:
        PidNode()
        : Node("pid_node"), controller_(kp_, ki_, kd_){
            
            // Create 2 subscribers
            /*
            Note in Modern C++ the better altnernative for readibility is lambda functions instead of std::bind
            */
            setpoint_subscription_ = this->create_subscription<std_msgs::msg::Float64>("setpoint", 10, std::bind(&PidNode::read_setpoint, this, std::placeholders::_1));
            state_subscription_ = this->create_subscription<std_msgs::msg::Float64>("state", 10, std::bind(&PidNode::read_state, this, std::placeholders::_1));

            // Create publisher that publishes on /effort
            effort_publisher_ = this->create_publisher<std_msgs::msg::Float64>("effort", 10);
            timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&PidNode::compute_effort, this));

            // Setting output limits
            controller_.set_output_limits(-50, 50);
        }
    private:
        // 2 Subscribers that stash the latest values from /setpoint and /state topics
        void read_setpoint(const std_msgs::msg::Float64 & msg){ // get current desired value from /setpoint
            setpoint_ = msg.data; //stash to memeber variable
        }
        void read_state(const std_msgs::msg::Float64 & msg){ // get current measured value from /state
            state_ = msg.data;
        }
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr setpoint_subscription_;
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr state_subscription_;
        double setpoint_ = 0.0;
        double state_ = 0.0; // (measurement_)

        // Publisher on /effort + timer
        void compute_effort(){ // Computes and publishes on /effort
            tick_count_++;
            rclcpp::Time now = this->get_clock()->now();

            if(tick_count_ <= 1){ // skip first tick
                last_time_ = now;
                return; /* early exit */
            } 
            
            double dt = (now - last_time_).seconds();
            
            auto message = std_msgs::msg::Float64();
            message.data = controller_.update(setpoint_, state_, dt);
            effort_publisher_->publish(message);

            last_time_ = now;
        }
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr effort_publisher_;
        size_t tick_count_ = 0;
        rclcpp::Time last_time_ = this->get_clock()->now();

        // pid controller
        double kp_ = 1.0;
        double ki_ = 0.0;
        double kd_ = 0.0;
        pid_core::PidController controller_; // namespace::Class variable_name
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PidNode>());
    rclcpp::shutdown();
    return 0;
}