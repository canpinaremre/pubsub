# PUBSUB
PUBSUB is a thread-safe publish/subscribe messaging library.

# BUILD

- To build:
    - chmod a+rx setup.sh
    - ./setup.sh

- Run the example:
    - ./run.sh


# DEFINING TOPICS

- Topics should be define in header files with including Topic.h header.
- Topics will be define as a struct. 
- After struct definition, topics should declare with TOPIC_DECLARE()

- Examples:
    - /topics/iso.h
    - /topics/test.h
    
- Topic should define in /topics/Topic.cpp with TOPIC_DEFINE()

- Example:
    - #include "iso.h"
    - TOPIC_DEFINE(iso)

# USAGE
- Subscribe or Publisher (or both) should be included
- Topic header file should be included

- Both Subscriber and Publisher initialized with:
    - TOPIC_ID()
    - topic struct

- Example:

    - Subscriber<iso_s> subiso{TOPIC_ID(iso)};

    - Publisher<iso_s> pubiso{TOPIC_ID(iso)};

- Multiple Publishers can advertise to a topic
- Multiple Subscribers can copy from topic if advertised
- Publishers can unadvertise from a topic
- All publishers must unadvertise for topic to be unadvertised
- Subscribers can check if topic is updated before copying data from topic
- Publisher can set() data and update() (publishing) anytime they want later

Example usage is in /src/test.cpp


