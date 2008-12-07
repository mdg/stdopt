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

