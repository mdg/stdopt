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

#include "stdopt/usage.h"
#include <testpp/test.h>
#include <sstream>

using namespace stdopt;


/**
 * Test that the short style command line usage works with booleans.
 */
TESTPP( test_short_usage_bool )
{
	usage_c usage;
	usage_option_c< bool > debug( 'g', "debug", "Add debugging logging." );
	usage.add( debug );

	static const int argc( 2 );
	const char *argv[20] = { "shessiond", "-g" };

	usage.parse_args( argc, argv );

	assertpp( debug.set() ).t();
	assertpp( debug.value() ).t();
}

/**
 * Test that the long style command line usage works.
 */
TESTPP( test_long_usage_bool )
{
	usage_c usage;
	usage_option_c< bool > debug( 'g', "debug", "Add debugging options." );
	usage.add( debug );

	static const int argc( 2 );
	const char *argv[20] = { "shessiond", "--debug" };

	usage.parse_args( argc, argv );
	assertpp( debug.set() ).t();
	assertpp( debug.value() ).t();
}

/**
 * Test that the short style usage works for integers
 */
TESTPP( test_short_usage_int )
{
	usage_option_c< int > depth( 'd', "depth", "Depth of something." );
	usage_c usage;
	usage.add( depth );

	const char *argv[20] = { "program", "-d", "5" };
	usage.parse_args( 3, argv );

	assertpp( depth.set() ).t();
	assertpp( depth.value() ).t();
}

/**
 * Test that the short style usage works for integers
 */
TESTPP( test_long_usage_int )
{
	usage_option_c< int > depth( 'd', "depth", "Depth of something." );
	usage_c usage;
	usage.add( depth );

	const char *argv[20] = { "program", "--depth=5" };
	usage.parse_args( 2, argv );

	assertpp( depth.set() ).t();
	assertpp( depth.value() ).t();
}

/**
 * Test multiple word for short usage
 */
TESTPP( test_short_usage_quoted_string )
{
	usage_option_c< std::string > include( 'i', "include", "Include x" );
	usage_c usage;
	usage.add( include );

	const char *argv[20] = { "bin", "-i", "string value" };
	usage.parse_args( 3, argv );

	assertpp( include.set() ).t();
	assertpp( include.value() ) == "string value";
}

/**
 * Test multiple word long usage
 */
TESTPP( test_long_usage_quoted_string )
{
	usage_option_c< std::string > include( 'i', "include", "Include x" );
	usage_c usage;
	usage.add( include );

	const char *argv[20] = { "bin", "--include=string value" };
	usage.parse_args( 2, argv );

	assertpp( include.set() ).t();
	assertpp( include.value() ) == "string value";
}

/**
 * Test multiple short options attached.
 */
TESTPP( test_attached_short_bool_options )
{
	usage_option_c< bool > debug( 'd', "debug", "Debug" );
	usage_option_c< bool > special( 's', "special", "Special" );
	usage_c usage;
	usage.add( debug );
	usage.add( special );

	const char *argv[20] = { "bin", "-ds" };
	usage.parse_args( 2, argv );

	assertpp( debug.set() ).t();
	assertpp( special.set() ).t();
	assertpp( debug.value() ).t();
	assertpp( special.value() ).t();
}

/**
 * Test multiple short options attached and with a parameter.
 */
TESTPP( test_attached_short_options_with_param )
{
	usage_option_c< bool > debug( 'd', "debug" );
	usage_option_c< std::string > file( 'f', "file" );
	usage_c usage;
	usage.add( debug );
	usage.add( file );

	const char *argv[20] = { "bin", "-df", "my_file.txt", "cmd" };
	usage.parse_args( 4, argv );

	assertpp( debug.set() ).t();
	assertpp( file.set() ).t();
	assertpp( debug.value() ).t();
	assertpp( file.value() ) == "my_file.txt";
}

/**
 * Test positional arguments
 */
TESTPP( test_positional_arg_success )
{
	positional_value_c< int > max_value;
	positional_value_c< std::string > file;
	usage_c usage;
	usage.add( max_value );
	usage.add( file );

	const char *argv[20] = { "bin", "6", "myfile" };
	usage.parse_args( 3, argv );

	assertpp( max_value.value() ) == 6;
	assertpp( file.value() ) == "myfile";
}


/**
 * Test that option document creation works and formats properly.
TESTPP( test_option_doc )
{
	usage_option_c< bool > debug( 'g', "debug", "Write debugging logging." );
	std::ostringstream doc;
	debug.write_usage_doc( doc );

	assertpp( doc.str() ) == "\t-g\t--debug\tWrite debugging logging.\n";
}
*/

