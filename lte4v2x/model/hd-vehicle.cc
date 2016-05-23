#include "hd-vehicle.h"
#include <cstdio>
#include <cstring>

namespace ns3{

HdVehicle::HdVehicle()
:m_totalPacketNum(0),
m_efficientPacketNum(0),
m_failPacketNum(0),
m_nextPacketId(0),
m_totalDelay(0),
m_packetSentLog(),
m_accessLog(),
m_packetNotSentLog()
{
	m_ptrRandom = CreateObject<UniformRandomVariable>();
}
HdVehicle::~HdVehicle(){}
void 	HdVehicle::InitVehicle(const std::string &zoneId, const unsigned int &vehicleId, const unsigned int &validTime
		const double &xLabel, const double &yLabel, const double &velocity, const double sendProbility)
{
	m_zoneId = zoneId;
	m_vehicleId = vehicleId;
	m_validTime = validTime;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
	m_velocity = velocity;
	m_sendProbility = sendProbility;
}

void 	HdVehicle::Update()
{
	UpdateLog();		// delete the validTime=0 items
	double random_value = m_ptrRandom->GetValue(double(0.0), double(1.0));
	if(random_value < m_sendProbility)	// If true, we get a new packet, inset it to m_packetNotSentLog
	{
		unsigned int 	packetId = m_nextPacketId++;
		unsigned int 	validTime = m_validTime;
		m_packetNotSentLog.push_back(std::pair<packetId, validTime>);
	}
}

void 	HdVehicle::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch (msg->GetSetPacketType)
	{
		case ACCESS_RESULT_PACKET:
		case RELAY_PACKET:
		default:
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