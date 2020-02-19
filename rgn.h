/*
	module to define variates
*/

#ifndef RNG_H
#define RNG_H

#include<functional>
#include<random>

template<typename T>
class RNG
{
private:
	std::function<T()> randStrat_;
	
public:
	RNG(const std::function<T()>& f): randStrat_{f} {};
	
	T operator() () {return randStrat_(); };
};

template<typename T>
class NormalRNG
{
private:
	std::normal_distribution<T> dist_;
	std::default_random_engine eng_;
	
public:
	NormalRNG(const RngModel<T>& rngModel): dist_{std::get<0>(rngModel), std::get<1>(rngModel)} {reseed();};
	
	T operator() () {return dist_(eng_); };
	
	void reseed(){
		eng_.seed(std::random_device{}());
	}
};

template<typename T>
class UniformRNG
{
private:
	std::uniform_real_distribution<T> dist_;
	std::default_random_engine eng_;
	
public:
	UniformRNG(const RngModel<T>& rngModel): dist_{std::get<0>(rngModel), std::get<1>(rngModel)} {reseed();};
	
	T operator() () {return dist_(eng_); };
	
	void reseed(){
		eng_.seed(std::random_device{}());
	}
};

#endif