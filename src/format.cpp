#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

#define SEC_IN_HR 3600
#define SEC_IN_MIN 60

using std::string;

string Format::ElapsedTime(long seconds) {
    int hr = seconds/(SEC_IN_HR);
    int min = (seconds - hr*(SEC_IN_HR))/(SEC_IN_MIN);
    int sec = seconds - hr*(SEC_IN_HR) - min*(SEC_IN_MIN);

    std::stringstream time_stream;
    time_stream << std::setw(2) << std::setfill('0') << hr;
    time_stream << ":";
    time_stream << std::setw(2) << std::setfill('0') << min;
    time_stream << ":";
    time_stream << std::setw(2) << std::setfill('0') << sec;
    return time_stream.str();
}
