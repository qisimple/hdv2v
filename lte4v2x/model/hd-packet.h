#ifndef HD_PACKET_H
#define HD_PACKET_H

#include <ns3/ptr.h>
#include <ns3/simple-ref-count.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>

namespace ns3 {

enum HdPacketType
{
	ACCESS_PACKET,	// From vehicle to RSU
	CONTROL_PACKET,	// From Rsu to vehicles
	// NOTIFY_BROADCAST_VEHICLES_PACKET,	// From RSU to its adjacent RSUs
	// RESPONSE_BROADCAST_VEHICLES_PACKET,	// From its adfacent RSUs to  RSU itself
	WARNINGS_PACKET,	// Vehicles broadcast warnings
	RELAY_PACKET		// Rsu broadcast its relayed packets
};

enum PriorityType
{
	HIGH,			// Warnings 
	MIDDLE,		// Beacons
	LOW			// Normal 
};

class 	HdPacket : public SimpleRefCount<HdPacket>
{
public:
	HdPacket ();
	virtual ~HdPacket ();
	/**
	* \brief Set the type of the packet
	* \param type the type of the packet
	*/
	void SetPacketType (const HdPacketType &type);
	/**
	* \brief Get the type of the packet
	* \return the type of the packet
	*/
	HdPacketType GetPacketType ();

private:
	HdPacketType m_type;
};


struct 	AccessInfo
{
	friend 	std::ostream &operator<<(std::ostream &out,const AccessInfo &value);
	AccessInfo &operator=(const AccessInfo &value);
	bool 	operator==(const AccessInfo &value);
	unsigned int 	m_vehicleId;
	unsigned int 	m_rsuId;		// The rsu a vehicle belongs to
	unsigned int	m_num;		//  Required number of rbs
	double 	m_xLabel;			// Pos of vehicle
	double 	m_yLabel;
	PriorityType 	m_priorityType;
};
class 	AccessPacket : public HdPacket
{

public:
	AccessPacket(const AccessInfo &accessInfo);
	virtual	~AccessPacket();
	AccessInfo	GetAccess();
private:
	AccessInfo 	m_accessInfo;
};


struct 	ControlInfo 
{
	friend 	std::ostream &operator<<(std::ostream &out,const ControlInfo &value);
	ControlInfo &operator=(const ControlInfo &value);
	bool 	operator==(const ControlInfo &value);
	unsigned int 	m_vehicleId;
	std::vector<unsigned int> 	m_rbs;
	/* data */
};
class 	ControlPacket : public HdPacket
{
public:
	ControlPacket();
	virtual	~ControlPacket();
	std::vector<unsigned int> GetRb(const unsigned int &vehicleId);
	bool 	GetRelayNode(const unsigned int &vehicleId);
	void 	AddControlInfo(const ControlInfo &value);
	void 	AddRelayNodeId(const unsigned int &vehicleId);
private:
	std::vector<ControlInfo> 	m_schedule;
	std::vector<unsigned int> 	m_relayNodeId;
};


// class 	NotifyBroadcastVehiclesPacket : public HdPacket
// {
// public:
// 	NotifyBroadcastVehiclesPacket(const std::string &zoneId, const std::vector<AccessInfo> &accessVehicles);
// 	virtual 	~NotifyBroadcastVehiclesPacket();
// 	std::vector<AccessInfo> GetAccessVechicles();
// 	std::string 	GetZoneId();
// private:
// 	std::vector<AccessInfo> 	m_accessVehicles;
// 	std::string	m_zoneId;
// };
// class 	ResponseBroadcastVehiclesPacket : public HdPacket
// {
// public:
// 	ResponseBroadcastVehiclesPacket(const std::string &zoneId, const std::map< AccessInfo , std::vector<unsigned int> > &schedule);
// 	virtual 	~ResponseBroadcastVehiclesPacket();
// 	std::map< AccessInfo , std::vector<unsigned int> > GetAccessSchedule();
// 	std::string 	GetZoneId();
// private:
// 	std::map< AccessInfo , std::vector<unsigned int> > m_schedule;
// 	std::string	m_zoneId;
// };



struct WarningsInfo
{
	friend 	std::ostream &operator<<(std::ostream &out,const WarningsInfo value);
	WarningsInfo &operator=(const WarningsInfo &value);
	bool 	operator==(const WarningsInfo &value);
	unsigned int 	m_vehicleId;
	unsigned int	m_packetId;
	std::vector<unsigned int> m_rbs;
	PriorityType 	m_priorityType;
	/* data */
};
class 	WarningsPacket : public HdPacket
{
public:
	WarningsPacket(const WarningsInfo &warningsInfo);
	virtual 	~WarningsPacket();
	WarningsInfo 	GetWarnings();
private:
	WarningsInfo 	m_warningsInfo;
};
class 	RelayPacket : public HdPacket
{
public:
	RelayPacket(const std::vector<WarningsInfo> &relay);
	virtual 	~RelayPacket();
	std::vector<WarningsInfo> GetRelay();
private:
	std::vector<WarningsInfo>	m_relay;
	// m_relayNodeId;		// Id of relay node
};

}// namespace ns3
#endif /* HD_PACKET_H*/