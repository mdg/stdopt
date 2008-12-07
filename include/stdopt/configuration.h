#ifndef STDOPT_CONFIGURATION_H
#define STDOPT_CONFIGURATION_H
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

#include <stdopt/option.h>
#include <map>

namespace stdopt {


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
	: option_value_c< T >()
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
	: option_value_c< T >( default_value )
	, m_option_name( name )
	, m_description( desc )
	, m_config_required( false )
	{}

	/**
	 * Get the name for this option.
	 */
	virtual const std::string & option_name() const
	{
		return m_option_name;
	}

	/**
	 * Get the description documentation for this option.
	 */
	virtual const std::string & description() const
	{
		return m_description;
	}

	/**
	 * Check if it's required that this value be set in the configuration.
	 */
	virtual bool config_required() const { return m_config_required; }

private:
	std::string m_option_name;
	std::string m_description;

	bool m_config_required;
};


/**
 * A parser class to get all the configurations from a file.
 */
class configuration_c
{
private:
	typedef std::map< std::string, config_option_i * > option_map;

public:
	/**
	 * Construct the config parser for a given input
	 * stream.
	 */
	configuration_c();

	/**
	 * Add an option that can be set in the configuration file.
	 */
	void add( config_option_i & );

	/**
	 * Parse the input from the given input stream.
	 */
	void parse( std::istream &input );

	/**
	 * Check if there was an error parsing the configuration.
	 */
	bool error() const { return m_error; }

private:
	option_map m_option;
	bool m_error;
};


} // end namespace

#endif

