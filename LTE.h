#ifndef LTE_H
#define LTE_H

namespace LTE {
    // Для формулы вычисления средней скорости.
    const double time_window = 1;

    // Количество транспортных блоков, передающихся за 1мс
    typedef enum class Bandwidth {
        MHz1_4 = 6,
        MHz3 = 15,
        MHz5 = 25,
        MHz10 = 50,
        MHz15 = 75,
        MHz20 = 100
    } NumberOfResourceBlocks;

    // Типа модуляции
    enum class Modulation {
        QPSK = 2,
        QAM16 = 4,
        QAM64 = 6,
        QAM256 = 8
    };

    // Пара значений модуляции и количества блоков в 1мс
    // Можно было и через std::pair, но тогда было бы попросту неудобно
    struct MCS {
        Modulation modulation;
        int TBS_index;

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


}


#endif // MCS_H
