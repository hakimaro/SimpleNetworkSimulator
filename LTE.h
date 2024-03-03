#ifndef LTE_H
#define LTE_H

#include <cmath>
#include <stdint.h>
#include <list>

struct Point {
    double x, y;

    Point(double x, double y):
        x(x), y(y)
    { }

    Point(Point *point):
        x(point->x), y(point->y)
    { }

    static double distance(Point p1, Point p2) {
        using namespace std;
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }
};


namespace LTE {
    // Для формулы вычисления средней скорости.
    constexpr double time_window = 1;

    // https://itechinfo.ru/content/cкорость-мобильного-интернета
    constexpr float slot_in_ms = 0.5;             // Время слота в мс
    constexpr float subframe_in_ms = 1;           // Время подфрейма (подкадра) в мс
    constexpr float frame_in_ms = 10;             // Время фрейма (кадра) в мс
    constexpr uint8_t number_of_subcarriers = 12; // Количество поднесущих

    // Разделение канала
    enum class ChannelSeparation {
        TDD, // частотное разделение канала
        FDD  // временное разделение канала
    };

    typedef enum class SubframeType {
        D,  // нисходящий
        U,  // восходящий
        S   // специальный (при смене DL->UL)
    } ST;

    // https://itechinfo.ru/content/cкорость-мобильного-интернета
    constexpr SubframeType TDD_configuration[7][10] = {
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::S, ST::U, ST::U, ST::U}, // соотношение UL:DL == 3:1, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::U, ST::D, ST::D, ST::S, ST::U, ST::U, ST::D}, // соотношение UL:DL == 1:1, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::D, ST::D, ST::D, ST::S, ST::U, ST::D, ST::D}, // соотношение UL:DL == 1:3, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 1:2, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 2:7, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 1:8, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::S, ST::U, ST::U, ST::D}, // соотношение UL:DL == 5:3, период переключения: 5мс
    };

    // Количество транспортных блоков, передающихся за 1мс
    // На самом деле тут примерные значения, так как здесь для удобства счета один ресурсный блок занимает не 180 КГц, а 200КГц.
    // В будущем можно от этого избавиться в пользу нормального рассчета количества ресурных блоков.
    typedef enum class Bandwidth {
        MHz1_4 = 6,
        MHz3 = 15,
        MHz5 = 25,
        MHz10 = 50,
        MHz15 = 75,
        MHz20 = 100
    } NumberOfResourceBlocks;

    // Тип модуляции
    typedef enum class Modulation {
        QPSK = 2,
        QAM16 = 4,
        QAM64 = 6,
        QAM256 = 8
    } NumberOfBitsPerSignal;

    // Пара значений модуляции и количества блоков в 1мс
    // Можно было и через std::pair, но тогда было бы попросту неудобно
    struct MCS {
        const Modulation modulation;
        const int TBS_index;

        MCS(Modulation modulation, int TBS) :
            modulation(modulation), TBS_index(TBS) {}
    };


    // Значения таблицы MCS.
    // ref: http://anisimoff.org/eng/lte_throughput.html
    const MCS MCS_table[] {
        MCS(Modulation::QPSK, 0),
        MCS(Modulation::QPSK, 1),
        MCS(Modulation::QPSK, 2),
        MCS(Modulation::QPSK, 3),
        MCS(Modulation::QPSK, 4),
        MCS(Modulation::QPSK, 5),
        MCS(Modulation::QPSK, 6),
        MCS(Modulation::QPSK, 7),
        MCS(Modulation::QPSK, 8),
        MCS(Modulation::QPSK, 9),
        MCS(Modulation::QAM16, 9),
        MCS(Modulation::QAM16, 10),
        MCS(Modulation::QAM16, 11),
        MCS(Modulation::QAM16, 12),
        MCS(Modulation::QAM16, 13),
        MCS(Modulation::QAM16, 14),
        MCS(Modulation::QAM16, 15),
        MCS(Modulation::QAM64, 15),
        MCS(Modulation::QAM64, 16),
        MCS(Modulation::QAM64, 17),
        MCS(Modulation::QAM64, 18),
        MCS(Modulation::QAM64, 19),
        MCS(Modulation::QAM64, 20),
        MCS(Modulation::QAM64, 21),
        MCS(Modulation::QAM64, 22),
        MCS(Modulation::QAM64, 23),
        MCS(Modulation::QAM64, 24),
        MCS(Modulation::QAM64, 25),
        MCS(Modulation::QAM64, 26)
    };

    enum class SHEDULLER { RR };

    // Класс для определения абонента
    class UE
    {
    public:
        UE(Point position);

        // скорость, азимут, время
        void move(double speed, double Q, double dt);

    private:
        Point m_position;

        double m_last_average_throughput_in_bytes = 0;
        double m_current_sending_speed_in_bytes = 0;
    };

    // Класс для определения базовой станции
    class BaseStation {
    public:
        BaseStation(LTE::Bandwidth bandwidth);

        void allocate_resources(std::list<UE*> abonents);

        const MCS calculate_MCS(double distance);
    };
}


#endif // MCS_H
