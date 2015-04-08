#include <fstream> 
#include <string> 
#include <iostream>
#include <ctype.h> 
#include<vector>


using namespace std;

//declaration of all the methods that has been defined below 
void split_lines(string, int);
bool isOperator(char);
bool isDelimiter(char);
string lexer(string);
bool isChar(char);
bool isHex(char);
int calculateNextState(int,char);

const int row=15;
const int col=14;

//DFA state table
int arr[row][col]={{ 1, 2, 4, 4, 4, 5,-1, 7,-1,-1, 1,-1,-1,-1},
                   { 1, 2,-1,-1,-1,-1,-1,-1,-1,-1, 1,13,-1,11},
                   { 3,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1},
                   { 3,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1},
                   { 4,-1, 4, 4, 4,-1,-1,-1,-1,-1, 4,-1,-1,-1},
                   { 5, 5, 5, 5, 5, 6, 5, 5,14,-1, 5, 5, 5, 5},
                   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                   { 9, 9, 9, 9, 9, 9, 9,-1, 8,-1, 9,-1,-1,-1},
                   {-1,-1,-1,-1,-1,-1,-1, 9, 9, 9,-1,-1,-1,-1},
                   {-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,-1},
                   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                   {12,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,12,-1},
                   {12,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,12,-1},
                   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,13,-1,-1},
		   {-1,-1,-1,-1,-1, 5,-1, 5, 5, 5, 5,-1,-1,-1}};


int doublequoteFlag=0;
int singlequoteFlag=0;
int count=0;
int zeroFlag=0;
int hexFlag=0;
int octFlag=0;
int firstdigitzero=0;
ifstream infile;
ofstream outfile;
int notflag=0;


class Token
{ public:
  string tokenName;
  string tokenWord;
  int tokenLineNo;
  
};


 
vector<Token> tokens;
vector<Token>::iterator it;

/*int main( int argc,const char* argv[] )
{ 
string line; 
infile.open(argv[1]); //input file
outfile.open(argv[2]); //output file
int lineno=1;
Token t;
tokens.push_back(t);
 
 while (getline(infile,line) ) 
  {  
  if(line!="")  // checks whether the line is blank or not. If blank, it will simply be skipped without performing anything.
  split_lines (line, lineno++ ); 
 }
tokens.push_back(t);

 for(it=tokens.begin()+1; it<tokens.end();it++)
 {
   Token t=*it;
    cout<<t.tokenName<<"\t"<<t.tokenWord<<"\t"<<t.tokenLineNo<<"\n";
}

cout<<"Successfully categorized all the words of inputfile : "<<argv[1]<<"  and stored in output file : "<<argv[2]<<endl;
infile.close();
outfile.close();
return 0;
}*/

//method which takes the whole line as an input and split it into words and then check whether they are categorized into any of the token or not.
void split_lines (string line,int linecount)
 { 
 string no;
 bool flag=0; 
 string word = ""; 
count=0; 

 for (int i = 0; i <= line.length() - 1; i++ ) { 
  
 if((line[i]=='"' && doublequoteFlag==0))
  {
   if(singlequoteFlag==1){}
   else
   doublequoteFlag=1;
  }
  else if(line[i]=='"' && doublequoteFlag==1)
  {
   if(line[i-1] == '\\' && line[i+1]!='\0'){}
   else
   doublequoteFlag=2;
  } 

  if(line[i]=='\'' && doublequoteFlag==0 && singlequoteFlag==0)
  {
   singlequoteFlag=1;
  }
  else if(line[i]=='\'' && singlequoteFlag==1)
  {
   if(line[i-1] == '\\' && line[i+1]=='\''){}
   else
   singlequoteFlag=2;
  }
  

  if((doublequoteFlag==0 && singlequoteFlag==0) && (line[i]==' ' || isDelimiter(line[i]) || isOperator(line[i])))
  {
   if(word!="") 
   { 
     if(word[0] == '0')
     {
      firstdigitzero=1;
     }
     //check for the RESERVED WORDS/KEYWORDS
     if (word == "IF" || word == "WHILE" || word == "print" || word == "SWITCH"  || word == "CASE" || word == "DEFAULT") 
     {
        //cout<<"KEYWORD \t"<<word<<endl;
        //outfile<<"KEYWORD \t"<<word<<endl;
        Token t;
        t.tokenName="KEYWORD";
        t.tokenWord=word;
        t.tokenLineNo=linecount;
        tokens.push_back(t);
     }
     else
     {
      //cout<<lexer(word)<<word<<endl;
      //outfile<<lexer(word)<<word<<endl;
      Token t;
        t.tokenName=lexer(word);
	
        t.tokenWord=word;
        t.tokenLineNo=linecount;
        tokens.push_back(t);
     }
   }
   if(isDelimiter(line[i]))
    { 
     //cout<<"DELIMITER \t"<<line[i]<<endl; 
     //outfile<<"DELIMITER \t"<<line[i]<<endl;
     Token t;
        t.tokenName="DELIMITER";
        t.tokenWord=line[i];
        t.tokenLineNo=linecount;
	//cout<<t.tokenName<<t.tokenWord<<t.tokenLine<<"\n";
        tokens.push_back(t);
    }
   else if (isOperator(line[i]))
   {
     //cout <<"OPERATOR \t"<<line[i]<<endl;
      if(notflag==1) 
      {
        
       // outfile<<"OPERATOR \t"<<"!="<<endl; 
      }   
     else if(notflag==2)
        {
         //outfile<<"OPERATOR \t"<<"!="<<endl;
         notflag=0; 
         Token t;
        t.tokenName="OPERATOR";
        t.tokenWord="!=";
        t.tokenLineNo=linecount;
	//cout<<t.tokenName<<t.tokenWord<<t.tokenLine<<"\n";
        tokens.push_back(t);
         
        }
    
    else  
    {       
    // outfile<<"OPERATOR \t"<<line[i]<<endl;
   

   Token t;
        t.tokenName="OPERATOR";
        t.tokenWord=line[i];
        t.tokenLineNo=linecount;
	//cout<<t.tokenName<<t.tokenWord<<t.tokenLine<<"\n";
        tokens.push_back(t); 
}
}
   if(i==line.length()-1)
   {
    flag=1;
   }
   word="";
   count=0;
  }
  else if(line[i+1]=='"' && doublequoteFlag==0 && singlequoteFlag==0)
  {
   //cout<<lexer(word+line[i])<<word<<line[i]<<endl;
   outfile<<lexer(word+line[i])<<word<<line[i]<<endl;
   word="";
   count=0;
  }
  else if(line[i+1]=='\'' && singlequoteFlag == 0 && doublequoteFlag == 0)
  {
   //cout<<lexer(word+line[i])<<word<<line[i]<<endl;
   outfile<<lexer(word+line[i])<<word<<line[i]<<endl;
   word="";
   count=0;
  }
  else if((doublequoteFlag==2 || singlequoteFlag==2))
  {
  // cout<<lexer(word+line[i])<<word<<line[i]<<endl;
   outfile<<lexer(word+line[i])<<word<<line[i]<<endl;
   word="";
   count=0;
   if(doublequoteFlag==2)
    doublequoteFlag=0;
   
   if(singlequoteFlag==2)
    singlequoteFlag=0;
   
   if(i==line.length()-1)
    flag=1;
  }
  else if((doublequoteFlag==1 ||(doublequoteFlag==0 && singlequoteFlag==1)) && i==line.length()-1)
  {
   flag=1;
   count=0;
   //cout<<lexer(word+line[i])<<word<<line[i]<<endl;
   //outfile<<lexer(word+line[i])<<word<<line[i]<<endl;
   if(doublequoteFlag==1)
    doublequoteFlag=0;
   else
    singlequoteFlag=0;
  }
  else 
  { 
   word = word + line[i];
   count++;
  } 
 }
 if(flag==0)
  {
   if(word[0] == '0')
     {
      firstdigitzero=1;
     }
   //check for the RESERVED WORDS/KEYWORDS
   if (word == "IF" || word == "WHILE" || word == "print" || word == "SWITCH"  || word == "CASE" || word == "DEFAULT")
     {
        //cout<<"KEYWORD \t"<<word<<endl;
        //outfile<<"KEYWORD \t"<<word<<endl;
        Token t;
        t.tokenName="KEYWORD";
        t.tokenWord=word;
        t.tokenLineNo=linecount;
	//cout<<t.tokenName<<t.tokenWord<<t.tokenLine<<"\n";
        tokens.push_back(t);    
}
     else
     {
      //cout<<lexer(word)<<word<<endl;
      //outfile<<lexer(word)<<word<<endl;
      Token t;
        t.tokenName=lexer(word);
        t.tokenWord=word;
        t.tokenLineNo=linecount;
	//cout<<t.tokenName<<t.tokenWord<<t.tokenLine<<"\n";
        tokens.push_back(t);
     }
  }
}

bool isOperator (char c)
{ 
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '>' || c == '<') 
 //code for not equal to  
{
 
  if(c=='!') notflag=1;
  if(c=='=' && notflag==1)
   notflag=2;
     return true; 

  } 
  return false; 
} 
 
bool isDelimiter(char c) { 
 
 if (c == ';' || c == ',' || c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']' || c==':' ) { 
 return true; 
 } 
 return false; 
}

bool isChar(char c){
 if(c =='?' || c == 'a' || c == 'b' || c == 't' || c == 'n' || c == 'r' || c == 'v' || c == 'f')
  return true;
 else
  return false;
}



//method which takes word as an input and output the name of the token in which it is categorized
string lexer(string word) 
 { 
 
 int state = 0; 
 
 for (int i = 0; i <= word.length() - 1; i++ ) 
 { 
 state = calculateNextState( state, word[i] );  //passing the character of a word one by one and getting the next state according to the DFA state table
 if(state==-1) // if undefined state returned then it won't check further and break the loop
  break;
 } 
// re-setting all the flags to zero
 zeroFlag=0;
 hexFlag=0;
 octFlag=0;
 singlequoteFlag=0;
 doublequoteFlag=0; 
 firstdigitzero=0;
//token definitions
 if (state == 1)      return "INTEGER"; 
 else if(state==4) return "IDENTIFIER";
 
 else 		      return "UNDEFINED"; 
 

 
} 

//method which takes current state and character as an input and return the next state according to the DFA state table defined above
int calculateNextState(int current_state, char letter)
 { 
  if(notflag==1) 
  {
   notflag=0;
   return -1;
  }
 int new_state;

//check whether the character is a digit between 0-9
if(isdigit(letter))
{
   if(letter=='0' && count==1) //if there is only one character in the word and it's zero then it should be classified as an integer
   {
     new_state=arr[current_state][10];
   }
   
    else if(letter == '0' && count>1)
    new_state=arr[current_state][10];
   
   else 
   {
     new_state=arr[current_state][0];
   }
}
else if(letter=='$') //IDENTIFIER
new_state=arr[current_state][2];
else if(letter=='_') //IDENTIFIER
new_state=arr[current_state][3];
else if(isalpha(letter)) //alphabets (both upper and lower case)
{
 
 
 if(isChar(letter) && (current_state == 8 || current_state == 14 )) // to identify'\a', '\b', '\n', '\t', '\f', '\r', '\v' characters 
  new_state=arr[current_state][9];
 else //for anything else
  new_state=arr[current_state][4]; 
}

else if(isOperator(letter) || isDelimiter(letter) || letter == ' ' || letter == '~' || letter == '@' || letter == '`') //for any other characters that are not yet been checked
 { 
   
    
  new_state=arr[current_state][6];
}
else if(letter == '\\') // to identify '\' while checking for CHARACTER token
 new_state=arr[current_state][8];

return new_state;  
} 
