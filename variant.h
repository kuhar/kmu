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
#include <tuple>
#include <cstddef>
#include <cassert>

#include "handy.h"

namespace kmu
{
	namespace impl
	{
		template<typename T>
		struct wrap_reference
		{
			static_assert( !std::is_rvalue_reference<T>::value,
						   "Usage of rvalue references as Variant subtype is not allowed" );

			using type = typename std::conditional<std::is_lvalue_reference<T>::value,
				typename std::reference_wrapper<typename std::remove_reference<T>::type>, T>::type;
		};

		template<typename T>
		using wrap_reference_t = typename wrap_reference<T>::type;
		
		template<typename First, typename... Rest>
		struct maxSizeof
		{
			using FirstType = typename wrap_reference<First>::type;

			static const size_t value = sizeof( FirstType ) > maxSizeof<Rest...>::value
						? sizeof( FirstType ) : maxSizeof<Rest...>::value;
		};

		template<typename First>
		struct maxSizeof<First>
		{
			static const size_t value = sizeof( typename wrap_reference<First>::type );
		};
		
		struct Uninitialized
		{
			Uninitialized() = delete;
			~Uninitialized() = delete;
			Uninitialized( const Uninitialized& ) = delete;
			Uninitialized( Uninitialized&& ) = delete;
			Uninitialized& operator= ( const Uninitialized& ) = delete;
			Uninitialized& operator= ( Uninitialized&& ) = delete;
		};
	} // namespace impl
	
	template<typename... Ts>
	class Variant
	{
		static_assert( kmu::are_all_unique<Ts...>::value, "Each and every type must be unique" );

	public:
		using UninitializedType = impl::Uninitialized;

		Variant()
			: m_currentTypeID( typeid( UninitializedType ) )
		{
		}

		~Variant()
		{
			Visitor<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
		}

		Variant( const Variant<Ts...>& other )
			: m_currentTypeID( typeid( UninitializedType ) )
		{
			Visitor<StorageType, Ts...>::copyInitialize( *this, other );
		}

		Variant<Ts...>& operator= ( const Variant<Ts...>& other )
		{
			Visitor<StorageType, Ts...>::copyInitialize( *this, other );
			return *this;
		}

		Variant( Variant<Ts...>&& other )
			: m_currentTypeID( typeid( UninitializedType ) )
		{
			Visitor<StorageType, Ts...>::moveInitialize( *this, std::move( other ) );
		}

		Variant<Ts...>& operator= ( Variant<Ts...>&& other )
		{
			Visitor<StorageType, Ts...>::moveInitialize( *this, std::move( other ) );
			return *this;
		}

		template<typename Type>
		void set()
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );

			Visitor<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );

			using wrappedType = typename impl::wrap_reference_t<Type>;
			new ( std::addressof( m_storage ) ) wrappedType();
		}

		template<typename Type, typename... Args>
		void set( Args&&... params )
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			Visitor<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );

			using wrappedType = typename impl::wrap_reference_t<Type>;
			new ( std::addressof( m_storage ) ) wrappedType( std::forward<Args>( params )... );
		}

		template<typename Type, typename... Args>
		void set( const Args&... params )
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			Visitor<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );

			using wrappedType = typename impl::wrap_reference_t<Type>;
			new ( std::addressof( m_storage ) ) wrappedType( params... );
		}

		template<typename Type>
		Type& get()
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			assert( m_currentTypeID == typeid( Type ) && "Type mismatch" );

			using wrappedType = typename impl::wrap_reference_t<Type>;

			return *reinterpret_cast<wrappedType*>( std::addressof( m_storage ) );
		}

		template<typename Type>
		const Type& get() const
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			assert( m_currentTypeID == typeid( Type ) && "Type mismatch" );
			
			using wrappedType = typename impl::wrap_reference_t<Type>;

			return *reinterpret_cast<const wrappedType*>( std::addressof( m_storage ) );
		}

		template<size_t Index, 
				typename Type = typename std::tuple_element<Index, std::tuple<Ts...>>::type>
		auto get() -> decltype( get<Type>() )
		{
			return get<Type>();
		}

		template<size_t Index,
				typename Type = typename std::tuple_element<Index, std::tuple<Ts...>>::type>
		auto get() const -> decltype( get<Type>() )
		{
				return get<Type>();
		}

		void reset()
		{
			Visitor<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( UninitializedType );
		}

		std::type_index getCurrentTypeID() const
		{
			return m_currentTypeID;
		}

	private:		
		using StorageType = typename 
			std::aligned_storage<impl::maxSizeof<Ts...>::value, sizeof( std::type_index )>::type;
		
		StorageType m_storage;
		std::type_index m_currentTypeID;

		template<typename StorageType, typename...>
		struct Visitor;

		template<typename StorageType, typename First, typename... Rest>
		struct Visitor<StorageType, First, Rest...>
		{
			static void destroy ( StorageType& storage, std::type_index& currentTypeID )
			{
				if( currentTypeID == typeid( First ) )
				{
					using wrappedType = typename impl::wrap_reference_t<First>;
					reinterpret_cast<wrappedType*> ( std::addressof( storage ) )->~wrappedType();
					currentTypeID = typeid( typename Variant::UninitializedType );
					return;
				}
				Visitor<StorageType, Rest...>::destroy( storage, currentTypeID );
			}

			template<typename VariantType>
			static void copyInitialize( VariantType& current, const VariantType& other )
			{
				if( other.getCurrentTypeID() == typeid( First ) )
				{
					current.set<First>( other.template get<First>() );
					return;
				}
				Visitor<StorageType, Rest...>::copyInitialize( current, other );
			}

			template<typename VariantType>
			static void moveInitialize( VariantType& current, VariantType&& other )
			{
				if( other.getCurrentTypeID() == typeid( First ) )
				{
					current.set<First>( std::forward<First>( other.template get<First>() ) );
					return;
				}
				Visitor<StorageType, Rest...>::moveInitialize( current, std::move( other ) );
			}
		};

		template<typename StorageType>
		struct Visitor<StorageType>
		{
			static void destroy ( StorageType&, std::type_index& currentTypeID )
			{
				assert( currentTypeID == typeid( impl::Uninitialized ) 
						&& "Type mismatch" );
			}
			
			template<typename VariantType>
			static void copyInitialize( VariantType& current, const VariantType& other )
			{
				assert( other.getCurrentTypeID() == typeid ( typename VariantType::UninitializedType ) 
						&& "Type mismatch" );
				current.reset();
			}

			template<typename VariantType>
			static void moveInitialize( VariantType& current, VariantType&& other )
			{
				assert( other.getCurrentTypeID() == typeid ( VariantType::UninitializedType )
						&& "Type mismatch" );
				current.reset();
			}
		};
	};

	template<typename Type, typename... Ts>
	inline auto get( kmu::Variant<Ts...>& variant )
		-> decltype ( variant.template get<Type>() )
	{
		return variant.template get<Type>();
	}

	template<typename Type, typename... Ts>
	inline auto get( const kmu::Variant<Ts...>& variant )
		-> decltype ( variant.template get<Type>() )
	{
		return variant.template get<Type>();
	}

	template<size_t Index, typename... Ts>
	inline auto get( kmu::Variant<Ts...>& variant )
		-> decltype ( variant.template get<Index>() )
	{
		return variant.template get<Index>();
	}

	template<size_t Index, typename... Ts>
	inline auto get( const kmu::Variant<Ts...>& variant )
		-> decltype ( variant.template get<Index>() )
	{
		return variant.template get<Index>();
	}
} // namespace kmu
