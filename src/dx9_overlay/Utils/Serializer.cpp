#include "Serializer.h"

Serializer::Serializer()
{
	_oa = safeMakeShared<oarchive>(_ss);
}

Serializer::Serializer(const char * const _data, const unsigned int len) : _ss(std::string(_data, len))
{
	_ia = safeMakeShared<iarchive>(_ss);
}

Serializer::~Serializer()
{
	safeExecute([&](){
		_oa.reset();
		_ia.reset();
	});
}

const char * Serializer::data()
{
	_ss_str = _ss.str();
	return _ss_str.c_str();
}

void Serializer::setData(const char *szData, const size_t size)
{
	_ss = std::stringstream(std::string(szData, size));
	_ia = safeMakeShared<iarchive>(_ss);
}

int Serializer::numberOfBytesUsed() const
{
	return _ss.str().length();
}
