#include "hd-rsu.h"

namespace ns3{ 

HdRsu::HdRsu()
:m_accessLog(),
m_warningsLog()
{

}
HdRsu::~HdRsu(){}
void 	HdRsu::InitRsu(const unsigned int &rsuId, const std::string &leftZoneId, const std::string &rightZoneId,
			const double &xLabel, const double &yLabel)
{
	m_rsuId = rsuId;
	m_leftZoneId = leftZoneId;
	m_rightZoneId = rightZoneId;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
}

void 	HdRsu::Update()
{}

void 	HdRsu::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch(msg->GetPacketType())
	{
		case ACCESS_REQUEST_PACKET:
		case NOTIFY_BROADCAST_VEHICLES_PACKET:
		case RESPONSE_BROADCAST_VEHICLES_PACKET:
		case WARNINGS_PACKET:
		default:
		{}
	}
}

void 	HdRsu::SendAccessResultPacket()
{}

void 	HdRsu::SendNotifyBroadcastVehiclesPacket()
{}

void 	HdRsu::SendRelayPacket()
{}

}//namespace ns3