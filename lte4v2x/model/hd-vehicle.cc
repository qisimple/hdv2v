#include "hd-vehicle.h"
#include "hd-rsu-scenario.h"
#include <cstdio>
#include <cstring>
#include <utility>
#include <cassert>
#include "ns3/simulation-singleton.h"

namespace ns3{

HdVehicle::HdVehicle(HdVehicleParameter &par, HdRsuScenario *hdSce)
:m_relayNode(false),
m_totalPacketNum(0),
m_efficientPacketNum(0),
m_actualSendPacketNum(0),
m_failPacketNum(0),
m_totalReceivePacketNum(0),
m_nextPacketId(0),
m_totalDelay(0),
m_hdSce(hdSce),
m_packetSentLog(),
m_accessLog(),
m_packetNotSentLog(),
m_packetRelayLog()
{
	m_ptrRandom = CreateObject<UniformRandomVariable>();
	m_rsuId = par.rsuId;
	m_vehicleId = par.vehicleId;
	m_validTime = par.validTime;
	m_xLabel = par.xLabel;
	m_yLabel = par.yLabel;
	m_velocity = par.velocity;
	m_sendProbility = par.sendProbility;
	Update();
}
HdVehicle::~HdVehicle(){}

void 	HdVehicle::Update()
{
	// std::cout<<"get into HdVehicle::Update"<<std::endl;
	UpdateLog();		// delete the validTime=0 items
	double random_value = m_ptrRandom->GetValue(double(0.0), double(1.0));
	unsigned int t = Simulator::Now().GetMilliSeconds();		// Check if MilliSecondes() exists in ns3
	if(random_value < m_sendProbility)	// If true, we get a new packet, inset it to m_packetNotSentLog
	{
		WarningsInfo 	war;
		war.m_vehicleId = m_vehicleId;
		war.m_packetId = m_nextPacketId++;
		// m_rb will be allocated when this packet is sended
		war.m_priorityType = HIGH;
		war.m_time = t + m_validTime;
		war.m_xLabel = m_xLabel;
		war.m_yLabel = m_yLabel;
		// std::cout<<"m_time:"<<war.m_time<<" "
		// 	<<"now:"<<Simulator::Now().GetMilliSeconds()<<" "
		// 	<<std::endl;
		Ptr<WarningsPacket> m_war = Create<WarningsPacket>(war);
		m_packetNotSentLog.push_back(m_war);
		m_totalPacketNum++;
	}

	if(m_relayNode)
	{
		if( t%2 == 0)
		{
			// 	Wait to receive warnings and store it.
		}
		else
		{
			Simulator::Schedule(Seconds(0.0001), &HdVehicle::SendRelayPacket, this);
		}
	}
	else
	{
		if(t%2 == 0)
		{
			if((m_packetNotSentLog.size()>0)&&(m_accessLog.size()>0))			// Necessary to access the channel;
			{
				Simulator::Schedule(Seconds(0.0001), &HdVehicle::SendWarningsPacket, this);
			}
			else if((m_packetNotSentLog.size()>0)&&(m_accessLog.size()==0))
			{
				Simulator::Schedule(Seconds(0.0001), &HdVehicle::SendAccessPacket, this);		// Send at the begining of time slot, and Rsu suppose to receive at the end of the same time slot
			}
			else
			{
				// Wait to receive warnings
			}			
		}
		else
		{
			// Wait to receive control messages and relay messages
		}

	}
	Simulator::Schedule(Seconds(0.001), &HdVehicle::Update, this);
}

void 	HdVehicle::ReceiveHdPacket(Ptr<HdPacket> &msg)
{
	// std::cout<<"get into HdVehicle::ReceiveHdPacket"<<std::endl;
	unsigned int t = Simulator::Now().GetMilliSeconds();		// Check if MilliSecondes() exists in ns3
	switch (msg->GetPacketType())
	{
		case CONTROL_PACKET:
		{
			if(m_relayNode == false && t%2 == 1)
			{
				Ptr<ControlPacket>	con = DynamicCast<ControlPacket>(msg);
				m_accessLog.clear();
				m_accessLog = con->GetRb(m_vehicleId);
				m_relayNode = con->GetRelayNode(m_vehicleId);
			}
			else
			{
				// Abundon the packets;
			}
			break;
		}
		case WARNINGS_PACKET:
		{
			Ptr<WarningsPacket>	war = DynamicCast<WarningsPacket>(msg);
			if(m_relayNode == true && t%2 == 0)
			{
				m_packetRelayLog.push_back(msg);
			}
			else
			{
				// Abundon the packets;
			}
			m_totalReceivePacketNum++;	// A vehicle send access while others are broadcasting, it will miss the packet. But it will receive it from RelayPacket.
			break;
		}
		case RELAY_PACKET:		// Check the m_packetSentLog, if m_packetSentLog in relayPacket,then take it as a success else take it as a failure;
		{
			if(m_relayNode == false && t%2 == 1)
			{
				Ptr<RelayPacket>	relay = DynamicCast<RelayPacket>(msg);
				std::vector<Ptr<WarningsPacket> >::iterator it;
				for(it = m_packetSentLog.begin();it!=m_packetSentLog.end();)
				{
					unsigned int v = (*it)->GetVehicleId();
					unsigned int p = (*it)->GetPacketId();
					if(relay->GetExist(v,p))
					{
						m_efficientPacketNum++;
						unsigned int t = Simulator::Now().GetMilliSeconds();
						m_totalDelay += t + m_validTime - (*it)->GetTime();
						m_packetSentLog.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
			else
			{
				// Abundon the packets
			}
			break;
		}
		default:
		{
			std::cout<<"Error Packet type!"<<std::endl;
		}
	}
}

void 	HdVehicle::SendAccessPacket()
{
	// std::cout<<"get into HdVehicle::SendAccessPacket"<<std::endl;
	AccessInfo a;
	a.m_vehicleId = m_vehicleId;
	a.m_rsuId = m_rsuId;
	a.m_num = m_packetNotSentLog.size();
	a.m_xLabel = m_xLabel;
	a.m_yLabel = m_yLabel;
	a.m_priorityType = HIGH;
	Ptr<AccessPacket> access = Create<AccessPacket>(a);
	Ptr<HdPacket> hd = access;		// type convert from a subclass to a baseclass
	Send(hd);
}

void 	HdVehicle::SendWarningsPacket()		// Select Warnings from m_packetNotSentLog according to the number of m_accessLog, add to m_packetSentLog
{
	// unsigned int t = Simulator::Now().GetMilliSeconds();
	// std::cout<<t<<"  "<<"get into HdVehicle::SendWarningsPacket"<<std::endl;
	std::vector<unsigned int>::iterator it_rb;
	unsigned int i;
	unsigned int m =0;
	if(m_packetNotSentLog.size()<m_accessLog.size())
	{
		m = m_packetNotSentLog.size();
	}
	else
	{
		m = m_accessLog.size();
	}
	for(i=0;i<m;i++)
	{
		m_packetNotSentLog[i]->SetRb(m_accessLog[i]);
		Ptr<HdPacket> hd = m_packetNotSentLog[i];
		m_packetSentLog.push_back(m_packetNotSentLog[i]);
		Send(hd);
	}
	m_actualSendPacketNum += m;
	m_packetNotSentLog.erase(m_packetNotSentLog.begin(),m_packetNotSentLog.begin()+i);
	m_accessLog.clear();
}

void	HdVehicle::SendRelayPacket()
{
	// unsigned int t = Simulator::Now().GetMilliSeconds();
	// std::cout<<t<<"  "<<"get into HdVehicle::SendRelayPacket"<<std::endl;
	std::vector<WarningsInfo> relay;
	for(unsigned int i=0;i<m_packetRelayLog.size();i++)
	{
		Ptr<WarningsPacket>	war = DynamicCast<WarningsPacket>(m_packetRelayLog[i]);
		WarningsInfo warInfo = war->GetWarnings();
		relay.push_back(warInfo);
	}
	Ptr<RelayPacket>  rea = Create<RelayPacket>(relay,m_vehicleId,m_xLabel,m_yLabel);
	Ptr<HdPacket>	hd = rea;
	Send(hd);
	m_packetRelayLog.clear();
	m_relayNode = false;
}

unsigned int HdVehicle::GetVehicleId()
{
	return m_vehicleId;
}
unsigned int HdVehicle::GetTotalDelay()
{
	return m_totalDelay;
}
int 	HdVehicle::GetTotalPacketNum()
{
	return m_totalPacketNum;
}
int 	HdVehicle::GetEfficientPacketNum()
{
	return m_efficientPacketNum;
}
int 	HdVehicle::GetActualSendPacketNum()
{
	return m_actualSendPacketNum;
}
int 	HdVehicle::GetFailPacketNum()
{
	return m_failPacketNum;
}
int 	HdVehicle::GetTotalReceivePacketNum()
{
	return m_totalReceivePacketNum;
}
int 	HdVehicle::GetNotSentPacketNum()
{
	return m_packetNotSentLog.size();
}
double HdVehicle::GetXLabel()
{
	return 	m_xLabel;
}
double HdVehicle::GetYLabel()
{
	return 	m_yLabel;
}

bool 	HdVehicle::IfSurround(double x, double y)		// Assume AR is 200m
{
	bool res = false;
	x = x - m_xLabel;
	y = y - m_yLabel;
	if(x*x + y*y <= 40000)
	{
		res = true;
	}
	return	res;
}

void 	HdVehicle::UpdateLog()		// Deal with m_packetNotSentLog m_packetSentLog m_packetRelayLog, remaining valid time minus 1
{
	// std::cout<<"get into HdVehicle::UpdateLog"<<std::endl;
	std::vector<Ptr<WarningsPacket> >::iterator it_sent;
	for(it_sent = m_packetSentLog.begin();it_sent!=m_packetSentLog.end();)
	{
		if((*it_sent)->GetTime() < Simulator::Now().GetMilliSeconds())
		{
			m_packetSentLog.erase(it_sent);
			m_failPacketNum++;
			m_totalDelay += m_validTime;
		}
		else
		{
			++it_sent;
		}
	}

	std::vector<Ptr<WarningsPacket> >::iterator it_not;
	for(it_not=m_packetNotSentLog.begin();it_not!=m_packetNotSentLog.end();)
	{
		if((*it_not)->GetTime() <= Simulator::Now().GetMilliSeconds())
		{
			m_packetNotSentLog.erase(it_not);
			m_failPacketNum++;
			m_totalDelay += m_validTime;
		}
		else
		{
			++it_not;
		}
	}

	std::vector<Ptr<HdPacket> >::iterator it_relay;
	for(it_relay=m_packetRelayLog.begin();it_relay!=m_packetRelayLog.end();)
	{
		Ptr<WarningsPacket> war = DynamicCast<WarningsPacket>(*it_relay);
		if( war->GetTime() < Simulator::Now().GetMilliSeconds())
		{
			m_packetRelayLog.erase(it_relay);
		}
		else
		{
			++it_relay;
		}
	}
}

void 	HdVehicle::Send(Ptr<HdPacket> &hd)			// TO BE CONTINUE,
{
	// std::cout<<"get into HdVehicle::Send"<<std::endl;
	switch (hd->GetPacketType())
	{
		case ACCESS_PACKET:
		{
			m_hdSce->DoSend(hd, m_vehicleId, m_rsuId);
			break;
		}
		case WARNINGS_PACKET:
		case RELAY_PACKET:
		{
			m_hdSce->DoSend(hd, m_vehicleId, BROADCAST);
			break;
		}
		default:
		{
			std::cout<<"HdVehicle:Error Wrong Type!"<<hd->GetPacketType()<<std::endl;
		}
	}
}

}//namespace ns3