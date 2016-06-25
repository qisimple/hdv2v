#include "hd-rsu.h"

namespace ns3{ 

HdRsu::HdRsu(unsigned int rsuId, double xLabel, double yLabel)
:m_accessLog(),
m_warningsLog()
{
	m_rsuId = rsuId;
	m_zoneId = zoneId;
	// m_leftZoneId = leftZoneId;
	// m_rightZoneId = rightZoneId;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
}
HdRsu::~HdRsu(){}

void 	HdRsu::Update()
{}

void 	HdRsu::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch(msg->GetPacketType())
	{
		case ACCESS_PACKET:
		{

		}
		// case NOTIFY_BROADCAST_VEHICLES_PACKET:
		// case RESPONSE_BROADCAST_VEHICLES_PACKET:
		// case WARNINGS_PACKET:
		default:
		{}
	}
}

void 	HdRsu::SendControlPacket()
{}

// void 	HdRsu::SendNotifyBroadcastVehiclesPacket()
// {}

// void 	HdRsu::SendRelayPacket()
// {}

}//namespace ns3