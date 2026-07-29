//////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2011.
// -------------------------------------------------------------------------
//  File name:   HardwareBreakpoint.h
//  Version:     v1.00
//  Created:     05/25/2011 by CarstenW
//  Description: Implements platform abstracted, programmatically triggered
//               data and code breakpoints.
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#ifndef _HARDWAREBREAKPOINT_H_
#define _HARDWAREBREAKPOINT_H_

#pragma once

// needs platform.h included first!
#include <assert.h>

#if defined(XENON)
#	include <xbdm.h>
#endif


struct HardwareBreakpointBase
{
public:
	enum Type
	{
		TYPE_CODE,
		TYPE_WRITE,
		TYPE_READWRITE
	};

	enum Size
	{
		SIZE_1,
		SIZE_2,
		SIZE_4,
#if defined(WIN32) || defined(WIN64)
		SIZE_8
#endif
	};
};


template <class HardwareBreakpointImpl>
class HardwareBreakpoint : public HardwareBreakpointImpl
{
public:
	HardwareBreakpoint() : HardwareBreakpointImpl() {}
	~HardwareBreakpoint() {}

	bool Set(void* thread, void* address, HardwareBreakpointBase::Size size, HardwareBreakpointBase::Type type)
	{
		return HardwareBreakpointImpl::Set(thread, address, size, type);
	}

	bool Clear()
	{
		return HardwareBreakpointImpl::Clear();
	}
};


#if defined(WIN32) || defined(WIN64)

class HardwareBreakpoint_Windows : public HardwareBreakpointBase
{
public:
	HardwareBreakpoint_Windows()
	: m_thread(0)
	, m_idx((unsigned char) -1)
	{
	}

	~HardwareBreakpoint_Windows()
	{
		Clear();
	}

	bool Set(void* thread, void* address, Size size, Type type)
	{
		if (m_thread)
		{
			assert(0);
			return false;
		}

		assert(m_idx == (unsigned char) -1);

		CONTEXT cxt;
		cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;

		if (GetThreadContext(thread, &cxt))
		{
			for (m_idx = 0; m_idx < 4; ++m_idx)
			{
				if ((cxt.Dr7 & (ULONG_PTR) (1 << (m_idx * 2))) == 0)
					break;
			}

			if (m_idx < 4)
			{
				switch (m_idx)
				{
				case 0:
					cxt.Dr0 = (ULONG_PTR) address;
					break;
				case 1:
					cxt.Dr1 = (ULONG_PTR) address;
					break;
				case 2:
					cxt.Dr2 = (ULONG_PTR) address;
					break;
				case 3:
					cxt.Dr3 = (ULONG_PTR) address;
					break;
				default:
					assert(0);
					break;
				}

				SetBits(cxt.Dr7, m_idx * 2, 1, 1);
				SetBits(cxt.Dr7, 16 + m_idx * 4, 2, RealType(type));
				SetBits(cxt.Dr7, 18 + m_idx * 4, 2, RealSize(type != TYPE_CODE ? size : SIZE_1));

				if (SetThreadContext(thread, &cxt))
					m_thread = thread;
				else
					m_idx = (unsigned char) -1;
			}
			else
			{
				m_idx = (unsigned char) -1;
				m_thread = 0;
			}
		}

		return m_thread != 0;
	}

	bool Clear()
	{
		bool ret = true;

		if (m_thread)
		{
			assert(m_idx < 4);

			CONTEXT cxt;
			cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			if (GetThreadContext(m_thread, &cxt))
			{
				SetBits(cxt.Dr7, m_idx * 2, 1, 0);

				ret = SetThreadContext(m_thread, &cxt) == TRUE;
			}

			m_idx = (unsigned char) -1;
			m_thread = 0;
		}

		return ret;
	}

private:
	static void SetBits(ULONG_PTR& v, ULONG_PTR lowBit, ULONG_PTR numBits, ULONG_PTR newValue)
	{
		ULONG_PTR mask = ((1 << numBits) - 1) << lowBit;
		v = (v & ~mask) | ((newValue << lowBit) & mask);
	}

	static ULONG_PTR RealType(Type type)
	{
		switch (type)
		{
		case TYPE_CODE: return 0;
		case TYPE_WRITE: return 1;
		case TYPE_READWRITE: return 3;
		default:
			assert(0);
			return 1;
		};
	}

	static ULONG_PTR RealSize(Size size)
	{
		switch (size)
		{
		case SIZE_1: return 0;
		case SIZE_2: return 1;
		case SIZE_4: return 3;
		case SIZE_8: return 2;
		default:
			assert(0);
			return 3;
		};
	}

private:
	void* m_thread;
	unsigned char m_idx;
};

typedef HardwareBreakpoint<HardwareBreakpoint_Windows> CHardwareBreakpoint;

#elif defined(XENON) // #if defined(WIN32) || defined(WIN64)

class HardwareBreakpoint_Xenon : public HardwareBreakpointBase
{
public:
	HardwareBreakpoint_Xenon()
	: m_address(0)
	, m_size(0)
	{
	}

	~HardwareBreakpoint_Xenon()
	{
		Clear();
	}

	bool Set(void* /*thread*/, void* address, Size size, Type type)
	{
		if (m_address)
		{
			assert(0);
			return false;
		}

		DWORD realSize = RealSize(size);
		HRESULT hr = DmSetDataBreakpoint(address, RealType(type), realSize);

		bool ret = hr == XBDM_NOERR;
		if (ret)
		{
			m_address = address;
			m_size = realSize;
		}

		return ret;
	}

	bool Clear()
	{
		bool ret = true;

		if (m_address)
		{
			HRESULT hr = DmSetDataBreakpoint(m_address, DMBREAK_NONE, m_size);
			ret = hr == XBDM_NOERR;

			m_address = 0;
			m_size = 0;
		}

		return ret;
	}

private:
	static DWORD RealType(Type type)
	{
		switch (type)
		{
		case TYPE_CODE: return DMBREAK_EXECUTE;
		case TYPE_WRITE: return DMBREAK_WRITE;
		case TYPE_READWRITE: return DMBREAK_READWRITE;
		default:
			assert(0);
			return DMBREAK_WRITE;
		};
	}

	static DWORD RealSize(Size size)
	{
		switch (size)
		{
		case SIZE_1: return 1;
		case SIZE_2: return 2;
		case SIZE_4: return 4;
		default:
			assert(0);
			return 4;
		};
	}

private:
	void* m_address;
	DWORD m_size;
};

typedef HardwareBreakpoint<HardwareBreakpoint_Xenon> CHardwareBreakpoint;

#else // #elif defined(XENON)

class HardwareBreakpoint_Null : public HardwareBreakpointBase
{
public:
	HardwareBreakpoint_Null() {}
	~HardwareBreakpoint_Null() {}

	bool Set(void* /*thread*/, void* /*address*/, Size /*size*/, Type /*type*/)
	{
		return true;
	}

	bool Clear()
	{
		return true;
	}
};

typedef HardwareBreakpoint<HardwareBreakpoint_Null> CHardwareBreakpoint;

#endif


#endif // #ifndef _HARDWAREBREAKPOINT_H_
