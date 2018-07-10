#pragma once
#pragma once
#include <iostream>
#include <memory>
#include<map>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

// For this code to work the major street should be coded in a particular way. 
using namespace std;

class detector_call
{
	//This shared pointer points to an object to class vehicles
	//typedef std::shared_ptr<vehicles> shared_veh;
	// Stores the vehicle which are on major road
	//std::map<int, shared_veh> veh_map;
	// Stores the position of signal heads from the starting of the link
	const double signal_head_pos_2, signal_head_pos_6;
public:
	detector_call(double sig_head_2_pos_, double sig_head_6_pos);
	bool is_call_needed(int route_decision_no, double veh_pos, double veh_speed);
	~detector_call();
};

