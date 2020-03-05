#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>


using date = boost::gregorian::date;
using time_point = boost::posix_time::ptime;
using hours = boost::posix_time::hours;

const double MONTHS_PER_TURN = 0.25;


