#include "ns3/core-module.h"
#include "ns3/hd-rsu-scenario.h"
#include <string>

using namespace ns3;

int main (int argc, char *argv[])
{
	double 		simTime = 10.0;	// Seconds
	std::string 	traceFile = "hello.mobility.tcl";
	std::string 	parFile = "hello.par.tcl";
	Ptr<HdRsuScenario> hdSce = Create<HdRsuScenario>(traceFile, parFile);
	hdSce->Start();

	Simulator::Stop(Seconds(simTime));
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}