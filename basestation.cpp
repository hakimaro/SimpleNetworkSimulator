
#include "LTE.h"

LTE::BaseStation::BaseStation(LTE::Bandwidth bandwidth)
{

}

void LTE::BaseStation::allocate_resources(std::list<UE *> abonents)
{

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
    } else if (distance < 9000) {
        return MCS_table[18];
    } else if (distance < 10000) {
        return MCS_table[16];
    } else if (distance < 15000) {
        return MCS_table[14];
    } else if (distance < 20000) {
        return MCS_table[12];
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
