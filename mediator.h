/*
	Mediator to put together all modules
*/

#include<functional>
#include<random>
#include<iostream>
#include<vector>
#include<memory>
#include "source.h"
#include "rgn.h"
#include "sde.h"
#include "fdm.h"
#include "pricer.h"
#include "builder.h"

template<typename T>
class Mediator final{
private:
	std::shared_ptr<FDM<T>> fdm_;
	ModelData<T> model_;
	CsvModelStream<T> source_;
public:
	Mediator(const std::string& sourceFile): source_{sourceFile}
	{
		source_ >> model_;
		fdm_ = build_fdm<T>(model_);
	};
	
	T priceNextOption(int number_of_paths)
	{
		source_ >> model_;
		
		T S;
		int t;
		OptionType optionType;
		std::tie(S, t, optionType) = std::get<2>(model_);
		
		auto pricer = build_pricer(optionType, S);
		
		T sum;
		for(int i = 0; i < number_of_paths; i++){
			std::vector<T> path = fdm_->generatePath(S, t);
			sum+= (*pricer)(path);
		}
		
		return sum / number_of_paths;
	};
	
	bool eof() {return source_.getStream().eof();};
};