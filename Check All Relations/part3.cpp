/*
	Muhammed Bedir ULUCAY
	1901042697
	CSE 211 Hw2 Part 3
*/

#include <iostream>
#include <vector>
#include <fstream>

struct SubSet{
	char first,second;
};
/*=====================================================================================================*/

class Set{

	public:
		bool getExceptionHandling(){return ExceptionHandling;};

		void Reflexive() ;
		void Symmetric() ;
		void Antisymmetric() ;
		void Transitive() ;
		void addText(std::string tmp);
		bool searchInList(SubSet tmp)const ;

		void clear();
		void push_elements(char element);
		void push_subSet(SubSet subset);
		void checkSubSets();
		bool checkSubSet(const SubSet& subset);

		friend std::ostream& operator<<(std::ostream& sout,const Set& writer);		//Obje datalarini yazdiriyor

	private:
		std::string text = "\n";			//String to print file for all object
		bool ExceptionHandling = false;
		std::vector<SubSet> SubSets;		//Keep reading subsets from file
		std::vector<char> elements;			//Elements of Set in reading file elements
};
/*=====================================================================================================*/

void readInputFile(std::vector<Set> &mainVec);

std::string setString(char ch1,char ch2){
	std::string str;
	str.push_back('(');
	str.push_back(ch1);	
	str.push_back(',');		
	str.push_back(ch2);
	str.push_back(')');
	return str;
}

/*=====================================================================================================*/
/*=====================================================================================================*/
int main(){

	std::vector<Set> mainVec;	//Keep all sets data 
	readInputFile(mainVec);		// Reading input file for all sets

	std::ofstream file("output.txt");

	for(auto set : mainVec){
		
		set.checkSubSets();		//If there is unknown element in subsets
								//checking subsets
		if(set.getExceptionHandling())//If there is unvalid character 
			file << set << "There is unvalid character in sets\n";
		else{
			set.Reflexive();	//We check the features one by one
			set.Symmetric();
			set.Antisymmetric();
			set.Transitive();
			file << set;
		}
	}
	std::cout << "Output.txt file is ready\n";
	file.close();
	return 0;
}
/*=====================================================================================================*/
/*=====================================================================================================*/

void readInputFile(std::vector<Set> &mainVec){

	Set tmpset;		//class obj
	SubSet tmpsub;	//Struct obj
	int size;
	std::string elements,subset , fname = "input.txt";

	std::ifstream file (fname);
	if(!file.is_open())
		std::cout << fname << " could not opened" << std::endl;

	while(file >> size && !file.eof()){		//Reading input file
		file >> elements;

		for(auto el: elements){		//Select all elements
			if(el != ',')
				tmpset.push_elements(el);   //we assaing elements to the temp object
		}
	
		for(int i=0 ; i < size ; ++i ){		//Read subset	
			file >> subset;
			tmpsub.first = subset[0] ;		//elements are setting as subset	
			tmpsub.second = subset[2] ;							
			tmpset.push_subSet(tmpsub);		//Assaign the subset temp obj
		}
		mainVec.push_back(tmpset);		//Add main Set obj
		tmpset.clear();					//Clearing tmp obj
	}	
}
/*=====================================================================================================*/


void Set::push_elements(char element){
	elements.push_back(element);
}
void Set::push_subSet(SubSet subset){
	SubSets.push_back(subset);
}
void Set::clear(){			//to clear exsiting object
	elements.clear();	
	SubSets.clear();
}

std::ostream& operator<<(std::ostream& sout,const Set& writer){

	sout << "n\n";
	for(auto sub: writer.SubSets)
		sout << "(" << sub.first << "," << sub.second << ")";
	sout << writer.text;	//We are returning information
	return sout;
}

void Set::addText(std::string tmp){
	text += tmp;		//Add string in class obj to get info about features of sets
}

void Set::checkSubSets(){
									//Checking the sub set elemet		
	for(auto subset : SubSets){		//Looking for the undefined elemet
		ExceptionHandling = checkSubSet(subset);
		if(ExceptionHandling)
			break;						//If there is undefined element ExceptionHandling = true 
	}									//then we are not doing reflexive,symmetric ... features
}

bool Set::checkSubSet(const SubSet& subset){

	bool result = false;				//We are checking elemets of subset 
	int counter = 0; 					
										
	for(auto element : elements){		
		if(subset.first == subset.second && element == subset.first){	//case (a,a) 
			counter = 2; 
			break;
		}
		else if(element == subset.first || element == subset.second)	//case (a,b)
			counter++;
	}
	if(counter < 2)		//There must be at least two similar element ex :(a,undef) -> (counter =1) 
		result = true;	
	return result;
}	

bool Set::searchInList(SubSet tmp)const {

	bool result = false;			//we are look tmp in all subsets
									//if ther is return true
	for(auto set : SubSets){		//else return false
		if(set.first == tmp.first && set.second == tmp.second){
			result = true;
			break;
		}
	}
	return result;
}

void Set::Symmetric(){
	SubSet tmp;
	std::string text;
	int counter = 0;
	for(auto subset : SubSets){
		if(subset.first == subset.second )	//case (a,a) acceptable conditions
			counter++;
		
		else if(searchInList({subset.second,subset.first})) // acceptable conditions
			tmp = {subset.first,subset.second};
		
		else{	//If there is an unacceptable condition adding text
			text = "Symmetric: No " + setString(subset.first,subset.second) + " is found whereas " 
				+ setString(subset.second,subset.first) + " is not found.\n";
			addText(text);
			return;
		}	
	}//If there is no unacceptable condition add text with an example
	if(SubSets.size() > counter)
		addText("Symmetric: Yes, " +setString(tmp.first,tmp.second)+ " and " +setString(tmp.second,tmp.first) + " also found\n" );
	else
		addText("Symmetric: Yes, all elements are reflexive\n");
}

void Set::Reflexive(){

	for(int i=0; i<elements.size(); ++i){				//All elemenst must be represent in subsets
		if(!searchInList({elements[i],elements[i]})){	//Sending same char for both value ex: (x,x)		
			addText("Reflexive: No, This set is not reflexive " + setString(elements[i],elements[i]) + " is not found\n");
			return;
		}	
	}
	addText("Reflexive: Yes all elements are present ex: " + setString(elements[0],elements[0]) + "\n");
}

void Set::Antisymmetric(){

	std::string tmp =  "";
	SubSet save; //Struct obj
	int counter = 0;

	for(auto subset : SubSets){

		if(subset.first == subset.second) //case for (x,x)
			counter++;
		else if(searchInList({subset.second,subset.first})){  //Serching reverse subset in all subsets
			tmp = "Antisymmetric: No, " + setString(subset.first,subset.second) 
				+ " and " + setString(subset.second,subset.first) + " are found\n";
			addText(tmp);
			return;
		}
		else if(subset.first != subset.second)	//if it is Antisymmetric we are save data to print
			save = subset;
	}

	if(counter == SubSets.size()) // case : (x,x) (y,y) ...
		tmp = "Antisymmetric: Yes, all subsets are reflexive\n";
	else 		
		tmp = "Antisymmetric: Yes, " + setString(save.first,save.second) + " is found whereas " 
			+ setString(save.second,save.first) + " is not found\n"; 
	addText(tmp);
}

void Set::Transitive(){

	int idx =0, idx2=0, counter = 0;
	for (int i=0; i<SubSets.size(); ++i){
		for(int j=0; j<SubSets.size(); ++j){
			if(SubSets[i].first == SubSets[i].second && SubSets[j].first == SubSets[j].second && SubSets[i].first == SubSets[j].first)
				counter++;		//	acceptable conditions to count 

			else if(SubSets[i].second == SubSets[j].first){		//Look for the (a,b)(b,c)  -> (a,c) in all Subsets
				if(!searchInList({SubSets[i].first,SubSets[j].second})){
					addText("Transitive: No, " + setString(SubSets[i].first,SubSets[i].second) +  " "  + setString(SubSets[j].first,SubSets[j].second)
						+ " found but " + setString(SubSets[i].first,SubSets[j].second) + " is not found\n");
					return;		
				}	
				else{	//We are keep an example to print output.txt
					idx = i;		idx2 = j;
				}
			}
		}
	}
	if(SubSets.size() > counter)
	addText("Transitive: Yes, " + setString(SubSets[idx].first,SubSets[idx2].second) + " is found since " 
			+ setString(SubSets[idx].first,SubSets[idx].second) + " and " + setString(SubSets[idx2].first,SubSets[idx2].second) + " are found\n");
	else if(SubSets.size() == counter)
		addText("Transitive: Yes, all subsets are reflexive\n");
}