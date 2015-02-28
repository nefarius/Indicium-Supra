#include "Bitstream.h"

Serializer::Serializer()
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

Serializer::Serializer(const char * const _data, const unsigned int len) : _ss(std::string(_data, len))
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

Serializer::~Serializer()
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

const char * Serializer::GetData()
{
	_ss_str = _ss.str();
	return _ss_str.c_str();
}

int Serializer::GetNumberOfBytesUsed() const
{
	return _ss.str().length();
}

void Serializer::SetData(const char *szData, const size_t size)
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

