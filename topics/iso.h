#ifndef __iso_S_H__
#define __iso_S_H__
#include "Topic.h"


struct iso_s{
    uint64_t timestamp;
    uint8_t iso;
    float float_val;
    double double_val;
    bool bool_val;
    uint8_t predefined = 99;
};
TOPIC_DECLARE(iso);

#endif // __iso_S_H__