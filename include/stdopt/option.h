#ifndef STDOPT_OPTION_H
#define STDOPT_OPTION_H
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

#include <sstream>
#include <vector>

namespace stdopt {


/**
 * Interface for storing the option value.
 */
class option_value_i
{
public:
	/**
	 * Virtual parser
	 */
	virtual bool parse_value( const std::string & ) = 0;

	/**
	 * Check if this option was set _correctly_ in the configuration file.
	 * It returns false if there was an error.
	 */
	virtual bool set() const = 0;
	/**
	 * Check if this option was set incorrectly in the configuration file.
	 */
	virtual bool error() const = 0;
};


/**
 * Usage option specification.
 */
class usage_option_i
: virtual public option_value_i
{
public:
	/**
	 * Get the character flag for this option.
	 */
	virtual char usage_character() const = 0;
	/**
	 * Get the longer name for this option.
	 */
	virtual const std::string & option_name() const = 0;
	/**
	 * Get the description for this option.
	 */
	virtual const std::string & description() const = 0;

	/**
	 * Check if this usage option requires a parameter.
	 */
	virtual bool requires_param() const = 0;


	/**
	 * Check if an option type takes a parameter.
	 * Most types require a parameter.
	 */
	template < typename T >
	static bool type_requires_param()
	{
		return true;
	}
};

/**
 * Declare the type required function for the boolean type.
 * This is the only type that doesn't require a parameter.
 */
template <>
bool usage_option_i::type_requires_param< bool >();


/**
 * Config option specification.
 */
class config_option_i
: virtual public option_value_i
{
public:
	/**
	 * Get the name of this option.
	 */
	virtual const std::string & option_name() const = 0;
	/**
	 * Get the description for this option.  Used to describe
	 * in the docs.
	 */
	virtual const std::string & description() const = 0;

	/**
	 * Check if this options _must_ be set in the configuration file.
	 */
	virtual bool config_required() const = 0;
};


/**
 * A templated implementation of the option_value_i interface.
 * This implements the code for parsing values and setting them
 * for later retrieval by the client code.
 * Any type can be used as long as it has a default constructor and supports
 * the istream >> operator.
 */
template < typename T >
class option_value_c
: virtual public option_value_i
{
private:
	/**
	 * The internal type for storing values set by command line or
	 * configuration file.
	 */
	typedef std::vector< T > value_list;
public:
	/**
	 * The iterator class for iterating over values set for a given
	 * option.  Values are read-only for client code.
	 */
	typedef typename value_list::const_iterator iterator;

public:
	/**
	 * Construct an option value with _no_ default value.
	 */
	option_value_c()
	: m_values()
	, m_default()
	, m_default_set( false )
	, m_set( false )
	, m_error( false )
	{}

	/**
	 * Construct an option value with a default value.
	 */
	option_value_c( const T &default_value )
	: m_values()
	, m_default( default_value )
	, m_default_set( true )
	, m_set( false )
	, m_error( false )
	{}

	/**
	 * Check if this option was set _correctly_ in the configuration file.
	 * It returns false if there was an error.
	 */
	virtual bool set() const
	{
		return m_set && ! m_error;
	}

	/**
	 * Check if this option was set incorrectly in the configuration file.
	 */
	virtual bool error() const { return m_error; }

	/**
	 * Get the value set.  If the value is set multiple times
	 * this will return the first value.
	 */
	const T & value() const
	{
		if ( ! m_set ) {
			// return default even if it's not set
			// to avoid seg faults
			return m_default;
		}
		return m_values.front();
	}

	/**
	 * If the value is set multiple times, this will return the
	 * most recently set value.
	 */
	const T & last_value() const
	{
		if ( ! m_set ) {
			// return default even if it's not set
			// to avoid seg faults
			return m_default;
		}
		return m_values.back();
	}

	/**
	 * Get the number of values set for this option.
	 */
	int size() const { return m_values.size(); }

	/**
	 * Get the ith value set for this option.
	 */
	const T & value( int i ) const { return m_values[ i ]; }

	/**
	 * Get the begin iterator for the list of values on this option.
	 */
	iterator begin() const { return m_values.begin(); }
	/**
	 * Get the end iterator for the list of values on this option.
	 */
	iterator end() const { return m_values.end(); }

	/**
	 * Implementation of parsing the string value into the templated
	 * type.  The templated type just needs an implementation of
	 *   istream >> T
	 */
	virtual bool parse_value( const std::string &str_value )
	{
		// don't keep parsing after an error
		if ( m_error )
			return false;

		std::istringstream input( str_value );
		T val( m_default );
		input >> val;
		m_error = input.fail();
		if ( ! m_error ) {
			m_set = true;
			m_values.push_back( val );
		}
		return ! m_error;
	}

private:
	value_list m_values;
	const T m_default;
	const bool m_default_set;
	bool m_set;
	bool m_error;
};


/**
 * An option that can be set on command line usage or a configuration file.
 */
template < typename T >
class shared_option_c
: public option_value_c< T >
, virtual public config_option_i
, virtual public usage_option_i
{
public:
	shared_option_c();

	virtual char option_character() const { return m_option_char; }
	virtual const std::string & option_name() const
	{
		return m_option_name;
	}
	virtual const std::string & description() const
	{
		return m_description;
	}

	virtual bool requires_param() const
	{
		return usage_option_i::type_requires_param< T >();
	}
	virtual bool config_required() const { return m_config_required; }

private:
	std::string m_option_name;
	std::string m_description;

	char m_option_char;
	bool m_config_required;
};


} // end namespace

#endif

