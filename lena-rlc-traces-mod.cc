#include "ns3/config-store-module.h"
#include "ns3/core-module.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/waypoint-mobility-model.h"
#include "string.h"
#include <iostream>

using namespace ns3;

int main(int argc, char* argv[]) {
    Time simTime = Seconds(100); // Увеличиваем время симуляции для достижения точки назначения
    double numberOfEnbs = 3;
    int schedType;

    // Конфигурация сценария
    CommandLine cmd(__FILE__);
    cmd.AddValue("scheduler", "type of scheduler to use with the network devices", schedType);
    cmd.Parse(argc, argv);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();

    Ptr<LteHelper>lteHelper = CreateObject<LteHelper>();

    switch(schedType) {
        case 0:
            lteHelper->SetSchedulerType("ns3::RrFfMacScheduler"); // Round Robin
            break;
        case 1:
            lteHelper->SetSchedulerType("ns3::PssFfMacScheduler"); // Priority Set
            lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS
            break;
        case 2:
            lteHelper->SetSchedulerType("ns3::PfFfMacScheduler"); // Proportional Fair
            break;
        case 3:
            lteHelper->SetSchedulerType("ns3::TdMtFfMacScheduler"); // Max TD
            break;
        case 4:
            lteHelper->SetSchedulerType("ns3::FdMtFfMacScheduler"); // Max FD
            break;
        case 5:
            lteHelper->SetSchedulerType("ns3::TtaFfMacScheduler"); // TTA
            break;
        case 6:
            lteHelper->SetSchedulerType("ns3::FdBetFfMacScheduler"); // FD-BET scheduler
            break;
        case 7:
            lteHelper->SetSchedulerType("ns3::TdBetFfMacScheduler"); // TD-BET scheduler
            break;
        case 8:
            lteHelper->SetSchedulerType("ns3::FdTbfqFfMacScheduler"); // FD-TBFQ scheduler
            break;
        case 9:
            lteHelper->SetSchedulerType("ns3::TdTbfqFfMacScheduler"); // TD-TBFQ scheduler
            break;
        case 10:
            lteHelper->SetSchedulerType("ns3::CqaFfMacScheduler"); // Channel and QoS Aware Scheduler
            break;
        default:
            lteHelper->SetSchedulerType("ns3::RrFfMacScheduler");
    }

    lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisSpectrumPropagationLossModel"));

    // Создание Nodes: eNodeB и UE
    NodeContainer enbNodes;
    NodeContainer ueNodes;
    enbNodes.Create(1);
    ueNodes.Create(numberOfEnbs);

    // Устанавливаем модель движения для второго абонента (ueNodes.Get(1))
    Ptr<WaypointMobilityModel> mobility = CreateObject<WaypointMobilityModel>();

	// Создаем объекты Waypoint для начальной и конечной точек
	Waypoint startWaypoint(Seconds(0.0), Vector(8000.0, 0.0, 0.0));
	Waypoint endWaypoint(Seconds(60.0), Vector(8500.0, 0.0, 0.0));

	// Добавляем точки маршрута в модель движения
	mobility->AddWaypoint(startWaypoint); // Начальная точка
	mobility->AddWaypoint(endWaypoint); // Точка назначения

    ueNodes.Get(1)->AggregateObject(mobility);

    // Установка модели движения для других узлов
	
    MobilityHelper mobilityHelper;
    mobilityHelper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityHelper.Install(enbNodes);

    mobilityHelper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityHelper.Install(ueNodes.Get(0));
    mobilityHelper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobilityHelper.Install(ueNodes.Get(2));

    // Создание устройств и установка модели движения
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;
    enbDevs = lteHelper->InstallEnbDevice(enbNodes);
    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    // Присоединение UE к eNB
    lteHelper->Attach(ueDevs, enbDevs.Get(0));

    // Активация канала связи
    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer(q);
    lteHelper->ActivateDataRadioBearer(ueDevs, bearer);

    // Остановка симуляции и запуск
    Simulator::Stop(simTime);
    lteHelper->EnablePhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();

	Ptr<ConstantPositionMobilityModel> mm1 = ueNodes.Get(0)->GetObject<ConstantPositionMobilityModel>();
	mm1->SetPosition(Vector(1000, 0.0, 0.0));

	Ptr<ConstantPositionMobilityModel> mm3 = ueNodes.Get(2)->GetObject<ConstantPositionMobilityModel>();
	mm3->SetPosition(Vector(20000, 0.0, 0.0));


    Simulator::Run();
    Simulator::Destroy();

    return 0;
}


/*
int main(int argc, char* argv[]) {
	Time simTime = Seconds(10);
	double interSiteDistance = 0;
	double numberOfEnbs = 1;
	std::cout << numberOfEnbs << std::endl;
	int schedType;
	// Command line arguments
	CommandLine cmd(__FILE__);
	cmd.AddValue("scheduler", "type of scheduler to use with the network devices", schedType);
	cmd.AddValue("simTime", "Total duration of the simulation (in seconds)", simTime);
	cmd.AddValue("interSiteDistance", "Inter-site distance in meter",interSiteDistance);
	cmd.AddValue("numberOfEnbs", "Number of eNBs", numberOfEnbs);
	//cmd.AddValue("numberOfEnbs2", "Number of eNBs", numberOfEnbs2);
	cmd.Parse(argc, argv);
	ConfigStore inputConfig;
	inputConfig.ConfigureDefaults();
	Ptr<LteHelper>lteHelper = CreateObject<LteHelper>();
	switch(schedType) {
		case 0:
			lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler"); //!!! round robin
		break;
		case 1:
			lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler"); // priority set

		//@@@ lteHelper->;SetSchedulerAttribute("nMux", UIntegerValue(numberOfEnbs)); // the maximum number of UE selected by TD scheduler

			lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS

		break;
		case 2:
			lteHelper->SetSchedulerType ("ns3::PfFfMacScheduler"); // proportional fair
		break;
		case 3:
			lteHelper->SetSchedulerType ("ns3::TdMtFfMacScheduler"); //max td
		break;
		case 4:
			lteHelper->SetSchedulerType ("ns3::FdMtFfMacScheduler"); //max fd
		break;
		case 5:
			lteHelper->SetSchedulerType ("ns3::TtaFfMacScheduler"); //tta
		break;
		case 6:
			lteHelper->SetSchedulerType("ns3::FdBetFfMacScheduler"); // FD-BET scheduler

		break;
		case 7:
			lteHelper->SetSchedulerType("ns3::TdBetFfMacScheduler"); // TD-BET scheduler

		break;
		case 8:
			lteHelper->SetSchedulerType("ns3::FdTbfqFfMacScheduler"); // FD-TBFQ scheduler
		break;
		case 9:
			lteHelper->SetSchedulerType("ns3::TdTbfqFfMacScheduler"); // TD-TBFQ scheduler

		break;
		case 10:
			lteHelper->SetSchedulerType("ns3::CqaFfMacScheduler"); // Channel and QoS Aware Scheduler
		break;
		default:
			lteHelper->SetSchedulerType("ns3::RrFfMacScheduler");

	}
	lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisSpectrumPropagationLossModel"));
	// Enable LTE log components
	// lteHelper-&gt;EnableLogComponents ();
	// Create Nodes: eNodeB and UE
	NodeContainer enbNodes;
	NodeContainer ueNodes;
	enbNodes.Create(1);
	ueNodes.Create(numberOfEnbs);
	// Install Mobility Model
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(enbNodes);
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(ueNodes);
	// Create Devices and install them in the Nodes (eNB and UE)
	NetDeviceContainer enbDevs;
	NetDeviceContainer ueDevs;
	enbDevs = lteHelper->InstallEnbDevice(enbNodes);
	//printf(enbDevs-&gt;GetSchedulerType());
	ueDevs = lteHelper->InstallUeDevice(ueNodes);
	// Attach a UE to a eNB
	lteHelper->Attach(ueDevs, enbDevs.Get(0));
	// Activate an EPS bearer
	enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
	EpsBearer bearer(q);
	lteHelper->ActivateDataRadioBearer(ueDevs, bearer);
	Simulator::Stop(simTime);
	lteHelper->EnablePhyTraces();
	lteHelper->EnableMacTraces();
	lteHelper->EnableRlcTraces();
	std::vector<double>userDistance;
	std::vector<double>co{40000};
	//double co = 1000;
	for (int i = 0; i < numberOfEnbs; i++) {
		userDistance.push_back(co[i]);
		//co+=400;
		Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get(i)->GetObject<ConstantPositionMobilityModel>();
		mm->SetPosition(Vector(userDistance[i], 0.0, 0.0));
	}
	Simulator::Run();
	Simulator::Destroy();
	return 0;
}
*/
