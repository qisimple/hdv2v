#include "hd-rsu.h"

namespace ns3{ 

HdRsu::HdRsu(unsigned int rsuId, double xLabel, double yLabel,std::vector<unsigned int> &zoneId)
:m_status(true),
m_usedRb(),
m_assignRb(),
m_assignRelayNode(),
m_accessLog()
{
	m_rsuId = rsuId;
	// m_leftZoneId = leftZoneId;
	// m_rightZoneId = rightZoneId;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
	m_zoneId = zoneId;
}
HdRsu::~HdRsu(){}

void 	HdRsu::Update()
{
	if(m_status == false)	// Send ControlPacket
	{
		Simulator::Schedule(Seconds(0.001),&StateConvert(),m_status);
		SendControlPacket();
	}
	else
	{
		Simulator::Schedule(Seconds(0.001),&StateConvert(),m_status);
		Simulator::Schedule(Seconds(0.001),&UpdateLog());	
	}
	Simulator::Schedule(Seconds(0.001),&Update());
}

void 	HdRsu::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch(msg->GetPacketType())
	{
		case ACCESS_PACKET:
		{
			if(m_status == true)
			{
				m_accessLog.push_back(msg->GetAccess());
			}
			else
			{
				// Wrong time slot;
			}
		}
		default:
		{}
	}
}

void 	HdRsu::SendControlPacket()
{
	Ptr<ControlPacket> con = CreateObject<ControlPacket>();
	std::map<unsigned int, std::vector<unsigned int> >::iterator it_rb;	
	for(it_rb=m_assignRb.begin();it_rb!=m_assignRb.end();)
	{
		ControlInfo info;
		info.m_vehicleId = it_rb->first;
		info.m_rbs = it_rb->second;
		con->AddControlInfo(info);
	}
	con->AddRelayNodeId(m_assignRelayNode);
	Ptr<HdPacket> = con;
	Send(con);
	m_assignRb.clear();
	m_assignRelayNode.clear();
}

// void 	HdRsu::SendNotifyBroadcastVehiclesPacket()
// {}

// void 	HdRsu::SendRelayPacket()
// {}

void 	HdRsu::UpdateLog()	// Assign at the end of access slot
{
	AssignRbs();
	AssignRelayNodes();
}

void 	HdVehicle::StateConvert(bool &relay)
{
	if(relay == true)
	{
		relay = false;
	}
	else
	{
		relay = true;
	}
}

void	HdVehicle::AssignRbs()
{

}

void 	HdVehicle::AssignRelayNodes()
{
	
}

void 	HdPacket::Send(Ptr<HdPacket> &con)			// TO BE CONTINUE,
{

}

}//namespace ns3