#ifndef __TEST_S_H__
#define __TEST_S_H__
#include "Topic.h"

struct test_s{
    uint64_t timestamp;
    uint8_t test;
};
TOPIC_DECLARE(test);

#endif // __TEST_S_H__