#include <string>
#include <stdio.h>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    auto h_str = std::to_string(h);
    auto m_str = std::to_string(m);
    auto s_str = std::to_string(s);

    if (h < 10)
    {
        h_str = "0" + h_str;
    }
    if (m < 10)
    {
        m_str = "0" + m_str;
    }
    if (s < 10)
    {
        s_str = "0" + s_str;
    }

    return h_str + ":" + m_str + ":" + s_str;
}