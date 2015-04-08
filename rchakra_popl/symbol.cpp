#include<string>
#include<vector>
#include<map>
#include<stack>
#include<sstream>
#include<iostream>



using namespace std;

string currentScope = "global";
string type="int";
stringstream label_list;


class SymbolTable
{
	public:
		string type;
		string scope;
		string id;   //also for label
	    int progcount; //added
		void insertSymbol(string id, string type, string scope);
                void insertSymbol(string id, int value);        
		bool search(string id);
                  
	 
		
} symTable;

class resultSet  //for getting the contents and pushing it to the set.
{
  public:
       string labelname;
       int count;
};


map<string,vector<SymbolTable> > symMap;
map<string,int> typesMap;
map<string,int> opMap;

vector<int> symStack;
vector<int> vecType;

map<string,vector<SymbolTable> >::iterator mapIter;
vector<SymbolTable>::iterator vecIter;

vector <resultSet> lab;





void SymbolTable::insertSymbol(string ids, string s_type, string s_scope)
{
        
	SymbolTable s;
        s.id=ids;
	s.type = s_type;
	s.scope = s_scope;
        s.progcount=0;
	vector<SymbolTable> symVector;
	
	

		symVector.push_back(s);
		symMap.emplace(s.id, symVector);

}

void SymbolTable::insertSymbol(string lab, int prog)  
{
  
  SymbolTable x;
  vector<SymbolTable> symVector;
  x.type="NULL";
  x.scope="NULL";
  x.id=lab;
  x.progcount=prog;
  
  symVector.push_back(x);
  symMap.emplace(x.id, symVector);
 }



bool SymbolTable::search(string id)
{	
	if(symMap.find(id) == symMap.end())
                 
		return false;
	else
		return true;		
}




void displaySymbolTable()
{
        
        
	for(mapIter = symMap.begin(); mapIter != symMap.end(); mapIter++)
	{
		
		vector<SymbolTable> vec = mapIter->second;
		for(vector<SymbolTable>::iterator iter = vec.begin(); iter != vec.end(); iter++)
		{
                        
			SymbolTable s = (*iter);
                         if(s.type!="NULL") 
                         {
                         
			//cout<<"("<<s.type<<","<<s.scope<<")";
                         }
                         else
                         {
                          
                         //cout<<s.progcount;
                         
                        } 
		}
		//cout<<")"<<endl;
	}

 

     
      
     for(mapIter = symMap.begin(); mapIter != symMap.end(); mapIter++)
	{
		SymbolTable b;
		vector<SymbolTable> vec = mapIter->second;
                //vector<SymbolTable>::iterator iter = vec.begin();
                 //cout<<mapIter->first<<"="<<b.progcount;
                 //vector<SymbolTable>::iterator iter1=iter++;
		for(vector<SymbolTable>::iterator iter = vec.begin(); iter != vec.end(); iter++)
		{
                         
			 SymbolTable s = (*iter);
                         if(s.type!="NULL") 
                         {
                         
			//cout<<"("<<s.type<<","<<s.scope<<")";
                         }
                         else
                         {
                           
                       
                           resultSet r;
                           r.count=s.progcount;
                           r.labelname=mapIter->first;
                           lab.push_back(r);
                        } 
		}
	
	}

   
     for(vector<resultSet>::iterator st = lab.begin(); st != lab.end(); st++)
      {
       
        resultSet m = (*st);
        
        if(st+1!=lab.end()) 
        label_list<<m.labelname<<"="<<m.count<<",";
        else
        label_list<<m.labelname<<"="<<m.count;
     }
  
      
    
}


	
