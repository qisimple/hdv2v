#ifndef HD_RSU_H
#define HD_RSU_H

#include "hd-packet.h"

namespace ns3 {

class HdRsu : public SimpleRefCount<HdRsu>
{
public:
	HdRsu();
	~HdRsu();
	void 	InitRsu(const unsigned int &rsuId, const std::string &leftZoneId, const std::string &rightZoneId,
			const double &xLabel, const double &yLabel);
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> msg);
	void 	SendAccessResultPacket();
	void 	SendNotifyBroadcastVehiclesPacket();
	void 	SendRelayPacket();
private:
	unsigned int 	m_rsuId;
	std::string 	m_leftZoneId;
	std::string	m_rightZoneId;
	double		m_xLabel;
	double 		m_yLabel;
	std::map<unsigned int, std::vector<AccessInfo> > 	m_accessLog;	//  key is timeslot, value is received accessInfos
	std::map<unsigned int, std::vector<WarningsInfo> > 	m_warningsLog;	// key is timeslot, value is received waringsInfo	
	/* data */
};
}// namespace ns3
#endif /* HD_RSU_H*/