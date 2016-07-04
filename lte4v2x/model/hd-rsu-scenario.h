#ifndef HD_RSU_SCENARIO_H
#define HD_RSU_SCENARIO_H

#include <ns3/ptr.h>
#include <ns3/simple-ref-count.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <utility>

#include "hd-rsu.h"
#include "hd-vehicle.h"

namespace ns3 {

class HdRsuScenario:public SimpleRefCount<HdRsuScenario>
{
public:
	HdRsuScenario(const std::string &traceFile, const std::string &parFile);
	~HdRsuScenario();
	void 	Start();
	void 	CalculateResult();
private:
	void 	ParseTraceFile();	// Deal with m_vehicles
	void 	ParseParFile();		// Deal with m_rsus, m_validTime, m_sendProbility
	void 	Init();				// Init HdRsu and HdVehicle
	void 	DoSend(Ptr<HdPacket> &msg, unsigned int src, unsigned int dest);		// Decide which object to receive according to packet type and src and dest
	std::string 	m_traceFile;		//  Sumo trace file
	std::string 	m_parFile;		// Configure file
	unsigned int 	m_validTime;
	double 	m_sendProbility;		// We get this from configure file

	std::vector<Ptr<HdVehicleInfo> > 	m_vehicleInfo;
	std::vector<Ptr<HdRsuInfo> > 	m_rsuInfo;
	std::map<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > > 	m_hdVehRsu;
	
	/* data */
};

}// namespace ns3
#endif /* HD_RSU_SCENARIO_H*/