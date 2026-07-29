////////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek.
// -------------------------------------------------------------------------
//  File name:   CrySocks.h
//  Created:     05/01/2012 by Andrew Catlender
//  Description: Cross platform socket behaviour
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
//
//	Notes:
//		Use the wrapped calls here instead of raw socket calls wherever
//	possible as these calls abstract away the differences between the
//	platforms.
//
//		If you want to make a socket non-blocking, use MakeSocketNonBlocking()
//	as it is platform independent.
//
//		Use IsRecvPending() as a replacement for the use case of checking if
//	a single socket has pending data, rather than trying to use select()
//	directly as this is a common cause of subtle platform bugs.  If you need
//	to check multiple sockets simultaneously, then use select() as you
//  *ought* to know what you're doing!
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CRYSOCKS_H__
#define __CRYSOCKS_H__

///////////////////////////////////////////////////////////////////////////////

#if defined(PS3) || defined(APPLE) || defined(LINUX)
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#if defined(PS3)
#include <netex/errno.h>
#else
#include <errno.h>
#endif
#include <netdb.h>
typedef socklen_t				CRYSOCKLEN;
#elif defined(ORBIS)
#include <net.h>
#include <sys/socket.h>
#include <libnet/socket.h>
typedef socklen_t				CRYSOCKLEN;
#define SD_SEND SHUT_WR
#define	SD_RECEIVE SHUT_RD
#define	SD_BOTH SHUT_RDWR
#define SO_NBIO SCE_NET_SO_NBIO
#define WSAGetLastError GetLastError
#elif defined(WIN32) || defined(WIN64) || defined(DURANGO)
#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib") 
typedef int							CRYSOCKLEN;
#elif defined(XENON)
#include <winsockx.h>
#pragma comment(lib,"xnet.lib") 
typedef int							CRYSOCKLEN;
#endif

#include <string.h>

///////////////////////////////////////////////////////////////////////////////

// Type wrappers for sockets
typedef fd_set					CRYFD_SET;
typedef timeval					CRYTIMEVAL;
typedef sockaddr				CRYSOCKADDR;
typedef sockaddr_in			CRYSOCKADDR_IN;
typedef int							CRYSOCKET;

///////////////////////////////////////////////////////////////////////////////

namespace CrySock
{

///////////////////////////////////////////////////////////////////////////////

	// All socket errors should be -ve since a 0 or +ve result indicates success
	enum eCrySockError
	{
		eCSE_NO_ERROR					= 0,		// No error reported

		eCSE_EACCES						= -1,		// Tried to establish a connection to an invalid address (such as a broadcast address)
		eCSE_EADDRINUSE				= -2,		// Specified address already in use
		eCSE_EADDRNOTAVAIL		= -3,		// Invalid address was specified
		eCSE_EAFNOSUPPORT			= -4,		// Invalid socket type (or invalid protocol family - PS3)
		eCSE_EALREADY					= -5,		// Connection is being established (if the function is called again in non-blocking state)
		eCSE_EBADF						= -6,		// Invalid socket number specified
		eCSE_ECONNABORTED			= -7,		// Connection was aborted
		eCSE_ECONNREFUSED			= -8,		// Connection refused by destination
		eCSE_ECONNRESET				= -9,		// Connection was reset (TCP only)
		eCSE_EFAULT						= -10,	// Invalid socket number specified
		eCSE_EHOSTDOWN				= -11,	// Other end is down and unreachable
		eCSE_EINPROGRESS			= -12,	// Action is already in progress (when non-blocking)
		eCSE_EINTR						= -13,	// A blocking socket call was cancelled
		eCSE_EINVAL						= -14,	// Invalid argument or function call
		eCSE_EISCONN					= -15,	// Specified connection is already established
		eCSE_EMFILE						= -16,	// No more socket descriptors available
		eCSE_EMSGSIZE					= -17,	// Message size is too large (for PS3: 9216 bytes for UDP/UDPP2P, 8192 bytes for RAW socket)
		eCSE_ENETUNREACH			= -18,	// Destination is unreachable
		eCSE_ENOBUFS					= -19,	// Insufficient working memory
		eCSE_ENOPROTOOPT			= -20,	// Invalid combination of 'level' and 'optname'
		eCSE_ENOTCONN					= -21,	// Specified connection is not established
		eCSE_ENOTINITIALISED	= -22,	// Socket layer not initialised (e.g. need to call WSAStartup() on Windows)
		eCSE_EOPNOTSUPP				= -23,	// Socket type cannot accept connections
		eCSE_EPIPE						= -24,	// The writing side of the socket has already been closed
		eCSE_EPROTONOSUPPORT	= -25,	// Invalid protocol family 
		eCSE_ETIMEDOUT				= -26,	// TCP resend timeout occurred
		eCSE_ETOOMANYREFS			= -27,	// Too many multicast addresses specified
		eCSE_EWOULDBLOCK			= -28,	// Time out occurred when attempting to perform action
		eCSE_EWOULDBLOCK_CONN	= -29,	// Only applicable to connect() - a non-blocking connection has been attempted and is in progress

		eCSE_MISC_ERROR				= -1000
	};

///////////////////////////////////////////////////////////////////////////////

	// Forward declarations
	static CRYSOCKET HandleInvalidSocket(SOCKET s);
	static int HandleSocketError(int r);
	static int TranslateOSError(int oserror);

///////////////////////////////////////////////////////////////////////////////

	static CRYSOCKET socket(int af, int type, int protocol)
	{
		return HandleInvalidSocket(::socket(af, type, protocol));
	}

///////////////////////////////////////////////////////////////////////////////

	// create default INET socket
	static CRYSOCKET socketinet()
	{
		return HandleInvalidSocket(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	}
	
///////////////////////////////////////////////////////////////////////////////

	static int setsockopt(CRYSOCKET s, int level, int optname, const char* optval, CRYSOCKLEN optlen)
	{
		return HandleSocketError(::setsockopt(s, level, optname, optval, optlen));
	}

///////////////////////////////////////////////////////////////////////////////

	static int closesocket(CRYSOCKET s)
	{
#if defined(PS3) 
		return HandleSocketError(::socketclose(s));
#elif defined(ORBIS)
		return HandleSocketError(::close(s));
#else
		return HandleSocketError(::closesocket(s));
#endif
	}

///////////////////////////////////////////////////////////////////////////////

	static int shutdown(CRYSOCKET s, int how)
	{
		return HandleSocketError(::shutdown(s, how));
	}

///////////////////////////////////////////////////////////////////////////////

	static int getsockname(CRYSOCKET s, CRYSOCKADDR* addr, CRYSOCKLEN* addrlen)
	{
		return HandleSocketError(::getsockname(s, addr, addrlen));
	}

///////////////////////////////////////////////////////////////////////////////

	static int connect(CRYSOCKET s, const CRYSOCKADDR* addr, CRYSOCKLEN addrlen)
	{
		int err = HandleSocketError(::connect(s, addr, addrlen));
#if defined(PS3) || defined(ORBIS)	// PS3/Orbis would have to be different
		if (err == eCSE_EINPROGRESS)
#else
		if (err == eCSE_EWOULDBLOCK)
#endif
			err = eCSE_EWOULDBLOCK_CONN;
		return err;
	}

///////////////////////////////////////////////////////////////////////////////

	static int listen(CRYSOCKET s, int backlog)
	{
		return HandleSocketError(::listen(s, backlog));
	}

///////////////////////////////////////////////////////////////////////////////

	static CRYSOCKET accept(CRYSOCKET s, CRYSOCKADDR* addr, CRYSOCKLEN* addrlen)
	{
#ifdef ORBIS
		return HandleInvalidSocket(sceNetAccept((SceNetId)s, (SceNetSockaddr*)addr, (SceNetSocklen_t*)addrlen));
#else
		return HandleInvalidSocket(::accept(s, addr, addrlen));
#endif
	}

///////////////////////////////////////////////////////////////////////////////

	static int send(CRYSOCKET s, const char* buf, int len, int flags)
	{
#ifdef ORBIS
		#if ORBIS_SDK_VERSION>990
			int msgNoSignal=MSG_NOSIGNAL;
		#else
			int msgNoSignal=0x20000;
		#endif
		return HandleSocketError(::send(s, buf, len, flags|msgNoSignal));
#elif defined(LINUX)
        return HandleSocketError(::send(s, buf, len, flags | MSG_NOSIGNAL));
#else
		return HandleSocketError(::send(s, buf, len, flags));
#endif
	}

///////////////////////////////////////////////////////////////////////////////

	static int recv(CRYSOCKET s, char* buf, int len, int flags)
	{
		return HandleSocketError(::recv(s, buf, len, flags));
	}

///////////////////////////////////////////////////////////////////////////////

	static int recvfrom(CRYSOCKET s, char* buf, int len, int flags, CRYSOCKADDR* addr, CRYSOCKLEN* addrLen)
	{		
		return HandleSocketError(::recvfrom(s, buf, len, flags, addr, addrLen));
	}

///////////////////////////////////////////////////////////////////////////////

	static int sendto(CRYSOCKET s, char* buf, int len, int flags, CRYSOCKADDR* addr, CRYSOCKLEN addrLen)
	{		
		return HandleSocketError(::sendto(s, buf, len, flags, addr, addrLen));
	}

///////////////////////////////////////////////////////////////////////////////

	static int SetRecvTimeout(CRYSOCKET s, const int seconds, const int microseconds)
	{
		struct timeval timeout;      
		timeout.tv_sec = seconds;
		timeout.tv_usec = microseconds;
		return CrySock::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	}

///////////////////////////////////////////////////////////////////////////////

	static int SetSendTimeout(CRYSOCKET s, const int seconds, const int microseconds)
	{
		struct timeval timeout;      
		timeout.tv_sec = seconds;
		timeout.tv_usec = microseconds;
		return CrySock::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	}

///////////////////////////////////////////////////////////////////////////////

	//
	//	Returns if the passed socket is readable
	//  Return values:
	//		1		= readable - use recv to get the data
	//		0		= timed out (exceeded passed timeout value)
	//		-ve	= an error occurred - see eCrySocketError
	//
	static int IsRecvPending(CRYSOCKET s, CRYTIMEVAL* timeout)
	{
		CRYFD_SET emptySet;
		FD_ZERO(&emptySet);

		CRYFD_SET readSet;
		FD_ZERO(&readSet);
		FD_SET(s, &readSet);

#if defined(PS3)
		int ret = HandleSocketError(::socketselect(s+1, &readSet, &emptySet, &emptySet, timeout));
#elif defined(ORBIS)
		int ret = HandleSocketError(::select(s+1, &readSet, &emptySet, &emptySet, timeout));
#elif defined(WIN32) || defined(XENON)
		int ret = HandleSocketError(::select(static_cast<int>(s)+1, &readSet, &emptySet, &emptySet, timeout));
#else
		int ret = HandleSocketError(::select(s+1, &readSet, &emptySet, &emptySet, timeout));
#endif

		if (ret >= 0)
		{
			ret = FD_ISSET(s, &readSet);
			if (ret != 0)
			{
				ret = 1;
			}
		}

		return ret;
	}

///////////////////////////////////////////////////////////////////////////////

	//
	//	Returns if the passed socket is writable
	//  Return values:
	//		1		= connected
	//		0		= timed out (exceeded passed timeout value)
	//		-ve	= an error occurred - see eCrySocketError
	//
	static int WaitForWritableSocket(CRYSOCKET s, CRYTIMEVAL* timeout)
	{
		CRYFD_SET emptySet;
		FD_ZERO(&emptySet);

		CRYFD_SET writeSet;
		FD_ZERO(&writeSet);
		FD_SET(s, &writeSet);

#if defined(PS3)
		int ret = HandleSocketError(::socketselect(s+1, &emptySet, &writeSet, &emptySet, timeout));
#elif defined(ORBIS)
		int ret = HandleSocketError(::select(s+1, &emptySet, &writeSet, &emptySet, timeout));
#elif defined(WIN32) || defined(XENON)
		int ret = HandleSocketError(::select(static_cast<int>(s)+1, &emptySet, &writeSet, &emptySet, timeout));
#else
		int ret = HandleSocketError(::select(s+1, &emptySet, &writeSet, &emptySet, timeout));
#endif

		if (ret >= 0)
		{
			ret = FD_ISSET(s, &writeSet);
			if (ret != 0)
			{
				ret = 1;
			}
		}

		return ret;
	}

///////////////////////////////////////////////////////////////////////////////

	//
	// IsRecvPending should be sufficient for most applications (see above).
	// The correct value for the first parameter 'nfds' should be the highest
	// numbered socket (as passed to FD_SET) +1.
	//
	// Only use this if you know what you're doing! Passing nfds as 0 on anything
	// other than WinSock is wrong!
	//
	static int select(int nfds, CRYFD_SET* readfds, CRYFD_SET* writefds, CRYFD_SET* exceptfds, CRYTIMEVAL* timeout)
	{
#if !defined(_RELEASE)
	if (nfds == 0)
	{
		CryFatalError("CrySock select detected first parameter is 0!  This *MUST* be fixed!");
	}
#endif

#if defined(PS3)
		return HandleSocketError(::socketselect(nfds, readfds, writefds, exceptfds, timeout));
#elif defined(ORBIS)
		return HandleSocketError(::select(nfds, readfds, writefds, exceptfds, timeout));
#else
		return HandleSocketError(::select(nfds, readfds, writefds, exceptfds, timeout));
#endif
	}

///////////////////////////////////c////////////////////////////////////////////

	static int bind(CRYSOCKET s, const CRYSOCKADDR* addr, CRYSOCKLEN addrlen)
	{
		return HandleSocketError(::bind(s, addr, addrlen));
	}

///////////////////////////////////////////////////////////////////////////////

	static bool MakeSocketNonBlocking(CRYSOCKET sock)
	{
	#if defined(WIN32) || defined(XENON) || defined(DURANGO)
		unsigned long nTrue = 1;
		if (::ioctlsocket(sock, FIONBIO, &nTrue) == SOCKET_ERROR)
		{
			return false;
		}
	#elif defined(PS3) || defined(ORBIS)
		int nonblocking = 1;
		if (::setsockopt(sock, SOL_SOCKET, SO_NBIO, &nonblocking, sizeof(int)) < 0)
		{
			return false;
		}
	#else
		int nFlags = ::fcntl(sock, F_GETFL);
		nFlags |= O_NONBLOCK;
		if (::fcntl(sock, F_SETFL, nFlags) == -1)
		{
			return false;
		}
	#endif

		return true;
	}

	static bool MakeSocketBlocking(CRYSOCKET sock)
	{
#if defined(WIN32) || defined(XENON) || defined(DURANGO)
		unsigned long nTrue = 0;
		if (::ioctlsocket(sock, FIONBIO, &nTrue) == SOCKET_ERROR)
		{
			return false;
		}
#elif defined(PS3) || defined(ORBIS)
		int nonblocking = 0;
		if (::setsockopt(sock, SOL_SOCKET, SO_NBIO, &nonblocking, sizeof(int)) < 0)
		{
			return false;
		}
#else
		int nFlags = ::fcntl(sock, F_GETFL);
		nFlags &= ~O_NONBLOCK;
		if (::fcntl(sock, F_SETFL, nFlags) == -1)
		{
			return false;
		}
#endif

		return true;
	}

  static uint32 GetAddrForHost(const char *hostname, uint32 timeoutMilliseconds = 0)
  {
    uint32 ret = 0;

#if defined(WIN32) || defined(WIN64) || defined(PS3) || defined(DURANGO)
		hostent *pHost = gethostbyname(hostname);
		if (pHost)
		{
			ret = ((in_addr *)(pHost->h_addr))->s_addr;
		}
#elif defined(ORBIS)
		bool bSuccess=false;
		SceNetInAddr sceaddr;
		SceNetId poolId = sceNetPoolCreate(__FUNCTION__, 4*1024, 0);
		if (poolId>=0)
		{
			SceNetId rid=sceNetResolverCreate("CrySock resolver", poolId, 0);
			if (rid>=0)
			{
				if (sceNetResolverStartNtoa(rid, hostname, &sceaddr, 0, 0, 0)<0)
				{
					printf("Failed DNS lookup of '%s'\n", hostname);
				}
				else
				{
					bSuccess=true;
				}
				sceNetResolverDestroy(rid);
			}
			else
			{
				printf("Failed to create DNS resolver\n");
			}
			sceNetPoolDestroy(poolId);
		}
		else
		{
			printf("Failed to create DNS resolver pool\n");
		}
		if (bSuccess)
			ret = sceaddr.s_addr;
		else
			ret = ::inet_addr(hostname);
#elif defined (XENON)
    ret = inet_addr(hostname);
    if (ret == INADDR_NONE || ret == INADDR_ANY)
    {
      WSAEVENT wsaEvent = WSACreateEvent();
      XNDNS *pXndns = NULL;
      int err = XNetDnsLookup( hostname, wsaEvent, &pXndns );
      if(pXndns)
      {
        WaitForSingleObject( wsaEvent, timeoutMilliseconds == 0 ? INFINITE : timeoutMilliseconds );
        if (pXndns->iStatus == 0 && pXndns->cina >= 1)
        {
          ret = pXndns->aina[0].S_un.S_addr;
        }
      }
      WSACloseEvent(wsaEvent);
      
      if(pXndns)
        XNetDnsRelease( pXndns );
    }
#endif

    return ret;
  }

///////////////////////////////////////////////////////////////////////////////
  
#define LOCAL_DOMAIN ".intern.crytek.de"

  static uint32 DNSLookup(const char *hostname, uint32 timeoutMilliseconds = 200)
  {
    uint32 ret = CrySock::GetAddrForHost(hostname, timeoutMilliseconds);

    if(ret == 0 || ret == ~0)
    {
      char host[256];

      size_t hostlen = strlen(hostname);
      size_t domainlen = sizeof(LOCAL_DOMAIN);
      if(hostlen + domainlen > sizeof(host)-1)
      {
        CryWarning(VALIDATOR_MODULE_NETWORK, VALIDATOR_ERROR, "hostname too long to fallback to local domain: '%s'", hostname);
        return 0;
      }

      strcpy(host, hostname);
      strcat(host, LOCAL_DOMAIN);
      
      host[hostlen+domainlen-1] = 0;

      ret = CrySock::GetAddrForHost(host, timeoutMilliseconds);
    }

    return ret;
  }

///////////////////////////////////////////////////////////////////////////////

	static CRYSOCKET HandleInvalidSocket(SOCKET s)
	{
		CRYSOCKET cs = static_cast<CRYSOCKET>(s);
#if defined(PS3) || defined(ORBIS) 
		if (s < 0)
		{
			cs = TranslateOSError(sys_net_errno);
		}
#else
		if (s == INVALID_SOCKET)
		{
			cs = TranslateOSError(WSAGetLastError());
		}
#endif

		return cs;
	}

///////////////////////////////////////////////////////////////////////////////

	static int HandleSocketError(int r)
	{
#if defined(PS3) || defined(ORBIS)
		if (r < 0)
		{
			r = TranslateOSError(sys_net_errno);
		}
#else
		if (r == SOCKET_ERROR)
		{
			r = TranslateOSError(WSAGetLastError());
		}
#endif

		return r;
	}

///////////////////////////////////////////////////////////////////////////////

	static int TranslateOSError(int oserror)
	{
		int error;

#define TRANSLATE(_from,_to) case (_from): error = (_to); break;
		switch (oserror)
		{
			TRANSLATE(0, eCSE_NO_ERROR);

#if defined(PS3) 
			TRANSLATE(SYS_NET_EACCES, eCSE_EACCES);
			TRANSLATE(SYS_NET_EADDRINUSE, eCSE_EADDRINUSE);
			TRANSLATE(SYS_NET_EADDRNOTAVAIL, eCSE_EADDRNOTAVAIL);
			TRANSLATE(SYS_NET_EAFNOSUPPORT, eCSE_EAFNOSUPPORT);
			TRANSLATE(SYS_NET_EALREADY, eCSE_EALREADY);
			TRANSLATE(SYS_NET_EBADF, eCSE_EBADF);
			TRANSLATE(SYS_NET_ECONNABORTED, eCSE_ECONNABORTED);
			TRANSLATE(SYS_NET_ECONNREFUSED, eCSE_ECONNREFUSED);
			TRANSLATE(SYS_NET_ECONNRESET, eCSE_ECONNRESET);
			TRANSLATE(SYS_NET_EFAULT, eCSE_EFAULT);
			TRANSLATE(SYS_NET_EHOSTDOWN, eCSE_EHOSTDOWN);
			TRANSLATE(SYS_NET_EINPROGRESS, eCSE_EINPROGRESS);
			TRANSLATE(SYS_NET_EINTR, eCSE_EINTR);
			TRANSLATE(SYS_NET_EINVAL, eCSE_EINVAL);
			TRANSLATE(SYS_NET_EISCONN, eCSE_EISCONN);
			TRANSLATE(SYS_NET_EMFILE, eCSE_EMFILE);
			TRANSLATE(SYS_NET_EMSGSIZE, eCSE_EMSGSIZE);
			TRANSLATE(SYS_NET_ENETUNREACH, eCSE_ENETUNREACH);
			TRANSLATE(SYS_NET_ENOBUFS, eCSE_ENOBUFS);
			TRANSLATE(SYS_NET_ENOPROTOOPT, eCSE_ENOPROTOOPT);
			TRANSLATE(SYS_NET_ENOTCONN, eCSE_ENOTCONN);
			TRANSLATE(SYS_NET_EOPNOTSUPP, eCSE_EOPNOTSUPP);
			TRANSLATE(SYS_NET_EPIPE, eCSE_EPIPE);
			TRANSLATE(SYS_NET_EPROTONOSUPPORT, eCSE_EPROTONOSUPPORT);
			TRANSLATE(SYS_NET_ETIMEDOUT, eCSE_ETIMEDOUT);
			TRANSLATE(SYS_NET_ETOOMANYREFS, eCSE_ETOOMANYREFS);
			TRANSLATE(SYS_NET_EWOULDBLOCK, eCSE_EWOULDBLOCK);
#elif defined(ORBIS)
			TRANSLATE(EACCES, eCSE_EACCES);
			TRANSLATE(EADDRINUSE, eCSE_EADDRINUSE);
			TRANSLATE(EADDRNOTAVAIL, eCSE_EADDRNOTAVAIL);
			TRANSLATE(EAFNOSUPPORT, eCSE_EAFNOSUPPORT);
			TRANSLATE(EALREADY, eCSE_EALREADY);
			TRANSLATE(EBADF, eCSE_EBADF);
			TRANSLATE(ECONNABORTED, eCSE_ECONNABORTED);
			TRANSLATE(ECONNREFUSED, eCSE_ECONNREFUSED);
			TRANSLATE(ECONNRESET, eCSE_ECONNRESET);
			TRANSLATE(EFAULT, eCSE_EFAULT);
			TRANSLATE(EHOSTDOWN, eCSE_EHOSTDOWN);
			TRANSLATE(EINPROGRESS, eCSE_EINPROGRESS);
			TRANSLATE(EINTR, eCSE_EINTR);
			TRANSLATE(EINVAL, eCSE_EINVAL);
			TRANSLATE(EISCONN, eCSE_EISCONN);
			TRANSLATE(EMFILE, eCSE_EMFILE);
			TRANSLATE(EMSGSIZE, eCSE_EMSGSIZE);
			TRANSLATE(ENETUNREACH, eCSE_ENETUNREACH);
			TRANSLATE(ENOBUFS, eCSE_ENOBUFS);
			TRANSLATE(ENOPROTOOPT, eCSE_ENOPROTOOPT);
			TRANSLATE(ENOTCONN, eCSE_ENOTCONN);
			TRANSLATE(EOPNOTSUPP, eCSE_EOPNOTSUPP);
			TRANSLATE(EPROTONOSUPPORT, eCSE_EAFNOSUPPORT);
			TRANSLATE(ETIMEDOUT, eCSE_ETIMEDOUT);
			TRANSLATE(ETOOMANYREFS, eCSE_ETOOMANYREFS);
			TRANSLATE(EWOULDBLOCK, eCSE_EWOULDBLOCK);
#else
			TRANSLATE(WSAEACCES, eCSE_EACCES);
			TRANSLATE(WSAEADDRINUSE, eCSE_EADDRINUSE);
			TRANSLATE(WSAEADDRNOTAVAIL, eCSE_EADDRNOTAVAIL);
			TRANSLATE(WSAEAFNOSUPPORT, eCSE_EAFNOSUPPORT);
			TRANSLATE(WSAEALREADY, eCSE_EALREADY);
			TRANSLATE(WSAEBADF, eCSE_EBADF);
			TRANSLATE(WSAECONNABORTED, eCSE_ECONNABORTED);
			TRANSLATE(WSAECONNREFUSED, eCSE_ECONNREFUSED);
			TRANSLATE(WSAECONNRESET, eCSE_ECONNRESET);
			TRANSLATE(WSAEFAULT, eCSE_EFAULT);
			TRANSLATE(WSAEHOSTDOWN, eCSE_EHOSTDOWN);
			TRANSLATE(WSAEINPROGRESS, eCSE_EINPROGRESS);
			TRANSLATE(WSAEINTR, eCSE_EINTR);
			TRANSLATE(WSAEINVAL, eCSE_EINVAL);
			TRANSLATE(WSAEISCONN, eCSE_EISCONN);
			TRANSLATE(WSAEMFILE, eCSE_EMFILE);
			TRANSLATE(WSAEMSGSIZE, eCSE_EMSGSIZE);
			TRANSLATE(WSAENETUNREACH, eCSE_ENETUNREACH);
			TRANSLATE(WSAENOBUFS, eCSE_ENOBUFS);
			TRANSLATE(WSAENOPROTOOPT, eCSE_ENOPROTOOPT);
			TRANSLATE(WSAENOTCONN, eCSE_ENOTCONN);
			TRANSLATE(WSAEOPNOTSUPP, eCSE_EOPNOTSUPP);
			TRANSLATE(WSAEPROTONOSUPPORT, eCSE_EAFNOSUPPORT);
			TRANSLATE(WSAETIMEDOUT, eCSE_ETIMEDOUT);
			TRANSLATE(WSAETOOMANYREFS, eCSE_ETOOMANYREFS);
			TRANSLATE(WSAEWOULDBLOCK, eCSE_EWOULDBLOCK);
#if !defined(APPLE) && ! defined(LINUX)
            // No equivalent in the posix api
			TRANSLATE(WSANOTINITIALISED, eCSE_ENOTINITIALISED);
#endif
#endif

		default:
			CryLog("CrySock could not translate OS error code %x, treating as miscellaneous", oserror);
			error = eCSE_MISC_ERROR;
			break;
		}
#undef TRANSLATE

		return error;
	}

///////////////////////////////////////////////////////////////////////////////

}; // End namespace CrySock

///////////////////////////////////////////////////////////////////////////////

#endif // __CRYSOCKS_H__
