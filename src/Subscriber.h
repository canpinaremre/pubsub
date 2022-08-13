/**
 * @file Subscriber.h
 * @author Emre Canpınar (canpinaremre@gmail.com)
 * @brief Subscriber class header file
 * 
 */

#ifndef __SUBS_H__
#define __SUBS_H__

#include "Manager.h"


/**
 * @brief Subscriber class to pull published topics
 * 
 * @tparam T Topic struct
 */
template <class T>
class Subscriber{

public:
	/**
	 * @brief Construct a new Subscriber object
	 * 
	 * @param topic Topic ID
	 */
    Subscriber(const struct topic_metadata* topic): 
    _node{topic}        
    {
		// After creating Subscriber, get() can be used
		Manager::copy(_node, &_data, _last_generation, false);
	}

	/**
	 * @brief Destroy the Subscriber object
	 * 
	 */
    ~Subscriber(){}


	/**
	 * @brief Returns if topic is advertised or not
	 * 
	 * @return true if advertised
	 * @return false if not advertised
	 */
	bool advertised(){
		return Manager::is_advertised(_node);
	}

	/**
	 * @brief Returns if any publish available after last copy (if topic is updated)
	 * 
	 * @return true if new publish available
	 * @return false if last copy is up to date
	 */
	bool updated()
	{
		if (!advertised()){
			return false;
		}

		return Manager::updates_available(_node, _last_generation);
	}

	/**
	 * @brief Copy published topic to param dst if topic is updated
	 * 
	 * @param dst Topic data copy destination
	 * @return true if successful or we are up to date
	 * @return false if failed
	 */
	bool update(void *dst){
		if (!advertised()){
			return false;
		}
		return Manager::copy(_node, dst, _last_generation, true);
	}

	/**
	 * @brief Update internal topic struct
	 * 
	 * @return true if successful or we are up to date
	 * @return false if failed
	 */
	bool update(){
		if (!advertised()){
			return false;
		}
		return Manager::copy(_node, &_data, _last_generation, true);
	}

	/**
	 * @brief Copy published topic to param dst
	 * 
	 * @param dst Topic data copy destination
	 * @return true if successful
	 * @return false if failed
	 */
	bool copy(void *dst){
		if (!advertised()){
			return false;
		}
		return Manager::copy(_node, dst, _last_generation, false);
	}
	
	/**
	 * @brief Get the last copied topic's generation
	 * 
	 * @return uint32_t last copied topic's generation
	 */
	uint32_t get_last_generation() const { return _last_generation; }

	/**
	 * @brief Gets the internal topic struct data
	 * 
	 * @return const T& topic struct referance
	 */
    const T &get() const { return _data;}

private:

	const void *_node{nullptr};
	uint32_t _last_generation{0}; 

    T _data{};
};

#endif // __SUBS_H__