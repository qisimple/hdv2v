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
	this->m_rusId = value.m_rusId;
	this->m_num = value.m_num;
	this->m_xLabel = value.m_xLabel;
	this->m_yLabel = value.m_yLabel;
	this->m_type = value.m_type;
	return *this;
}
bool 	AccessInfo::operator==(const AccessInfo &value)
{
	bool 	res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_rusId != value.m_rusId)||(this->m_num != value.m_num)||
		(this->m_xLabel != value.m_xLabel)||(this->m_yLabel != value.m_yLabel)||(this->m_type != value.m_type))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const AccessInfo &value)
{
	out	<<" m_vehicleId = "<<value.m_vehicleId
		<<" m_rusId = "<<value.m_rusId
		<<" m_num = "<<value.m_num
		<<" m_xLabel = "<<value.m_xLabel
		<<" m_yLabel = "<<value.m_yLabel
		<<" m_type = "<<value.m_type
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
	for(unsigned int i=0; i< m_rbs.size(); i++)
	{
		out <<" rb: "<<m_rbs[i] << std::endl;
	}
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
std::vector<unsigned int> 	ControlPacket::GetRelayNode(const unsigned int &vehicleId)
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
void 	ControlPacket::AddControlInfo(const ControlInfo &value);
{
	ControlInfo m_value = value;
	m_rbs.push_back(m_value);
}
void 	ControlPacket::AddRelayNodeId(const unsigned int &vehicleId);
{
	unsigned int	m_value = vehicleId;
	m_relayNodeId.push_back(m_value);
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
	this->m_rbs = value.m_rbs;
	this->m_priorityType = value.m_priorityType;
	return *this;
}
bool 	WarningsInfo::operator==(const WarningsInfo &value)
{
	bool res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_packetId = value.m_packetId)||
		(this->m_rbs = value.m_rbs)||(this->m_priorityType = value.m_priorityType))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const WarningsInfo &value)
{
	out	<<" m_vehicleId = "<<value.m_vehicleId
		<<" m_packetId = "<<value.m_packetId
		<<" m_priorityType = "<<value.m_priorityType
		<<std::endl;
	for(unsigned int i=0; i< m_rbs.size(); i++)
	{
		out <<" rb: "<<m_rbs[i] << std::endl;
	}
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
}//namespace ns3