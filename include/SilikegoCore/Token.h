/* Token.h: Token passed from tokenizer to parser
 * Copyright 2012-2025 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined SILIKEGO_CORE_TOKEN_H
#define SILIKEGO_CORE_TOKEN_H

#include <string>

#include <SilikegoCore/Api.h>

namespace Silikego
{
	enum class TokenStatus
	{
		Unset,
		Integer,
		Float,
		Operator,
		Id,
		EndOfInput
	};

	struct SILIKEGOCORE_EXPORT EndOfInput{};

	class SILIKEGOCORE_EXPORT Token
	{
	public:
		Token();

		Token(char);
		Token(short int);
		Token(int);
		Token(long int);
		Token(long long int);
		Token(float);
		Token(double);
		Token(EndOfInput);
		Token(const std::string&);

		Token(const Token&);
		~Token();

		Token& operator=(const Token&);

		TokenStatus status() const;
		long long int integerValue() const;
		double floatValue() const;
		char operatorValue() const;
		const std::string& idValue() const;

	private:
		class Impl;
		Impl *impl;
	};
};

#endif // SILIKEGO_CORE_TOKEN_H
