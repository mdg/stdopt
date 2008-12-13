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
#include <iostream>
#include <sstream>
#include <cstring>

using namespace stdopt;


/*
void usage_option_c::write_usage_doc( std::ostream &doc ) const
{
	if ( m_short_opt ) {
		doc << "\t-" << m_short_opt;
	}

	if ( ! m_long_opt.empty() ) {
		doc << "\t--" << m_long_opt;
	}

	if ( ! m_description.empty() ) {
		doc << "\t" << m_description;
	}

	doc << std::endl;
}
*/


void usage_c::add( usage_option_i &option )
{
	m_option.push_back( &option );
}

bool usage_c::parse_args( int argc, const char **argv )
{
	// skip the first arg which is the command
	for ( int i(1); i<argc; ++i ) {
		// usage_error = usage_error || 
		// std::cerr << "argv[" << i << "] = " << argv[i] << std::endl;

		bool consumed_param( false );
		if ( long_style_arg( argv[i] ) ) {
			parse_long_arg( argv[i] );
		} else if ( short_style_arg( argv[i] ) ) {
			std::string short_param;
			if ( i + 1 < argc ) {
				short_param = argv[i+1];
			}
			parse_short_args( argv[i] + 1, short_param
					, consumed_param );
		} else {
			// positional arg
			// not yet supported.  ignore for now.
		}

		if ( consumed_param ) {
			++i;
		}
	}

	return ! m_error;
}

bool usage_c::short_style_arg( const char *arg )
{
	return arg[0] == '-' && arg[1] != '-';
}

bool usage_c::long_style_arg( const char *arg )
{
	return arg[0] == '-' && arg[1] == '-';
}

void usage_c::parse_short_args( const std::string &args
		, const std::string &param, bool &consumed_param )
{
	std::string::const_iterator it( args.begin() );
	for ( ; it!=args.end(); ++it ) {
		// short option
		usage_option_i *option = find_short_option( *it );
		if ( ! option ) {
			// this option is not found
			// flag as error
			m_error = true;
		}

		if ( option->requires_param() ) {
			if ( ! param.empty() ) {
				consumed_param = true;
				option->parse_value( param );
			} else {
				m_error = true;
			}
		} else {
			std::string no_param;
			option->parse_value( no_param );
		}
	}
}

void usage_c::parse_long_arg( const std::string &arg )
{
	std::string option_name;
	std::string option_value;
	bool has_value;

	std::size_t equal_pos( arg.find( "=" ) );
	if ( equal_pos == std::string::npos ) {
		option_name = arg;
	} else {
		option_name = arg.substr( 0, equal_pos );
		option_value = arg.substr( equal_pos + 1 );
		has_value = true;
	}

	usage_option_i *option = find_long_option( option_name );
	if ( ! option ) {
		m_error = true;
		return;
	}

	if ( option->requires_param() && ! has_value ) {
		m_error = true;
		return;
	}

	option->parse_value( option_value );
}

usage_option_i * usage_c::find_short_option( char short_opt )
{
	option_list::iterator it;
	for ( it=m_option.begin(); it!=m_option.end(); ++it ) {
		usage_option_i &opt( **it );
		if ( opt.usage_character() == short_opt ) {
			return &opt;
		}
	}
	return NULL;
}

usage_option_i * usage_c::find_long_option( const std::string &long_opt )
{
	option_list::iterator it;
	for ( it=m_option.begin(); it!=m_option.end(); ++it ) {
		usage_option_i &opt( **it );
		if ( opt.option_name() == long_opt ) {
			return &opt;
		}
	}
	return NULL;
}

/*
void usage_doc_c::write( const usage_c &usage, std::ostream &doc ) const
{
	doc << "Usage: shessiond [OPTIONS]\n\n";
	doc << "Options:\n";
	usage_option_c::list::const_iterator it;
	for ( it=usage.m_option.begin(); it!=usage.m_option.end(); ++it ) {
		(*it)->write_usage_doc( doc );
	}
	doc << "\n";
}
*/

