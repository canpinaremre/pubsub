#ifndef __TOPIC_H__
#define __TOPIC_H__

#include <stdint.h>
#include <stdbool.h>

struct topic_metadata {
	const char *name;		    // Unique topic name 
	const uint16_t size;		// Topic size 
};


#define TOPIC_DECLARE(_name)		extern "C" const struct topic_metadata __topic_##_name 

#define TOPIC_DEFINE(_name)		            \
	const struct topic_metadata __topic_##_name = {	\
		#_name,					\
		sizeof(struct _name##_s),		\
	};


#define TOPIC_ID(_name)		&__topic_##_name



#endif
