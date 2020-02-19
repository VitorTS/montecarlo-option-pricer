#include<iostream>
#include "mediator.h"
int main()
{	
	Mediator<double> med{"source.csv"};
	double value;
	
	for(int i = 0; !med.eof(); i++){
		value = med.priceNextOption(1000);
		std::cout << "Price for option " + std::to_string(i+1) << " : " << value << '\n';
	}
	
	return 0;
}