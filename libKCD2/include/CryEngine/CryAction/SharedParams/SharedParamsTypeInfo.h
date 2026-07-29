/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2010.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Shared parameters type information.

-------------------------------------------------------------------------
History:
- 15:07:2010: Created by Paul Slinger

*************************************************************************/

#ifndef __SHAREDPARAMSTYPEINFO_H__
#define __SHAREDPARAMSTYPEINFO_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include <StringUtils.h>

#ifdef _DEBUG
#define DEBUG_SHARED_PARAMS_TYPE_INFO	1
#else
#define DEBUG_SHARED_PARAMS_TYPE_INFO	0
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared parameters type information class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CSharedParamsTypeInfo
{
	public:

		CSharedParamsTypeInfo(size_t size, const char *pName, const char *pFileName, uint32 line) : m_size(size)
		{
			if(pName)
			{
				size_t	length = strlen(pName), pos = 0;

				if(length > MaxNameLength)
				{
					pos = length - MaxNameLength;
				}

				strncpy(m_name, pName + pos, MaxNameLength);
			}
			else
			{
				m_name[0] = '\0';
			}

			if(pFileName)
			{
				size_t	length = strlen(pFileName), pos = 0;

				if(length > MaxFileNameLength)
				{
					pos = length - MaxFileNameLength;
				}

				strncpy(m_fileName, pFileName + pos, MaxFileNameLength);
			}
			else
			{
				m_fileName[0] = '\0';
			}
			
			m_line = line;

			CryFixedStringT<256>	temp;

			temp.Format("%d%s%s%d", size, m_name, m_fileName, m_line);

			m_uniqueId = CryStringUtils::CalculateHash(temp.c_str());
		}

		inline size_t GetSize() const
		{
			return m_size;
		}

		inline const char *GetName() const
		{
			return m_name;
		}

		inline const char *GetFileName() const
		{
			return m_fileName;
		}

		inline uint32 GetLine() const
		{
			return m_line;
		}

		inline uint32 GetUniqueId() const
		{
			return m_uniqueId;
		}

		inline bool operator == (const CSharedParamsTypeInfo &right) const
		{
#if DEBUG_SHARED_PARAMS_TYPE_INFO
			if(m_uniqueId == right.m_uniqueId)
			{
				CRY_ASSERT(m_size == right.m_size);

				CRY_ASSERT(!strcmp(m_name, right.m_name));

				CRY_ASSERT(!strcmp(m_fileName, right.m_fileName));

				CRY_ASSERT(m_line == right.m_line);
			}
#endif //DEBUG_SHARED_PARAMS_TYPE_INFO

			return m_uniqueId == right.m_uniqueId;
		}

		inline bool operator != (const CSharedParamsTypeInfo &right) const
		{
#if DEBUG_SHARED_PARAMS_TYPE_INFO
			if(m_uniqueId == right.m_uniqueId)
			{
				CRY_ASSERT(m_size == right.m_size);

				CRY_ASSERT(!strcmp(m_name, right.m_name));

				CRY_ASSERT(!strcmp(m_fileName, right.m_fileName));

				CRY_ASSERT(m_line == right.m_line);
			}
#endif //DEBUG_SHARED_PARAMS_TYPE_INFO

			return m_uniqueId != right.m_uniqueId;
		}

	private:

		enum { MaxNameLength = 63, MaxFileNameLength = 63 };

		size_t	m_size;

		char		m_name[MaxNameLength + 1], m_fileName[MaxFileNameLength + 1];

		uint32	m_line, m_uniqueId;
};

#endif //__SHAREDPARAMSTYPEINFO_H__