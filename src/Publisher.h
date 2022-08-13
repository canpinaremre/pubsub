/**
 * @file Publisher.h
 * @author Emre Canpınar (canpinaremre@gmail.com)
 * @brief Publisher class header file
 *
 * 
 */

#ifndef __PUB_H__
#define __PUB_H__

#include "Manager.h"


/**
 * @brief Publisher class to advertise and publish topic
 * 
 * @tparam T Topic struct
 */
template <class T>
class Publisher{

public:

	/**
	 * @brief Construct a new Publisher object
	 * 
	 * @param topic Topic ID
	 */
    Publisher(const struct topic_metadata* topic): 
    _node{topic},
	_advertised{false},
	_data{}
    {
		// Advertise when object is created
        advertise();
    }

	// TODO: constructor overload with data to be published

	/**
	 * @brief Destroy the Publisher object
	 * 
	 */
    ~Publisher(){
		// Do not advertise anymore
        unadvertise();
    }

	/**
	 * @brief Returns if this object advertised the topic
	 * 
	 * @return true if advertised
	 * @return false if not advertised
	 */
    bool advertised(){
		return _advertised;
	}

	/**
	 * @brief Unadvertise the topic
	 * 
	 * @return true if successfuly unadvertised
	 * @return false if unadvertise failed
	 */
	bool unadvertise() {
    
        if(Manager::unadvertise(_node))
		{
			_advertised = false;
			return true;
		}
		else
		{
			return false;
		}
		return false;
    }
    
	/**
	 * @brief Advertise to topic
	 * 
	 * @return true if successfuly advertised
	 * @return false if advertise failed
	 */
    bool advertise()
	{
		if (!advertised()) {
			_advertised = Manager::advertise(_node);
		}

		return advertised();
	}

	// TODO: advertise and publish overload

	/**
	 * @brief Publish to topic
	 * 
	 * @param data Topic data to be published
	 * @return true if published successfuly
	 * @return false if publish failed
	 */
	bool publish(T *data)
	{
		if (!advertised()) {
			advertise();
		}

		return (Manager::publish(_node,data));
	}

	// TODO: publish overload -> publish with Manager::time

	/**
	 * @brief Returns the topic struct stored in object
	 * 
	 * @return const T& internal topic struct referance
	 */
    const T &get(){
		return _data;
	}

	/**
	 * @brief Sets the internal topic struct data
	 * 
	 * @param data New data to be setted
	 */
	void set(const T &data){
		_data = data;
	}

	/**
	 * @brief Publish the internal topic struct data
	 * 
	 * @return true if published successfuly
	 * @return false if publish failed
	 */
	bool update(){
		return publish(&_data);
	}

private:

    const void *_node{nullptr};
    const char *_topic_name;
	bool _advertised;

    T _data;
    void* _handle;
};

#endif // __PUB_H__