#include "hd-rsu-scenario.h"
#include <sstream>
#include <fstream>

namespace ns3{

HdRsuScenario::HdRsuScenario(const std::string &traceFile, const std::string &parFile)
:m_validTime(0),
m_sendProbility(0),
m_packetLossRate(0),
m_packetAverageDelay(0),
m_packetNum(0),
m_vehicles(),
m_rsus(),
m_zones(),
m_hdVehicle(),
m_hdRsu(),
m_zoneWithVehicles(),
m_zoneWithRsus(),
m_rsuWithZones(),
m_vehicleWithZones(),
m_vehicleWithSurroundedVehicles(),
m_rsuWithSurroundedRsus()
{
	m_traceFile = traceFile;
	m_parFile = parFile;
}

HdRsuScenario::~HdRsuScenario(){}

void 	HdRsuScenario::Start()
{
	ParseTraceFile();
	ParseParFile();
	BuildRtoZMap();
	BuildVtoZMap();
}

void 	HdRsuScenario::CalculateResult()
{

}

void 	HdRsuScenario::ParseTraceFile()
{
	std::ifstream file(m_traceFile.c_str (), std::ios::in);
 	if (file.is_open ())
	{
		while (!file.eof () )
		{
			Ptr<HdVehicleInfo> hdVehicleInfo = Create<HdVehicleInfo>();
			std::vector<std::string> lineInfo;
			std::istringstream	s;
			std::string 	nodeId;
			std::string 	xLabel;
			std::string 	yLabel;
			std::string 	velocity;
			std::string 	line;
			getline (file, line);
			if (line.empty ())	// ignore empty lines
			{
				continue;
			}
			s.str(line);
			while(!s.eof())
			{
				std::string 	x;
				s >> x;
				lineInfo.push_back(x);
			}
			if(lineInfo[2]=="40.0")		// A normal log
			{
				std::string::size_type startNodeId = lineInfo[3].find_first_of ("("); // index of left bracket
				std::string::size_type endNodeId   = lineInfo[3].find_first_of (")"); // index of right bracket
				nodeId = lineInfo[3].substr(startNodeId + 1, endNodeId - (startNodeId + 1));
				xLabel = lineInfo[5];
				yLabel = lineInfo[6];
				velocity = lineInfo[7];
				hdVehicleInfo->vehicleId =(unsigned int) strtol(nodeId.c_str(), NULL, 10);
				hdVehicleInfo->xLabel = strtod(xLabel.c_str(), NULL);
				hdVehicleInfo->yLabel = strtod(yLabel.c_str(), NULL);
				hdVehicleInfo->velocity = strtod(velocity.c_str(), NULL);
				m_vehicles.push_back(hdVehicleInfo);
			}
			else
			{
				continue;
			}
		}
	}
	file.close ();
}

void 	HdRsuScenario::ParseParFile()		// Deal with m_zones, m_rsus, m_validTime, m_sendProbility
{
	m_validTime = 10;
	m_sendProbility = 0.1;
	double xLabel = 0;
	double yLabel = 0;
	for(unsigned int i=1; i<5; i++)	// configure m_rsus
	{
		xLabel += 200;
		Ptr<HdRsuInfo> hdRsuInfo = Create<HdRsuInfo>();
		hdRsuInfo->rsuId = i;
		hdRsuInfo->xLabel = xLabel;
		hdRsuInfo->yLabel = yLabel;
		m_rsus.push_back(hdRsuInfo);
	}
	std::string 	zoneId("zone"); 
	for(int i=1; i<6; i++)		// configure m_zones
	{
		char 	temp[10];
		sprintf(temp, "%d", i);
		std::string 	z(temp);
		z = zoneId + z;
		m_zones.push_back(z);	
	}
}

// void 	HdRsuScenario::BuildRtoZMap()
// {
// 	for(unsigned int i=0;i<m_rsus.size();i++)
// 	{
// 		unsigned int 	rsuId = m_rsus[i]->rsuId;
// 		std::vector<std::string> zoneId;
// 		zoneId.push_back(m_zones[rsuId-1]);
// 		zoneId.push_back(m_zones[rsuId]);
// 		m_rsuWithZones.insert(std::make_pair<unsigned int ,std::vector<std::string> >(rsuId, zoneId));
// 	}
// }

// void 	HdRsuScenario::BuildVtoZMap()
// {
// 	for(unsigned int i=0;i<m_vehicles.size(); i++)
// 	{
// 		unsigned int 	vehicleId = m_vehicles[i]->vehicleId;

// 	}
// }

// void 	HdRsuScenario::CreateHdVehicles()	// create m_hdVehicle based on m_vehicles
// {
// 	for(unsigned int i=0;i<m_vehicles.size();++i)
// 	{
// 		Ptr<HdVehicle> hdVehicle = Create<HdVehicle>(m_vehicles[i]) 
// 	}
// }

}//namespace ns3