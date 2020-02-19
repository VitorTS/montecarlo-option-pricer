/*
	module to implement different options as pricing functors
*/

#ifndef PRICER_H
#define PRICER_H

#include<functional>
#include<vector>
#include<algorithm>

template<typename T>
//accepts arbitrary pay offs to create novel options on the fly
class Pricer
{
using function_type = std::function<T(std::vector<T>)>;
private:
	function_type priceStrat_;
public:
	Pricer(const function_type& strat): priceStrat_{strat} {};
	T operator () (std::vector<T> path){
		return priceStrat_(path);
	};
	
	void setStrat(const function_type& strat){priceStrat_ = strat;}
};

template<typename T>
class PlainCallPricer
{
private:
	T S_;
public:
	PlainCallPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		return std::max(path.back() - S_, 0.0);
	}
};

//can be used by itself or as part of Pricer
template<typename T>
class AsianCallPricer
{
private:
	T S_;
public:
	AsianCallPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		T sum;
		//apparently there's no straightforward STL function for a std::vector sum.. weird
		for(const auto& value: path){
			sum += value;
		}
		
		return std::max((sum / path.size()) - S_, 0.0);
	}
};

template<typename T>
class LookbackCallPricer
{
private:
	T S_;
public:
	LookbackCallPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		T biggestPrice = 0.0;
		for(const T& value : path){
			if(value > biggestPrice){
				biggestPrice = value;
			}
		};
		return std::max(biggestPrice - S_, 0.0);
	}
};

template<typename T>
class PlainPutPricer
{
private:
	T S_;
public:
	PlainPutPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		return std::max(S_ - path.back(), 0.0);
	}
};

template<typename T>
class AsianPutPricer
{
private:
	T S_;
public:
	AsianPutPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		T sum;
		for(const auto& value: path){
			sum += value;
		}
		
		return std::max(S_ - (sum / path.size()), 0.0);
	}
};

template<typename T>
class LookbackPutPricer
{
private:
	T S_;
public:
	LookbackPutPricer(const T& s): S_{s} {};
	T operator () (std::vector<T> path){
		T biggestPrice = 0.0;
		for(const T& value : path){
			if(value > biggestPrice){
				biggestPrice = value;
			}
		};
		return std::max(S_ - biggestPrice, 0.0);
	}
};

#endif

