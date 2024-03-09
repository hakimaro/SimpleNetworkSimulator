#include "LTE.h"

LTE::UE::UE(uint16_t id, Point position) :
    m_position(position)
{
    m_id = id;

    std::string filename = "UE";
    filename += std::to_string(id);
    filename += ".txt";
    m_fout.open(filename, std::ios_base::binary | std::ios_base::out);
}

void LTE::UE::move(double speed, double Q, double dt)
{
    m_position.x += speed * dt;
    //m_position.y += speed * cos(Q) * dt;
}


Point LTE::UE::position() const
{
    return m_position;
}

void LTE::UE::setPosition(Point newPosition)
{
    m_position = newPosition;
}

double LTE::UE::current_speed() const
{
    return m_current_speed;
}

void LTE::UE::set_current_speed(double new_current_speed)
{
    m_current_speed = new_current_speed;
}

double LTE::UE::average_speed() const
{
    return m_average_speed;
}

void LTE::UE::set_average_speed(double new_average_speed)
{
    m_average_speed = new_average_speed;
}

uint64_t LTE::UE::total_speed() const
{
    return m_total_speed;
}

void LTE::UE::set_total_speed(uint64_t newTotal_speed)
{
    m_total_speed = newTotal_speed;
}

void LTE::UE::write_total_speed(uint64_t time_in_s)
{
    m_fout << time_in_s << " " << (double) m_total_speed / (1024*8*1024) << std::endl;
    m_total_speed_to_count_average += (double) m_total_speed / (1024*8*1024);
    m_total_speed = 0;
}

uint16_t LTE::UE::id() const
{
    return m_id;
}

std::vector<uint64_t> LTE::UE::all_speeds() const
{
    return m_all_speeds;
}

void LTE::UE::append_last_speed()
{
    if (m_all_speeds.size() >= LTE::window_time)
        m_all_speeds.erase(m_all_speeds.begin());
    m_all_speeds.push_back(m_current_speed);
}
