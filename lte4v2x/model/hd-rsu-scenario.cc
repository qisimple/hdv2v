#include "hd-rsu-scenario.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <cassert>

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
	Init();				// Init HdRsu and HdVehicle, schedule the start func of HdRsus and HdVehicles
	// Simulator::Schedule(Seconds(1.0), &HdRsuScenario::CalculateResult, this);
}


/* There are two way to calculate result: 
one is focus on one vehicle, m_receivedPacketNum/sum_of_surroundings(m_totalPacketNum)
the other is average_of_all(vehicles)
TO Be Finished
*/
void 	HdRsuScenario::CalculateResult()
{
	std::cout<<"get into HdRsuScenario::CalculateResult()"<<std::endl;
	std::map<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >::iterator it;
	for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();++it)
	{
		for(unsigned int i=0;i<(it->second).size();i++)
		{
			std::cout << "vehicleId:"<<(it->second)[i]->GetVehicleId()<<" "
				<<"GetTotalPacketNum:"<<(it->second)[i]->GetTotalPacketNum()<<" "
				<<"GetEfficientPacketNum:"<<(it->second)[i]->GetEfficientPacketNum()<<" "
				<<"GetFailPacketNum:"<<(it->second)[i]->GetFailPacketNum()<<" "
				<<"GetTotalReceivePacketNum:"<<(it->second)[i]->GetTotalReceivePacketNum()<<" "
				<<std::endl;
		}
	}
}

void 	HdRsuScenario::ParseTraceFile()
{
	// std::cout<<"get into HdRsuScenario::ParseTraceFile()"<<std::endl;
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
	else
	{
		assert(file.is_open ());
	}
	file.close ();
	std::cout<<"size:"<<m_vehicleInfo.size()<<std::endl;
}

void 	HdRsuScenario::ParseParFile()
{
	// std::cout<<"get into HdRsuScenario::ParseParFile()"<<std::endl;
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
	// std::cout<<"get into HdRsuScenario::Init()"<<std::endl;
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
		Ptr<HdRsu> rsu = Create<HdRsu>(m_rsuInfo[i]->rsuId, m_rsuInfo[i]->xLabel, m_rsuInfo[i]->yLabel, zoneId, vehInfo, this);
		std::vector<Ptr<HdVehicle> > v;
		m_hdVehRsu.insert(std::make_pair<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >(rsu, v));
	}

	for(unsigned int i=0;i<m_vehicleInfo.size();i++)	// Init HdVehicle
	{
		rsuId = 10000;			// Suppose rsuId is less than 10000; usigned int type
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
		HdVehicleParameter par = {rsuId, m_vehicleInfo[i]->vehicleId, m_validTime, m_vehicleInfo[i]->xLabel, m_vehicleInfo[i]->yLabel, m_vehicleInfo[i]->velocity, m_sendProbility};
		Ptr<HdVehicle> veh = Create<HdVehicle>(par, this);
		std::map<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >::iterator it;
		for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();it++)
		{
			if(it->first->GetRsuId() == rsuId)
			{
				(it->second).push_back(veh);
				break;
			}
		}
	}
}

void HdRsuScenario::DoSend(Ptr<HdPacket> &msg, unsigned int src, unsigned int dest)
{
	// std::cout<<"get into HdRsuScenario::DoSend()"<<std::endl;
	std::map<Ptr<HdRsu>, std::vector<Ptr<HdVehicle> > >::iterator it;
	bool res = false;
	switch (msg->GetPacketType())
	{
		case ACCESS_PACKET:		// From vehicle to rsu
		{
			for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();++it)
			{
				for(unsigned int i=0;i<(it->second).size();i++)
				{
					if((it->second)[i]->GetVehicleId() == src)
					{
						res = true;
						break;
					}
				}
				if(res)
				{
					it->first->ReceiveHdPacket(msg);
					break;
				}
			}
			break;
		}
		case CONTROL_PACKET:	// From rsu to vehicle,, there are security problems here, i just leave it here, don't care....
		{
			for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();++it)
			{
				if(it->first->GetRsuId() == src)		// 
				{
					assert(dest == BROADCAST);
					res = true;
					for(unsigned int i=0;i<(it->second).size();i++)
					{
						(it->second)[i]->ReceiveHdPacket(msg);
					}
					break;
				}
			}
			if(!res)
			{
				assert(0);		// No this rsuId
			}
			break;
		}
		case WARNINGS_PACKET:		// From vehicle to surrounded vehicles which are within its AR(awareness range)
		{
			Ptr<WarningsPacket>	war = DynamicCast<WarningsPacket>(msg);
			double x = war->GetXLabel();
			double y = war->GetYLabel();
			unsigned int vehId = war->GetVehicleId();
			for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();++it)
			{
				for(unsigned int i=0;i<(it->second).size();i++)
				{
					if((it->second)[i]->IfSurround(x, y))
					{
						if((it->second)[i]->GetVehicleId()!=vehId)
						{
							(it->second)[i]->ReceiveHdPacket(msg);
						}
					}
				}
			}
			break;
		}
		case RELAY_PACKET:		// From transfer vehicle to surrounded vehicles which are within its AR(awareness range)
		{
			Ptr<RelayPacket>	relay = DynamicCast<RelayPacket>(msg);
			double x = relay->GetXLabel();
			double y = relay->GetYLabel();
			unsigned int vehId = relay->GetRelayNodeId();
			for(it=m_hdVehRsu.begin();it!=m_hdVehRsu.end();++it)
			{
				for(unsigned int i=0;i<(it->second).size();i++)
				{
					if((it->second)[i]->IfSurround(x, y))
					{
						if((it->second)[i]->GetVehicleId()!=vehId)
						{
							(it->second)[i]->ReceiveHdPacket(msg);
						}
					}
				}
			}
			break;
		}
		default:
		{
			std::cout<<"Error Packet type!"<<std::endl;
		}
	}
}

}//namespace ns3