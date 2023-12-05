#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
static std::string formatTime(long time) {
    return (time < 10) ? "0" + std::to_string(time) : std::to_string(time);
}

string Format::ElapsedTime(long seconds) {
    string time = "00:00:00";
    long hour, minute, second;
    if (seconds > 0) {
        hour = seconds/3600;
    	minute = (seconds % 3600) / 60;
    	second = seconds % 60;
    	time = formatTime(hour) + ":" + formatTime(minute) + ":" + formatTime(second);
    }
    return time;
}
