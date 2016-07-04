#ifndef HD_RSU_H
#define HD_RSU_H

#include "hd-packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/simple-ref-count.h"
#include <vector>
#include <map>
#include <string>

namespace ns3 {

class 	HdRsuScenario;

class HdRsu:public SimpleRefCount<HdRsu>
{
public:
	HdRsu( unsigned int rsuId, double xLabel, double yLabel, std::vector<unsigned int> &zoneId, std::vector<Ptr<HdVehicleInfo> > &vehInfo, HdRsuScenario *hdSce);
	~HdRsu();
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> &msg);
	void 	SendControlPacket();
	unsigned int 	GetRsuId();
	// void 	SendNotifyBroadcastVehiclesPacket();
	// void 	SendRelayPacket();
private:
	void 	UpdateLog();
	void 	InitLog();
	void 	StateConvert(bool &relay);
	void 	AssignRbs();
	void 	AssignRelayNodes();
	void 	Send(Ptr<HdPacket> &con);
	bool 	InAccess(const Ptr<HdVehicleInfo> vehicleInfo);
	unsigned int 	m_rsuId;
	double		m_xLabel;
	double 		m_yLabel;
	bool 	m_status;		// True means access states, false means control states
	Ptr<HdRsuScenario> 	m_hdSce;		// The Ptr of the scenario manager
	std::vector<unsigned int> 	m_zoneId;	// The start pos of a zone is x=zoneId*200, and the rb id of a zone is (zoneId%3)*16~ (zoneId%3)*16+15
	std::vector<unsigned int> 	m_usedRb;	// Rbs are assigned from little to large successively, Round Robin, m_usedRb has the same size with m_zoneId
	// std::string 	m_leftZoneId;
	// std::string	m_rightZoneId;
	std::map<unsigned int, std::vector<unsigned int> > 	m_assignRb;		// Key is vehicleId, value is its assigned rbs
	std::vector<unsigned int> 	m_assignRelayNode;	// VehicleId
	std::vector<unsigned int> 	m_lastRelayNode;	// VehicleId
	std::vector<std::vector<AccessInfo> >  	m_accessLog;	//  Value is received accessInfos, they are divided into several zones,it has the same size with m_zoneId
	std::vector<Ptr<HdVehicleInfo> > 	m_vehicle;		// All vehicles in the charge of this rsu
	// std::map<unsigned int, std::vector<WarningsInfo> > 	m_warningsLog;	// key is timeslot, value is received waringsInfo	
	/* data */
};
}// namespace ns3
#endif /* HD_RSU_H*/