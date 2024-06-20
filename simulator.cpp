#include "simulator.h"

double Simulator::time_ = 0.0;
std::priority_queue<Schedule> Simulator::schedules_;