/*
* any.h
*
*  Created on: Oct 26, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include <memory>
#include <typeindex>

#include "handy.h"
#include "uninitialized.h"

namespace kmu
{
	namespace impl
	{
		struct x_Base
		{
			virtual std::unique_ptr<x_Base> clone() const = 0;
			virtual ~x_Base() = default;
		};

		template<typename T>
		struct x_Concrete : public x_Base
		{
			template<typename... Ts>
			x_Concrete(Ts&&... ts)
				: m_content(std::forward<Ts>(ts)...)
			{}

			std::unique_ptr<x_Base> clone() const override
			{
				return std::make_unique<x_Concrete>(m_content);
			}

			T m_content;
		};
	} // namespace impl


	class any
	{
	public:
		using NullType = null_t;

		any()
			: m_pValue(std::make_unique<impl::x_Concrete<NullType>>())
			, m_typeHash(getTypeHash<NullType>())
		{}

		template<typename T>
		any(T&& value)
			: m_pValue(std::make_unique<impl::x_Concrete<T>>(std::forward<T>(value)))
			, m_typeHash(getTypeHash<T>())
		{}

		any(const any& that)
			: m_typeHash(that.m_typeHash)
		{
			assert(that.m_pValue);
			m_pValue = that.m_pValue->clone();
		}

		any& operator=(const any& that)
		{
			assert(that.m_pValue);
			m_typeHash = that.m_typeHash;
			m_pValue = that.m_pValue->clone();

			return *this;
		}

		any(any&& that)
			: m_typeHash(that.m_typeHash)
		{
			assert(that.m_pValue);
			m_pValue = std::move(that.m_pValue);

			that.m_typeHash = getTypeHash<NullType>();
			m_pValue = nullptr;
		}

		any& operator=(any&& that)
		{
			assert(that.m_pValue);
			m_typeHash = that.m_typeHash;
			m_pValue = std::move(that.m_pValue);

			that.m_typeHash = getTypeHash<NullType>();
			that.m_pValue = nullptr;

			return *this;
		}

		template<typename T>
		bool is() const noexcept
		{
			return m_typeHash == getTypeHash<T>();
		}

		bool isNull() const noexcept
		{
			return is<NullType>();
		}

		template<typename T>
		T& as()
		{
			assert(is<T>());
			assert(dynamic_cast<impl::x_Concrete<T>*>(m_pValue.get()));

			return static_cast<impl::x_Concrete<T>&>(*m_pValue).m_content;
		}

	private:
		std::unique_ptr<impl::x_Base> m_pValue;
		size_t m_typeHash;
	};

	template<typename T, typename... Ts>
	any makeAny(Ts&&... ts)
	{
		return {T(std::forward<Ts>(ts)...)};
	}

} // namespace kmu