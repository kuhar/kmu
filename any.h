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
			virtual ~HolderBase() = default;
			virtual std::unique_ptr<HolderBase> clone() const = 0;
			virtual std::type_index getTypeIndex() const noexcept = 0;
			virtual size_t getHash() const noexcept = 0;
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

			std::type_index getTypeIndex() const noexcept override
			{
				return typeid(T);
			}

			size_t getHash() const noexcept override
			{
				return getTypeIndex().hash_code();
			}	

			T m_content;
		};
	} // namespace impl


	class any
	{
	public:
		using NullType = kmu::null_t;

		any()
			: m_pValue(std::make_unique<impl::ConcreteHolder<NullType>>())
		{}

		template<typename T>
		any(T&& value)
			: m_pValue(std::make_unique<impl::ConcreteHolder<T>>(std::forward<T>(value)))
		{}

		any(const any& that)
		{
			assert(that.m_pValue);
			m_pValue = that.m_pValue->clone();
		}

		any& operator=(const any& that)
		{
			assert(that.m_pValue);
			m_pValue = that.m_pValue->clone();

			return *this;
		}

		any(any&& that)
		{
			assert(that.m_pValue);
			m_pValue = std::move(that.m_pValue);
		}

		any& operator=(any&& that)
		{
			assert(that.m_pValue);
			m_pValue = std::move(that.m_pValue);

			return *this;
		}

		template<typename T>
		bool is() const noexcept
		{
			assert(m_pValue);
			return m_pValue->getHash() == getTypeHash<T>();
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
			return static_cast<impl::ConcreteHolder<T>&>(*m_pValue).m_content;
		}

		template<typename T>
		const T& as() const noexcept
		{
			assert(is<T>());
			return static_cast<const impl::ConcreteHolder<T>&>(*m_pValue).m_content;
		}

		std::type_index getTypeIndex() const noexcept
		{
			assert(m_pValue);
			return m_pValue->getTypeIndex();
		}

		void swap(any& that) noexcept
		{
			std::swap(m_pValue, that.m_pValue);
		}

	private:
		std::unique_ptr<impl::HolderBase> m_pValue;
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