/*
	module to model finite difference methods 
*/

#ifndef FDM_H
#define FDM_H

#include<functional>
#include<memory>
#include<vector>
#include "rgn.h"
#include "sde.h"
#include "types.h"

template<typename T>
class FDM
{
using function_type = std::function<T (T, T)>;//how to avoid declaring this alias twice? to use param_type directly?

private:
	function_type strat_;
public:

	FDM(const function_type& s): strat_{s} {};
	
	std::vector<T> generatePath(T s, int t){
		std::vector<T> path{};
		for(int i = 0; i < t; i++){
			s += strat_(t, s);
			path.push_back(s);
		}
		
		return path;
	};
	
	using param_type = function_type;
};

template<typename T>
class EulerFDM
{
private:
	std::shared_ptr<SDE<T>> sde_;
	std::shared_ptr<RNG<T>> rng_;
public:
	EulerFDM(const std::shared_ptr<SDE<T>>& s, const std::shared_ptr<RNG<T>>& r): sde_{s}, rng_{r} {};
	
	T operator () (T t, T s){
		return sde_->drift(t, s) + sde_->diffusion(t, s) * (*rng_)();
	}
};

#endif