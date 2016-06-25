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

struct HdVehicleInfo: public SimpleRefCount<HdVehicleInfo>
{
	unsigned int	vehicleId;
	double 	xLabel;
	double yLabel;
	double velocity;
	/* data */
};

struct HdRsuInfo: public SimpleRefCount<HdRsuInfo>
{
	unsigned int	rsuId;
	double 	xLabel;
	double yLabel;
	/* data */
};

class HdRsuScenario : public SimpleRefCount<HdRsuScenario>
{
public:
	HdRsuScenario(const std::string &traceFile, const std::string &parFile);
	~HdRsuScenario();
	void 	Start();
	void 	CalculateResult();
private:
	void 	ParseTraceFile();	// Deal with m_vehicles
	void 	ParseParFile();		// Deal with m_zones, m_rsus, m_validTime, m_sendProbility
	// void 	BuildRtoZMap();		// Deal with m_rsuWithZones
	// void 	BuildVtoZMap();		//Deal with m_vehicleWithZones
	// void 	CreateHdVehicles();
	// void 	CreateHdRsus();	
	// void 	BuildZtoVMap();		// Deal with m_zoneWithVehicles
	// void	BuildZtoRMap();		// Deal with m_zoneWithRsus
	// void 	BuildVtoVMap();		// Deal with m_vehicleWithSurroundedVehicles
	// void 	BuildRtoRMap();		// Deal with m_rsuWithSurroundedRsus
	std::string 	m_traceFile;		//  Sumo trace file
	std::string 	m_parFile;		// Configure file
	double 	m_validTime;
	double 	m_sendProbility;		// We get this from configure file
	double 	m_packetLossRate;
	double 	m_packetAverageDelay;
	double 	m_packetNum;		// Efficient packet number in a simulation zone
	std::vector<Ptr<HdVehicleInfo> > m_vehicles;
	std::vector<Ptr<HdRsuInfo> > m_rsus;
	std::vector<std::string> m_zones;
	std::vector<Ptr<HdVehicle> > m_hdVehicle;
	std::vector<Ptr<HdRsu> > m_hdRsu;
	std::map<std::string, std::vector<Ptr<HdVehicle> > > m_zoneWithVehicles;	// Key is a zone, value is hdvehicles in this zone
	std::map<std::string, std::vector<Ptr<HdRsu> > > m_zoneWithRsus;		// Key is a zone ,value is hdrsus related to this zone
	std::map<unsigned int, std::vector<std::string> > m_rsuWithZones;		// Key is a  HdRsu, value is zones related to this hdrsu
	std::map<unsigned int, std::string> m_vehicleWithZones;	// Key is a HdVehicle, value is zones related to this hdvehicle
	std::map<Ptr<HdVehicle>, std::vector<Ptr<HdVehicle> > > m_vehicleWithSurroundedVehicles;		// Key is a hdvehicle, value is its surrounded hdvehicles
	std::map<Ptr<HdRsu>, std::vector<Ptr<HdRsu> > > m_rsuWithSurroundedRsus;			// Key is a rsu, value is its surrounded hdrsus
	/* data */
};

}// namespace ns3
#endif /* HD_RSU_SCENARIO_H*/