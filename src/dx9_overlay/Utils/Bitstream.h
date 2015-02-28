#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/shared_ptr.hpp>

#include <sstream>
#include <string>

#define SERIALIZATION_READ(S, T, V) T V; S >> V;

class CSerializer
{
	boost::shared_ptr<boost::archive::text_oarchive> _oa;
	boost::shared_ptr<boost::archive::text_iarchive> _ia;

	std::stringstream _ss;
	std::string _ss_str;

public:
	CSerializer();

	CSerializer(const char * const _data, const unsigned int len);

	~CSerializer();

	const char *GetData();

	int GetNumberOfBytesUsed() const;

	void SetData(const char *szData, const size_t size);

	template<class T> void Write(const T& t)
	{
		try
		{
			if (!_oa)
				return;

			*_oa & t;
		}
		catch (...)
		{
		}
	}

	template<class T> void Read(T &t)
	{
		try
		{
			if (!_ia)
				return;

			*_ia & t;
		}
		catch (...)
		{
		}
	}

	template<class T> CSerializer& operator<<(const T& t)
	{
		Write<T>(t);
		return *this;
	}

	template<class T> CSerializer& operator>>(T& t)
	{
		Read<T>(t);
		return *this;
	}
};
