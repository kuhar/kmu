/*
* variant.h
*
*  Created on: Mar 30, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <cstddef>
#include <cassert>

#include "handy.h"

namespace kmu
{

	namespace impl
	{
		template<typename First, typename... Rest>
		struct maxSizeof
		{
			enum : size_t
			{
				value = sizeof( First ) > maxSizeof<Rest...>::value
						? sizeof( First ) : maxSizeof<Rest...>::value
			};
			
		};

		template<typename First>
		struct maxSizeof<First>
		{
			enum : size_t
			{
				value = sizeof( First )
			};
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
		static_assert( kmu::are_all_unique<Ts...>::value, "Each type must be unique" );

	public:

		using uninitializedType = impl::Uninitialized;

		Variant()
			: m_currentTypeID( typeid( uninitializedType ) )
		{
		}

		~Variant()
		{
			Destroyer<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
		}

		template<typename Type>
		void set()
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );

			Destroyer<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );
			new ( &m_storage ) Type();
		}

		template<typename Type, typename... Args>
		void set( Args&&... params )
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			Destroyer<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );
			new ( &m_storage ) Type( std::forward<Args>( params )... );
		}

		template<typename Type, typename... Args>
		void set( const Args&... params )
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			Destroyer<StorageType, Ts...>::destroy( m_storage, m_currentTypeID );
			m_currentTypeID = typeid( Type );
			new ( &m_storage ) Type( params... );
		}

		template<typename Type>
		Type& get()
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			assert( m_currentTypeID == typeid( Type ) && "Type mismatch" );
			return *reinterpret_cast<Type*>( &m_storage );
		}

		template<typename Type>
		const Type& get() const
		{
			static_assert( kmu::is_one_of<Type, Ts...>::value,
						   "Given type is not a subtype of this variant" );
			
			assert( m_currentTypeID == typeid( Type ) && "Type mismatch" );
			return *reinterpret_cast<Type*>( &m_storage );
		}

		std::type_index getCurrentTypeID() const
		{
			return m_currentTypeID;
		}

	private:		
		using StorageType = typename 
			std::aligned_storage<impl::maxSizeof<Ts...>::value>::type;
		
		StorageType m_storage;
		std::type_index m_currentTypeID;

		template<typename StorageType, typename...>
		struct Destroyer;

		template<typename StorageType, typename First, typename... Rest>
		struct Destroyer<StorageType, First, Rest...>
		{
			static void destroy ( StorageType& storage, std::type_index& currentTypeID )
			{
				if( currentTypeID == typeid( First ) )
				{
					reinterpret_cast<First*>( &storage )->~First();
					currentTypeID = typeid( impl::Uninitialized );
					return;
				}
				Destroyer<StorageType, Rest...>::destroy( storage, currentTypeID );
			}
		};

		template<typename StorageType>
		struct Destroyer<StorageType>
		{
			static void destroy ( StorageType&, std::type_index& currentTypeID )
			{
				assert( currentTypeID == typeid( impl::Uninitialized ) && "Type mismatch" );
			}
		};

	};
}
