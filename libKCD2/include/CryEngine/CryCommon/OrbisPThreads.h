#ifndef __ORBIS_PTHREADS_H__
#define __ORBIS_PTHREADS_H__

#define ORBIS_SCE_THREADS

#ifdef ORIS_SCE_THREADS

// Undefine everything pthread related as SCE uses own but also include pthread header
#undef pthread_cleanup_pop
#undef pthread_cleanup_push

#undef PTHREAD_DESTRUCTOR_ITERATIONS
#undef PTHREAD_KEYS_MAX
#undef PTHREAD_STACK_MIN
#undef PTHREAD_THREADS_MAX
#undef PTHREAD_BARRIER_SERIAL_THREAD
#undef PTHREAD_DETACHED
#undef PTHREAD_SCOPE_SYSTEM
#undef PTHREAD_INHERIT_SCHED
#undef PTHREAD_NOFLOAT
#undef PTHREAD_CREATE_DETACHED
#undef PTHREAD_CREATE_JOINABLE
#undef PTHREAD_SCOPE_PROCESS
#undef PTHREAD_EXPLICIT_SCHED
#undef PTHREAD_PROCESS_PRIVATE
#undef PTHREAD_PROCESS_SHARED
#undef PTHREAD_CANCEL_ENABLE
#undef PTHREAD_CANCEL_DISABLE
#undef PTHREAD_CANCEL_DEFERRED
#undef PTHREAD_CANCEL_ASYNCHRONOUS
#undef PTHREAD_CANCELED
#undef PTHREAD_NEEDS_INIT
#undef PTHREAD_DONE_INIT
#undef PTHREAD_ONCE_INIT
#undef PTHREAD_MUTEX_INITIALIZER
#undef PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP
#undef PTHREAD_COND_INITIALIZER
#undef PTHREAD_RWLOCK_INITIALIZER
#undef pthread_condattr_default
#undef pthread_mutexattr_default
#undef pthread_attr_default
#undef PTHREAD_PRIO_NONE
#undef PTHREAD_PRIO_INHERIT
#undef PTHREAD_PRIO_PROTECT
#define PTHREAD_MUTEX_ERRORCHECK	SCE_PTHREAD_MUTEX_ERRORCHECK
#define PTHREAD_MUTEX_RECURSIVE		SCE_PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_MUTEX_NORMAL			SCE_PTHREAD_MUTEX_NORMAL
#define PTHREAD_MUTEX_ADAPTIVE_NP	SCE_PTHREAD_MUTEX_ADAPTIVE
#define PTHREAD_MUTEX_TYPE_MAX		SCE_PTHREAD_MUTEX_TYPE_MAX

#define PTHREAD_CREATE_JOINABLE		SCE_PTHREAD_CREATE_JOINABLE
#define PTHREAD_CREATE_DETACHED		SCE_PTHREAD_CREATE_DETACHED

// Now wrap Sony's pthread like library
#define pthread_t				ScePthread
#define pthread_attr_t	ScePthreadAttr
#define pthread_once_t  ScePthreadOnce
#define pthread_mutex_t	ScePthreadMutex
#define pthread_mutexattr_t ScePthreadMutexattr
#define pthread_cond_t	ScePthreadCond
#define pthread_condattr_t ScePthreadCondattr
#define pthread_rwlock_t	ScePthreadRwlock
#define pthread_rwlockattr_t ScePthreadRwlockattr
#define pthread_spinlock_t ScePthreadSpinlock
#define pthread_barrier_t ScePthreadBarrier
#define pthread_barrierattr_t ScePthreadBarrierattr

#define pthread_cleanup_pop scePthreadCleanupPop
#define pthread_cleanup_push scePthreadCleanupPush
#define pthread_create(a,b,c,d)	scePthreadCreate(a,b,c,d,"Thread")
#define pthread_cancel scePthreadCancel
#define pthread_detach scePthreadDetach
#define pthread_equal scePthreadEqual
#define pthread_getprio scePthreadGetprio
#define pthread_getspecific scePthreadGetspecific
#define pthread_join scePthreadJoin
#define pthread_key_create scePthreadKeyCreate
#define pthread_key_delete scePthreadKeyDelete
#define pthread_once scePthreadOnce
#define pthread_self scePthreadSelf
#define pthread_setcancelstate scePthreadSetcancelstate
#define pthread_setcanceltype scePthreadSetcanceltype
#define pthread_setprio	scePthreadSetprio
#define pthread_setspecific scePthreadSetspecific
#define pthread_yield scePthreadYield

#define pthread_attr_destory scePthreadAttrDestroy
#define pthread_attr_getinheritsched	scePthreadAttrGetinheritsched
#define pthread_attr_getschedparam scePthreadAttrGetschedparam
#define pthread_attr_getschedpolicy	scePthreadAttrGetschedpolicy
#define pthread_attr_getstackaddr scePthreadAttrGetstackaddr
#define pthread_attr_getstacksize scePthreadAttrGetstacksize
#define pthread_attr_getdetachstate scePthreadAttrGetdetachstate
#define pthread_attr_init scePthreadAttrInit
#define pthread_attr_setinheritsched	scePthreadAttrSetinheritsched
#define pthread_attr_setschedparam scePthreadAttrSetschedparam
#define pthread_attr_setschedpolicy	scePthreadAttrSetschedpolicy
#define pthread_attr_setstackaddr scePthreadAttrSetstackaddr
#define pthread_attr_setstacksize scePthreadAttrSetstacksize

#define pthread_mutex_getprioceiling	scePthreadMutexGetprioceiling
#define pthread_mutex_setprioceiling	scePthreadMutexSetprioceiling
#define pthread_mutex_destroy					scePthreadMutexDestroy
#define pthread_mutex_init(a,b)				scePthreadMutexInit(a,b,"Mutex")
#define pthread_mutex_lock						scePthreadMutexLock
#define pthread_mutex_timedlock				scePthreadMutexTimedlock
#define pthread_mutexattr_init				scePthreadMutexattrInit
#define pthread_mutexattr_destroy			scePthreadMutexattrDestroy
#define pthread_mutexattr_setprioceiling	scePthreadMutexSetprioceiling
#define pthread_mutexattr_getprioceiling	scePthreadMutexGetprioceiling
#define pthread_mutexattr_setprotocol			scePthreadMutexattrSetprotocol
#define pthread_mutexattr_getprotocol			scePthreadMutexattrGetprotocol
#define pthread_mutexattr_settype					scePthreadMutexattrSettype
#define pthread_mutexattr_gettype					scePthreadMutexattrGettype

#define pthread_cond_broadcast						scePthreadCondBroadcast
#define pthread_cond_destory							scePthreadCondDestroy
#define pthread_cond_init(a,b)						scePthreadCondInit(a,b,"Condition")
#define pthread_cond_signal								scePthreadCondSignal
#define pthread_cond_timedwait						scePthreadCondTimedwait
#define pthread_cond_wait									scePthreadCondWait
#define pthread_condattr_init							scePthreadCondattrInit
#define pthread_condattr_destroy					scePthreadCondattrDestroy

#define pthread_rwlock_destroy						scePthreadRwlockDestroy
#define pthread_rwlock_init(a,b)					scePthreadRwlockInit(a,b,"RWLock")
#define pthread_rwlock_rdlock							scePthreadRwlockRdlock
#define pthread_rwlock_tryrdlock					scePthreadRwlockTryrdlock
#define pthread_rwlock_timedrdlock				scePthreadRwlockTimedrdlock
#define pthread_rwlock_timedwrlock				scePthreadRwlockTimedwrlock
#define pthread_rwlock_unlock							scePthreadRwlockUnlock
#define pthread_rwlock_wrlock							scePthreadRwlockWrlock
#define pthread_rwlock_trywrlock					scePthreadRwlockTrywrlock
#define pthread_rwlockattr_destroy				scePthreadRwlockattrDestroy
#define pthread_rwlockattr_init						scePthreadRwlockattrInit

#define pthread_spin_init									scePthreadSpinInit
#define pthread_spin_destroy							scePthreadSpinDestroy
#define pthread_spin_lock									scePthreadSpinLock
#define pthread_spin_trylock							scePthreadSpinTrylock
#define pthread_spin_unlock								scePthreadSpinUnlock

#define pthread_barrier_destroy						scePthreadBarrierDestroy
#define pthread_barrier_init(a,b)					scePthreadBarrierInit(a,b,"Barrier")
#define pthread_barrier_wait							scePthreadBarrierWait
#define pthread_barrierattr_destroy				scePthreadBarrierattrDestroy
#define pthread_barrierattr_init					scePthreadBarrierattrInit

#else

#include <pthread.h>
#include <semaphore.h>

#endif

#endif // __ORBIS_PTHREADS_H__