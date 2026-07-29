#ifndef __IONLINE_H__
#define __IONLINE_H__

#pragma once

#include "CryOnline.h"
#include "functor.h"
#include <IXml.h> // <> required for Interfuscator

class CErrorHandlingPolicy
{
public:
	typedef Functor2<EOnlineError, const char *> TErrback;

	void BindErrback(const TErrback & errback)
	{
		assert(m_errback.getCallee() == 0);
		m_errback = errback;
	}

	void DoErrback(const XmlNodeRef & error)
	{
		if (!m_errback.getCallee())
			return;

		int code = 0;
		error->getAttr("code", code);
		const char * desc = error->getContent();
		m_errback((EOnlineError)code, desc);
	}

	void DoErrback(EOnlineError code, const char * desc)
	{
		if (!m_errback.getCallee())
			return;

		m_errback(code, desc);
	}

private:
	TErrback  m_errback;
};

class CErrorHandlingPolicy2
{
public:
	typedef Functor0 TErrback;

	void BindErrback(const TErrback & errback)
	{
		assert(m_errback.getCallee() == 0);
		m_errback = errback;
	}

	void DoErrback(const XmlNodeRef & error)
	{
		if (!m_errback.getCallee())
			return;
		m_errback();
	}

	void DoErrback(EOnlineError code, const char * desc)
	{
		if (!m_errback.getCallee())
			return;
		m_errback();
	}

private:
	TErrback  m_errback;
};

template<typename Serializer, typename ErrorHandlingPolicy = CErrorHandlingPolicy>
class COnlineQueryBinder : 
	public IOnlineQueryBinder,
	public Serializer,
	public ErrorHandlingPolicy
{
public:
	typedef typename Serializer::TCallback        TCallback;
	typedef std::vector<XmlNodeRef>               TQueries;
	
	COnlineQueryBinder()
	{
		CryOnlineGetInstance()->RegisterConnectionListener(this, Serializer::GetDomain());
	}

	COnlineQueryBinder(const COnlineQueryBinder& binder)
	{
		*this = binder;
		CryOnlineGetInstance()->RegisterConnectionListener(this, Serializer::GetDomain());
	}

	~COnlineQueryBinder()
	{
		CryOnlineGetInstance()->UnregisterQueryBinder(this);
		CryOnlineGetInstance()->UnregisterConnectionListener(this);
	}

	virtual void OnConnectionAvailable(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionLost(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionTick(IOnlineConnection * connection)
	{
		for (TQueries::iterator it = m_queries.begin(); it != m_queries.end(); ++it)
		{
			XmlString s = (*it)->getXML();
			connection->Query(s.c_str(), Serializer::GetQueryReceiverId(), this);
		}
		m_queries.clear();
	}

	virtual void OnQueryCompleted(const char * xml, int queryId)
	{
		XmlNodeRef xmlNode = gEnv->pSystem->LoadXmlFromString(xml);

		XmlNodeRef error = xmlNode->findChild("error");
		if (!error)
		{
			if (!Serializer::OnResponse(xmlNode->findChild("query"), m_callback))
			{
				ErrorHandlingPolicy::DoErrback(eOnlineError_ParseError, "Query result parsing error");
			}
		}
		else
		{
			ErrorHandlingPolicy::DoErrback(error);
		}
	}

	virtual void OnRequest(const char * xml, IOnlineConnection * connection)
	{
		XmlNodeRef xmlNode = gEnv->pSystem->LoadXmlFromString(xml);
		Serializer::OnResponse(xmlNode->findChild("query"), m_callback);
		string result = xmlNode->getXML();

		connection->Response(result.c_str());
	}

	void BindCallback(const TCallback & callback)
	{
		assert(m_callback.getCallee() == 0);
		m_callback = callback;
	}

	void BindCallback(const TCallback & callback, const char * tag)
	{
		assert(m_callback.getCallee() == 0);
		CryOnlineGetInstance()->RegisterQueryBinder(this, tag);
		m_callback = callback;
	}

	void Request()
	{
		m_queries.push_back(Serializer::OnRequest());
	}

	template <typename P1>
	void Request(const P1 & p1)
	{
		m_queries.push_back(Serializer::OnRequest(p1));
	}

	template <typename P1, typename P2>
	void Request(const P1 & p1, const P2 & p2)
	{
		m_queries.push_back(Serializer::OnRequest(p1, p2));
	}

	template <typename P1, typename P2, typename P3>
	void Request(const P1 & p1, const P2 & p2, const P3 & p3)
	{
		m_queries.push_back(Serializer::OnRequest(p1, p2, p3));
	}

	template <typename P1, typename P2, typename P3, typename P4>
	void Request(const P1 & p1, const P2 & p2, const P3 & p3, const P4 & p4)
	{
		m_queries.push_back(Serializer::OnRequest(p1, p2, p3, p4));
	}

private:
	TCallback m_callback;
	TQueries  m_queries;
};

template<typename Serializer>
class COnlineDownloadBinder : 
	public IOnlineDownloadBinder,
	public Serializer
{
public:
	typedef typename Serializer::TCallback        TCallback;
	typedef Functor2<EOnlineError, const char *>  TErrback;
	typedef std::vector<string>                   TDownloads;

	COnlineDownloadBinder()
	{
		CryOnlineGetInstance()->RegisterConnectionListener(this, Serializer::GetDomain());
	}

	~COnlineDownloadBinder()
	{
		CryOnlineGetInstance()->UnregisterConnectionListener(this);
	}

	virtual void OnConnectionAvailable(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionLost(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionTick(IOnlineConnection * connection)
	{
		for (TDownloads::iterator it = m_downloads.begin(); it != m_downloads.end(); ++it)
		{
			connection->Download(it->c_str(), this);
		}
		m_downloads.clear();
	}

	virtual void OnDownloadCompleted(const char * filename, const void * data, int size)
	{
		Serializer::OnDownloadCompleted(filename, data, size, m_callback);
	}

	void BindCallback(const TCallback & callback)
	{
		assert(m_callback.getCallee() == 0);
		m_callback = callback;
	}

	void Request(const char * filename)
	{
		m_downloads.push_back(filename);
	}

private:
	TCallback  m_callback;
	TErrback   m_errback;
	TDownloads m_downloads;
};

template<typename Serializer>
class COnlineUploadBinder : 
	public IOnlineUploadBinder,
	public Serializer
{
public:
	struct SUploadDescriptor
	{
		SUploadDescriptor(const char * uploadName, const void * uploadData, int uploadSize) : 
			filename(uploadName), 
			data(reinterpret_cast<const char *>(uploadData), reinterpret_cast<const char *>(uploadData) + uploadSize)
		{
		}

		string            filename;
		std::vector<char> data;
	};

	typedef typename Serializer::TCallback       TCallback;
	typedef Functor2<EOnlineError, const char *> TErrback;
	typedef std::vector<SUploadDescriptor>       TUploads;

	COnlineUploadBinder()
	{
		CryOnlineGetInstance()->RegisterConnectionListener(this, Serializer::GetDomain());
	}

	~COnlineUploadBinder()
	{
		CryOnlineGetInstance()->UnregisterConnectionListener(this);
	}

	virtual void OnConnectionAvailable(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionLost(IOnlineConnection * connection)
	{
	}

	virtual void OnConnectionTick(IOnlineConnection * connection)
	{
		for (typename TUploads::iterator it = m_uploads.begin(); it != m_uploads.end(); ++it)
		{
			connection->Upload(it->filename.c_str(), &(it->data[0]), it->data.size(),  Serializer::GetReceiverId(), Serializer::GetResource(), this);
		}
		m_uploads.clear();
	}

	virtual void OnUploadCompleted(const char * filename)
	{
		Serializer::OnUploadCompleted(filename, m_callback);
	}

	void BindCallback(const TCallback & callback)
	{
		assert(m_callback.getCallee() == 0);
		m_callback = callback;
	}

	void Request(const char * filename, const void * data, int size)
	{
		m_uploads.push_back(SUploadDescriptor(filename, data, size));
	}

private:
	TCallback m_callback;
	TErrback  m_errback;
	TUploads  m_uploads;
};

#endif // __IONLINE_H__