#include "hd-vehicle.h"
#include <cstdio>
#include <cstring>

namespace ns3{

HdVehicle::HdVehicle(unsigned int rsuId, unsigned int vehicleId, unsigned int validTime, 
		double xLabel, double yLabel, double velocity,double sendProbility))
:m_relayNode(false),
m_relaying(false),
m_totalPacketNum(0),
m_efficientPacketNum(0),
m_failPacketNum(0),
m_totalReceivePacketNum(0),
m_nextPacketId(0),
m_totalDelay(0),
m_packetSentLog(),
m_accessLog(),
m_packetNotSentLog(),
m_packetRelayLog()
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
		WarningsInfo 	war;
		war.m_vehicleId = m_vehicleId;
		war.m_packetId = m_nextPacketId++;
		war.m_priorityType = PriorityType::HIGH;
		unsigned int t = (unsigned int)(Simulator::Now()::GetMilliSeconds());		// Check if MilliSecondes() exists in ns3
		war.m_time = t + m_validTime;
		Ptr<WarningsPacket> m_war = CreateObject<WarningsPacket>(war);
		m_packetNotSentLog.push_back(m_war);
		m_totalPacketNum++;
	}
	if(m_relayNode == true && m_relaying == false)	// Control step, do nothing
	{
		assert(m_accessLog.size()==0);
		Simulator::Schedule(Seconds(0.001),&StateConvert(),m_relaying);
	}
	else if(m_relayNode == true && m_relaying == true)	// Listen step, store the received warings
	{
		assert(m_accessLog.size()==0);
		Simulator::Schedule(Seconds(0.001),&StateConvert(),m_relayNode);
	}
	else if(m_relayNode == false && m_relaying == true)		// Relay step, relay the received warings
	{
		assert(m_accessLog.size()==0);
		Simulator::Schedule(Seconds(0.001),&StateConvert(),m_relaying);
		SendRelayPacket();		
	}
	else
	{
		if((m_packetSentLog.size()>0)&&(m_accessLog.size()>0))			// Necessary to access the channel;
		{
			SendWarningsPacket();
		}
		else if((m_packetSentLog.size()>0)&&(m_accessLog.size()==0))
		{
			SendAccessPacket();			// Send at the begining of time slot, and Rsu suppose to receive at the end of the same time slot
		}
		else
		{
			// Do nothing
		}
	}
	Simulator::Schedule(Seconds(0.001),&Update());
}

void 	HdVehicle::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch (msg->GetPacketType())
	{
		case CONTROL_PACKET:
		{
			m_accessLog.clear();
			m_accessLog = msg->GetRb(m_vehicleId);
			m_relayNode = msg->GetRelayNode();
			m_relaying = false;
			break;
		}
		case WARNINGS_PACKET:
		{
			if(m_relayNode == true && m_relaying == true)
			{
				m_packetRelayLog.push_back(msg);
			}
			else
			{
				// Take actions according to the received warnings;
			}
			m_totalReceivePacketNum++;
			break;
		}
		case RELAY_PACKET:		// Check the m_packetSentLog, if m_packetSentLog in relayPacket,then take it as a success else take it as a failure;
		{
			std::map<unsigned int, unsigned int>::iterator it;
			for(it = m_packetSentLog.begin();it!=m_packetSentLog.end();)
			{
				if(msg->GetExist(m_vehicleId, it->first))
				{
					m_efficientPacketNum++;
					m_totalDelay += validTime - (it->second);
					m_packetSentLog.erase(it);
				}
				else
				{
					++it;
				}
			}
			break;
		}
		default:
		{
			cout<<"Error Packet type!"<<endl;
		}
	}
}

void 	HdVehicle::SendAccessPacket()
{
	AccessInfo a;
	a.m_vehicleId = m_vehicleId;
	a.m_rsuId = m_rsuId;
	a.m_num = m_packetNotSentLog.size();
	a.m_xLabel = m_xLabel;
	a.m_yLabel = m_yLabel;
	a.m_priorityType = PriorityType::HIGH;
	Ptr<AccessPacket> access = CreateObject<AccessPacket>(a);
	Ptr<HdPacket> hd = access;		// type convert from a subclass to a baseclass
	Send(hd);
}

void 	HdVehicle::SendWarningsPacket()		// Select Warnings from m_packetNotSentLog according to the number of m_accessLog, add to m_packetSentLog
{
	std::vector<unsigned int>::iterator it_rb;
	unsigned int i;
	for(i=0;i<m_accessLog.size();i++)
	{
		m_packetNotSentLog[i].m_rb = m_accessLog[i];
		Ptr<HdPacket> hd = m_packetSentLog[i];
		unsigned int t = (unsigned int)(Simulator::Now()::GetMilliSeconds());		// Check if MilliSecondes() exists in ns3
		m_packetSentLog.insert(make_pair<unsigned int, unsigned int>(m_packetNotSentLog[i].m_packetId,m_packetNotSentLog[i].m_time - t));
		Send(hd);
	}
	m_packetNotSentLog.erase(m_packetNotSentLog.begin(),m_packetNotSentLog.begin()+i);
	m_accessLog.clear();
}

void	HdVehicle::SendRelayPacket()
{
	for(unsigned int i=0;i<m_packetRelayLog.size();i++)
	{
		Send(m_packetRelayLog[i]);
	}
	m_packetRelayLog.clear();
}

void 	HdVehicle::UpdateLog()		// Deal with m_packetNotSentLog m_packetSentLog m_packetRelayLog, remaining valid time minus 1
{
	std::map<unsigned int, unsigned int>::iterator it_sent;
	for(it_sent = m_packetSentLog.begin();it_sent!=m_packetSentLog.end())
	{
		(it_sent->second)--;
		if(it_sent->second < 0)
		{
			m_packetSentLog.erase(it_sent);
			m_failPacketNum++;
		}
		else
		{
			++it_sent;
		}
	}

	std::vector<Ptr<WarningsPacket> >::iterator it_not;
	for(it_not=m_packetNotSentLog.begin();it_not!=m_packetNotSentLog.end();)
	{
		if((*it_not)->m_time <= Simulator::Now())
		{
			m_packetNotSentLog.erase(it_not);
			m_failPacketNum++;
		}
		else
		{
			++it_not;
		}
	}

	std::vector<Ptr<HdPacket> >::iterator it_relay;
	for(it_relay=m_packetNotSentLog.begin();it_relay!=m_packetNotSentLog.end();)
	{
		if((*it_relay)->m_time <= Simulator::Now())
		{
			m_packetNotSentLog.erase(it_relay);
		}
		else
		{
			++it_relay;
		}
	}
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

void 	HdPacket::Send(Ptr<HdPacket> &hd)			// TO BE CONTINUE,
{

}

}//namespace ns3