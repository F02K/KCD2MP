#ifndef _LOADSEQ_H_
#define _LOADSEQ_H_ 1

#if defined(PS3)
//#define USE_LOADSEQ 1
#endif

#if defined(USE_LOADSEQ)
namespace LoadSeq
{
	enum IOMode { PASSTHROUGH, PLAYBACK, RECORD, DELETE };
	void Mode(IOMode);
	void SetAutoRecord(bool);
	void Cancel();
	void Begin(const char *);
	void End(const char *);

	class Loading
	{
		const char *m_Name;
		bool m_bLoading;

		Loading();
		Loading(const Loading &);

	public:
		Loading(const char *name) : m_Name(name), m_bLoading(true)
		{
			LoadSeq::Begin(name);
		}

		~Loading()
		{
			if (m_bLoading)
				LoadSeq::End(m_Name);
		}

		void Done()
		{
			if (m_bLoading)
				LoadSeq::End(m_Name);
			m_bLoading = false;
		}
	};
};

#define BEGIN_LOADING(NAME) (LoadSeq::Begin((NAME)))
#define END_LOADING(NAME) (LoadSeq::End((NAME)))
#define LOADING(NAME) LoadSeq::Loading _loading((NAME));
#define LOADING_DONE (_loading.Done())
#else
#define BEGIN_LOADING(NAME) ((void)0)
#define END_LOADING(NAME) ((void)0)
#define LOADING(NAME) ((void)0)
#if defined(PS3)
	namespace NMemCont{	extern void Activate();}
	#define LOADING_DONE {NMemCont::Activate();}	//activate big heap allocator
#else
	#define LOADING_DONE ((void)0)
#endif
#endif

#endif

