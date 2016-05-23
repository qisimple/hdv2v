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
	this->m_preambleId = value.m_preambleId;
	this->m_subSlot = value.m_subSlot;
	this->m_rb = value.m_rb;
	return *this;
}
bool 	AccessInfo::operator==(const AccessInfo &value)
{
	bool 	res = true;
	if((this->m_preambleId != value.m_preambleId)||(this->m_subSlot != value.m_subSlot)||(this->m_rb != value.m_rb))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const AccessInfo value)
{
    out	<<" m_preambleId = "<<value.m_preambleId
    	<<" m_subSlot = "<<value.m_subSlot
    	<<" m_rb = "<<value.m_rb
    	<<std::endl;
    return out;
}
AccessRequestPacket::AccessRequestPacket(const AccessInfo &accessInfo)
{
	memcpy(&m_accessInfo, &accessInfo, sizeof(AccessInfo));
	this->SetPacketType(ACCESS_REQUEST_PACKET);
}
AccessRequestPacket::~AccessRequestPacket(){}
AccessInfo 	AccessRequestPacket::GetAccess()
{
	return m_accessInfo;
}


AccessResultPacket::AccessResultPacket(const std::map< AccessInfo , std::vector<unsigned int> > &schedule)
{
	m_schedule = schedule;
	this->SetPacketType(ACCESS_RESULT_PACKET);
}
AccessResultPacket::~AccessResultPacket(){}
std::vector<unsigned int> 	AccessResultPacket::GetRb(const AccessInfo &accessInfo)
{
	std::vector<unsigned int> 	m_rbs;
	std::map< AccessInfo , std::vector<unsigned int> >::iterator it;
	for(it = m_schedule.begin(); it!=m_schedule.end(); ++it)
	{
		if(memcmp(&accessInfo, &(it->first), sizeof(AccessInfo)) == 0)
		{
			m_rbs = it->second;
			break;
		}
	}
	return	m_rbs;
}


NotifyBroadcastVehiclesPacket::NotifyBroadcastVehiclesPacket(const std::string &zoneId, const std::vector<AccessInfo> &accessVehicles)
{
	m_accessVehicles = accessVehicles;
	m_zoneId = zoneId;
	this->SetPacketType(NOTIFY_BROADCAST_VEHICLES_PACKET);
}
NotifyBroadcastVehiclesPacket::~NotifyBroadcastVehiclesPacket(){}
std::vector<AccessInfo>	NotifyBroadcastVehiclesPacket::GetAccessVechicles()
{
	return m_accessVehicles;
}
std::string	NotifyBroadcastVehiclesPacket::GetZoneId()
{
	return m_zoneId;
}


ResponseBroadcastVehiclesPacket::ResponseBroadcastVehiclesPacket(const std::string &zoneId, const std::map< AccessInfo , std::vector<unsigned int> > &schedule)
{
	m_schedule = schedule;
	m_zoneId = zoneId;
	this->SetPacketType(RESPONSE_BROADCAST_VEHICLES_PACKET);
}
ResponseBroadcastVehiclesPacket::~ResponseBroadcastVehiclesPacket(){}
std::map< AccessInfo , std::vector<unsigned int> >	ResponseBroadcastVehiclesPacket::GetAccessSchedule()
{
	return m_schedule;
}
std::string	ResponseBroadcastVehiclesPacket::GetZoneId()
{
	return m_zoneId;
}


WarningsInfo &WarningsInfo::operator=(const WarningsInfo &value)
{
	this->m_vehicleId = value.m_vehicleId;
	this->m_packetId = value.m_packetId;
	this->m_priorityType = value.m_priorityType;
	return *this;
}
bool 	WarningsInfo::operator==(const WarningsInfo &value)
{
	bool res = true;
	if((this->m_vehicleId != value.m_vehicleId)||(this->m_packetId = value.m_packetId)||(this->m_priorityType = value.m_priorityType))
	{
		res = false;
	}
	return res;
}
std::ostream& operator<<(std::ostream &out,const WarningsInfo value)
{
    out	<<" m_vehicleId = "<<value.m_vehicleId
    	<<" m_packetId = "<<value.m_packetId
    	<<" m_priorityType = "<<value.m_priorityType
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