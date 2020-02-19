#ifndef TYPES_H
#define TYPES_H

#include<tuple>

enum class SdeType{
	GME
};

enum class RngType{
	NORMAL,
	UNIFORM
};

enum class FdmType{
	EULER
};

enum class OptionType{
	PLAIN_CALL,
	ASIAN_CALL,
	LOOKBACK_CALL
};

template<typename T>
using SdeModel = std::tuple<T, T, SdeType, FdmType>;
template<typename T>
using RngModel = std::tuple<T, T, RngType>;
template<typename T>
using OptionModel = std::tuple<T, T, OptionType>;
template<typename T>
using ModelData = std::tuple< SdeModel<T>, RngModel<T>, OptionModel<T> >;

#endif