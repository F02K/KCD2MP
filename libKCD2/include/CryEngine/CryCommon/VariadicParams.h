/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2011.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Variadic parameters class.

-------------------------------------------------------------------------
History:
- 10:08:2011: Created by Paul Slinger

*************************************************************************/

#ifndef __VARIADICPARAMS_H__
#define __VARIADICPARAMS_H__

template <typename PARAM> class CVariadicParams
{
	public:

		typedef PARAM TParam;

		enum { MaxParamCount = 8 };

		inline CVariadicParams() : m_paramCount(0) {}

		inline CVariadicParams(const TParam &param1) : m_paramCount(1)
		{
			m_params[0] = param1;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2) : m_paramCount(2)
		{
			m_params[0] = param1;
			m_params[1] = param2;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3) : m_paramCount(3)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3, const TParam &param4) : m_paramCount(4)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
			m_params[3] = param4;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3, const TParam &param4, const TParam &param5) : m_paramCount(5)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
			m_params[3] = param4;
			m_params[4] = param5;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3, const TParam &param4, const TParam &param5, const TParam &param6) : m_paramCount(6)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
			m_params[3] = param4;
			m_params[4] = param5;
			m_params[5] = param6;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3, const TParam &param4, const TParam &param5, const TParam &param6, const TParam &param7) : m_paramCount(7)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
			m_params[3] = param4;
			m_params[4] = param5;
			m_params[5] = param6;
			m_params[6] = param7;
		}

		inline CVariadicParams(const TParam &param1, const TParam &param2, const TParam &param3, const TParam &param4, const TParam &param5, const TParam &param6, const TParam &param7, const TParam &param8) : m_paramCount(8)
		{
			m_params[0] = param1;
			m_params[1] = param2;
			m_params[2] = param3;
			m_params[3] = param4;
			m_params[4] = param5;
			m_params[5] = param6;
			m_params[6] = param7;
			m_params[7] = param8;
		}

		inline CVariadicParams(const CVariadicParams &rhs) : m_paramCount(rhs.m_paramCount)
		{
			for(size_t iParam = 0; iParam < m_paramCount; ++ iParam)
			{
				m_params[iParam] = rhs.m_params[iParam];
			}
		}

		inline bool PushParam(const TParam &param)
		{
#if TELEMETRY_DEBUG
			CRY_ASSERT(m_paramCount < MaxParamCount);
#endif //TELEMETRY_DEBUG

			m_params[m_paramCount ++] = param;

			return true;
		}

		inline size_t ParamCount() const
		{
			return m_paramCount;
		}

		inline const TParam & operator [] (size_t i) const
		{
#if TELEMETRY_DEBUG
			CRY_ASSERT(i < m_paramCount);
#endif //TELEMETRY_DEBUG

			return m_params[i];
		}

	private:

		TParam	m_params[MaxParamCount];

		size_t	m_paramCount;
};

#endif //__VARIADICPARAMS_H__