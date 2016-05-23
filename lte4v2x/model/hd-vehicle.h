#ifndef HD_VEHICLE_H
#define HD_VEHICLE_H

#include "hd-packet.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class HdVehicle
{
public:
	HdVehicle();
	~HdVehicle();
	void 	InitVehicle(const std::string &zoneId, const unsigned int &vehicleId, const unsigned int &validTime
		const double &xLabel, const double &yLabel, const double &velocity,const double sendProbility);
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> msg);
	void 	SendAccessRequestPacket();
	void 	SendWarningsPacket();
private:
	UpdateLog();
	std::string 	m_zoneId;
	unsigned int	m_vehicleId;
	unsigned int	m_validTime;		// Unit is ms
	double 	m_xLabel;	// pos of vehicle
	double m_yLabel;
	double m_velocity;	// velocity of vehicle
	double m_sendProbility;
	int	m_totalPacketNum;	
	int 	m_efficientPacketNum;		// If all vehicles in its AR correctly receive the packet, access packet is not involved
	int 	m_failPacketNum;	// If it didn't get radio resource within certain time or not all vehicles in its AR correctly receive the packet
	unsigned int 	m_nextPacketId;
	unsigned int 	m_totalDelay;	// no larger than validTime
	std::map<unsigned int, unsigned int> m_packetSentLog;	// PacketId is key, remaining time is value;
	std::map<AccessInfo, unsigned int> m_accessLog;	// no larger than 3
	std::map<unsigned int, unsigned int> m_packetNotSentLog;	// Not access yet
	Ptr<UniformRandomVariable> m_ptrRandom;
	/* data */
};

}// namespace ns3
#endif /* HD_VEHICLE_H*/