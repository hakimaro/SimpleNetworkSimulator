#ifndef MODULATION_H
#define MODULATION_H

#include <stdint.h>
#include "LTE.h"
#include <fstream>
#include <iostream>

void start_modulation(uint32_t total_work_time_in_s) {
    srand(5);
    uint64_t total_work_time_in_ms = total_work_time_in_s * 1000;
    uint8_t current_configuration = 7;

    std::list<LTE::UE*> UEs;
//    for(int i = 0; i < 5; ++i) {
//        LTE::UE *UE = new LTE::UE(i, Point(rand() % 50000, 0));
//        UEs.push_back(UE);
//    }


    LTE::UE *UE1 = new LTE::UE(1, Point(1000, 0));
    LTE::UE *UE2 = new LTE::UE(2, Point(19000, 0));
    LTE::UE *UE3 = new LTE::UE(3, Point(40000, 0));
    UEs.push_back(UE1);
    UEs.push_back(UE2);
    UEs.push_back(UE3);

    LTE::BaseStation *station = new LTE::BaseStation(LTE::Bandwidth::MHz5);

    for(uint64_t current_tti = 0; current_tti < total_work_time_in_ms; ++current_tti) {
        station->allocate_resources(UEs);

        for (auto UE : UEs) {
            UE->set_total_speed(UE->total_speed() + UE->current_speed());

            if (current_tti % 1 == 0) {
                UE->write_total_speed(current_tti / 1);
            }

            //
            if (UE->id() == 2 && current_tti / 1000 <= 60) {
                UE->move(8, 90.0 / 180.0 * 3.14, 0.001);
            }
        }
    }

    for (auto UE : UEs) {
        std::cout << UE->id() << " " << UE->total_speed_to_count_average() / total_work_time_in_s << std::endl;
    }

    std::string filename = "UE=";
    filename += std::to_string(UEs.size());
    filename += "_AV.plt";
    std::ofstream f_plot(filename);
    f_plot << "plot ";

    for (auto UE : UEs) {
        std::cout << UE->id() << " " << UE->position().x << " " << UE->position().y << std::endl;
        std::string line = "\"UE";
        line += std::to_string(UE->id());
        line += ".txt\" w l title\"UE";
        line += std::to_string(UE->id());
        line += "\", ";

        f_plot << line;
    }

    f_plot << std::endl;
}


#endif // MODULATION_H
