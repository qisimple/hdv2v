#ifndef HD_RSU_H
#define HD_RSU_H

#include "hd-packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"

namespace ns3 {

class HdRsu : public SimpleRefCount<HdRsu>
{
public:
	HdRsu( unsigned int rsuId, double xLabel,  double yLabel, std::vector<unsigned int> &zoneId);
	~HdRsu();
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> msg);
	void 	SendControlPacket();
	// void 	SendNotifyBroadcastVehiclesPacket();
	// void 	SendRelayPacket();
private:
	void 	UpdateLog();
	void 	StateConvert(bool &relay);
	void 	AssignRbs();
	void 	AssignRelayNodes();
	void 	Send(Ptr<HdPacket> &con);
	unsigned int 		m_rsuId;
	double		m_xLabel;
	double 		m_yLabel;
	bool 	m_status;		// True means access states, false means control states
	std::vector<unsigned int> 	m_zoneId;	// The start pos of a zone is x=zoneId*200, and the rb id of a zone is (zoneId%3)*16~ (zoneId%3)*16+15
	std::vector<unsigned int> 	m_usedRb;	// Rbs are assigned from little to large successively, Round Robin, m_usedRb has the same size with m_zoneId
	// std::string 	m_leftZoneId;
	// std::string	m_rightZoneId;
	std::map<unsigned int, std::vector<unsigned int> > 	m_assignRb;		// Key is vehicleId, value is its assigned rbs
	std::vector<unsigned int> 	m_assignRelayNode;
	std::vector<AccessInfo> 	m_accessLog;	//  Value is received accessInfos
	// std::map<unsigned int, std::vector<WarningsInfo> > 	m_warningsLog;	// key is timeslot, value is received waringsInfo	
	/* data */
};
}// namespace ns3
#endif /* HD_RSU_H*/