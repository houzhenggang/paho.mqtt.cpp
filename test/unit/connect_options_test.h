// connect_options_test.h
// Unit tests for the connect_options class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016 Frank Pagliughi <fpagliughi@mindspring.com>
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

#ifndef __mqtt_connect_options_test_h
#define __mqtt_connect_options_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/connect_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class connect_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( connect_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment );
	CPPUNIT_TEST( test_move_assignment );
	CPPUNIT_TEST( test_set_user );
	CPPUNIT_TEST( test_set_will );
	CPPUNIT_TEST( test_set_ssl );
	CPPUNIT_TEST( test_set_token );

	CPPUNIT_TEST_SUITE_END();

	// C struct signature/eyecatcher
	const char* CSIG = "MQTC";
	const size_t CSIG_LEN = std::strlen(CSIG);

	const std::string USER { "wally" };
	const std::string PASSWD { "xyzpdq" };

	const std::string EMPTY_STR;

public:
	void setUp() {
	}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::connect_options opts;
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_password());

		const auto& c_struct = opts.opts_;
		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(c_struct.username == nullptr);
		CPPUNIT_ASSERT(c_struct.password == nullptr);

		// No callbacks without a context token
		CPPUNIT_ASSERT(c_struct.context == nullptr);
		CPPUNIT_ASSERT(c_struct.onSuccess == nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure == nullptr);

		// No will or SSL, for default
		CPPUNIT_ASSERT(c_struct.will == nullptr);
		CPPUNIT_ASSERT(c_struct.ssl == nullptr);
	}

// ----------------------------------------------------------------------
// Test the constructor that takes user/password
// ----------------------------------------------------------------------

	void test_user_constructor() {
		mqtt::connect_options opts { USER, PASSWD };

		const auto& c_struct = opts.opts_;
		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));

		// Not callbacks without a context token
		CPPUNIT_ASSERT(c_struct.context == nullptr);
		CPPUNIT_ASSERT(c_struct.onSuccess == nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure == nullptr);
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_constructor() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { orgOpts };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_constructor() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { std::move(orgOpts) };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_password());

		CPPUNIT_ASSERT(orgOpts.opts_.username == nullptr);
		CPPUNIT_ASSERT(orgOpts.opts_.password == nullptr);
	}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts;

		opts = orgOpts;

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		// Self assignment should cause no harm
		opts = opts;

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());
	}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

	void test_move_assignment() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { std::move(orgOpts) };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_password());

		CPPUNIT_ASSERT(orgOpts.opts_.username == nullptr);
		CPPUNIT_ASSERT(orgOpts.opts_.password == nullptr);

		// Self assignment should cause no harm
		// (clang++ is smart enough to warn about this)
		#if !defined(__clang__)
			opts = std::move(opts);
			CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
			CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());
		#endif
	}


// ----------------------------------------------------------------------
// Test set/get of the user and password.
// ----------------------------------------------------------------------

	void test_set_user() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		opts.set_user_name(USER);
		opts.set_password(PASSWD);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password());

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(!strcmp(PASSWD.c_str(), c_struct.password));
	}

// ----------------------------------------------------------------------
// Test set/get of will options
// ----------------------------------------------------------------------

	void test_set_will() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.will);
		mqtt::will_options willOpts;
		opts.set_will(willOpts);
		CPPUNIT_ASSERT(nullptr != c_struct.will);
		CPPUNIT_ASSERT_EQUAL(&opts.will_.opts_, c_struct.will);
	}

// ----------------------------------------------------------------------
// Test set/get of ssl options
// ----------------------------------------------------------------------

	void test_set_ssl() {
#if defined(OPENSSL)
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.ssl);
		mqtt::ssl_options sslOpts;
		opts.set_ssl(sslOpts);
		CPPUNIT_ASSERT(nullptr != c_struct.ssl);
		CPPUNIT_ASSERT_EQUAL(&opts.ssl_.opts_, c_struct.ssl);
#endif
	}

// ----------------------------------------------------------------------
// Test set/get of context token
// ----------------------------------------------------------------------

	void test_set_token() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.context);
		mqtt::test::dummy_async_client ac;
		mqtt::token_ptr tok = std::make_shared<token>(ac);
		opts.set_token(tok);
		CPPUNIT_ASSERT(c_struct.context == tok.get());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_connect_options_test_h

