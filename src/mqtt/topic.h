/////////////////////////////////////////////////////////////////////////////
/// @file topic.h
/// Declaration of MQTT topic class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2013-2016 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_topic_h
#define __mqtt_topic_h

#include "MQTTAsync.h"
#include "mqtt/delivery_token.h"
#include "mqtt/message.h"
#include <string>
#include <vector>
#include <memory>

namespace mqtt {

class async_client;

/////////////////////////////////////////////////////////////////////////////

/**
 * Represents a topic destination, used for publish/subscribe messaging.
 */
class topic
{
	/** The topic name */
	std::string name_;

	/** The client to which this topic is connected */
	// TODO: Make this a smart pointer
	iasync_client* cli_;

public:
	/**
	 * A smart/shared pointer to this class.
	 */
	using ptr_t = std::shared_ptr<topic>;
	/**
	 * Construct an MQTT topic destination for messages.
	 * @param name
	 * @param cli
	 */
	topic(const std::string& name, iasync_client& cli) : name_{name}, cli_{&cli} {}
	/**
	 * Returns the name of the queue or topic.
	 * @return std::string
	 */
	std::string get_name() const { return name_; }
	/**
	 * Publishes a message on the topic.
	 * @param payload
	 * @param n
	 * @param qos
	 * @param retained
	 *
	 * @return delivery_token
	 */
	idelivery_token_ptr publish(const void* payload, size_t n, int qos, bool retained);
	/**
	 * Publishes a message on the topic.
	 * @param payload
	 * @param qos
	 * @param retained
	 *
	 * @return delivery_token
	 */
	idelivery_token_ptr publish(const std::string& payload, int qos, bool retained);
	/**
	 * Publishes the specified message to this topic, but does not wait for
	 * delivery of the message to complete.
	 * @param msg
	 * @return delivery_token
	 */
	idelivery_token_ptr publish(const_message_ptr msg);
	/**
	 * Returns a string representation of this topic.
	 * @return std::string
	 */
	std::string to_str() const { return name_; }
};

/**
 * A shared pointer to the topic class.
 */
using topic_ptr = topic::ptr_t ;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_topic_h

