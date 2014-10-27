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
#include <algorithm>

#include "handy.h"
#include "uninitialized.h"

namespace kmu
{
	namespace impl
	{
		struct HolderBase
		{
			virtual std::unique_ptr<HolderBase> clone() const = 0;
			virtual ~HolderBase() = default;
		};

		template<typename T>
		struct ConcreteHolder : public HolderBase
		{
			template<typename... Ts>
			ConcreteHolder(Ts&&... ts)
				: m_content(std::forward<Ts>(ts)...)
			{}

			std::unique_ptr<HolderBase> clone() const override
			{
				return std::make_unique<ConcreteHolder>(m_content);
			}

			T m_content;
		};
	} // namespace impl


	class any
	{
	public:
		using NullType = null_t;

		any()
			: m_pValue(std::make_unique<impl::ConcreteHolder<NullType>>())
			, m_typeHash(getTypeHash<NullType>())
		{}

		template<typename T>
		any(T&& value)
			: m_pValue(std::make_unique<impl::ConcreteHolder<T>>(std::forward<T>(value)))
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
		}

		any& operator=(any&& that)
		{
			assert(that.m_pValue);
			m_typeHash = that.m_typeHash;
			m_pValue = std::move(that.m_pValue);

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

		bool isNotNull() const noexcept
		{
			return isNull() == false;
		}

		template<typename T>
		T& as() noexcept
		{
			assert(is<T>());
			assert(dynamic_cast<impl::ConcreteHolder<T>*>(m_pValue.get()));

			return static_cast<impl::ConcreteHolder<T>&>(*m_pValue).m_content;
		}

		template<typename T>
		const T& as() const noexcept
		{
			assert(is<T>());
			assert(dynamic_cast<const impl::ConcreteHolder<T>*>(m_pValue.get()));

			return static_cast<const impl::ConcreteHolder<T>&>(*m_pValue).m_content;
		}

		void swap(any& that) noexcept
		{
			std::swap(m_pValue, that.m_pValue);
			std::swap(m_typeHash, that.m_typeHash);
		}

	private:
		std::unique_ptr<impl::HolderBase> m_pValue;
		size_t m_typeHash;
	};

	inline void swap(any& lhs, any& rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<typename T, typename... Ts>
	any makeAny(Ts&&... ts)
	{
		return {T(std::forward<Ts>(ts)...)};
	}

} // namespace kmu