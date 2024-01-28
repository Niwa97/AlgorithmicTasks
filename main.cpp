#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string.h>

//1.********************************************************************************************************************************************************

std::string compress(std::string str){ //works for small letters
	int array[26];
	for(int i = 0; i < 26; ++i){
		array[i] = 0;
	}
	char arr[str.length() + 1]; 
    strcpy(arr, str.c_str()); 
    char holder;	
	for(int i = 0; i < str.length() + 1; ++i){ //apperances of i-th letter is remembered  under index i
		holder = arr[i];
		array[static_cast<int>(holder) - 'a']++;	
	}
	std::string result = "";
	std::stringstream converter;
	for(int i = 0; i < 26; ++i){
		if(array[i] != 0){
			holder = i + 'a'; //letter and number of apperances are written to string
			converter << holder;
			converter << array[i];
			result += converter.str();
		}
	converter.str(std::string());
	}
	return result;
}

std::string decompress(std::string str){ //string must be in form aibj... etc
	char arr[str.length() + 1]; 
    strcpy(arr, str.c_str()); 
    std::string coresult = "";
    std::stringstream converter;
    
    for(int i = 0; i < str.length() + 1; i+=2){  //counting every two - index i is always a letter and i+1 number of iterations
    	for(int j = 0; j < (arr[i+1] - '0'); ++j){
    		converter << arr[i];
		}
		coresult += converter.str();
		converter.str(std::string());
	}
return coresult;
}

//2.******************************************************************************************************************************************************** 

std::vector<int> del(std::vector<int> V, int index){ //implemented for integeers deletes in time O(1) element of given index
	if(index < 0 || index >= V.size()){
		return V;
	}
	else if(index  == V.size() - 1){
		V.pop_back();
		return V;
	}
	else{
		V[index] = V[V.size() - 1]; //if element is not last and in range we assign value of last one into given index nd then delete the last one
		V.pop_back(); 
		return V;
	} //function works the same with any size so complexity is constant
}


int main(int argc, char** argv) {
    std::cout << compress("tumultuous") << std::endl;
	return 0;
}
