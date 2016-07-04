#ifndef HD_VEHICLE_H
#define HD_VEHICLE_H

#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "hd-packet.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class 	HdRsuScenario;

class HdVehicle:public SimpleRefCount<HdVehicle>
{
public:
	HdVehicle(HdVehicleParameter &par, HdRsuScenario *hdSce);
	~HdVehicle();
	void 	Update();
	void 	ReceiveHdPacket(Ptr<HdPacket> &msg);
	void 	SendAccessPacket();
	void 	SendWarningsPacket();
	void 	SendRelayPacket();
	unsigned int GetVehicleId();
	bool 	IfSurround(double x,double y);
private:
	void 	UpdateLog();
	void 	StateConvert(bool &relay);
	void 	Send(Ptr<HdPacket> &hd);
	// std::string 	m_zoneId;
	bool 	m_relayNode;		// If it is assigned as relayNode, first bit;	00 -> access; 10 -> control; 11 -> listen; 01 -> relay;
	bool 	m_relaying;			// If it is in relay procedure, second bit;
	unsigned int 	m_rsuId;		// the rsu it belongs to
	unsigned int	m_vehicleId;
	unsigned int	m_validTime;		// Unit is ms
	double 	m_xLabel;	// pos of vehicle
	double 	m_yLabel;
	double 	m_velocity;	// velocity of vehicle
	double 	m_sendProbility;
	int		m_totalPacketNum;	
	int 	m_efficientPacketNum;		// If all vehicles in its AR correctly receive the packet, access packet is not involved
	int 	m_failPacketNum;	// If it didn't get radio resource within certain time or not all vehicles in its AR correctly receive the packet
	int 	m_totalReceivePacketNum;	// Num of received different packet
	unsigned int 	m_nextPacketId;		// Starting from 0
	unsigned int 	m_totalDelay;	// No larger than validTime
	Ptr<HdRsuScenario> 	m_hdSce;		// The Ptr of the scenario manager

	std::map<unsigned int, unsigned int> 	m_packetSentLog;	// PacketId is key, remaining time is value,it is used to log those sented but not relayed packets; it is supposed to fail if remaining time is larger than valid time
	std::vector<unsigned int> 	m_accessLog;	// Access procedure takes two time slots, so it only shows the current rbs it get 
	std::vector<Ptr<WarningsPacket> > 	m_packetNotSentLog;	// Not access yet,value is the ptr of this packet
	std::vector<Ptr<HdPacket> > 	m_packetRelayLog;	// If vehicle is a relaynode, then keep the received in this log
	Ptr<UniformRandomVariable>	m_ptrRandom;
	/* data */
};

}// namespace ns3
#endif /* HD_VEHICLE_H*/