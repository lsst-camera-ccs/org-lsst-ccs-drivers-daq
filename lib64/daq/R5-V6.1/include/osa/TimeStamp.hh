#ifndef OSA_TIMESTAMP
#define OSA_TIMESTAMP

#include "dvi/TimeStamp.hh"

namespace OSA {

class __attribute__((__packed__)) TimeStamp : public DVI::TimeStamp
{
public:
 TimeStamp() : DVI::TimeStamp() {}; 
 TimeStamp(bool zero) : DVI::TimeStamp(zero) {};
 TimeStamp(time_t time) : DVI::TimeStamp(time) {};        
 TimeStamp(const TimeStamp& clone) : DVI::TimeStamp(clone) {};
 TimeStamp(const DVI::TimeStamp& clone) : DVI::TimeStamp(clone) {};
};
}

#endif
