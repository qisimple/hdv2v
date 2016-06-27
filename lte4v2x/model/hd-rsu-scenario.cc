#include "hd-rsu-scenario.h"
#include <sstream>
#include <fstream>

namespace ns3{

HdRsuScenario::HdRsuScenario(const std::string &traceFile, const std::string &parFile)
:m_traceFile(traceFile),
m_parFile(parFile),
m_validTime(0),
m_sendProbility(0),
m_vehicleInfo(),
m_rsuInfo(),
m_hdVehRsu()
{

}

HdRsuScenario::~HdRsuScenario(){}

void 	HdRsuScenario::Start()
{
	ParseParFile();		// Get HdRsuInfo
	ParseTraceFile();	//	Get HdVehicleInfo
	Init();				// Init HdRsu and HdVehicle
}


/* There are two way to calculate result: 
one is focus on one vehicle, m_receivedPacketNum/sum_of_surroundings(m_totalPacketNum)
the other is average_of_all(vehicles)
TO Be Finished
*/
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
				m_vehicleInfo.push_back(hdVehicleInfo);
			}
			else
			{
				continue;
			}
		}
	}
	file.close ();
}

void 	HdRsuScenario::ParseParFile()
{
	m_validTime = 10;
	m_sendProbility = 0.1;
	double xLabel = 500;
	double yLabel = 0;
	for(unsigned int i=0; i<3; i++)	// configure m_rsuInfo
	{
		if(i != 0)
		{
			xLabel += 800;
		}
		Ptr<HdRsuInfo> hdRsuInfo = Create<HdRsuInfo>();
		hdRsuInfo->rsuId = i;
		hdRsuInfo->xLabel = xLabel;
		hdRsuInfo->yLabel = yLabel;
		m_rsuInfo.push_back(hdRsuInfo);
	}
}

void	HdRsuScenario::Init()
{
	std::vector<unsigned int> 	zoneId;
	unsigned int rsuId;
	std::vector<Ptr<HdVehicleInfo> > vehInfo;
	for(unsigned int i=0;i<m_rsuInfo.size();i++)		// Init HdRsu
	{
		zoneId.clear();
		vehInfo.clear();
		if(m_rsuInfo[i]->xLabel<800)
		{
			for(unsigned int j=0;j<4;j++)
			{
				zoneId.push_back(j);
			}
		}
		else if(m_rsuInfo[i]->xLabel<1600)
		{
			for(unsigned int j=4;j<8;j++)
			{
				zoneId.push_back(j);
			}
		}
		else if(m_rsuInfo[i]->xLabel<2600)
		{
			for(unsigned int j=8;j<=12;j++)
			{
				zoneId.push_back(j);
			}
		}
		else
		{
			// Beyond 
		}

		double lboundry = i*800;		// Calculate the boundry of each rsu;
		double rboundry; 
		if(i == m_rsuInfo.size()-1)
		{
			rboundry = (i+1)*800 + 200;
		}
		else
		{
			rboundry = (i+1)*800;
		}
		for(unsigned int k=0;k<m_vehicleInfo.size();k++)
		{
			if((m_vehicleInfo[k]->xLabel >= lboundry) && (m_vehicleInfo[k]->xLabel < rboundry))
			{
				vehInfo.push_back(m_vehicleInfo[k]);
			}
		}
		Ptr<HdRsu> rsu = Create<HdRsu>(m_rsuInfo[i]->rsuId, m_rsuInfo[i]->xLabel, m_rsuInfo[i]->yLabel, zoneId, vehInfo);
		std::vector<Ptr<HdVehicle> > v;
		m_hdVehRsu.insert(make_pair<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >(rsu, v));
	}

	for(unsigned int i=0;i<m_vehicleInfo.size();i++)	// Init HdVehicle
	{
		rsuId = 10000;			// Suppose rsuId is less than 10000;
		if(m_vehicleInfo[i]->xLabel<800)
		{
			rsuId = 0;
		}
		else if(m_vehicleInfo[i]->xLabel<1600)
		{
			rsuId = 1;
		}
		else if(m_vehicleInfo[i]->xLabel<2600)
		{
			rsuId = 2;
		}
		else
		{
			// Beyond 
		}

		Ptr<HdVehicle> veh  = Create<HdVehicle>(rsuId, m_vehicleInfo[i]->vehicleId, m_validTime, m_vehicleInfo[i]->xLabel, 
												m_vehicleInfo[i]->yLabel, m_vehicleInfo[i]->velocity, m_sendProbility);
		std::map<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >::iterator it;
		for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();it++)
		{
			if(it->first->GetRsuId() == rsuId)
			{
				(it->second).push_back(veh);
			}
		}
	}
}

}//namespace ns3