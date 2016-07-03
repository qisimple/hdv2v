#include "hd-packet.h"
#include <cstdio>
#include <cstring>

namespace ns3{

HdPacket::HdPacket(){}
HdPacket::~HdPacket(){}
void 	HdPacket::SetPacketType(const HdPacketType &type)
{
	m_type = type;
}
HdPacketType 	HdPacket::GetPacketType()
{
	return 	m_type;
}


AccessInfo& AccessInfo::operator=(const AccessInfo &value)
{
	this->m_vehicleId = value.m_vehicleId;
	this->m_rsuId = value.m_rsuId;
	this->m_num = value.m_num;
	this->m_xLabel = value.m_xLabel;
	this->m_yLabel = value.m_yLabel;
	this->m_priorityType = value.m_priorityType;
	return *this;
}
bool 	AccessInfo::operator==(const AccessInfo &value)
{
	bool 	res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_rsuId != value.m_rsuId)||(this->m_num != value.m_num)||
		(this->m_xLabel != value.m_xLabel)||(this->m_yLabel != value.m_yLabel)||(this->m_priorityType != value.m_priorityType))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const AccessInfo &value)
{
	out	<<" m_vehicleId = "<<value.m_vehicleId
		<<" m_rsuId = "<<value.m_rsuId
		<<" m_num = "<<value.m_num
		<<" m_xLabel = "<<value.m_xLabel
		<<" m_yLabel = "<<value.m_yLabel
		<<" m_priorityType = "<<value.m_priorityType
		<<std::endl;
	return out;
}
AccessPacket::AccessPacket(const AccessInfo &accessInfo)
{
	memcpy(&m_accessInfo, &accessInfo, sizeof(AccessInfo));
	this->SetPacketType(ACCESS_PACKET);
}
AccessPacket::~AccessPacket(){}
AccessInfo 	AccessPacket::GetAccess()
{
	return m_accessInfo;
}


ControlInfo& ControlInfo::operator=(const ControlInfo &value)
{
	this->m_vehicleId = value.m_vehicleId;
	this->m_rbs = value.m_rbs;
	return *this;
}
bool 	ControlInfo::operator==(const ControlInfo &value)
{
	bool 	res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_rbs != value.m_rbs))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const ControlInfo &value)
{
	out	<<" m_vehicleId = "<<value.m_vehicleId << std::endl;
	for(unsigned int i=0; i< value.m_rbs.size(); i++)
	{
		out <<" rb: "<<value.m_rbs[i];
	}
	out<<std::endl;
	return out;
}
ControlPacket::ControlPacket()
{
	this->SetPacketType(CONTROL_PACKET);
}
ControlPacket::~ControlPacket(){}
std::vector<unsigned int> 	ControlPacket::GetRb(const unsigned int &vehicleId)
{
	std::vector<unsigned int> 	m_rbs;
	std::vector< ControlInfo>::iterator it;
	for(it = m_schedule.begin(); it!=m_schedule.end(); ++it)
	{
		if( it->m_vehicleId == vehicleId)
		{
			m_rbs = it->m_rbs;
			break;
		}
	}
	return	m_rbs;
}
bool 	ControlPacket::GetRelayNode(const unsigned int &vehicleId)
{
	bool res = false;
	for(unsigned int i=0;i<m_relayNodeId.size(); i++)
	{
		if(vehicleId == m_relayNodeId[i])
		{
			res = true;
			break;
		}
	}
	return	res;
}
void 	ControlPacket::AddControlInfo(const ControlInfo &value)
{
	ControlInfo m_value = value;
	m_schedule.push_back(m_value);
}
void 	ControlPacket::AddRelayNodeId(const std::vector<unsigned int> &vehicleId)
{
	m_relayNodeId = vehicleId;
} 	


// NotifyBroadcastVehiclesPacket::NotifyBroadcastVehiclesPacket(const std::string &zoneId, const std::vector<AccessInfo> &accessVehicles)
// {
// 	m_accessVehicles = accessVehicles;
// 	m_zoneId = zoneId;
// 	this->SetPacketType(NOTIFY_BROADCAST_VEHICLES_PACKET);
// }
// NotifyBroadcastVehiclesPacket::~NotifyBroadcastVehiclesPacket(){}
// std::vector<AccessInfo>	NotifyBroadcastVehiclesPacket::GetAccessVechicles()
// {
// 	return m_accessVehicles;
// }
// std::string	NotifyBroadcastVehiclesPacket::GetZoneId()
// {
// 	return m_zoneId;
// }


// ResponseBroadcastVehiclesPacket::ResponseBroadcastVehiclesPacket(const std::string &zoneId, const std::map< AccessInfo , std::vector<unsigned int> > &schedule)
// {
// 	m_schedule = schedule;
// 	m_zoneId = zoneId;
// 	this->SetPacketType(RESPONSE_BROADCAST_VEHICLES_PACKET);
// }
// ResponseBroadcastVehiclesPacket::~ResponseBroadcastVehiclesPacket(){}
// std::map< AccessInfo , std::vector<unsigned int> >	ResponseBroadcastVehiclesPacket::GetAccessSchedule()
// {
// 	return m_schedule;
// }
// std::string	ResponseBroadcastVehiclesPacket::GetZoneId()
// {
// 	return m_zoneId;
// }


WarningsInfo &WarningsInfo::operator=(const WarningsInfo &value)
{
	this->m_vehicleId = value.m_vehicleId;
	this->m_packetId = value.m_packetId;
	this->m_rb = value.m_rb;
	this->m_priorityType = value.m_priorityType;
	this->m_time = value.m_time;
	this->m_xLabel = value.m_xLabel;
	this->m_yLabel = value.m_yLabel;
	return *this;
}
bool 	WarningsInfo::operator==(const WarningsInfo &value)
{
	bool res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_packetId = value.m_packetId)||
		(this->m_rb = value.m_rb)||(this->m_priorityType = value.m_priorityType)||(this->m_time = value.m_time))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const WarningsInfo &value)
{
	out	<<" m_vehicleId = "<<value.m_vehicleId
		<<" m_packetId = "<<value.m_packetId
		<<" m_rb = "<<value.m_rb
		<<" m_priorityType = "<<value.m_priorityType
		<<" m_time = "<<value.m_time
		<<" m_xLabel = "<<value.m_xLabel
		<<" m_yLabel = "<<value.m_yLabel
		<<std::endl;
 	return out;
}
WarningsPacket::WarningsPacket(const WarningsInfo &warningsInfo)
{
	memcpy(&m_warningsInfo, &warningsInfo, sizeof(WarningsInfo));
	this->SetPacketType(WARNINGS_PACKET);
}
WarningsPacket::~WarningsPacket(){}
WarningsInfo 	WarningsPacket::GetWarnings()
{
	return 	m_warningsInfo;
}
void 	WarningsPacket::SetRb(unsigned int rb)
{
	m_warningsInfo.m_rb = rb;
}
unsigned int 	WarningsPacket::GetTime()
{
	return	m_warningsInfo.m_time;
}
unsigned int 	WarningsPacket::GetPacketId()
{
	return m_warningsInfo.m_packetId;
}
unsigned int 	WarningsPacket::GetRb()
{
	return m_warningsInfo.m_rb;
}
double 		WarningsPacket::GetXLabel()
{
	return m_warningsInfo.m_xLabel;
}
double 		WarningsPacket::GetYLabel()
{
	return m_warningsInfo.m_yLabel;
}

RelayPacket::RelayPacket(const std::vector<WarningsInfo> &relay)
{
	m_relay = relay;
	this->SetPacketType(RELAY_PACKET);
}
RelayPacket::~RelayPacket(){}
std::vector<WarningsInfo>	RelayPacket::GetRelay()
{
	return 	m_relay;
}
bool 	RelayPacket::GetExist(unsigned int vehicleId, unsigned int packetId)
{
	bool res = false;
	for(unsigned int i=0;i<m_relay.size();i++)
	{
		if(m_relay[i].m_vehicleId == vehicleId && m_relay[i].m_packetId == packetId)
		{
			res = true;
			break;
		}
	}
	return res;
}


}//namespace ns3