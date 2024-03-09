#include "LTE.h"
#include <iostream>

LTE::BaseStation::BaseStation(LTE::Bandwidth bandwidth)
{
    m_total_RB = bandwidth;
}

void LTE::BaseStation::allocate_resources(std::list<UE *> abonents)
{
    int index_of_UE_to_send = 1;
    uint64_t min_speed = -1;

    // Вычисление абонента, которому выделят ресурс
    for (auto ue : abonents) {
        if (ue->average_speed() < min_speed) {
            min_speed = ue->average_speed();
            index_of_UE_to_send = ue->id();
        }
    }

    // Выделение ресурса
    for (auto ue : abonents) {
        //std::cout << ue_mcs.TBS_index << " " << (uint64_t) m_total_RB << " " << LTE::TB_size_table[ue_mcs.TBS_index][(uint64_t) m_total_RB - 1] / 8 << std::endl;
        if (ue->id() != index_of_UE_to_send) {
            ue->set_current_speed(0);
        } else {
            LTE::MCS ue_mcs = calculate_MCS(Point::distance(m_position, ue->position()));
            ue->set_current_speed(LTE::TB_size_table[ue_mcs.TBS_index][((uint64_t) (m_total_RB) - 1)]);
        }

        if (type_of_calculation == LTE::AverageSpeedCalculation::BRUTE_FORCE) {
            ue->append_last_speed(); // Добавить скорость в вектор значений скоростей
        }
    }

    // Пересчет средней скорости
    for (auto ue : abonents) {
        if (ue->average_speed() == 0) {
            ue->set_average_speed(ue->current_speed());
        } else {
            switch(type_of_calculation) {
            case LTE::AverageSpeedCalculation::FAST:
                ue->set_average_speed((ue->average_speed() * LTE::beta) + (ue->current_speed() * (1-LTE::beta)));
                break;

            case LTE::AverageSpeedCalculation::BRUTE_FORCE:
                double average_speed = 0;
                std::vector<uint64_t> all_speeds = ue->all_speeds();
                for (int i = 0; i < all_speeds.size(); ++i) {
                    average_speed += (double) all_speeds[i] / all_speeds.size();
                }
                ue->set_average_speed(average_speed);
                break;
            }
        }
    }
}

const LTE::MCS LTE::BaseStation::calculate_MCS(double distance)
{
    if (distance < 3200) {
        return MCS_table[28];
    } else if (distance < 4000) {
        return MCS_table[26];
    } else if (distance < 5000) {
        return MCS_table[24];
    } else if (distance < 6000) {
        return MCS_table[22];
    } else if (distance < 7000) {
        return MCS_table[20];
    } else if (distance < 8250) {
        return MCS_table[18];
    } else if (distance < 10000) {
        return MCS_table[16];
    } else if (distance < 15000) {
        return MCS_table[14];
    } else if (distance < 17000) {
        return MCS_table[12];
    } else if (distance < 19350) {
        return MCS_table[10];
    } else if (distance < 30000) {
        return MCS_table[8];
    } else if (distance < 40000) {
        return MCS_table[6];
    } else if (distance < 50000) {
        return MCS_table[2];
    } else {
        return MCS_table[0];
    }
}
