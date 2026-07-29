#ifndef BUCKETALLOCATORPOLICY_H
#define BUCKETALLOCATORPOLICY_H

#ifdef WIN32
#include <windows.h>
#endif

#if defined(XENON) || defined(PS3)
#define BUCKET_ALLOCATOR_PACK_SMALL_SIZES
#define BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS 1
#elif defined(ORBIS)
#define MEMORY_ALLOCATION_ALIGNMENT	16
#define BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS 8
#elif defined(LINUX32)
	#define MEMORY_ALLOCATION_ALIGNMENT	8
	#define BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS 8
#elif defined(LINUX64) || defined(MAC)
	#define MEMORY_ALLOCATION_ALIGNMENT	16
	#define BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS 8
#else
#define BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS 8
#endif


namespace BucketAllocatorDetail
{

	struct AllocHeader
	{
		AllocHeader* volatile next;

#ifdef BUCKET_ALLOCATOR_TRAP_DOUBLE_DELETES
		uint32 magic;
#endif
	};

	struct SyncPolicyLocked
	{
#if defined(XENON) || defined(_WIN32)
		typedef SLIST_HEADER FreeListHeader;
#elif defined(PS3) || defined(ORBIS)
        typedef AllocHeader* volatile FreeListHeader;
#elif defined(APPLE) || defined(LINUX)
        typedef SLockFreeSingleLinkedListHeader FreeListHeader;
#endif

		typedef CryCriticalSectionNonRecursive Lock;

		Lock& GetRefillLock()
		{
			static Lock m_refillLock;
			return m_refillLock;
		}

		struct RefillLock
		{
			RefillLock(SyncPolicyLocked& policy) : m_policy(policy) { policy.GetRefillLock().Lock(); }
			~RefillLock() { m_policy.GetRefillLock().Unlock(); }

		private:
			SyncPolicyLocked& m_policy;
		};

#if defined (XENON)

		static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
		{
			InterlockedPushEntrySListRelease(&list, reinterpret_cast<PSLIST_ENTRY>(ptr));
		}

		static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
		{
			AllocHeader* item = head;
			AllocHeader* next;

			while (item)
			{
				next = item->next;
				item->next = NULL;

				PushOnto(list, item);

				item = next;
			}
		}
		
		ILINE static AllocHeader* PopOff(FreeListHeader& list)
		{
			return reinterpret_cast<AllocHeader*>(InterlockedPopEntrySListAcquire(&list));
		}

		ILINE static AllocHeader* PopListOff(FreeListHeader& list)
		{
			return reinterpret_cast<AllocHeader*>(InterlockedFlushSList(&list));
		}

		ILINE static bool IsFreeListEmpty(FreeListHeader& list)
		{
			return reinterpret_cast<AllocHeader*>(list.Next.Next) == NULL;
		}
#elif defined(APPLE) || defined(LINUX)

        static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
	    {
		    CryInterlockedPushEntrySList(list, *reinterpret_cast<SLockFreeSingleLinkedListEntry*>(ptr));
	    }

	    static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
	    {
		    AllocHeader* item = head;
		    AllocHeader* next;

		    while (item)
	    	{
			    next = item->next;
			    item->next = NULL;

			    PushOnto(list, item);

			    item = next;
		    }
	    }

	    static AllocHeader* PopOff(FreeListHeader& list)
	    {
		    return reinterpret_cast<AllocHeader*>(CryInterlockedPopEntrySList(list));
	    }

	    static AllocHeader* PopListOff(FreeListHeader& list)
	    {
		    return reinterpret_cast<AllocHeader*>(CryInterlockedFlushSList(list));
	    }

	    ILINE static bool IsFreeListEmpty(FreeListHeader& list)
	    {
		    return list.pNext == 0;
	    }   
#elif defined(PS3)

		static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
		{
#if !defined(__SPU__)
			AllocHeader* currentTop;

			__lwsync();
			do
			{
				currentTop = (AllocHeader*) __lwarx((volatile void*) &list);

				ptr->next = currentTop;
			}
			while (!__stwcx((volatile void*) &list, (long) ptr));
			__lwsync();
#endif
		}

		static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
		{
#if !defined(__SPU__)
			AllocHeader* headItem;

			__lwsync();
			do
			{
				headItem = (AllocHeader*) __lwarx((volatile void*) &list);

				tail->next = headItem;
			}
			while (!__stwcx((volatile void*) &list, (unsigned int)head));
			__lwsync();
#endif
		}

		static AllocHeader* PopOff(FreeListHeader& list)
		{
			AllocHeader* currentTop = NULL;

#if !defined(__SPU__)
			AllocHeader* next;

			__lwsync();
			do
			{
				currentTop = (AllocHeader*) __lwarx((volatile void*) &list);
				if (!currentTop)
					return NULL;

				next = currentTop->next;
			}
			while (!__stwcx((volatile void*) &list, (long) next));
			__lwsync();
#endif

			return currentTop;
		}

		static AllocHeader* PopListOff(FreeListHeader& list)
		{
			AllocHeader* pList;
			do 
			{
				pList = const_cast<AllocHeader*>(list);
			}
			while (CryInterlockedCompareExchangePointer((void * volatile *) &list, NULL, pList) != pList);
			return pList;
		}

		ILINE static bool IsFreeListEmpty(FreeListHeader& list)
		{
			return list == NULL;
		}

#elif defined(WIN32) || defined(DURANGO)

		static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
		{
			InterlockedPushEntrySList(&list, reinterpret_cast<PSLIST_ENTRY>(ptr));
		}

		static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
		{
			AllocHeader* item = head;
			AllocHeader* next;

			while (item)
			{
				next = item->next;
				item->next = NULL;

				PushOnto(list, item);

				item = next;
			}
		}

		static AllocHeader* PopOff(FreeListHeader& list)
		{
			return reinterpret_cast<AllocHeader*>(InterlockedPopEntrySList(&list));
		}

		static AllocHeader* PopListOff(FreeListHeader& list)
		{
			return reinterpret_cast<AllocHeader*>(InterlockedFlushSList(&list));
		}

		ILINE static bool IsFreeListEmpty(FreeListHeader& list)
		{
			return QueryDepthSList(&list) == 0;
		}

#elif defined(ORBIS)

		#define LIST_PTR_MASK	 0x007FFFFFFFFFFFFFULL	// Top of Orbis address space according to docs
		#define LIST_SALT_MASK 0xFF80000000000000ULL	// Upper bits used as salt
		#define LIST_SALT_INC  0x0080000000000000ULL

		static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
		{
			void *head;
			void *newHead;
			do 
			{
				head=(void*)list;
				newHead=(void*)(((UINT_PTR)head&LIST_SALT_MASK)+LIST_SALT_INC+(UINT_PTR)ptr);
				ptr->next=(AllocHeader*)((UINT_PTR)head&LIST_PTR_MASK);
			} while (CryInterlockedCompareExchangePointer((void* volatile*)&list, newHead, head)!=head);
		}

		static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
		{
			AllocHeader* item = head;
			AllocHeader* next;

			while (item)
			{
				next = item->next;
				item->next = NULL;

				PushOnto(list, item);

				item = next;
			}
		}

		static AllocHeader* PopOff(FreeListHeader& list)
		{
			void *head;
			void *newHead;
			AllocHeader* ret;
			do 
			{
				head=(void*)list;
				ret=(AllocHeader*)((UINT_PTR)head&LIST_PTR_MASK);
				if (ret==NULL)
					return NULL;
				newHead=(void*)(((UINT_PTR)head&LIST_SALT_MASK)+ret->next);
			} while (CryInterlockedCompareExchangePointer((void* volatile*)&list, newHead, head)!=head);
			return ret;
		}

		static AllocHeader* PopListOff(FreeListHeader& list)
		{
			void *head;
			void *newHead;
			do 
			{
				head=(void*)list;
				if (((UINT_PTR)head&LIST_PTR_MASK)==0)
					return NULL;
				newHead=(void*)(((UINT_PTR)head&LIST_SALT_MASK));
			} while (CryInterlockedCompareExchangePointer((void* volatile*)&list, newHead, head)!=head);
			return (AllocHeader*)((UINT_PTR)head&LIST_PTR_MASK);
		}

		ILINE static bool IsFreeListEmpty(FreeListHeader& list)
		{
			return ((UINT_PTR)list&LIST_PTR_MASK)==0;
		}

#endif

	};

	struct SyncPolicyUnlocked
	{
		typedef AllocHeader* FreeListHeader;

		struct RefillLock { RefillLock(SyncPolicyUnlocked&) {} };

		ILINE static void PushOnto(FreeListHeader& list, AllocHeader* ptr)
		{
			ptr->next = list;
			list = ptr;
		}

		ILINE static void PushListOnto(FreeListHeader& list, AllocHeader* head, AllocHeader* tail)
		{
			tail->next = list;
			list = head;
		}

		ILINE static AllocHeader* PopOff(FreeListHeader& list)
		{
			AllocHeader* top = list;
			if (top)
				list = *(AllocHeader**)(&top->next); // cast away the volatile
			return top;
		}

		ILINE static AllocHeader* PopListOff(FreeListHeader& list)
		{
			AllocHeader* pRet = list;
			list = NULL;
			return pRet;
		}

		ILINE static bool IsFreeListEmpty(FreeListHeader& list)
		{
			return list == NULL;
		}
	};

	template <size_t Size, typename SyncingPolicy, bool FallbackOnCRT = true, size_t MaxSegments = BUCKET_ALLOCATOR_DEFAULT_MAX_SEGMENTS>
	struct DefaultTraits
	{
		enum
		{
			MaxSize = 512,
			
#ifdef BUCKET_ALLOCATOR_PACK_SMALL_SIZES
			NumBuckets = 32 / 4 + (512 - 32) / 8,
#else
			NumBuckets = 512 / MEMORY_ALLOCATION_ALIGNMENT,
#endif

			PageLength = 64 * 1024,
			SmallBlockLength = 1024,
			SmallBlocksPerPage = 64,

			NumGenerations = 4,
			MaxNumSegments = MaxSegments,

			NumPages = Size / PageLength,

			FallbackOnCRTAllowed = FallbackOnCRT,
		};

		typedef SyncingPolicy SyncPolicy;

		static uint8 GetBucketForSize(size_t sz)
		{
#ifdef BUCKET_ALLOCATOR_PACK_SMALL_SIZES
			if (sz <= 32)
			{
				const int alignment = 4;
				size_t alignedSize = (sz + (alignment - 1)) & ~(alignment - 1);
				return alignedSize / alignment - 1;
			}
			else
			{
				const int alignment = 8;
				size_t alignedSize = (sz + (alignment - 1)) & ~(alignment - 1);
				alignedSize -= 32;
				return alignedSize / alignment + 7;
			}
#else
			const int alignment = MEMORY_ALLOCATION_ALIGNMENT;
			size_t alignedSize = (sz + (alignment - 1)) & ~(alignment - 1);
			return alignedSize / alignment - 1;
#endif
		}

		static size_t GetSizeForBucket(uint8 bucket)
		{
			size_t sz;

#ifdef BUCKET_ALLOCATOR_PACK_SMALL_SIZES
			if (bucket <= 7)
				sz = (bucket + 1) * 4;
			else
				sz = (bucket - 7) * 8 + 32;
#else
			sz = (bucket + 1) * MEMORY_ALLOCATION_ALIGNMENT;
#endif

#ifdef BUCKET_ALLOCATOR_TRAP_DOUBLE_DELETES
			return sz < sizeof(AllocHeader) ? sizeof(AllocHeader) : sz;
#else
			return sz;
#endif
		}

		static size_t GetGenerationForStability(uint8 stability)
		{
			return 3 - stability / 64;
		}
	};
}

#endif
