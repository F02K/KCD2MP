//////////////////////////////////////////////////////////////////////
//
//	STL Utils header
//	
//	File: STLutils.h
//	Description : Various convenience utility functions for STL and alike
//  Used in Animation subsystem, and in some tools
//
//	History:
//	-:Created by Sergiy Migdalskiy
//
//////////////////////////////////////////////////////////////////////
#ifndef _STL_UTILS_HEADER_
#define _STL_UTILS_HEADER_

#define USE_HASH_MAP

#if (_MSC_VER >= 1400) && !defined(_STLP_BEGIN_NAMESPACE) // Visual Studio 2005 without STLPort
    #include <unordered_map>
    #include <unordered_set>
  #if _MSC_VER < 1930 // <hash_map> removed in VS 2022 (MSVC 14.30+)
    #define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    #include <hash_map>
    #undef std__hash_map
    #define std__hash_map stdext::hash_map
    #define std__hash_compare stdext::hash_compare
    #define std__hash_multimap stdext::hash_multimap
  #else
    #define std__hash_map std::unordered_map
    #define std__hash_compare std::hash<Key>
    #define std__hash_multimap std::unordered_multimap
  #endif
#elif (defined(LINUX) || defined(APPLE)) && !defined(USING_STLPORT)
    #include "platform.h"
    // Uncomment when compiling with clang and c++11 support
    #if defined(__clang__) && (__clang_major__ >= 5)
        #include <unordered_map>
        #include <unordered_set>
        #define std__hash_map std::unordered_map
        #define std__hash_compare std::hash_compare
        #define std__hash_multimap std::unordered_multimap
        #define std__unordered_map std::unordered_map
        #define std__unordered_set std::unordered_set
    #elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)
        #include <tr1/unordered_map>
        #include <tr1/unordered_set>
        #define std__hash_map std::tr1::unordered_map
        #define std__hash_compare std::tr1::hash_compare
		#define std__hash_multimap std::tr1::unordered_multimap
    #else
        #include <ext/hash_map>
        #define std__hash_map __gnu_cxx::hash_map
        #define std__hash_compare __gnu_cxx::hash_compare
		#define std__hash_multimap __gnu_cxx::hash_multimap
    #endif
#else
    #include <hash_map>
	#include <unordered_map>
	#include <unordered_set>
#endif

#ifndef std__hash_map
#define std__hash_map std::hash_map
#define std__hash_compare std::hash_compare
#endif

#ifndef std__hash_multimap
#define std__hash_multimap std::hash_multimap
#endif

#ifndef std__unordered_set
#define std__unordered_set std::tr1::unordered_set
#endif

#ifndef std__unordered_map
#define std__unordered_map std::tr1::unordered_map
#endif
// Location of std::hash
#if defined(APPLE) && (__clang_major__ >= 5)
    #define std__hash std::hash
#elif defined(LINUX) || defined(APPLE) 
    #define std__hash std::tr1::hash
#else
    #define std__hash std::hash
#endif

#include "CompileTimeUtils.h"
#include "STLGlobalAllocator.h"

#include <map>
#include <set>
#include <algorithm>
#include <deque>

/*
{
	typename Map::const_iterator it = mapKeyToValue.find (key);
	if (it == mapKeyToValue.end())
		return valueDefault;
	else
		return it->second;
}

// searches the given entry in the map by key, and if there is none, returns the default value
// The values are taken/returned in REFERENCEs rather than values
template <typename Map>
inline typename Map::mapped_type& find_in_map_ref(Map& mapKeyToValue, typename Map::key_type key, typename Map::mapped_type& valueDefault)
{
	typename Map::iterator it = mapKeyToValue.find (key);
	if (it == mapKeyToValue.end())
		return valueDefault;
	else
		return it->second;
}
*/

// auto-cleaner: upon destruction, calls the clear() method
template <class T>
class CAutoClear
{
public:
	CAutoClear (T* p): m_p(p) {}
	~CAutoClear () {m_p->clear();}
protected:
	T* m_p;
};


template <class Container>
unsigned sizeofArray (const Container& arr)
{
	return (unsigned)(sizeof(typename Container::value_type)*arr.size());
}

template <class Container>
unsigned sizeofVector (const Container& arr)
{
	return (unsigned)(sizeof(typename Container::value_type)*arr.capacity());
}

template <class Container>
unsigned sizeofArray (const Container& arr, unsigned nSize)
{
	return arr.empty()?0u:(unsigned)(sizeof(typename Container::value_type)*nSize);
}

template <class Container>
unsigned capacityofArray (const Container& arr)
{
	return (unsigned)(arr.capacity()*sizeof(arr[0]));
}

template <class T>
unsigned countElements (const std::vector<T>& arrT, const T& x)
{
	unsigned nSum = 0;
	for (typename std::vector<T>::const_iterator iter = arrT.begin(); iter != arrT.end(); ++iter)
		if (x == *iter)
			++nSum;
	return nSum;
}

// [Timur]
/** Contain extensions for STL library.
*/
namespace stl
{
#if !defined(__CRYCG__)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compare member of class/struct.
	//
	// e.g. Sort Vec3s by x component
	//
	//           std::sort(vec3s.begin(), vec3s.end(), stl::member_compare<Vec3, float, &Vec3::x>());
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename OWNER_TYPE, typename MEMBER_TYPE, MEMBER_TYPE OWNER_TYPE::*MEMBER_PTR, typename EQUALITY = std::less<MEMBER_TYPE> > struct member_compare
	{
		inline bool operator () (const OWNER_TYPE &lhs, const OWNER_TYPE &rhs) const
		{
			return EQUALITY()(lhs.*MEMBER_PTR, rhs.*MEMBER_PTR);
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compare member of class/struct against parameter.
	//
	// e.g. Find Vec3 with x component less than 1.0
	//
	//           std::find_if(vec3s.begin(), vec3s.end(), stl::member_compare_param<Vec3, float, &Vec3::x>(1.0f));
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename OWNER_TYPE, typename MEMBER_TYPE, MEMBER_TYPE OWNER_TYPE::*MEMBER_PTR, typename EQUALITY = std::less<MEMBER_TYPE> > struct member_compare_param
	{
		inline member_compare_param(const MEMBER_TYPE &_value)
			: value(_value)
		{
		}

		inline bool operator () (const OWNER_TYPE &rhs) const
		{
			return EQUALITY()(rhs.*MEMBER_PTR, value);
		}

		const MEMBER_TYPE	&value;
	};
#endif //!defined(__CRYCG__)	

	//////////////////////////////////////////////////////////////////////////
	//! Searches the given entry in the map by key, and if there is none, returns the default value
	//////////////////////////////////////////////////////////////////////////
#if defined(USING_STLPORT)
	template <typename Map, typename Key>
	inline typename Map::mapped_type find_in_map(const Map& mapKeyToValue, const Key& key, typename Map::mapped_type valueDefault)
	{
		typename Map::const_iterator it = mapKeyToValue.find (key);
		if (it == mapKeyToValue.end())
			return valueDefault;
		else
			return it->second;
	}
#else
	template <typename Map>
	inline typename Map::mapped_type find_in_map(const Map& mapKeyToValue, const typename Map::key_type& key, typename Map::mapped_type valueDefault)
	{
		typename Map::const_iterator it = mapKeyToValue.find (key);
		if (it == mapKeyToValue.end())
			return valueDefault;
		else
			return it->second;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	//! Inserts and returns a reference to the given value in the map, or returns the current one if it's already there.
	//////////////////////////////////////////////////////////////////////////
	template <typename Map>
	inline typename Map::mapped_type& map_insert_or_get(Map& mapKeyToValue, const typename Map::key_type& key, const typename Map::mapped_type& defValue = typename Map::mapped_type())
	{
		std::pair<typename Map::iterator, bool> iresult = mapKeyToValue.insert(typename Map::value_type(key, defValue));
		return iresult.first->second;
	}

	// searches the given entry in the map by key, and if there is none, returns the default value
	// The values are taken/returned in REFERENCEs rather than values
	template <typename Key, typename mapped_type, typename Traits, typename Allocator>
	inline mapped_type& find_in_map_ref(std::map<Key, mapped_type, Traits, Allocator>& mapKeyToValue, const Key& key, mapped_type& valueDefault)
	{
		typedef std::map<Key, mapped_type, Traits, Allocator> Map;
		typename Map::iterator it = mapKeyToValue.find (key);
		if (it == mapKeyToValue.end())
			return valueDefault;
		else
			return it->second;
	}

	template <typename Key, typename mapped_type, typename Traits, typename Allocator>
	inline const mapped_type& find_in_map_ref(const std::map<Key, mapped_type, Traits, Allocator>& mapKeyToValue, const Key& key, const mapped_type& valueDefault)
	{
		typedef std::map<Key, mapped_type, Traits, Allocator> Map;
		typename Map::const_iterator it = mapKeyToValue.find (key);
		if (it == mapKeyToValue.end())
			return valueDefault;
		else
			return it->second;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Fills vector with contents of map.
	//////////////////////////////////////////////////////////////////////////
	template <class Map,class Vector>
	inline void map_to_vector( const Map& theMap,Vector &array )
	{
		array.resize(0);
		array.reserve( theMap.size() );
		for (typename Map::const_iterator it = theMap.begin(); it != theMap.end(); ++it)
		{
			array.push_back( it->second );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//! Fills vector with contents of set.
	//////////////////////////////////////////////////////////////////////////
	template <class Set,class Vector>
	inline void set_to_vector( const Set& theSet,Vector &array )
	{
		array.resize(0);
		array.reserve( theSet.size() );
		for (typename Set::const_iterator it = theSet.begin(); it != theSet.end(); ++it)
		{
			array.push_back( *it );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find and erase element from container.
	// @return true if item was find and erased, false if item not found.
	//////////////////////////////////////////////////////////////////////////
	template <class Container,class Value>
	inline bool find_and_erase( Container& container,const Value &value )
	{
		typename Container::iterator it = std::find( container.begin(),container.end(),value );
		if (it != container.end())
		{
			container.erase( it );
			return true;
		}
		return false;
	}

	template <typename K, typename P, typename A>
	inline bool find_and_erase( std::set<K, P, A>& container,const K &value )
	{
		return container.erase(value) > 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find and erase element from container.
	// @return true if item was find and erased, false if item not found.
	//////////////////////////////////////////////////////////////////////////
	template <class CONTAINER, class PREDICATE> inline bool find_and_erase_if(CONTAINER &container, const PREDICATE &predicate)
	{
		typename CONTAINER::iterator end = container.end(), i = std::find_if(container.begin(), end, predicate);

		if(i != end)
		{
			container.erase(i);

			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find and erase all elements matching value from container.
	// Assume that this will invalidate any exiting iterators.
	// Commonly used for removing NULL pointers from collections.
	//////////////////////////////////////////////////////////////////////////
	template <class Container>
	inline void find_and_erase_all(Container& container, const typename Container::value_type&value)
	{
		// Shuffles all elements != value to the front and returns the start of the removed elements.
		typename Container::iterator endIter(container.end());
		typename Container::iterator newEndIter(std::remove(container.begin(), endIter, value));

		// Delete the removed range at the back of the container (low-cost for vector).
		container.erase(newEndIter, endIter);
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find and erase element from map.
	// @return true if item was find and erased, false if item not found.
	//////////////////////////////////////////////////////////////////////////
	template <class Container,class Key>
	inline bool member_find_and_erase( Container& container,const Key &key )
	{
		typename Container::iterator it = container.find (key);
		if (it != container.end())
		{
			container.erase( it );
			return true;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	//! Push back to container unique element.
	// @return true if item added, false overwise.
	template <class Container,class Value>
		inline bool push_back_unique( Container& container,const Value &value )
	{
		if (std::find(container.begin(),container.end(),value) == container.end())
		{
			container.push_back( value );
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Push back to container unique element.
	// @return true if item added, false overwise.
	template <class CONTAINER, class PREDICATE, typename VALUE> inline bool push_back_unique_if(CONTAINER &container, const PREDICATE &predicate, const VALUE &value)
	{
		typename CONTAINER::iterator	end = container.end();

		if(std::find_if(container.begin(), end, predicate) == end)
		{
			container.push_back(value);

			return true;
		}
		else
		{
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//! Push back to container contents of another container
	template <class Container,class Iter>
	inline void push_back_range( Container& container,Iter begin,Iter end )
	{
		for (Iter it = begin; it != end; ++it)
		{
			container.push_back(*it);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//! Push back to container contents of another container, if not already present
	template <class Container,class Iter>
	inline void push_back_range_unique( Container& container,Iter begin,Iter end )
	{
		for (Iter it = begin; it != end; ++it)
		{
			push_back_unique(container, *it);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find element in container.
	// @return true if item found.
	template <class Container,class Value>
		inline bool find( Container& container,const Value &value )
	{
		return std::find(container.begin(),container.end(),value) != container.end();
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find element in a sorted container using binary search with logarithmic efficiency.
	//
	template <class Iterator,class T>
		inline Iterator binary_find(Iterator first,Iterator last,const T& value)
	{
		Iterator it = std::lower_bound(first,last,value);
		return (it == last || value != *it) ? last : it;
	}

	//////////////////////////////////////////////////////////////////////////
	//! Find element in a sorted container using binary search with logarithmic efficiency.
	// @return true if item was inserted.
	template <class Container,class Value>
		inline bool binary_insert_unique( Container& container,const Value &value )
	{
		typename Container::iterator it = std::lower_bound(container.begin(),container.end(),value);
		if (it != container.end())
		{
			if (*it == value)
				return false;
			container.insert( it,value );
		}
		else
			container.insert( container.end(),value );
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	//! Find element in a sorted container using binary search with logarithmic efficiency.
	// and erases if element found.
	// @return true if item was erased.
	template <class Container,class Value>
		inline bool binary_erase( Container& container,const Value &value )
	{
		typename Container::iterator it = std::lower_bound(container.begin(),container.end(),value);
		if (it != container.end() && *it == value)
		{
			container.erase(it);
			return true;
		}
		return false;
	}

	template <typename ItT, typename Func>
		ItT remove_from_heap(ItT begin, ItT end, ItT at, Func order)
	{
		using std::swap;

		-- end;
		if (at == end)
			return at;

		size_t idx = std::distance(begin, at);
		swap(*end, *at);

		size_t length = std::distance(begin, end);
		size_t parent, child;

		if (idx > 0 && order(*(begin + idx / 2), *(begin + idx)))
		{
			do 
			{
				parent = idx / 2;
				swap(*(begin + idx), *(begin + parent));
				idx = parent;

				if (idx == 0 || order(*(begin + idx), *(begin + idx / 2)))
					return end;
			}
			while (true);
		}
		else
		{
			do
			{
				child = idx * 2 + 1;
				if (child >= length)
					return end;

				ItT left = begin + child;
				ItT right = begin + child + 1;

				if (right < end && order(*left, *right))
					++ child;

				if (order(*(begin + child), *(begin + idx)))
					return end;

				swap(*(begin + child), *(begin + idx));
				idx = child;
			}
			while (true);
		}

		return end;
	}

	struct container_object_deleter
	{
		template<typename T>
			void operator()(const T* ptr) const
		{
			delete ptr;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//! Convert arbitary class to const char*
	//////////////////////////////////////////////////////////////////////////
	template <class Type>
		inline const char* constchar_cast( const Type &type )
	{
		return type;
	}

	//! Specialization of string to const char cast.
	template <>
		inline const char* constchar_cast( const string &type )
	{
		return type.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	//! Case sensetive less key for any type convertable to const char*.
	//////////////////////////////////////////////////////////////////////////
	template <class Type>
	struct less_strcmp  
	{
		bool operator()( const Type &left,const Type &right ) const
		{
			return strcmp(constchar_cast(left),constchar_cast(right)) < 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//! Case insensetive less key for any type convertable to const char*.
	template <class Type>
	struct less_stricmp  
	{
		bool operator()( const Type &left,const Type &right ) const
		{
			return stricmp(constchar_cast(left),constchar_cast(right)) < 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// Hash map usage:
	// typedef stl::hash_map<string,int, stl::hash_stricmp<string> > StringToIntHash;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// useful when the key is already the result of an hash function
	// key needs to be convertible to size_t
	//////////////////////////////////////////////////////////////////////////
	template <class Key>
	class hash_simple
	{
	public:
		enum {	// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8
		};// min_buckets = 2 ^^ N, 0 < N

		size_t operator()( const Key& key ) const
		{
			return size_t(key);
		};
		bool operator()( const Key& key1,const Key& key2 ) const
		{
			return key1 < key2;
		}
	};

	// simple hash class that has the avalanche property (a change in one bit affects all others)
	// ... use this if you have uint32 key values!
	class hash_uint32
	{
	public:
		enum {	// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8  // min_buckets = 2 ^^ N, 0 < N
		};

		ILINE size_t operator()( uint32 a ) const
		{
			a = (a+0x7ed55d16) + (a<<12);
			a = (a^0xc761c23c) ^ (a>>19);
			a = (a+0x165667b1) + (a<<5);
			a = (a+0xd3a2646c) ^ (a<<9);
			a = (a+0xfd7046c5) + (a<<3);
			a = (a^0xb55a4f09) ^ (a>>16);
			return a;
		};
		bool operator()( uint32 key1,uint32 key2 ) const
		{
			return key1 < key2;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//! Case sensitive string hash map compare structure.
	//////////////////////////////////////////////////////////////////////////
	template <class Key>
	class hash_strcmp
	{
	public:
		enum {	// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8
		};// min_buckets = 2 ^^ N, 0 < N

		size_t operator()( const Key& key ) const
		{
			unsigned int h = 0; 
			const char *s = constchar_cast(key);
			for (; *s; ++s) h = 5*h + *(unsigned char*)s;
			return size_t(h);

		};
		bool operator()( const Key& key1,const Key& key2 ) const
		{
			return strcmp(constchar_cast(key1),constchar_cast(key2)) < 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//! Case insensitive string hash map compare structure.
	template <class Key>
	class hash_stricmp
	{
	public:
		enum {	// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8
		};// min_buckets = 2 ^^ N, 0 < N

		size_t operator()( const Key& key ) const
		{
			unsigned int h = 0; 
			const char *s = constchar_cast(key);
			for (; *s; ++s) h = 5*h + tolower(*(unsigned char*)s);
			return size_t(h);

		};
		bool operator()( const Key& key1,const Key& key2 ) const
		{
			return stricmp(constchar_cast(key1),constchar_cast(key2)) < 0;
		}
	};

#if defined(USE_HASH_MAP) && !defined(JOB_LIB_COMP)
	//C:\Work\Main\Code\SDKs\STLPORT\stlport

	// Support for both Microsoft and SGI kind of hash_map.

#if defined(_STLP_HASH_MAP) || defined(APPLE) || defined(LINUX)
 	// STL Port
	template <class _Key,class _Predicate=std::less<_Key> >
	struct hash_compare
	{
		enum
		{	// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8   // min_buckets = 2 ^^ N, 0 < N
		};

		size_t operator()(const _Key& _Keyval) const
		{
			// return hash value.
			uint32 a = _Keyval;
			a = (a+0x7ed55d16) + (a<<12);
			a = (a^0xc761c23c) ^ (a>>19);
			a = (a+0x165667b1) + (a<<5);
			a = (a+0xd3a2646c) ^ (a<<9);
			a = (a+0xfd7046c5) + (a<<3);
			a = (a^0xb55a4f09) ^ (a>>16);
			return a;
		}

		// Less then function.
		bool operator()(const _Key& _Keyval1, const _Key& _Keyval2) const
		{	// test if _Keyval1 ordered before _Keyval2
			_Predicate comp;
			return (comp(_Keyval1, _Keyval2));
		}
	};

	template <class Key,class HashFunc>
	struct stlport_hash_equal
	{
		// Equal function.
		bool operator()(const Key& k1, const Key& k2) const
		{
			HashFunc less;
			// !(k1 < k2) && !(k2 < k1)
			return !less(k1,k2) && !less(k2,k1);
		}
	};

	template <class Key,class Value,class HashFunc = hash_compare<Key>, class Alloc = std::allocator< std::pair<Key,Value> > >
	struct hash_map : public std__hash_map<Key,Value,HashFunc,stlport_hash_equal<Key,HashFunc>,Alloc>
	{
		hash_map() : std__hash_map<Key,Value,HashFunc,stlport_hash_equal<Key,HashFunc>,Alloc>( HashFunc::min_buckets ) {}
	};

	template <class Key,class Value,class HashFunc = hash_compare<Key>, class Alloc = std::allocator< std::pair<Key,Value> > >
	struct hash_multimap : public std__hash_multimap<Key,Value,HashFunc,stlport_hash_equal<Key,HashFunc>,Alloc>
	{
		hash_multimap() : std__hash_multimap<Key,Value,HashFunc,stlport_hash_equal<Key,HashFunc>,Alloc>( HashFunc::min_buckets ) {}
	};

#elif defined(ORBIS)
	using std::hash_compare;
	using std::hash_map;
#else	// !defined(ORBIS)

#if _MSC_VER < 1930  // stdext::hash_compare removed in VS 2022+
}	// End STL NS (temporarily for hash_compare partial specialization)

// hash_compare is a MS extension and exists in the stdext NS
namespace stdext {

	// Partial template specialisation of hash_compare for std::pair.
	template <typename FIRST, typename SECOND, class COMPARE> struct hash_compare<std::pair<FIRST, SECOND>, COMPARE>
	{
		enum { bucket_size = 4, min_buckets = 8 };

		inline size_t operator () (const std::pair<FIRST, SECOND> &rhs) const
		{
			return hash_compare<FIRST>()(rhs.first) ^ hash_compare<SECOND>()(rhs.second);
		}

		inline bool operator() (const std::pair<FIRST, SECOND> &lhs, const std::pair<FIRST, SECOND> &rhs) const
		{
			return COMPARE()(lhs, rhs);
		}
	};

}	// End stdext NS

namespace stl {

	// MS STL
	using stdext::hash_compare;
	using stdext::hash_map;
	using stdext::hash_multimap;
#else  // VS 2022+: use unordered_map instead of deprecated hash_map
	using hash_map = std::unordered_map;
#endif

#endif // !defined(ORBIS)

#else // USE_HASH_MAP
		
#endif //USE_HASH_MAP




	//////////////////////////////////////////////////////////////////////////
	template<class T>
	class intrusive_linked_list_node
	{
	public:
		intrusive_linked_list_node()  { link_to_intrusive_list(static_cast<T*>(this)); }
		// Not virtual by design
		~intrusive_linked_list_node() { unlink_from_intrusive_list(static_cast<T*>(this)); }

		static T* get_intrusive_list_root() { return m_root_intrusive; };

		static void link_to_intrusive_list( T *pNode )
		{
			if (m_root_intrusive)
			{
				// Add to the beginning of the list.
				T *head = m_root_intrusive;
				pNode->m_prev_intrusive = 0;
				pNode->m_next_intrusive = head;
				head->m_prev_intrusive = pNode;
				m_root_intrusive = pNode;
			}
			else
			{
				m_root_intrusive = pNode;
				pNode->m_prev_intrusive = 0;
				pNode->m_next_intrusive = 0;
			}
		}
		static void unlink_from_intrusive_list( T *pNode )
		{
			if (pNode == m_root_intrusive) // if head of list.
			{
				m_root_intrusive = pNode->m_next_intrusive;
				if (m_root_intrusive)
				{
					m_root_intrusive->m_prev_intrusive = 0;
				}
			}
			else
			{
				if (pNode->m_prev_intrusive)
				{
					pNode->m_prev_intrusive->m_next_intrusive = pNode->m_next_intrusive;
				}
				if (pNode->m_next_intrusive)
				{
					pNode->m_next_intrusive->m_prev_intrusive = pNode->m_prev_intrusive;
				}
			}
			pNode->m_next_intrusive = 0;
			pNode->m_prev_intrusive = 0;
		}

	public:
		static T* m_root_intrusive;
		T *m_next_intrusive;
		T *m_prev_intrusive;
	};

	template <class T>
	inline void reconstruct(T &t)
	{
		t.~T();
		new(&t) T;
	}

	template <typename T, typename A1>
	inline void reconstruct(T& t, const A1& a1)
	{
		t.~T();
		new (&t) T(a1);
	}

	template <typename T, typename A1, typename A2>
	inline void reconstruct(T& t, const A1& a1, const A2& a2)
	{
		t.~T();
		new (&t) T(a1, a2);
	}

	template <typename T, typename A1, typename A2, typename A3>
	inline void reconstruct(T& t, const A1& a1, const A2& a2, const A3& a3)
	{
		t.~T();
		new (&t) T(a1, a2, a3);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4>
	inline void reconstruct(T& t, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		t.~T();
		new (&t) T(a1, a2, a3, a4);
	}

	template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
	inline void reconstruct(T& t, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
	{
		t.~T();
		new (&t) T(a1, a2, a3, a4, a5);
	}

	template <class T>
	inline void free_container(T &t)
	{
		reconstruct(t);
	}

	template <class T, class A>
	inline void free_container( std::deque<T,A> &t)
	{
		ScopedSwitchToGlobalHeap GlobalHeap;
		reconstruct(t);
	}

	template <class K,class D, class H, class A>
	inline void free_container( std__hash_map<K,D,H,A> &t)
	{
		ScopedSwitchToGlobalHeap GlobalHeap;
		reconstruct(t);
	}

	struct container_freer
	{
		template <typename T> void operator () (T& container) const
		{
			stl::free_container(container);
		}
	};

	template <typename T>
	struct scoped_set
	{
		scoped_set(T& ref, T val)
			: m_ref(&ref)
			, m_oldVal(ref)
		{
			ref = val;
		}

		~scoped_set()
		{
			(*m_ref) = m_oldVal;
		}

	private:
		scoped_set(const scoped_set<T>& other);
		scoped_set<T>& operator = (const scoped_set<T>& other);

	private:
		T* m_ref;
		T m_oldVal;
	};

	template <typename T, size_t Length, typename Func>
	inline void for_each_array(T (&buffer)[Length], Func func)
	{
		std::for_each(&buffer[0], &buffer[Length], func);
	}

	template <typename T>
	inline void destruct(T* p)
	{
		p->~T();
	}
}

#define DEFINE_INTRUSIVE_LINKED_LIST( Class ) \
	template<> Class* stl::intrusive_linked_list_node<Class>::m_root_intrusive = 0;

#endif
