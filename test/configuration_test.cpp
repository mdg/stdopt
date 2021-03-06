/**
 * Copyright 2008 Matthew Graham
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdopt/configuration.h"
#include <testpp/test.h>
#include <sstream>

using namespace stdopt;


/// Tests for the config_option_c class first

/**
 * Test that preconditions are set properly
 * in the config_option class.
 */
TESTPP( test_config_option_initialization )
{
	config_option_c< std::string > opt( "dog", "description of a dog" );

	// assert preconditions
	assertpp( opt.option_name() ) == "dog";
	assertpp( opt.set() ).f();
	assertpp( opt.error() ).f();
}

/**
 * Test that preconditions are set properly
 * in the config_option constructor with a
 * default value.
 */
TESTPP( test_config_option_default_init )
{
	config_option_c< int > opt( 58, "dog", "description of a dog" );

	// assert preconditions
	assertpp( opt.option_name() ) == "dog";
	assertpp( opt.value() ) == 58;
	assertpp( opt.set() ).f();
	assertpp( opt.error() ).f();
}

/**
 * Test that the config option class works for the
 * string type.
 */
TESTPP( test_string_config_option )
{
	config_option_c< std::string > opt( "dog", "description" );
	opt.parse_value( "cat" );

	// assert postconditions
	assertpp( opt.value() ) == "cat";
	assertpp( opt.set() ).t();
	assertpp( opt.error() ).f();
}

/**
 * Test that the config_option class works for integers.
 */
TESTPP( test_int_config_option )
{
	config_option_c< int > opt( "dog", "description" );
	opt.parse_value( "89" );

	// assert postconditions
	assertpp( opt.value() ) == 89;
	assertpp( opt.set() ).t();
	assertpp( opt.error() ).f();
}


/**
 * Test that the config_option class works for booleans.
 */
TESTPP( test_bool_config_option )
{
	config_option_c< bool > opt( "dog", "desc" );
	opt.parse_value( "1" );

	// assert postconditions
	assertpp( opt.value() ).t();
	assertpp( opt.set() ).t();
	assertpp( opt.error() ).f();
}

/**
 * Test that parsing an invalid string for integers fails properly.
 */
TESTPP( test_invalid_int_option )
{
	config_option_c< int > opt( "dog", "description" );
	opt.parse_value( "cat" );

	// assert postconditions
	assertpp( opt.error() ).t();
	assertpp( opt.set() ).f();
	assertpp( opt.value() ) == 0;
}


/// Tests for the config_option_list_c class

/**
 * Test that the config_option_c class accepts multiple items.
 */
TESTPP( test_option_parse_multi_values )
{
	config_option_c< int > ports( "port", "description" );
	ports.parse_value( "5" );
	ports.parse_value( "6" );
	ports.parse_value( "7" );
	ports.parse_value( "8" );

	// verify the array access
	assertpp( ports.size() ) == 4;
	assertpp( ports.value( 0 ) ) == 5;
	assertpp( ports.value( 1 ) ) == 6;
	assertpp( ports.value( 2 ) ) == 7;
	assertpp( ports.value( 3 ) ) == 8;

	// verify the iterator access
	config_option_c< int >::iterator it( ports.begin() );
	assertpp( *(it++) ) == 5;
	assertpp( *(it++) ) == 6;
	assertpp( *(it++) ) == 7;
	assertpp( *(it++) ) == 8;
}


/// Tests for the configuration_c class

/**
 * Test the most basic functionality, 2 string options
 */
TESTPP( test_basic_string_options )
{
	config_option_c< std::string > option1( "option1", "desc1" );
	config_option_c< std::string > option2( "option2", "desc2" );
	config_option_c< std::string > option3( "option3", "desc3" );

	configuration_c config;
	config.add( option1 );
	config.add( option2 );

	std::istringstream input( "option1=dog\noption2=cat" );

	config.parse( input );

	assertpp( option1.set() ).t();
	assertpp( option2.set() ).t();
	assertpp( option3.set() ).f();
	assertpp( option1.value() ) == "dog";
	assertpp( option2.value() ) == "cat";
}

/**
 * Test the integer and boolean options
 */
TESTPP( test_int_options )
{
	config_option_c< int > port( "port", "pdesc" );
	config_option_c< int > timeout( "timeout", "todesc" );
	config_option_c< bool > debug( "debug", "debug desc" );
	std::istringstream input( "port=4000\ntimeout=20\ndebug=1" );

	configuration_c config;
	config.add( port );
	config.add( timeout );
	config.add( debug );
	config.parse( input );

	assertpp( port.set() ).t();
	assertpp( timeout.set() ).t();
	assertpp( debug.set() ).t();

	assertpp( port.value() ) == 4000;
	assertpp( timeout.value() ) == 20;
	assertpp( debug.value() ).t();
}

/**
 * Test that option_lists are parsed properly
 */
TESTPP( test_parse_config_list )
{
	config_option_c< int > ports( "port", "desc" );
	std::istringstream input( "port=4000\nport=2000\nport=4001" );

	configuration_c config;
	config.add( ports );
	config.parse( input );

	assertpp( ports.set() ).t();
	assertpp( ports.error() ).f();

	assertpp( ports.size() ) == 3;
	assertpp( ports.value() ) == 4000;
	assertpp( ports.value( 0 ) ) == 4000;
	assertpp( ports.value( 1 ) ) == 2000;
	assertpp( ports.value( 2 ) ) == 4001;
	assertpp( ports.last_value() ) == 4001;
}


/**
 * Test that it still works with windows line endings.
 */
TESTPP( test_crlf )
{
	config_option_c< int > timeout( "session-timeout", "desc" );
	config_option_c< std::string > split( "split", "desc" );

	const char str_input[] =
		"session-timeout=30\r\n"
		"split=dog";
	std::stringstream input( str_input );

	configuration_c config;
	config.add( split );
	config.add( timeout );
	config.parse( input );

	assertpp( timeout.set() ).t();
	assertpp( split.set() ).t();

	assertpp( timeout.value() ) == 30;
	assertpp( split.value() ) == "dog";
}

/**
 * Test that whitespace doesn't mess up the parsing of a config file.
 */
TESTPP( test_whitespace )
{
	config_option_c< int > timeouts( "session-timeout", "desc1" );
	config_option_c< int > ports( "port", "desc2" );
	const char str_input[] = "\nsession-timeout = 20  \n" \
				  "\n session-timeout  = 23 \n "\
				  "\n port = 9000\n" \
				  "\n port = 9001 \n";
	std::stringstream input( str_input );

	configuration_c config;
	config.add( timeouts );
	config.add( ports );
	config.parse( input );

	assertpp( timeouts.size() ) == 2;
	assertpp( ports.size() ) == 2;

	assertpp( timeouts.value( 0 ) ) == 20;
	assertpp( timeouts.value( 1 ) ) == 23;

	assertpp( ports.value( 0 ) ) == 9000;
	assertpp( ports.value( 1 ) ) == 9001;
}


/**
 * Test that a non integer value doesn't freak out when an integer
 * is requested.
 */
TESTPP( test_non_integer )
{
	config_option_c< int > timeout( "session-timeout", "desc" );
	const char str_input[] = "session-timeout=hh340\n";
	std::stringstream input( str_input );

	configuration_c config;
	config.add( timeout );
	config.parse( input );

	assertpp( timeout.error() ).t();
	assertpp( config.error() ).t();
}

