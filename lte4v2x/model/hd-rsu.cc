#include "hd-rsu.h"
#include <cassert>
#include <utility>

namespace ns3{ 

HdRsu::HdRsu(unsigned int rsuId, double xLabel, double yLabel,std::vector<unsigned int> &zoneId, std::vector<Ptr<HdVehicleInfo> > &vehInfo)
:m_status(true),
m_usedRb(0),
m_assignRb(),
m_assignRelayNode(),
m_accessLog(),
m_vehicle()
{
	m_rsuId = rsuId;
	// m_leftZoneId = leftZoneId;
	// m_rightZoneId = rightZoneId;
	m_xLabel = xLabel;
	m_yLabel = yLabel;
	m_zoneId = zoneId;
	m_vehicle = vehInfo;
}
HdRsu::~HdRsu(){}

void 	HdRsu::Update()
{
	if(m_status == false)	// Send ControlPacket
	{
		Simulator::Schedule(Seconds(0.001),&HdRsu::StateConvert, this ,m_status);
		SendControlPacket();
	}
	else
	{
		Simulator::Schedule(Seconds(0.001),&HdRsu::StateConvert, this, m_status);
		Simulator::Schedule(Seconds(0.001),&HdRsu::UpdateLog, this);	
	}
	Simulator::Schedule(Seconds(0.001),&HdRsu::Update, this);
}

void 	HdRsu::ReceiveHdPacket(Ptr<HdPacket> msg)
{
	switch(msg->GetPacketType())
	{
		case ACCESS_PACKET:
		{
			if(m_status == true)
			{
				Ptr<AccessPacket>	accPkt = dynamic_cast<Ptr<AccessPacket> >(msg);
				AccessInfo 	acc = accPkt->GetAccess();
				double 	x = acc.m_xLabel;
				for(unsigned int j=0;j<m_zoneId.size();j++)
				{
					if(x<(m_zoneId[j]+1)*200 && x>=m_zoneId[j]*200)
					{
						m_accessLog[j].push_back(acc);
					}
				}
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
	Ptr<ControlPacket> con = Create<ControlPacket>();
	std::map<unsigned int, std::vector<unsigned int> >::iterator it_rb;	
	for(it_rb=m_assignRb.begin();it_rb!=m_assignRb.end();)
	{
		ControlInfo info;
		info.m_vehicleId = it_rb->first;
		info.m_rbs = it_rb->second;
		con->AddControlInfo(info);
	}
	con->AddRelayNodeId(m_assignRelayNode);
	Ptr<HdPacket>  hd = con;
	Send(hd);
	m_assignRb.clear();
	m_assignRelayNode.clear();
}

// void 	HdRsu::SendNotifyBroadcastVehiclesPacket()
// {}

// void 	HdRsu::SendRelayPacket()
// {}

unsigned int 	HdRsu::GetRsuId()
{
	return m_rsuId;
}

void 	HdRsu::UpdateLog()	// Assign at the end of access slot
{
	AssignRbs();
	AssignRelayNodes();
}

void 	HdRsu::StateConvert(bool &relay)
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

void	HdRsu::AssignRbs()		// Round Robin
{
	std::map<unsigned int, std::vector<unsigned int> >::iterator it;
	std::vector<AccessInfo>::iterator 	it_a;
	for(unsigned int i=0;i<m_zoneId.size();i++)		// Init m_usedRb, the value of Rb is from 0 to 47, with each zone 16 rbs
	{
		m_usedRb[i] = (m_zoneId[i]%3)*16;
	}
	for(unsigned int i=0;i<m_accessLog.size();i++)
	{
		bool res = true;
		bool veh = true;
		while(res)
		{
			if((m_usedRb[i]<(m_zoneId[i]%3)*16+15)&&(veh==true))
			{
				// Allocation is not finished
				unsigned int tmp = m_usedRb[i];
				for(it_a=m_accessLog[i].begin();it_a!=m_accessLog[i].end();++it_a)
				{
					double x = (*it_a).m_xLabel;
					assert(x>=0);
					if(m_usedRb[i]<(m_zoneId[i]%3)*16+15)				// Check if zone's rbs has used up
					{
						for(it=m_assignRb.begin();it!=m_assignRb.end();it++)
						{
							if(it->first == (*it_a).m_vehicleId)
							{
								if(it->second.size() < (*it_a).m_num)
								{
									m_usedRb[i]++;
									it->second.push_back(m_usedRb[i]);
									break;
								}
							}
						}
						if(it==m_assignRb.end())
						{
							std::vector<unsigned int> 	rb;
							m_usedRb[i]++;
							rb.push_back(m_usedRb[i]);
							m_assignRb.insert(std::make_pair<unsigned int, std::vector<unsigned int> >((*it_a).m_vehicleId, rb));
						}
					}
					else
					{
						break;
					}
				}
				if(tmp == m_usedRb[i])		// Check if all vehicles has got enough rbs
				{
					veh = false;
				}
			}
			else
			{
				res = false;
			}
		}
	}
	m_accessLog.clear();
}

void 	HdRsu::AssignRelayNodes()	// Best fit vehicle, (Position, not involved in accessLog)
{
	std::vector<double>	dis;
	for(unsigned int i=0;i<m_zoneId.size();i++)
	{
		double x = 10000;			// Assume the length of scenario is less then 10000
		unsigned int veh = 1000000;		// Assume the id of veh in scenario is less than 1000000
		dis.push_back(x);
		m_assignRelayNode.push_back(veh);
	}
	for(unsigned int i=0;i<m_vehicle.size();i++)
	{
		for(unsigned int j=0;j<m_zoneId.size();j++)
		{
			if(m_vehicle[i]->xLabel - m_zoneId[j]*200 < dis[j])
			{
				dis[j] = m_vehicle[i]->xLabel - m_zoneId[j]*200;
				m_assignRelayNode[j] = m_vehicle[i]->vehicleId;
			}
		}
	}
}

void 	HdRsu::Send(Ptr<HdPacket> &con)			// TO BE CONTINUE,
{

}

bool	HdRsu::InAccess(const Ptr<HdVehicleInfo> vehicleInfo)
{
	bool 	res = false;
	double 	x = vehicleInfo->xLabel;
	unsigned int i;
	for(i=0;i<m_zoneId.size();i++)
	{
		if(x<(m_zoneId[i]+1)*200 && x>=m_zoneId[i]*200)
		{
			break;
		}
	}
	for(unsigned int j=0;j<m_accessLog[i].size();j++)
	{
		if(vehicleInfo->vehicleId == m_accessLog[i][j].m_vehicleId)
		{
			res = true;
			break;
		}
	}
	return res;
}

}//namespace ns3