/*
* variant.h
*
*  Created on: Mar 30, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include <type_traits>
#include <typeindex>
#include <memory>
#include <functional>
#include <cstddef>
#include <cassert>

#include "handy.h"
#include "type_list.h"
#include "uninitialized.h"

#ifdef _MSC_VER
#define KMU_THIS_TEMPLATED_METHOD
#else
#define KMU_THIS_TEMPLATED_METHOD this->template
#endif

namespace kmu
{
	namespace impl
	{
		template<typename T>
		struct wrap_reference 
			: identity<if_t<std::is_lvalue_reference<T>::value,
						typename std::reference_wrapper<
							typename std::remove_reference<T>::type>,
						T>
			>
		{
			static_assert(!std::is_rvalue_reference<T>::value,
						   "Usage of rvalue references as Variant subtype is not allowed");
		};

		template<typename T>
		using wrap_reference_t = type_t<wrap_reference<T>>;
		
		template<typename First, typename... Rest>
		struct maxSizeof
		{
			using FirstType = wrap_reference_t<First>;

			static const size_t value = sizeof(FirstType) > maxSizeof<Rest...>::value
						? sizeof(FirstType) : maxSizeof<Rest...>::value;
		};

		template<typename First>
		struct maxSizeof<First>
		{
			static const size_t value = sizeof(wrap_reference_t<First>);
		};

		template<typename First, typename... Rest>
		struct maxAlignof
		{
			using FirstType = wrap_reference_t<First>;

			static const size_t value = std::alignment_of<FirstType>::value 
										> maxAlignof<Rest...>::value
				? std::alignment_of<FirstType>::value : maxAlignof<Rest...>::value;
		};

		template<typename First>
		struct maxAlignof<First>
		{
			static const size_t value = std::alignment_of<typename wrap_reference<First>::type>::value;
		};

	} // namespace impl
	
	template<typename... Ts>
	class Variant
	{
		static_assert(kmu::are_all_unique<Ts...>::value, "Each and every type must be unique");
		static const unsigned char UninitializedIndex = Constants::UChar_Max;
		static_assert(sizeof... (Ts) < static_cast<size_t>(UninitializedIndex), 
						"Given count of types exceeds the limit");

	public:
		using UninitializedType = Uninitialized;

		Variant()
			: m_currentIndexOfType(UninitializedIndex)
		{
		}

		~Variant()
		{
			VisitorHelper<Ts...>::destroy(m_storage, m_currentIndexOfType);
		}

		Variant(const Variant<Ts...>& other)
			: m_currentIndexOfType(UninitializedIndex)
		{
			VisitorHelper<Ts...>::initialize(*this, other);
		}

		Variant<Ts...>& operator=(const Variant<Ts...>& other)
		{
			VisitorHelper<Ts...>::initialize(*this, other);
			return *this;
		}

		Variant( Variant<Ts...>&& other )
			: m_currentIndexOfType( UninitializedIndex )
		{
			VisitorHelper<Ts...>::initialize(*this, std::move(other));
		}

		Variant<Ts...>& operator=(Variant<Ts...>&& other)
		{
			VisitorHelper<Ts...>::initialize(*this, std::move(other));
			return *this;
		}

		template<typename Type, typename... Args>
		void set(Args&&... params)
		{
			static_assert(kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant");
			
			VisitorHelper<Ts...>::destroy(m_storage, m_currentIndexOfType);
			m_currentIndexOfType = kmu::get_index_of_type<Type, Ts...>::value;

			using wrappedType = impl::wrap_reference_t<Type>;
			new (std::addressof(m_storage)) wrappedType(std::forward<Args>(params)...);
		}

		template<typename Type, size_t Index = kmu::get_index_of_type<Type, Ts...>::value>
		Type& get()
		{			
			assert(m_currentIndexOfType == Index && "Type mismatch");

			using wrappedType = impl::wrap_reference_t<Type>;
			return *reinterpret_cast<wrappedType*>(std::addressof(m_storage));
		}

		template<typename Type>
		const Type& get() const
		{
			assert((getCurrentTypeIndex() == typeid(Type)) && "Type mismatch");

			using wrappedType = impl::wrap_reference_t<Type>;
			return *reinterpret_cast<typename std::add_const<wrappedType>::type*>(
													std::addressof(m_storage));
		}

		template<size_t Index, 
				typename Type = kmu::get_type_at_t<Index, Ts...>>
		auto get() -> decltype(KMU_THIS_TEMPLATED_METHOD get<Type>())
		{
			return get<Type>();
		}

		template<size_t Index,
				typename Type = kmu::get_type_at_t<Index, Ts...>>
		auto get() const -> decltype(KMU_THIS_TEMPLATED_METHOD get<Type>())
		{
			return get<Type>();
		}

		void reset()
		{
			VisitorHelper<Ts...>::destroy(m_storage, m_currentIndexOfType);
			m_currentIndexOfType = UninitializedIndex;
		}

		std::type_index getCurrentTypeIndex() const
		{
			assert((m_currentIndexOfType < sizeof... (Ts) 
					|| m_currentIndexOfType == UninitializedIndex) && "Type mismatch");

			if (m_currentIndexOfType == UninitializedIndex )
			{
				return typeid(UninitializedType);
			}

			return kmu::getTypeIndexOfTypeAt<Ts...>(m_currentIndexOfType);
		}

	private:		
		using StorageType = typename std::aligned_storage<impl::maxSizeof<Ts...>::value, 
															impl::maxAlignof<Ts...>::value>::type;
		
		StorageType m_storage;
		unsigned char m_currentIndexOfType;

		template<typename...>
		struct Visitor;

		using PlaceholderType = void; // To avoid explicit specialization of Visitor in this class
		template<typename... Types>
		using VisitorHelper = Visitor<PlaceholderType, Types...>;

		template<typename Placeholder, typename First, typename... Rest>
		struct Visitor<Placeholder, First, Rest...>
		{
			static void destroy(StorageType& storage, unsigned char& currentIndex)
			{
				if (currentIndex == kmu::get_index_of_type<First, Ts...>::value)
				{
					using wrappedType = impl::wrap_reference_t<First>;
					reinterpret_cast<wrappedType*> (std::addressof( storage))->~wrappedType();
					currentIndex = UninitializedIndex;
					
					return;
				}
				VisitorHelper<Rest...>::destroy(storage, currentIndex);
			}

			template<typename CurrentVariantType, typename OtherVariantType>
			static void initialize(CurrentVariantType& current, OtherVariantType&& other)
			{
				if (other.m_currentIndexOfType == kmu::get_index_of_type<First, Ts...>::value)
				{
					current.template set<First>( other.template get<First>() );
					return;
				}
				VisitorHelper<Rest...>::initialize(current, std::forward<OtherVariantType>(other));
			}
		};

		template<typename Placeholder>
		struct Visitor<Placeholder>
		{
			static void destroy(StorageType&, unsigned char& currentIndex)
			{
				assert((currentIndex == UninitializedIndex) && "Type mismatch");
			}
			
			template<typename CurrentVariantType, typename OtherVariantType>
			static void initialize(CurrentVariantType& current, OtherVariantType&& other)
			{
				assert((other.m_currentIndexOfType == UninitializedIndex) && "Type mismatch");
				current.reset();
			}
		};
	};

	template<typename Type, typename... Ts>
	inline auto get(kmu::Variant<Ts...>& variant)
		-> decltype(variant.template get<Type>())
	{
		return variant.template get<Type>();
	}

	template<typename Type, typename... Ts>
	inline auto get(const kmu::Variant<Ts...>& variant)
		-> decltype(variant.template get<Type>())
	{
		return variant.template get<Type>();
	}

	template<size_t Index, typename... Ts>
	inline auto get(kmu::Variant<Ts...>& variant)
		-> decltype(variant.template get<Index>())
	{
		return variant.template get<Index>();
	}

	template<size_t Index, typename... Ts>
	inline auto get(const kmu::Variant<Ts...>& variant)
		-> decltype(variant.template get<Index>())
	{
		return variant.template get<Index>();
	}
} // namespace kmu

#undef KMU_THIS_TEMPLATED_METHOD