#include "detector_call.h"



detector_call::detector_call(double sig_head_2_pos_, double sig_head_6_pos) :signal_head_pos_2(sig_head_2_pos_), signal_head_pos_6(sig_head_6_pos)
{
}



bool detector_call::is_call_needed(int route_decision_no, double veh_pos, double veh_speed, double veh_len)
{
	bool put_call = false;
	double signal_head_pos = -9999;
	if (route_decision_no == 60)
		signal_head_pos = signal_head_pos_6;
	else //route_decision_no=20
		signal_head_pos = signal_head_pos_2;
	double veh_speed_mph = veh_speed / 1.46667;

	//2nd condition - Check if vehicle has crossed the signal 
	// Check if veh is 6 sec from the stopbar
	//3rd condition - Check if vehicle speed is greater than 30 mph //Edit 12/07 - Speed>45 Mph
	//4rth condition - Check if distance of vehicle from signal head is equal to 4 seconds (spacing)
	//5th condition - Check if distance of vehicle from signal head is equal to 5.5 seconds (spacing)
	if (
		veh_pos <= signal_head_pos&&
		signal_head_pos - (veh_pos)<=6*(veh_speed) &&
		veh_speed_mph>45 &&
		signal_head_pos - (veh_pos)>=((veh_speed)*5.5-veh_len) && signal_head_pos - (veh_pos) <= (6+(veh_speed)*5.5) &&
		signal_head_pos - (veh_pos)>=((veh_speed)*4-veh_len) && signal_head_pos - (veh_pos) <= (6+(veh_speed)*4)) {
		put_call = true;
			cout << "Vehicle position from signal head" << signal_head_pos - (veh_pos)<<endl;
		cout<<"2.5 seconds from the stop bar" << (veh_speed)*2.5<<endl;
		cout << "5.5 seconds from the stop bar" << (veh_speed)*5.5<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	return put_call;
}



detector_call::~detector_call()
{
}






//My initial approach- Wrong 

/*
void detector_call::add_vehicles(int veh_num_, double veh_speed_, double veh_pos_, int veh_route_decision_no_)
{
shared_veh veh=make_shared<vehicles>();
if (veh_map.count(veh_num_)==0) {
veh->set_values(veh_num_, veh_speed_, veh_pos_, veh_route_decision_no_);
veh_map.insert(std::pair<int, shared_veh>(veh_num_, veh));
}
else {
veh_map[veh_num_]->set_values(veh_num_, veh_speed_, veh_pos_, veh_route_decision_no_);
}
}
*/

/*

bool detector_call::is_call_needed(int route_decision_no)
{
bool put_call = false;
for (auto it = veh_map.begin(); it != veh_map.end(); it++) {
double veh_speed_mph = (it->second->veh_speed) / 1.46667;

double signal_head_pos;
if (route_decision_no == 60)
signal_head_pos = signal_head_pos_6;
else //route_decision_no=20
signal_head_pos = signal_head_pos_2;

//1st condition - Check the detector (route_decision_no=1 is for detector 100
//2nd condition - Check if vehicle has crossed the signal
//3rd condition - Check if vehicle speed is greater than 30 mph
//4rth condition - Check if distance of vehicle from signal head is greater than 2.5 seconds (spacing)
//5th condition - Check if distance of vehicle from signal head is less than 5.5 seconds (spacing)
if (it->second->veh_route_decision_no == route_decision_no&&
it->second->veh_pos <= signal_head_pos&&
veh_speed_mph>30 &&
signal_head_pos - (it->second->veh_pos)>(it->second->veh_speed)*2.5 &&
signal_head_pos - (it->second->veh_pos)< (it->second->veh_speed)*5.5) {
put_call = true;
cout << "Vehicle position from signal head" << signal_head_pos - (it->second->veh_pos);
cout << "2.5 seconds from the stop bar" << (it->second->veh_speed)*2.5;
cout << "5.5 seconds from the stop bar" << (it->second->veh_speed)*5.5;
std::this_thread::sleep_for(std::chrono::seconds(1));
break;
}
}
return put_call;
}
*/
