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

#include "stdopt/option.h"

using namespace stdopt;


template <>
bool usage_option_i::type_requires_param< bool >()
{
	return false;
}

template <>
bool option_value_c< bool >::parse_value( const std::string &str_value )
{
	m_values.push_back( true );
	m_set = true;
}

template <>
bool option_value_c< std::string >::parse_value( const std::string &str_value )
{
	m_values.push_back( str_value );
	m_set = true;
}

