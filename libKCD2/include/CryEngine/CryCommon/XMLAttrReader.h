#pragma once

#ifndef XMLAttrReader_h
#define XMLAttrReader_h

template <typename T>
class CXMLAttrReader
{
	typedef std::pair<const char*, T> TRecord;
	std::vector<TRecord> m_dictionary;

public: 
	void Reserve(const size_t elementCount)
	{
		m_dictionary.reserve(elementCount);
	}

	void Add(const char* szKey, T tValue) { m_dictionary.push_back(TRecord(szKey, tValue)); }

	bool Get(const XmlNodeRef& node, const char* szAttrName, T& tValue, bool bMandatory = false)
	{
		const char* szKey = node->getAttr(szAttrName);
		for (typename std::vector<TRecord>::iterator it = m_dictionary.begin(), end = m_dictionary.end(); it != end; ++it)
		{
			if (!stricmp(it->first, szKey))
			{
				tValue = it->second;
				return true;
			}
		}

		if (bMandatory)
		{
			CryWarning(VALIDATOR_MODULE_UNKNOWN, VALIDATOR_WARNING, "Unable to get mandatory attribute '%s' of node '%s' at line %d.",
				szAttrName, node->getTag(), node->getLine());
		}

		return false;
	}

	const char* GetFirstRecordName(const T value)
	{
		for (typename std::vector<TRecord>::iterator it = m_dictionary.begin(), end = m_dictionary.end(); it != end; ++it)
		{
			if (it->second == value)
			{
				return it->first;
			}
		}

		return NULL;
	}
};

#endif // XMLAttrReader_h