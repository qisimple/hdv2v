#include "ns3/core-module.h"
#include "ns3/hd-rsu-scenario.h"
#include <string>

using namespace ns3;

int main (int argc, char *argv[])
{
	double 		simTime = 0.1;	// Seconds
	std::string 	traceFile = "hello.mobility.tcl";
	std::string 	parFile = "hello.par.tcl";
	double 		sendProbility = 0.1;
	unsigned int 	mode = 0;

	CommandLine cmd;
	cmd.AddValue("traceFile", "The infos of vehicles", traceFile);
	cmd.AddValue ("sendProbility", "Tell vehicle the possibility of sending messages", sendProbility);
	cmd.AddValue("mode","Tell which mode it is, 0 means hd-transfer mode, 1 means non-transfer mode",mode);	
	cmd.Parse(argc, argv);

 	if(sendProbility > 1)
 	{
	 	sendProbility = sendProbility/100;
 	}
	
	Ptr<HdRsuScenario> hdSce = Create<HdRsuScenario>(traceFile, parFile, sendProbility);
 	if(mode == 0)
 	{
		hdSce->Start();
 	}

	Simulator::Stop(Seconds(simTime));
	Simulator::Run();
	Simulator::Destroy();

 	if(mode == 0)
 	{
		hdSce->CalculateResult();
 	}

	return 0;
}