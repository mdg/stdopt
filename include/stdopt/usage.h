#ifndef STDOPT_USAGE_H
#define STDOPT_USAGE_H
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
#include <list>
#include <string>

namespace stdopt {


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
			, const std::string &desc = std::string() )
	: option_value_c< T >()
	, m_option_name( name )
	, m_description( desc )
	, m_usage_char( short_opt )
	{}

	/**
	 * Construct the config option with a default value.  The name
	 * is the key in the configuration file.
	 */
	usage_option_c( const T &default_value, char short_opt
			, const std::string &name
			, const std::string &desc = std::string() )
	: option_value_c< T >( default_value )
	, m_option_name( name )
	, m_description( desc )
	, m_usage_char( short_opt )
	{}


	/**
	 * Get the character value for this option.
	 */
	virtual char usage_character() const { return m_usage_char; }
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

	char m_usage_char;
};


template < typename T >
class positional_value_c
: public option_value_c< T >
{
public:
	positional_value_c()
	: option_value_c< T >()
	{}

	positional_value_c( const T &default_value )
	: option_value_c< T >( default_value )
	{}
};


/**
 * An argument parser class
 */
class usage_c
{
	friend class usage_doc_c;
	typedef std::list< usage_option_i * > option_list;
	typedef std::list< option_value_i * > positional_list;

public:
	/**
	 * Construct an empty arg parser
	 */
	usage_c() {}

	/**
	 * Add a usage option.
	 */
	void add( usage_option_i & );

	/**
	 * Add a positional option.
	 */
	template < typename T >
	void add( positional_value_c< T > &val )
	{
		m_positional.push_back( &val );
	}

	/**
	 * Parse a given set of args
	 * @return true if the usage was parsed successfully
	 */
	bool parse_args( int argc, const char **argv );

	/**
	 * Check if there was an error parsing the options.
	 */
	bool error() const { return m_error; }

private:
	static bool short_style_arg( const char *arg );
	static bool long_style_arg( const char *arg );

	void parse_short_args( const std::string &args
			, const std::string &param, bool &consumed_param );
	void parse_long_arg( const std::string &arg );

	/**
	 * search for an option given a short style character
	 */
	usage_option_i * find_short_option( char short_opt );
	/**
	 * search for an option given a long style string
	 */
	usage_option_i * find_long_option( const std::string &long_opt );

	option_list m_option;
	positional_list m_positional;
	bool m_error;
};


/**
 * Class for formating usage options into a usable online document.
 * Consider modifying it to write a man page as well.
 */
class usage_doc_c
{
public:
	/**
	 * Build a usage document for all options.
	 * This should be a separate class.
	 */
	void write( const usage_c &, std::ostream & ) const;

private:
};


} // end namespace

#endif

