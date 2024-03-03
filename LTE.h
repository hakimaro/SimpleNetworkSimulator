#ifndef LTE_H
#define LTE_H

#include <cmath>
#include <stdint.h>
#include <list>
#include <fstream>
#include <vector>

struct Point {
    double x, y;

    Point(double x, double y):
        x(x), y(y)
    { }

    Point(Point *point):
        x(point->x), y(point->y)
    { }

    static double distance(Point p1, Point p2) {

        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }
};

namespace LTE {
    // Для формулы вычисления средней скорости.
    constexpr double beta = 0.99; // Коэффициент забывания

    // https://itechinfo.ru/content/cкорость-мобильного-интернета
    constexpr float slot_in_ms = 0.5;             // Время слота в мс
    constexpr float subframe_in_ms = 1;           // Время подфрейма (подкадра) в мс
    constexpr float frame_in_ms = 10;             // Время фрейма (кадра) в мс

    constexpr uint8_t N_subcarriers = 12; // Количество поднесущих
    constexpr uint8_t N_symbols_prb = 7; // Количество символов в PRB (RB, но за слот)

    // Разделение канала (на будущее)
    enum class ChannelSeparation {
        TDD, // частотное разделение канала
        FDD  // временное разделение канала
    };

    // (на будущее)
    typedef enum class SubframeType {
        D,  // нисходящий
        U,  // восходящий
        S   // специальный (при смене DL->UL)
    } ST;

    // https://itechinfo.ru/content/cкорость-мобильного-интернета
    // (на будущее)
    constexpr SubframeType TDD_configuration[8][10] = {
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::S, ST::U, ST::U, ST::U}, // соотношение UL:DL == 3:1, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::U, ST::D, ST::D, ST::S, ST::U, ST::U, ST::D}, // соотношение UL:DL == 1:1, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::D, ST::D, ST::D, ST::S, ST::U, ST::D, ST::D}, // соотношение UL:DL == 1:3, период переключения: 5мс
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 1:2, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 2:7, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D}, // соотношение UL:DL == 1:8, период переключения: 10мс
        {ST::D, ST::S, ST::U, ST::U, ST::U, ST::D, ST::S, ST::U, ST::U, ST::D}, // соотношение UL:DL == 5:3, период переключения: 5мс
        {ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D, ST::D}, // ТЕСТ, где всё DL
    };

    // Количество транспортных блоков, передающихся за 1мс
    // На самом деле тут примерные значения, так как здесь для удобства счета один ресурсный блок занимает не 180 КГц, а 200КГц.
    // В будущем можно от этого избавиться в пользу нормального рассчета количества ресурных блоков.
    typedef enum class Bandwidth {
        MHz1_4 = 6,
        MHz3 = 15,
        MHz5 = 25,
        //MHz10 = 50,
        //MHz15 = 75,
        //MHz20 = 100
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

    // ref: https://www.etsi.org/deliver/etsi_ts/136200_136299/136213/17.05.00_60/ts_136213v170500p.pdf
    // страница 113-118
    const uint64_t TB_size_table[27][25] = {
        {16,  32,   56,   88,   120,  152,  176,  208,  224,  256,  288,  328,  344,  376,   392,   424,   456,   488,   504,   536,   568,   600,   616,   648,   680},
        {24,  56,   88,   144,  176,  208,  224,  256,  328,  344,  376,  424,  456,  488,   520,   568,   600,   632,   680,   712,   744,   776,   808,   872,   904},
        {32,  72,   144,  176,  208,  256,  296,  328,  376,  424,  472,  520,  568,  616,   648,   696,   744,   776,   840,   872,   936,   968,   1000,  1064,  1096},
        {40,  104,  176,  208,  256,  328,  392,  440,  504,  568,  616,  680,  744,  808,   872,   904,   968,   1032,  1096,  1160,  1224,  1256,  1320,  1384,  1416},
        {56,  120,  208,  256,  328,  408,  488,  552,  632,  696,  776,  840,  904,  1000,  1064,  1128,  1192,  1288,  1352,  1416,  1480,  1544,  1608,  1736,  1800},
        {72,  144,  224,  328,  424,  504,  600,  680,  776,  872,  968,  1032, 1128, 1224,  1320,  1384,  1480,  1544,  1672,  1736,  1864,  1928,  2024,  2088,  2216},
        {88,  176,  256,  392,  504,  600,  712,  808,  936,  1032, 1128, 1224, 1352, 1480,  1544,  1672,  1736,  1864,  1992,  2088,  2216,  2280,  2408,  2472,  2600},
        {104, 224,  328,  472,  584,  712,  840,  968,  1096, 1224, 1320, 1480, 1608, 1672,  1800,  1928,  2088,  2216,  2344,  2472,  2536,  2664,  2792,  2984,  3112},
        {120, 256,  392,  536,  680,  808,  968,  1096, 1256, 1384, 1544, 1672, 1800, 1928,  2088,  2216,  2344,  2536,  2664,  2792,  2984,  3112,  3240,  3368,  3496},
        {136, 296,  456,  616,  776,  936,  1096, 1256, 1416, 1544, 1736, 1864, 2024, 2216,  2344,  2536,  2664,  2856,  2984,  3112,  3368,  3496,  3624,  3752,  4008},
        {144, 328,  504,  680,  872,  1032, 1224, 1384, 1544, 1736, 1928, 2088, 2280, 2472,  2664,  2792,  2984,  3112,  3368,  3496,  3752,  3880,  4008,  4264,  4392},
        {176, 376,  584,  776,  1000, 1192, 1384, 1608, 1800, 2024, 2216, 2408, 2600, 2792,  2984,  3240,  3496,  3624,  3880,  4008,  4264,  4392,  4584,  4776,  4968},
        {208, 440,  680,  904,  1128, 1352, 1608, 1800, 2024, 2280, 2472, 2728, 2984, 3240,  3368,  3624,  3880,  4136,  4392,  4584,  4776,  4968,  5352,  5544,  5736},
        {224, 488,  744,  1000, 1256, 1544, 1800, 2024, 2280, 2536, 2856, 3112, 3368, 3624,  3880,  4136,  4392,  4584,  4968,  5160,  5352,  5736,  5992,  6200,  6456},
        {256, 552,  840,  1128, 1416, 1736, 1992, 2280, 2600, 2856, 3112, 3496, 3752, 4008,  4264,  4584,  4968,  5160,  5544,  5736,  5992,  6200,  6456,  6968,  7224},
        {280, 600,  904,  1224, 1544, 1800, 2152, 2472, 2728, 3112, 3368, 3624, 4008, 4264,  4584,  4968,  5160,  5544,  5736,  6200,  6456,  6712,  6968,  7224,  7736},
        {328, 632,  968,  1288, 1608, 1928, 2280, 2600, 2984, 3240, 3624, 3880, 4264, 4584,  4968,  5160,  5544,  5992,  6200,  6456,  6712,  7224,  7480,  7736,  7992},
        {336, 696,  1064, 1416, 1800, 2152, 2536, 2856, 3240, 3624, 4008, 4392, 4776, 5160,  5352,  5736,  6200,  6456,  6712,  7224,  7480,  7992,  8248,  8760,  9144},
        {376, 776,  1160, 1544, 1992, 2344, 2792, 3112, 3624, 4008, 4392, 4776, 5160, 5544,  5992,  6200,  6712,  7224,  7480,  7992,  8248,  8760,  9144,  9528,  9912},
        {408, 840,  1288, 1736, 2152, 2600, 2984, 3496, 3880, 4264, 4776, 5160, 5544, 5992,  6456,  6968,  7224,  7736,  8248,  8504,  9144,  9528,  9912,  10296, 10680},
        {440, 904,  1384, 1864, 2344, 2792, 3240, 3752, 4136, 4584, 5160, 5544, 5992, 6456,  6968,  7480,  7992,  8248,  8760,  9144,  9912,  10296, 10680, 11064, 11448},
        {488, 1000, 1480, 1992, 2472, 2984, 3496, 4008, 4584, 4968, 5544, 5992, 6456, 6968,  7480,  7992,  8504,  9144,  9528,  9912,  10680, 11064, 11448, 12216, 12576},
        {520, 1064, 1608, 2152, 2664, 3240, 3752, 4264, 4776, 5352, 5992, 6456, 6968, 7480,  7992,  8504,  9144,  9528,  10296, 10680, 11448, 11832, 12576, 12960, 13536},
        {552, 1128, 1736, 2280, 2856, 3496, 4008, 4584, 5160, 5736, 6200, 6968, 7480, 7992,  8504,  9144,  9912,  10296, 11064, 11448, 12216, 12576, 12960, 13536, 14112},
        {584, 1192, 1800, 2408, 2984, 3624, 4264, 4968, 5544, 5992, 6712, 7224, 7992, 8504,  9144,  9912,  10296, 11064, 11448, 12216, 12960, 13536, 14112, 14688, 15264},
        {616, 1256, 1864, 2536, 3112, 3752, 4392, 5160, 5736, 6200, 6968, 7480, 8248, 8760,  9528,  10296, 10680, 11448, 12216, 12576, 13536, 14112, 14688, 15264, 15840},
        {712, 1480, 2216, 2984, 3752, 4392, 5160, 5992, 6712, 7480, 8248, 8760, 9528, 10296, 11064, 11832, 12576, 13536, 14112, 14688, 15264, 16416, 16992, 17568, 18336}
    };

    enum class SHEDULLER { RR };

    enum class AverageSpeedCalculation {
        BRUTE_FORCE, FAST
    };

    // Класс для определения абонента
    class UE
    {
    public:
        UE(uint16_t id, Point position);

        // скорость, азимут, время
        void move(double speed, double Q, double dt);

        Point position() const;
        void setPosition(Point newPosition);

        double current_speed() const;
        void set_current_speed(double newV_per_ms);

        double average_speed() const;
        void set_average_speed(double newAV);

        uint64_t total_speed() const;
        void set_total_speed(uint64_t newTotal_speed);

        void write_total_speed(uint64_t time_in_s);
        uint16_t id() const;

        std::vector<uint64_t> all_speeds() const;
        void append_last_speed();

    private:
        std::vector<uint64_t> m_all_speeds;

        std::ofstream m_fout;
        uint64_t m_total_speed = 0;

        uint16_t m_id = 0;
        Point m_position;

        double m_average_speed = 0;        // Средняя скорость
        double m_current_speed = 0;  // Текущая скорость
    };

    // Класс для определения базовой станции
    class BaseStation {
    public:
        BaseStation(LTE::Bandwidth bandwidth);

        void allocate_resources(std::list<UE*> abonents);

        const MCS calculate_MCS(double distance);

    private:
        Point m_position = Point(0, 0);
        NumberOfResourceBlocks m_total_RB = Bandwidth::MHz5;
        LTE::AverageSpeedCalculation type_of_calculation = LTE::AverageSpeedCalculation::BRUTE_FORCE;
    };
}


#endif // MCS_H
