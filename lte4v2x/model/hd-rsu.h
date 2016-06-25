#ifndef HD_RSU_H
#define HD_RSU_H

#include "hd-packet.h"

namespace ns3 {

class HdRsu : public SimpleRefCount<HdRsu>
{
public:
	HdRsu( unsigned int rsuId, double xLabel,  double yLabel);
	~HdRsu();
	void 	InitRsu();
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> msg);
	void 	SendAccessResultPacket();
	void 	SendNotifyBroadcastVehiclesPacket();
	void 	SendRelayPacket();
private:
	unsigned int 	m_rsuId;
	std::vector<std::string> m_zoneId;
	// std::string 	m_leftZoneId;
	// std::string	m_rightZoneId;
	double		m_xLabel;
	double 		m_yLabel;
	std::map<unsigned int, std::vector<AccessInfo> > 	m_accessLog;	//  key is timeslot, value is received accessInfos
	std::map<unsigned int, std::vector<WarningsInfo> > 	m_warningsLog;	// key is timeslot, value is received waringsInfo	
	/* data */
};
}// namespace ns3
#endif /* HD_RSU_H*/