/**
 * @file Manager.h
 * @author Emre Canpınar (canpinaremre@gmail.com)
 * @brief Manager singleton class header file
 * 
 * 
 */

#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Topic.h"

#include <stdint.h>
#include <stdbool.h>
#include <utility>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

#include <string.h>

/**
 * @brief Topic data struct
 * 
 */
struct topic_data{
    void* topic_struct;
    uint32_t generations;
    uint16_t publisher_count;
    void **callbacks;
};

/**
 * @brief Manager singleton class for Subscriber and Publisher
 * 
 */
class Manager{

public:
    /**
     * @brief Get the instance object
     * 
     * @return Manager& Instance of singleton
     */
    static Manager& get_instance();

    /**
     * @brief Returns if topic advertised or not
     * 
     * @param node Topic ID
     * @return true if topic is advertised
     * @return false if topis is not advertised
     */
    static bool is_advertised(const void * node){
        return get_instance()._is_advertised(node);
    }

    /**
     * @brief Copies published topic data to dst param
     * 
     * @param node Topic ID
     * @param dst Topic data destination
     * @param generation Subscriber's generation counter for topic
     * @param only_if_updated Indicates that should it be copied regarding it is updated or not
     * @return true if copied or already up to date
     * @return false copy failed
     */
    static bool copy(const void *node, void *dst, uint32_t &generation, bool only_if_updated){
        return get_instance()._copy(node,dst,generation,only_if_updated);
    }

    /**
     * @brief Returns if any update available on topic
     * 
     * @param node Topic ID
     * @param last_generation Subscriber's generation counter for topic
     * @return true if updates available on topic
     * @return false if updates do not available on topic
     */
    static bool updates_available(const void *node, uint32_t last_generation){
        return get_instance()._updates_available(node,last_generation);
    }

    /**
     * @brief Publishes the topic data
     * 
     * @param node Topic ID
     * @param data Topic data to be published
     * @return true if published successfuly
     * @return false if publish fail
     */
    static bool publish(const void *node, void *data){
        return get_instance()._publish(node,data);
    }

    /**
     * @brief Advertise the topic
     * 
     * @param node Topic ID
     * @return true if advertised successfuly
     * @return false if advertise fail
     */
    static bool advertise(const void *node){
        return get_instance()._advertise(node);
    }

    /**
     * @brief Unadvertise the topic
     * 
     * @param node Topic ID
     * @return true if unadvertised successfuly
     * @return false if unadvertised fail
     */
    static bool unadvertise(const void *node){
        return get_instance()._unadvertise(node);
    }

private:
    // Private functions of public static functions'
    bool _is_advertised(const void * node);
    bool _copy(const void *node, void *dst, uint32_t &generation, bool only_if_updated);
    bool _updates_available(const void *node, uint32_t last_generation);
    bool _publish(const void *node, void *data);
    bool _advertise(const void *node);
    bool _unadvertise(const void *node);

    // Private constructor
    Manager(){
    }
    std::map<std::string,topic_data> topics;
    std::mutex(_mutex);

public:
    // Delete of copy constructor and equal operator
    Manager(Manager const&) = delete;
    void operator=(Manager const&) = delete;

};

#endif //__MANAGER_H__
