#pragma once
#ifdef DURANGO
#ifndef __INETWORKINGSTATE_H__
#define __INETWORKINGSTATE_H__

#ifndef noexcept
#define noexcept throw()
#define UNDEFINE_NOEXCEPT
#endif

namespace Live
{ 

	typedef unsigned long long Xuid;

	namespace State
	{
		struct INetworkingUser
		{
		public:
			virtual ~INetworkingUser() {}
		public:
			virtual Xuid Xuid() const noexcept = 0;
			virtual bool IsHost() const noexcept = 0;
			virtual bool HasValidSession() const noexcept = 0;

			virtual GUID GetSessionName() noexcept = 0;
			virtual HRESULT ChangeSessionAsync( GUID const & scid, LPCWSTR const templateName, GUID const & sessionId, ABI::Windows::Foundation::IAsyncAction ** action ) noexcept = 0;
			virtual HRESULT DeleteSessionAsync( ABI::Windows::Foundation::IAsyncAction ** action ) noexcept = 0;
			virtual HRESULT GetSessionAsync( ABI::Windows::Foundation::IAsyncAction ** action ) noexcept = 0;

			virtual HRESULT GetHostSecureDeviceAddress( ABI::Windows::Xbox::Networking::ISecureDeviceAddress ** secureDeviceaddresses, unsigned int * addresses ) const noexcept = 0;
			virtual HRESULT GetRemoteSecureDeviceAddresses( unsigned int const countAddresses, ABI::Windows::Xbox::Networking::ISecureDeviceAddress ** secureDeviceaddresses, unsigned int * addresses ) const noexcept = 0;
			virtual HRESULT GetRemoteXuids( unsigned int const countXuids, Live::Xuid * xuids, unsigned int * returnedXuids ) const noexcept = 0;
			virtual HRESULT GetHostXuid(Live::Xuid& hostXuid) const noexcept = 0;
		};
	}
}

#ifdef UNDEFINE_NOEXCEPT
#undef UNDEFINE_NOEXCEPT
#undef noexcept
#endif // UNDEFINE_NOEXCEPT


#endif //__INETWORKINGSTATE_H__

#endif