/*
	module to model stochastic diferential equations
*/

#ifndef SDE_H
#define SDE_H

#include<tuple>
#include<functional>

template<typename T>
class SDE
{
using function_type = std::function<T(T , T)>;
private:
	function_type drift_;
	function_type diffusion_;
	
public:
	SDE(const std::tuple<function_type, function_type> functions): 
		drift_{std::get<0>(functions)}, diffusion_{std::get<1>(functions)} {};
		
	SDE(function_type dr, function_type diff): drift_{dr}, diffusion_{diff} {};
	
	function_type drift = drift_;

	function_type diffusion = diffusion_;
};

#endif