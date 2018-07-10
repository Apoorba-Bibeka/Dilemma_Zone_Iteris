//= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
//Changing the detection value for high speed intersections. 
//Apoorba Bibeka 2016
//Date: Dec 9 2016
//Date Modified: July 27 2017
// Date Modified : July 10 2018
// -----------------
//Adding dilemma zone detection with Iteris Detector
//= == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =


// import the *.exe file providing the COM interface 
// use your Vissim installation path here as the example shows 
//#import "C:\\Program Files (x86)\\PTV Vision\\PTV Vissim 7\\Exe\\Vissim.exe" rename_namespace ("VISSIMLIB") 
#import "C:\\Program Files\\PTV Vision\\PTV Vissim 9\\Exe\\VISSIM90.exe" rename_namespace ("VISSIMLIB") 

// #include <string>
#include <iostream>
#include <iomanip>
#include "detector_call.h"
#include "Source.h"
#include <fstream>
#include<chrono>

// using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define DETECTION 4
#define NO_DETECTION 1

int main(int argc, char* argv[]) {
	auto start = std::chrono::high_resolution_clock::now();
	// initialize COM 
	CoInitialize(NULL); {
		// create Vissim object 
		VISSIMLIB::IVissimPtr Vissim;
		//Create instance for Vissim 32 bit and version 7
		//Vissim.CreateInstance("Vissim.Vissim-32.700");
		Vissim.CreateInstance("Vissim.Vissim-64.900");
		bool flag_read_additionally = false; // you can read network(elements) additionally, in this case set "flag_read_additionally" to true



		bstr_t Path_of_COM_example_network = "C:\\Users\\a-bibeka\\Dropbox\\TTI_Projects\\0-1-6934 TSC Settings Sunkari\\Through_UpStrmRadar_Scenario_2ln\\"; // always use \\ at the end
		//Network file name																					 // Load a Vissim Network :

		//bstr_t Path_of_COM_example_network = "C:\\Users\\a-bibeka\\Dropbox\\TTI_Projects\\0-1-6934 TSC Settings Sunkari\\SimultaneiousGapOutSen\\"; // always use \\ at the end

		bstr_t Filename = Path_of_COM_example_network;
		Filename = Filename + "Through_2Ln.inpx";

		ofstream fout; //writing vehicles trapped in delimma zone
		string result_file = "dilemma_zone.csv";
		result_file = string(Path_of_COM_example_network) + result_file;
		fout.open(result_file, std::fstream::trunc);
		fout << "Scenario," << "Run," << "VehNo," << "Phase" << endl;
		//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
		// Simulation
		//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == 
		/***********************************************************************************************************
		*******************************************Run Simulation at max speed for 500 sec**************************
		************************************************************************************************************/


		for (auto ScenarioN = 1; ScenarioN <= 18; ScenarioN++) {
			try {
				bstr_t save_file = "Scenario";
				int scenario = ScenarioN;
				wchar_t temp_str[11]; // we assume that the maximal string length can be 10
				_itow_s(scenario, temp_str, 10);
				BSTR mybstr = SysAllocString(temp_str);
				Filename = Path_of_COM_example_network + save_file + mybstr + ".inpx";
				Vissim->LoadNet(Filename, flag_read_additionally);

			}
			catch (HRESULT error) {
				return error;
			}
			catch (...) {
				return E_FAIL;
			}


			/***********************************************************************************************************
			*******************************************Changing the Detector Status*************************************
			************************************************************************************************************/
			//Create a Signal group object pointer
			VISSIMLIB::ISignalGroupPtr Sig;
			//Create an interator for the signal groups for signal controller 1
			auto sig_iter = Vissim->Net->SignalControllers->GetItemByKey(1)->SGs->GetIterator();
			//Keep on looping though all the signal group objects while the interator is still valid; not out of bound. 
			while (sig_iter->GetValid()) {
				//Assign the first value to the iterator to hte Sig pointer
				Sig = sig_iter->GetItem();
				//Get the Signal group No
				int sg_no = Sig->GetAttValue("No");
				cout << sg_no;
				//Move the iterator to the next value
				sig_iter->Next();
			}
			//Wait for 100 ms before going to the next line 
			//Sleep(100);

			/***********************************************************************************************************
			*******************************************Changing the Detector Status- Practice***************************
			************************************************************************************************************/
			//Get the position of signal head 22 which is on a high speed link and intialize signal_head_pos (westbound)
			//This signal Head corresponds to signal group 2
			int sig_head_no = 22;
			auto sig_head_2 = Vissim->Net->SignalHeads->GetItemByKey(sig_head_no);
			double pos_2 = sig_head_2->GetAttValue("Pos");
			bstr_t signal_state_2 = bstr_t(sig_head_2->GetAttValue("State"));

			//Get the position of signal head 62 which is on a high speed link and intialize signal_head_pos (Easttbound)
			//This signal Head corresponds to signal group 6
			sig_head_no = 62;
			auto sig_head_6 = Vissim->Net->SignalHeads->GetItemByKey(sig_head_no);
			double pos_6 = sig_head_6->GetAttValue("Pos");
			bstr_t signal_state_6 = bstr_t(sig_head_6->GetAttValue("State"));


			//Create an object of detector_call and set the value of signal_head_pos
			detector_call det_call(pos_2, pos_6);

			//Create a pointer for Vehicles in VISSIM
			VISSIMLIB::IVehiclePtr Vehicle_vissim;
			int veh_number;
			double veh_speed;
			double veh_position;
			int veh_Route_Dec_no; //Purple part of the routing decision 
			int veh_Route_no;    // blue part of the routing decision

								 //Create a pointer for detector object det_600- calls sg 6 and det_200 calls sg 2
			VISSIMLIB::IDetectorPtr det_600, det_200;
			auto det_iter = Vissim->Net->Detectors->GetIterator();
			int det_no = -99;
			while (det_iter->GetValid()) {
				det_600 = det_iter->GetItem();
				det_no = det_600->GetAttValue("PortNo");
				if (det_no == 600) break;
				det_iter->Next();
			}

			cout << "detector port no is" << det_no << endl;
			det_no = -99;
			while (det_iter->GetValid()) {
				det_200 = det_iter->GetItem();
				det_no = det_200->GetAttValue("PortNo");
				if (det_no == 200) break;
				det_iter->Next();
			}
			cout << "detector port no is" << det_no << endl;

			bool break_200 = false;
			bool break_600 = false;

			Vissim->Simulation->PutAttValue("UseMaxSimSpeed", true);
			Vissim->Simulation->PutAttValue("UseAllCores", true);
			int run = 1;
			int numRuns = 5;
			Vissim->Simulation->PutAttValue("NumRuns", numRuns);
			double endSim = 1200; //sec
			Vissim->Simulation->PutAttValue("SimPeriod", endSim);
			Vissim->Simulation->PutAttValue("RandSeed", 1);
			Vissim->Simulation->PutAttValue("RandSeedIncr", 1);
			//   Change to quick mode later
			Vissim->Graphics->CurrentNetworkWindow->PutAttValue("QuickMode", 1);

			bool switch_Amber_2 = FALSE, switch_Amber_6 = FALSE;

			int run_no = 1;
			for (auto i = 1; i < endSim * 10 * numRuns; i++) {
				auto simsec = Vissim->Simulation->SimulationSecond;
				if (i>(endSim * 10 * numRuns) - 10) {
					Vissim->Simulation->RunContinuous();
					break;
				}
				else {
					Vissim->Simulation->RunSingleStep();
				}
				break_200 = false;
				break_600 = false;
				signal_state_2 = bstr_t(sig_head_2->GetAttValue("State"));
				signal_state_6 = bstr_t(sig_head_6->GetAttValue("State"));
				bstr_t gr = "GREEN";
				bool is_sg_2_gr = (signal_state_2 == gr);
				bool is_sg_6_gr = (signal_state_6 == gr);
				if (is_sg_2_gr) switch_Amber_2 = TRUE;
				if (is_sg_6_gr) switch_Amber_6 = TRUE;



				VISSIMLIB::IIteratorPtr Vehicles_Iterator = Vissim->Net->Vehicles->GetIterator();

				while (Vehicles_Iterator->GetValid() && (is_sg_2_gr || is_sg_6_gr) && simsec >= 300)
				{
					Vehicle_vissim = Vehicles_Iterator->GetItem();
					//Check if the route no and route decision no are what we want 
					veh_Route_Dec_no = Vehicle_vissim->GetAttValue("RoutDecNo");
					veh_Route_no = Vehicle_vissim->GetAttValue("RouteNo");

					//cout << veh_Route_Dec_no<<endl;
					//Check if the route decision number and route number are 20 and 200 respectively (This is based on the network coding)
					// This would change if we made changes to the routing parameters in the network.
					if (veh_Route_Dec_no == 60 && veh_Route_no == 600 && is_sg_6_gr) {
						veh_number = Vehicle_vissim->GetAttValue("No");
						veh_speed = Vehicle_vissim->GetAttValue("Speed");
						//speed in ft/sec
						veh_speed = 1.46667*veh_speed;
						veh_position = Vehicle_vissim->GetAttValue("Pos");

						//Check for detector 200 and 600 which contains the routing decision number 1
						if (det_call.is_call_needed(veh_Route_Dec_no, veh_position, veh_speed) && signal_state_6 == gr) {
							det_600->PutAttValue("ManActuation", DETECTION);
							//cout << "Dilemma zone detection 600 detector" << endl;
							break_600 = true;
						}
						else {
							det_600->PutAttValue("ManActuation", NO_DETECTION);

						}
					}
					else if (veh_Route_Dec_no == 20 && veh_Route_no == 200 && is_sg_6_gr) {
						veh_number = Vehicle_vissim->GetAttValue("No");
						veh_speed = Vehicle_vissim->GetAttValue("Speed");
						//speed in ft/sec
						veh_speed = 1.46667*veh_speed;
						veh_position = Vehicle_vissim->GetAttValue("Pos");
						//	det_call.add_vehicles(veh_number, veh_speed, veh_position, veh_Route_Dec_no);
						//Check for detector 200 and 600 which contains the routing decision number 1
						if (det_call.is_call_needed(veh_Route_Dec_no, veh_position, veh_speed) && signal_state_2 == gr) {
							det_200->PutAttValue("ManActuation", DETECTION);
							//cout << "Dilemma zone detection 200 detector" << endl;
							break_200 = true;
						}
					}
					else {
						//nothing
					}

					if (break_200 == true && is_sg_6_gr == false) {
						break;
					}
					else if (break_600 == true && is_sg_2_gr == false) {
						break;
					}
					else if (break_200 == true && break_600 == true && is_sg_6_gr == true && is_sg_2_gr == true) {
						break;
					}
					else {
						//continue
					}
					//cout << std::setprecision(4) << veh_number << "  |  " << veh_speed << "  |  " << veh_position << endl;
					Vehicles_Iterator->Next();
				}//ends the while loop for detector call



				bstr_t amber = "AMBER";
				bool is_sg_2_amber = (signal_state_2 == amber);
				bool is_sg_6_amber = (signal_state_6 == amber);
				while (Vehicles_Iterator->GetValid() && (is_sg_2_amber || is_sg_6_amber) && simsec >= 300 && (switch_Amber_2 || switch_Amber_6))
				{
					Vehicle_vissim = Vehicles_Iterator->GetItem();
					//Check if the route no and route decision no are what we want 
					veh_Route_Dec_no = Vehicle_vissim->GetAttValue("RoutDecNo");
					veh_Route_no = Vehicle_vissim->GetAttValue("RouteNo");

					//cout << veh_Route_Dec_no<<endl;
					//Check if the route decision number and route number are 20 and 200 respectively (This is based on the network coding)
					// This would change if we made changes to the routing parameters in the network.
					if (veh_Route_Dec_no == 60 && veh_Route_no == 600 && is_sg_6_amber) {
						veh_number = Vehicle_vissim->GetAttValue("No");
						veh_speed = Vehicle_vissim->GetAttValue("Speed");
						//speed in ft/sec
						veh_speed = 1.46667*veh_speed;
						veh_position = Vehicle_vissim->GetAttValue("Pos");

						//Check for detector 200 and 600 which contains the routing decision number 1
						if (det_call.is_call_needed(veh_Route_Dec_no, veh_position, veh_speed) && is_sg_6_amber) {
							//cout <<"Run No"<<run_no<< "Vehicle No is: " << veh_number << "Signal state for 6: " << signal_state_6<< endl;
							fout << ScenarioN << "," << run_no << "," << veh_number << "," << 6 << endl;
						}
					}
					else if (veh_Route_Dec_no == 20 && veh_Route_no == 200 && is_sg_2_amber) {
						veh_number = Vehicle_vissim->GetAttValue("No");
						veh_speed = Vehicle_vissim->GetAttValue("Speed");
						//speed in ft/sec
						veh_speed = 1.46667*veh_speed;
						veh_position = Vehicle_vissim->GetAttValue("Pos");
						//	det_call.add_vehicles(veh_number, veh_speed, veh_position, veh_Route_Dec_no);
						//Check for detector 200 and 600 which contains the routing decision number 1
						if (det_call.is_call_needed(veh_Route_Dec_no, veh_position, veh_speed) && is_sg_2_amber) {
							//cout << "Run No" << run_no << "Vehicle No is: " << veh_number << "Signal state for 2: " << signal_state_6 << endl;
							fout << ScenarioN << "," << run_no << "," << veh_number << "," << 2 << endl;
						}
					}
					else {
						//nothing
					}
					//cout << std::setprecision(4) << veh_number << "  |  " << veh_speed << "  |  " << veh_position << endl;
					Vehicles_Iterator->Next();
				}//ends the while loop for detector call
				if (is_sg_2_amber || is_sg_6_amber) {
					switch_Amber_2 = FALSE;
					switch_Amber_6 = FALSE;
				}
				if (i % int(endSim * 10) == 0) {
					//cout << run_no << endl;
					run_no++;
				}
			} //end for loop - simulation second increment
		}//end for loop - simulation run increment
		fout.close();
	} //end Coinitialize
	  // uninitialize COM 
	CoUninitialize();
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long double microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	cout << "Run time : " << microseconds / (1e6) << " seconds " << endl;
	char anything[1] = { '1' };
	cin >> anything;
	return 0;
}
