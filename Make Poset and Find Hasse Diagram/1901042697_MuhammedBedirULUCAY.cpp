#include <iostream>
#include <fstream>
#include <vector>

class SubSet{				//SubSet class and base properties
	public:
		SubSet(){};
		SubSet(char x, char y) : first(x), second(y) {}
		char first,second;
		bool operator==(const SubSet& obj){
			return first == obj.first && second == obj.second;
		}
		friend std::ostream& operator<<(std::ostream& sout, const SubSet& obj);
};

std::ostream& operator<<(std::ostream& sout, const SubSet& obj){		//Subset overload operator stream extraction 
	sout << "(" << obj.first << ", " << obj.second << ")";
	return sout;
}

class POSET{

	public:

		POSET();
		void push_elements(char& element);	//Add new element in class elements
		void push_subSet(SubSet& subset);	//Add new subset in class Subsets
		void clear();

		SubSet reverse(const SubSet& tmp);

		bool IsPOSET();						//checkin all rules to be reflexive
		bool IsReflexive();					//Checking reflexive at the same time making reflexive
		bool IsAntiSymmetric();				//Checking AntiSymmetric at the same time making AntiSymmetric
		bool IsTransitive();				//Checking Transitive at the same time making Transitive

		bool searchIn(const SubSet& subset) const;			//Search subset in Subset there is or not
		bool isNotWrite(const SubSet& subset) const;		//We keep not writing data in Not write checking in it

		void checkError();
		bool checkElements(const SubSet& subset) const;
		bool getErrorStatus(){	checkError();
			return errorHandling;}

		friend std::ostream& operator<<(std::ostream& sout, const POSET& obj); 

	private:

		bool errorHandling = false;
		std::vector<SubSet> SubSets;
		std::vector<SubSet> NotWrite;
		std::vector<char> elements;
};

void readInputFile(std::vector<POSET> &mainVec){

	int size;
	SubSet tmpSub;
	POSET tmpPoset;
	std::string fname = "input.txt", 
				elements, subset;
	
	std::ifstream file (fname);
	if(!file.is_open())
		std::cout << fname << " could not opened" << std::endl;

	while(file >> size && !file.eof()){		//Reading input file
		file >> elements;

		for(auto el: elements){				//Select all elements
			if(el != ',')
				tmpPoset.push_elements(el);   //we assaing elements to the temp object
		}
		for(int i=0 ; i < size ; ++i ){		//Read subset	
			file >> subset;
			tmpSub.first = subset[0] ;		//elements are setting as subset	
			tmpSub.second = subset[2] ;							
			tmpPoset.push_subSet(tmpSub);		//Assaign the subset temp obj
		}
		mainVec.push_back(tmpPoset);		//Add main Set obj
		tmpPoset.clear();					//Clearing tmp obj
	}	
}

int main(){

	std::vector<POSET> mainVec;				//Keep all sets data 
	readInputFile(mainVec);					// Reading input file for all sets

	std::ofstream file("output.txt");

	for(auto poset : mainVec){				//Doing process in IsPoset method
		file << "n\nPOSET: ";
		if(!poset.getErrorStatus()){
			poset.IsPOSET();
			file << poset;
		}else{
			file << "\n";
		}
	}
	std::cout << " output.txt was created \n";
	file.close();

	return 0;
}

POSET::POSET()
{}

void POSET::push_elements(char& element){	//add new element
	elements.push_back(element);
}
void POSET::push_subSet(SubSet& subset){	//add new subset
	SubSets.push_back(subset);
}
void POSET::clear(){						//to clerar tmp obj
	elements.clear();
	SubSets.clear();
}

std::ostream& operator<<(std::ostream& sout, const POSET& obj){			//POSET overload operator stream extraction to print file as wanted

	for(int i=0 ; i< obj.SubSets.size(); ++i){				//Writirng all subset
		sout << obj.SubSets[i];
		if(i != obj.SubSets.size()-1)
			sout << ", ";
	}
	sout << "\n";

	for(int i=0; i<obj.SubSets.size(); ++i){				//Writing Hash Table
		if(!obj.isNotWrite(obj.SubSets[i]) && obj.SubSets[i].first != obj.SubSets[i].second)
			sout << obj.SubSets[i].first << "," << obj.SubSets[i].second << "\n";
	}

	return sout;
}

bool POSET::searchIn(const SubSet& subset) const{		//Looking for the subset is there in Subsets vector in class

	for(auto SubSet : SubSets)
		if(SubSet == subset)
			return true;
	return false;
}

bool POSET::isNotWrite(const SubSet& subset) const{		//Looking for the subset is there in NotWrite vector in class
	for(auto SubSet : NotWrite)
		if(SubSet == subset)
			return true;
	return false;
}	

SubSet POSET::reverse(const SubSet& tmp){				//this method take a subset like (a,b) and returning (b,a)
	return SubSet(tmp.second,tmp.first);				//reverce place of elements
}

void POSET::checkError(){	
	for(int i=0; i<SubSets.size(); ++i){			//Sending subset to check their elements
		if(!checkElements(SubSets[i])){
			errorHandling = true;
			break;
		}
	}
}
bool POSET::checkElements(const SubSet& subset) const{
	int counter = 0;
	for(auto element : elements){		//Counter must be 2 other wise there is an error
		if(subset.first == subset.second && subset.first == element)	//(a,a)
			counter += 2;
		else if(subset.first == element || subset.second == element)	//(a,b)
			counter++;
	}
	return counter == 2;
}

bool POSET::IsPOSET(){
	return IsReflexive() && IsAntiSymmetric() && IsTransitive();		//We checking all of rules
}

bool POSET::IsReflexive() {
	int counter = 0;
	for(auto element : elements){				// We are searching all elemets reflexive 
		SubSet tmp(element,element);
		if(!searchIn(tmp))						//if it is not adding it
			push_subSet(tmp);
	}
	return true;
}

bool POSET::IsAntiSymmetric() {
	
	for(int i=0; i<SubSets.size(); ++i){			//We are search is there un make AntiSymmetric to remove subset
		SubSet tmp = reverse(SubSets[i]);
		if(tmp.first != tmp.second && searchIn(tmp))	//(a,a) not checking but if there is (a,b) look for (b,a) if there is (b,a) too	
			SubSets.erase(SubSets.begin() + i);			//Deleting (a,b)
	}
	return true;
}

bool POSET::IsTransitive(){

	for(int i=0; i<SubSets.size(); ++i){
		for(int j=0; j<SubSets.size(); ++j){

			if(SubSets[i].first == SubSets[i].second)		//To make less complexity 
				break;
			
			/*	if subset is not like (a,a) and 
				there is (a,b) (b,e) then check is there (a,e) if it is not add
				also add in NotWrite veftor in class
			*/
			if(SubSets[j].first != SubSets[j].second && SubSets[i].second == SubSets[j].first){		
				SubSet tmp(SubSets[i].first, SubSets[j].second);									
				if(!searchIn(tmp))
					push_subSet(tmp);
				NotWrite.push_back(tmp);
			}
		}
	}

	return true;
}