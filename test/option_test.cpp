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

#include <testpp/test.h>
#include "stdopt/option.h"

using namespace stdopt;


// USAGE_OPTION_I
/**
 * Assert that the type_requires_param() function returns the right
 * response in all necessary cases.
 */
TESTPP( test_type_requires_param )
{
	assertpp( usage_option_i::type_requires_param< bool >() ).f();
	assertpp( usage_option_i::type_requires_param< int >() ).t();
	assertpp( usage_option_i::type_requires_param< std::string >() ).t();
}


// OPTION_VALUE_C
/**
 * Test that the option_value_c constructor sets the values correctly.
 */
TESTPP( test_option_value_constructor )
{
	option_value_c< int > value;

	assertpp( value.set() ).f();
	assertpp( value.error() ).f();
	assertpp( value.size() ) == 0;
	assertpp( value.value() ) == 0;
}

/**
 * Test that the option_value_c constructor sets the values correctly
 * when given a default value.
 */
TESTPP( test_option_value_constructor_with_default )
{
	option_value_c< int > num( 4 );

	assertpp( num.set() ).f();
	assertpp( num.error() ).f();
	assertpp( num.size() ) == 0;
	assertpp( num.value() ) == 4;
}

/**
 * Test the parse value function in expected operation.
 */
TESTPP( test_option_value_parse_value )
{
	option_value_c< int > value;

	value.parse_value( "45" );

	assertpp( value.set() ).t();
	assertpp( value.error() ).f();
	assertpp( value.size() ) == 1;

	assertpp( value.value() ) == 45;
	assertpp( value.last_value() ) == 45;
}

/**
 * Test the pars value function when called multiple times.
 */
TESTPP( test_option_value_parse_multiple_values )
{
	option_value_c< int > num;

	num.parse_value( "50" );
	num.parse_value( "24" );
	num.parse_value( "22" );

	assertpp( num.set() ).t();
	assertpp( num.error() ).f();
	assertpp( num.size() ) == 3;

	assertpp( num.value() ) == 50;
	assertpp( num.last_value() ) == 22;

	assertpp( num.value( 0 ) ) == 50;
	assertpp( num.value( 1 ) ) == 24;
	assertpp( num.value( 2 ) ) == 22;

	option_value_c< int >::iterator it( num.begin() );
	assertpp( *(it++) ) == 50;
	assertpp( *(it++) ) == 24;
	assertpp( *(it++) ) == 22;
}

/**
 * Test behavior when parsing invalid values.
 */
TESTPP( test_option_value_parse_invalid_value )
{
	option_value_c< int > num;

	num.parse_value( "dog" );

	assertpp( num.set() ).f();
	assertpp( num.error() ).t();
}

/**
 * Test behavior when parsing an invalid value after already having parsed
 * valid values.
 */
TESTPP( test_option_value_parse_invalid_value_after_valid )
{
	option_value_c< int > num;

	num.parse_value( "37" );
	num.parse_value( "dog" );

	assertpp( num.set() ).f();
	assertpp( num.error() ).t();
}

