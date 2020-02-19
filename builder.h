/*
	define factory functions to build instances of the other modules
*/

#include<memory>
#include<functional>
#include<tuple>
#include "types.h"
#include "rgn.h"
#include "sde.h"
#include "fdm.h"
#include "pricer.h"

class unknow_option : std::exception
{
private:
	std::string message_;
public:
	unknow_option(std::string str) : message_{str} {};
	virtual const char* what() const throw(){
		return message_.c_str();
	};
};

template<typename T>
//new SDEs, FMDs and pricers can be made 'on the fly' using lambdas
std::shared_ptr<SDE<T>> build_gme(T mu, T vol)
{
	auto drift = [mu](T t, T s){
		return mu * s;
	};

	auto diff = [vol](T t, T s){
		return vol * s;
	};

	return std::make_shared<SDE<T>>(drift, diff);
}

template <typename T>
std::shared_ptr<FDM<T>> build_fdm(ModelData<T> model)
{
	T mu, vol, min, max;
	SdeType sdeType;
	RngType rngType;
	FdmType fdmType;
	std::tie(mu, vol, sdeType, fdmType) = std::get<0>(model);
	auto rngModel = std::get<1>(model);
	
	std::shared_ptr< SDE<T> > sde;
	
	switch(sdeType){
	case SdeType::GME:
		sde = build_gme<T>(mu, vol);
	break;
	default:
		throw unknow_option{"Unknow SDE type"};
	}
	
	std::shared_ptr< RNG<T> > rng;
	
	switch(std::get<2>(rngModel)){
	case RngType::NORMAL:
		rng = std::make_shared< RNG<double> >(NormalRNG<double>{rngModel});
	break;
	case RngType::UNIFORM:
		rng = std::make_shared< RNG<double> >(UniformRNG<double>{rngModel});
	default:
		throw unknow_option{"Unknow RNG type"};
	}
	
	std::shared_ptr< FDM<double> > fdm;
	
	switch(fdmType){
	case FdmType::EULER :
	{
		fdm = std::make_shared< FDM<double> >(EulerFDM<double>{sde, rng});
	}
	break;
	default:
		throw unknow_option{"Unknow FDM type"};
	}
	
	return fdm;
}

template<typename T>
std::shared_ptr<Pricer<T>> build_pricer(OptionType optType, T S)
{
	std::shared_ptr<Pricer<T>> call;
	switch(optType){
	case OptionType::PLAIN_CALL:
		call = std::make_shared<Pricer<T>>(PlainCallPricer<T>{S});
	break;
	case OptionType::ASIAN_CALL:
		call = std::make_shared<Pricer<T>>(AsianCallPricer<T>{S});
	break;
	case OptionType::LOOKBACK_CALL:
		call = std::make_shared<Pricer<T>>(LookbackCallPricer<T>{S});
	break;
	}
	
	return call;
}