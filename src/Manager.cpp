#include "Manager.h"


Manager& Manager::get_instance(){

    // Singleton instantiation, guaranteed to be destroyed.
    static Manager _instance{};

    // Returning instance of singleton
    return _instance;
}

bool Manager::_is_advertised(const void * node){
    // Can not place unique mutex beacuse of recursiveness
    // Since we are only reading, it is OK

    if(node == nullptr){
        // Topic does not exist (should not possible) TODO err
        return false;
    }
    
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);
    
    // .find() returns .end() iterator if unsuccessful
    bool found = (topics.end() != topics.find(mtd->name));
    
    if(found){
        const topic_data &res = topics.at(mtd->name);
        // Return if any publisher advertised
        return (res.publisher_count);
    }
    else{
        // Not even in map
        return false;
    }
}

bool Manager::_copy(const void *node, void *dst, uint32_t &generation, bool only_if_updated){   
    std::unique_lock<std::mutex> lock(_mutex);
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);

    // Check if advertised
    if(!_is_advertised(node)){
        return false;
    }

    // Since we are only copying, use .at() to get const ref
    const topic_data &res = topics.at(mtd->name);

    // Checking if data still there
    if(res.topic_struct ==nullptr){
        return false;
    }

    // If only_if_updated, check generation counts and return true if up to date
    if((only_if_updated) && (generation == res.generations)){
        return true;
    }

    
    bool succ = nullptr != memcpy(dst,res.topic_struct,mtd->size);
    if(succ)
    {
        generation = res.generations;
        return true;
    }
    else{
        return false;
    }
}

bool Manager::_updates_available(const void *node, uint32_t last_generation){
    std::unique_lock<std::mutex> lock(_mutex);
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);
    
    // Check if advertised
    if(!_is_advertised(node)){
        return false;
    }

    const topic_data &res = topics.at(mtd->name);

    if(res.topic_struct ==nullptr){
        // Topic struct is not initialized, return false
        return false;
    }

    if(res.generations > last_generation){
        // If data generation is bigger than the last genereartion,
        // updates available
        return true;
    }
    else{
        // Else we are up to date
        return false;
    }

}

bool Manager::_publish(const void *node, void *data){
    std::unique_lock<std::mutex> lock(_mutex);
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);

    // Check if advertised
    if(!_is_advertised(node)){
        return false;
    }

    // We are writing so get ref with [] operator
    topic_data &res = topics[mtd->name];
    if(res.topic_struct ==nullptr){
        // If memory is not initialized
        return false;
    }
    
    bool succ = nullptr != memcpy(res.topic_struct,data,mtd->size);
    if(succ){
        res.generations++;
        return true;
    }
    else{
        return false;
    }
    
    return succ;
}

bool Manager::_advertise(const void *node){
    std::unique_lock<std::mutex> lock(_mutex);
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);

    if(_is_advertised(node))
    {
        // Already advertised
        // Increase publisher count of topic data that exist
        topic_data &td = topics[(mtd->name)];
        td.publisher_count++;

        return true;
    }

    // Advertised, unadvertised and advertised again:
    // Key in map and memory still exist for the struct data
    if((topics.end() != topics.find(mtd->name)))
    {
        // Set publisher count as 1 and return true
        topic_data &td = topics[(mtd->name)];
        td.publisher_count++;

        return true;
    }


    // This index operation creating key in the map
    // We are allocation memory size of the struct
    topics[mtd->name] = topic_data{malloc(mtd->size),0,1};
    
    // Checking if creating the key is successful
    if(!_is_advertised(node))
    {
        return false;
    }

    const topic_data &res = topics.at(mtd->name);

    // Return if we can allocate memory
    return (res.topic_struct != nullptr);
}

bool Manager::_unadvertise(const void *node){
    std::unique_lock<std::mutex> lock(_mutex);
    const topic_metadata* mtd = static_cast<const topic_metadata*>(node);

    if(_is_advertised(node))
    {
        topic_data &td = topics[(mtd->name)];

        if(td.publisher_count > 0){
            td.publisher_count--;
            return true;
        }
        else{
            // Publisher count already 0, can not unadvertise
            td.publisher_count = 0;
            return false;
        }

        return true;
    }

    // Not advertised, can not unadvertise
    return false;

}


