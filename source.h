/*
	module to get both model data and option data as a stream 
*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include "types.h"

class value_empty : std::exception
{
private:
	std::string message_;
public:
	value_empty(std::string str) : message_{str} {};
	virtual const char* what() const throw(){
		return message_.c_str();
	};
};

template<typename T>
class ModelStream
{
public:
	virtual void fillModel(ModelData<T>& ModelData) =0;
};

template<typename T>
class CsvModelStream : public ModelStream<T>
{
private:
	std::ifstream fstream_;
	int currentLine_ = 0;
	std::map<std::string, SdeType> strToSde{
		{"GME", SdeType::GME}
	};
	std::map<std::string, RngType> strToRng{
		{"NORMAL", RngType::NORMAL}, 
		{"UNIFORM", RngType::UNIFORM}
	};
	std::map<std::string, OptionType> strToOption{
		{"PLAIN_CALL", OptionType::PLAIN_CALL}, 
		{"ASIAN_CALL", OptionType::ASIAN_CALL}, 
		{"LOOKBACK_CALL", OptionType::LOOKBACK_CALL}
	};
	std::map<std::string, FdmType> strToFdm{
		{"EULER", FdmType::EULER}
	};
	
public:
	CsvModelStream(const std::string& str): fstream_{str} {};
	
	CsvModelStream(const std::ifstream& s) = delete;
	std::ifstream& operator= (const std::ifstream& s) = delete;
	
	CsvModelStream(const std::ifstream&& s): fstream_{s} {};
	void operator= (const std::ifstream&& s){fstream_ = s;};
	
	void fillModel(ModelData<T>& modelData){
		//too much sstream being used here?
		std::string line;
		std::getline(fstream_, line);
		
		while(line.front() == '#'){ //skip comments
			std::getline(fstream_, line);
		}
		
		std::stringstream ss{line};
		
		std::string word;
		currentLine_++;
		
		for(int i = 0; std::getline(ss, word, ','); i++){
			std::stringstream convert{word};
			if(word.empty()){
				throw value_empty{"Empty value at position " + std::to_string(i+1) + " line " + std::to_string(currentLine_)};
			}
			
			if(currentLine_ == 1){
				switch(i){
					case 0:
						convert >> std::get<0>(std::get<0>(modelData));
					break;
					case 1:
						convert >> std::get<1>(std::get<0>(modelData));
					break;
					case 2:
						std::get<2>(std::get<0>(modelData)) = strToSde[word];
					break;
					case 3:
						std::get<3>(std::get<0>(modelData)) = strToFdm[word];
					break;
					case 4:
						convert >> std::get<0>(std::get<1>(modelData));
					break;
					case 5:
						convert >> std::get<1>(std::get<1>(modelData));
					break;
					case 6:
						std::get<2>(std::get<1>(modelData)) = strToRng[word];
					break;
				}
			}else{
				switch(i){
					case 0:
						convert >> std::get<0>(std::get<2>(modelData));
					break;
					case 1:
						convert >> std::get<1>(std::get<2>(modelData));
					break;
					case 2:
						std::get<2>(std::get<2>(modelData)) = strToOption[word];
					break;
				}
			}
		}
	}
	
	std::ifstream& getStream(){return fstream_;}
};

template<typename T>
std::ifstream& operator>>(CsvModelStream<T>& source, ModelData<T>& modelData){
	source.fillModel(modelData);
	return source.getStream();
}
