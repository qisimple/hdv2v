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
				unsigned int len = lineInfo[7].size();
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

}

}//namespace ns3