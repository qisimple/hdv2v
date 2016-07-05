#include "hd-rsu.h"
#include "hd-rsu-scenario.h"
#include <cassert>
#include <cstdlib>
#include <utility>
#include "ns3/simulation-singleton.h"

namespace ns3{ 

HdRsu::HdRsu(unsigned int rsuId, double xLabel, double yLabel,std::vector<unsigned int> &zoneId, std::vector<Ptr<HdVehicleInfo> > &vehInfo, HdRsuScenario *hdSce)
:m_status(false),
m_hdSce(hdSce),
m_usedRb(0),
m_assignRb(),
m_assignRelayNode(),
m_lastRelayNode(),
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
	Update();
}
HdRsu::~HdRsu(){}

void 	HdRsu::Update()
{
	// std::cout<<"get into HdRsu::Update"<<std::endl;
	StateConvert(m_status);
	if(m_status == false)	// Send ControlPacket
	{
		UpdateLog();
		Simulator::Schedule(Seconds(0.0001), &HdRsu::SendControlPacket, this);
	}
	else
	{
		InitLog();
	}
	Simulator::Schedule(Seconds(0.001),&HdRsu::Update, this);
}

void 	HdRsu::ReceiveHdPacket(Ptr<HdPacket> &msg)
{
	// std::cout<<"get into HdRsu::ReceiveHdPacket"<<std::endl;
	switch(msg->GetPacketType())
	{
		case ACCESS_PACKET:
		{
			if(m_status == true)
			{
				Ptr<AccessPacket>	accPkt = DynamicCast<AccessPacket>(msg);
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
				// Wrong time slot; rsu can only send control packet now
			}
			break;
		}
		default:
		{
			std::cout<<"HdRsu:Error!Wrong Type!"<<msg->GetPacketType()<<std::endl;
		}
	}
}

void 	HdRsu::SendControlPacket()
{
	unsigned int t = Simulator::Now().GetMilliSeconds();
	std::cout<<t<<"  "<<"get into HdRsu::SendControlPacket"<<std::endl;
	Ptr<ControlPacket> con = Create<ControlPacket>();
	std::map<unsigned int, std::vector<unsigned int> >::iterator it_rb;	
	for(it_rb=m_assignRb.begin();it_rb!=m_assignRb.end();++it_rb)
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
	m_usedRb.clear();
	m_accessLog.clear();
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
	// std::cout<<"get into HdRsu::UpdateLog"<<std::endl;
	AssignRbs();
	AssignRelayNodes();
}

void 	HdRsu::InitLog()
{
	for(unsigned int i=0;i<m_zoneId.size();i++)		// Init m_usedRb, the value of Rb is from 0 to 47, with each zone 16 rbs
	{
		unsigned int m = m_zoneId[i] % 3;
		m_usedRb.push_back(m*16);
	}
	for(unsigned int i=0;i<m_zoneId.size();i++)		// Init m_accessLog
	{
		std::vector<AccessInfo> v;
		m_accessLog.push_back(v);
	}
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
	std::cout<<"m_accessLog[0].size"<<m_accessLog[0].size()
		<<"m_accessLog[1].size"<<m_accessLog[1].size()
		<<"rsuId:"<<m_rsuId
		<<std::endl;
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
}

void 	HdRsu::AssignRelayNodes()	// Best fit vehicle, (Position, not involved in m_assignRb)
{
	std::vector<double>	dis;
	for(unsigned int i=0;i<m_zoneId.size();i++)
	{
		double x;
		if(m_accessLog[i].size() == 0)
		{
			x = -1;				// Init, empty zone doesn't need any transfer nodes
		}
		else
		{
			x = 10000;			// Init, and assume the length of scenario is less then 10000
		}
		unsigned int veh = 1000000;		// Init, and assume the id of veh in scenario is less than 1000000
		dis.push_back(x);
		m_assignRelayNode.push_back(veh);
	}
	for(unsigned int i=0;i<m_vehicle.size();i++)
	{
		std::map<unsigned int, std::vector<unsigned int> >::iterator it;
		bool 	res = false;
		for(it=m_assignRb.begin();it!=m_assignRb.end();++it)
		{
			if(m_vehicle[i]->vehicleId == it->first)
			{
				res = true;
				break;
			}
		}
		for(unsigned int k=0;k<m_lastRelayNode.size();k++)
		{
			if(m_vehicle[i]->vehicleId == m_lastRelayNode[k])
			{
				res = true;
				break;
			}
		}
		if(!res)
		{
			for(unsigned int j=0;j<m_zoneId.size();j++)
			{
				if(abs(m_vehicle[i]->xLabel - m_zoneId[j]*200) < dis[j])
				{
					dis[j] = m_vehicle[i]->xLabel - m_zoneId[j]*200;
					m_assignRelayNode[j] = m_vehicle[i]->vehicleId;
				}
			}
		}
	}
	m_lastRelayNode.clear();
	m_lastRelayNode = m_assignRelayNode;
	
	unsigned int t = Simulator::Now().GetMilliSeconds();
	std::cout<<t<<"  "<<"get into HdRsu::AssignRelayNodes"
		<<"size:"<<m_assignRelayNode.size()
		<<std::endl;
}

void 	HdRsu::Send(Ptr<HdPacket> &con)			// TO BE CONTINUE
{
	// std::cout<<"get into HdRsu::Send"<<std::endl;
	m_hdSce->DoSend(con, m_rsuId, BROADCAST);
}

// bool	HdRsu::InAccess(const Ptr<HdVehicleInfo> vehicleInfo)
// {
// 	// std::cout<<"get into HdRsu::InAccess"<<std::endl;
// 	bool 	res = false;
// 	double 	x = vehicleInfo->xLabel;
// 	unsigned int i;
// 	for(i=0;i<m_zoneId.size();i++)
// 	{
// 		if(x<(m_zoneId[i]+1)*200 && x>=m_zoneId[i]*200)
// 		{
// 			break;
// 		}
// 	}
// 	for(unsigned int j=0;j<m_accessLog[i].size();j++)
// 	{
// 		if(vehicleInfo->vehicleId == m_accessLog[i][j].m_vehicleId)
// 		{
// 			res = true;
// 			break;
// 		}
// 	}
// 	return res;
// }

}//namespace ns3