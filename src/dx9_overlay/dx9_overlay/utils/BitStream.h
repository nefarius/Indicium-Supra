#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <sstream>
#include <string>


class BitStream
{
	boost::shared_ptr<boost::archive::text_oarchive> _oa;
	boost::shared_ptr<boost::archive::text_iarchive> _ia;

	std::stringstream _ss;
	std::string _ss_str;

public:
	BitStream()
	{
		try
		{
			_oa.reset(new boost::archive::text_oarchive(_ss));
		}
		catch (...)
		{
			_oa.reset();
		}
	}

	BitStream(const char * const _data, const unsigned int len, bool dummy) : _ss(std::string(_data, len))
	{
		try
		{
			_ia.reset(new boost::archive::text_iarchive(_ss));
		}
		catch (...)
		{
			_ia.reset();
		}
	}

	~BitStream()
	{
		try
		{
			_oa.reset();
			_ia.reset();
		}
		catch (...)
		{
		}
	}

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

	template<class T> BitStream& operator<<(const T& t)
	{
		Write<T>(t);
		return *this;
	}

	template<class T> BitStream& operator>>(T& t)
	{
		Read<T>(t);
		return *this;
	}

	const char *GetData()
	{
		_ss_str = _ss.str();
		return _ss_str.c_str();
	}

	int GetNumberOfBytesUsed() const
	{
		return _ss.str().length();
	}

	void SetData(const char *szData, const size_t size)
	{
		_ss = std::stringstream(std::string(szData, size));

		try
		{
			_ia.reset(new boost::archive::text_iarchive(_ss));
		}
		catch (...)
		{
			_ia.reset();
		}
	}
};
