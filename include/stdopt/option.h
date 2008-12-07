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
#include <map>
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
	virtual char option_character() const = 0;
	virtual const std::string & option_name() const = 0;
	virtual const std::string & description() const = 0;

	virtual bool requires_param() const = 0;


	/**
	 * Check if an option type takes a parameter.
	 */
	template < typename T >
	static bool type_requires_param();
};

template < bool >
bool usage_option_i::type_requires_param< bool >();


/**
 * Config option specification.
 */
class config_option_i
: virtual public option_value_i
{
public:
	virtual const std::string & option_name() const = 0;
	virtual const std::string & description() const = 0;

	virtual bool config_required() const = 0;
};


TESTPP( test_usage )
{
	stdopt::option_c< int > ls_files( "ls-files"
			, "List the files included in this test." );
	stdopt::config_option_c< int > ls_files( "ls-files"
			, "List the files included in this test." );
}


template < typename T >
class option_value_c
: virtual public option_value_i
{
public:
	typedef typename std::vector< T >::const_iterator iterator;
private:
	typedef std::vector< T > value_list;

public:
	option_value_c();
	option_value_c( const T &default_value );

	/**
	 * Check if this option was set _correctly_ in the configuration file.
	 * It returns false if there was an error.
	 */
	virtual bool set() const;

	/**
	 * Check if this option was set incorrectly in the configuration file.
	 */
	virtual bool error() const;

	/**
	 * Get the value set.  If the value is set multiple times
	 * this will return the first value.
	 */
	const T & value() const;
	/**
	 * If the value is set multiple times, this will return the
	 * most recently set value.
	 */
	const T & last_value() const;

	int num_values() const;
	const T & value( int i ) const;

	/**
	 * Get the begin iterator for the list of values.
	 */
	iterator begin() const { return m_values.begin(); }
	/**
	 * Get the end iterator for the list of values.
	 */
	iterator end() const { return m_values.end(); }

	/**
	 * Virtual parser
	 */
	virtual bool parse_value( const std::string &str_value )
	{
		// don't keep parsing after an error
		if ( m_error )
			return false;

		std::istringstream input( str_value );
		T val();
		input >> val;
		m_error = input.fail();
		m_set = ! m_error;
		m_values.push_back( val );
		return ! m_error;
	}

private:
	value_list m_values;
	bool m_set;
	bool m_error;
};


/**
 * An option that can be set on the command line.
 */
template < typename T >
class usage_option_c
: public option_value_c< T >
, virtual public usage_option_i
{
public:
	/**
	 * Construct the config option.  The name is the key in the
	 * configuration file.
	 */
	usage_option_c( char short_opt, const std::string &name
			, const std::string &desc )
	: option_value_c()
	, m_option_name( name )
	, m_description( desc )
	, m_option_char( short_opt )
	{}

	/**
	 * Construct the config option with a default value.  The name
	 * is the key in the configuration file.
	 */
	usage_option_c( const T &default_value, char short_opt
			, const std::string &name, const std::string &desc )
	: option_value_c( default_value )
	, m_option_name( name )
	, m_description( desc )
	, m_option_char( short_opt )
	{}


	virtual char option_character() const { return m_option_char; }
	/**
	 * Get the name of this option.
	 */
	virtual const std::string & option_name() const
	{
		return m_option_name;
	}
	/**
	 * Get the description for this option.
	 */
	virtual const std::string & description() const
	{
		return m_description;
	}

	/**
	 * Check if this option requires a parameter.  The implementation
	 * is that bool will not require a param, everything else will.
	 */
	virtual bool requires_param() const
	{
		return usage_option_i::type_requires_param< T >();
	}

private:
	std::string m_option_name;
	std::string m_description;

	char m_option_char;
};


/**
 * An option to be set in the configuration file.
 */
template < typename T >
class config_option_c
: public option_value_c< T >
, virtual public config_option_i
{
public:
	/**
	 * Construct the config option.  The name is the key in the
	 * configuration file.
	 */
	config_option_c( const std::string &name, const std::string &desc )
	: option_value_c()
	, m_option_name( name )
	, m_description( desc )
	, m_config_required( false )
	{}

	/**
	 * Construct the config option with a default value.  The name
	 * is the key in the configuration file.
	 */
	config_option_c( const T &default_value, const std::string &name
			, const std::string &desc )
	: option_value_c( default_value )
	, m_option_name( name )
	, m_description( desc )
	, m_config_required( false )
	{}

	virtual const std::string & option_name() const { return m_option_name; }
	virtual const std::string & description() const { return m_description; }

	virtual bool config_required() const { return m_config_required; }

private:
	std::string m_option_name;
	std::string m_description;

	bool m_config_required;
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

