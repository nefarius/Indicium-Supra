#pragma once
#include "SafeBlock.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <sstream>
#include <string>
#include <memory>

#define SERIALIZATION_READ(S, T, V) T V; S >> V;

class Serializer
{
	typedef boost::archive::text_iarchive iarchive;
	typedef boost::archive::text_oarchive oarchive;

	std::shared_ptr<oarchive> _oa;
	std::shared_ptr<iarchive> _ia;

	std::stringstream _ss;
	std::string _ss_str;

public:
	Serializer();
	Serializer(const char * const _data, const unsigned int len);

	~Serializer();

	const char *data();
	void setData(const char *szData, const size_t size);

	int numberOfBytesUsed() const;

	template<class T>
	Serializer& operator<<(const T& t)
	{
		write<T>(t);
		return *this;
	}
	template<class T>
	Serializer& operator>>(T& t)
	{
		read<T>(t);
		return *this;
	}

private:
	template<class T>
	void write(const T& t)
	{
		safeExecute([&](){
			*_oa & t;
		});
	}

	template<class T>
	void read(T& t)
	{
		safeExecute([&](){
			*_ia & t;
		});
	}
};
