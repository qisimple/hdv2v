#include "hd-vehicle.h"
#include <cstdio>
#include <cstring>

namespace ns3{

HdVehicle::HdVehicle(unsigned int rsuId, unsigned int vehicleId, unsigned int validTime, 
		double xLabel, double yLabel, double velocity,double sendProbility))
:m_totalPacketNum(0),
m_efficientPacketNum(0),
m_failPacketNum(0),
m_totalReceivePacketNum(0),
m_nextPacketId(0),
m_totalDelay(0),
m_packetSentLog(),
m_accessLog(),
m_packetNotSentLog()
{
	m_ptrRandom = CreateObject<UniformRandomVariable>();
	m_rsuId = rsuId;
	m_vehicleId = vehicleId;
	m_validTime = validTime;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
	m_velocity = velocity;
	m_sendProbility = sendProbility;
}
HdVehicle::~HdVehicle(){}

void 	HdVehicle::Update()
{
	UpdateLog();		// delete the validTime=0 items
	double random_value = m_ptrRandom->GetValue(double(0.0), double(1.0));
	if(random_value < m_sendProbility)	// If true, we get a new packet, inset it to m_packetNotSentLog
	{
		unsigned int 	packetId = m_nextPacketId++;
		unsigned int 	validTime = m_validTime;
		m_packetNotSentLog.insert(std::pair<unsigned int, unsigned int>(packetId,validTime));
	}
	// Simulator::Schedule()
}

void 	HdVehicle::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch (msg->GetPacketType())
	{
		case CONTROL_PACKET:
		case WARNINGS_PACKET:
		{
			if(m_relayNode == true)
			{
				
			}
		}
		case RELAY_PACKET:
		default:
		{}
	}
}

void 	HdVehicle::SendAccessRequestPacket()
{

}

void 	HdVehicle::SendWarningsPacket()
{

}

void 	HdVehicle::UpdateLog()
{

}

}//namespace ns3