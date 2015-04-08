
/*-----------------------------------------------------------------------------------------------------------------------*/
  /* ASSIGNMENT 4 : Compiler (parser.cpp includes lexer.cpp and symbol.cpp)
     AUTHOR - SHREEKANTH ANANTHANARAYANAN MOHAN
     ASU ID - 1206321463 
     COPYRIGHT (C) 2014 ALL RIGHTS RESERVED TO THE SOLE AUTHOR */
/*----------------------------------------------------------------------------------------------------------------------*/


  #include "lexer.cpp"
  #include<iostream>
  #include<ctype.h>
  #include<vector>
  #include "symbol.cpp"
  #include<sstream>

  Token nextToken();
  void backToken();

  int currentToken = 0;
  Token nextToken()
  {	
  	Token next=tokens[currentToken];
  	currentToken++;
  	return next;
  }


  void backToken()
  {
  	currentToken--;
  }

  void program();
  void var_section();
  void body();
  void id_list();
  void stmt_list();
  void stmt();
  void assign_stmt();
  void primary();
  void print_stmt();
  void expr();
  void op();
  void while_stmt();
  void relop();
  void condition();
  void if_stmt();
  void switch_stmt();
  void case_stmt(string,string);
  void case_list(string,string);
  void default_case();

  Token t;
  string assign_variable;
  int pc=1;
  int opcode;
  int relopcode;
  int whileflag=0;
  int labelcounter=0;
  int c=1;
  int while_counter;
  string label;
  int switch_flag=0;

  stringstream intercode; 

  int main( int argc,const char* argv[] )
  { 
  string line; 
  infile.open(argv[1]); //input file`
  outfile.open(argv[2]); //output file
  int lineno=1;
  int result;
  tokens.push_back(t);
   
   while (getline(infile,line) ) 
    {  
    if(line!="")  
    split_lines (line, lineno++ ); 
   }
  tokens.push_back(t);

 symTable.insertSymbol("#pc",pc);
  program();
  displaySymbolTable();
  cout<<endl;
  outfile<<label_list.str()<<"\n";
  outfile<<intercode.str()<<"\n";

  
  infile.close();
  outfile.close();
  return 0;

  } // end of main

   
  void program()
   {
   var_section();
   for(it=tokens.begin()+1; it<tokens.end();it++)
   {
     currentToken++;
     Token t=*it;
      if(t.tokenWord==";") { break; }
      else
        {
        outfile<<t.tokenWord; 
         
         } 
   }  
    outfile<<"\n"; 
    body();           
    intercode<<"OPR 0,0 "<<"\n";
}

  void var_section()
   {
     
      id_list();
      
   }

  void id_list()
   {
             
       for(it=tokens.begin()+1; it<tokens.end();it++)
      {
       Token t=*it;
         
       if(t.tokenName=="IDENTIFIER")
        {
         
        symTable.insertSymbol(t.tokenWord, "INTEGER", "global");
        } 
       
      }      

     
    } 

  void body()
  {
   if(whileflag==0)
        nextToken();
    else
      {}
    if(nextToken().tokenWord!="{") 
     { 
     cout<<"\nDError"; 
      return;
      }
    
     stmt_list();
   
    if(nextToken().tokenWord!="}")  
   {
     cout<<"\nDEeError"; return;
   }
   
    else
     while_counter=pc;
   if (switch_flag==1)
      backToken();
    
   } 

  void stmt_list()
   {
     stmt();
     if(tokens[currentToken].tokenWord=="}") //check for this condition
      {}
     else
     stmt_list();
   }


  void stmt()
  {

  Token local=nextToken();


   if( local.tokenName=="IDENTIFIER")
      assign_stmt();
   else if ( local.tokenWord=="print") 
      print_stmt();
   else if(local.tokenWord=="WHILE")
      while_stmt();
   
   else if(local.tokenWord=="IF")
     if_stmt();
   else if(local.tokenWord=="SWITCH")
     switch_stmt();
   else {}
     


  }

  void assign_stmt()
   {
   backToken();
    
    assign_variable=tokens[currentToken].tokenWord;
   

   nextToken();
   if(tokens[currentToken].tokenWord=="=")
    {
      if(tokens[currentToken+2].tokenName=="OPERATOR")
       {
         expr();
        }
       else
        primary();
    }
   
   if(nextToken().tokenWord==";")  //token reached semicolon
    {
     
    intercode<<"STO"<<" "<<assign_variable<<",0"<<"\n";
     pc++;
     
      
    }
    else
     cout<<"\nEffrror";
   
  }

  void primary()
   {
    
     nextToken();
      if(tokens[currentToken].tokenName=="IDENTIFIER")
      {
        if(symTable.search(tokens[currentToken].tokenWord))
         {
           
           intercode<<"LOD"<<" "<<tokens[currentToken].tokenWord<<",0"<<"\n";
           pc++; 
           
            
           nextToken();
         }
      }
      else 
    { 
      
         intercode<<"LIT"<<" "<<tokens[currentToken].tokenWord<<",0"<<"\n";
       pc++;  
       
       nextToken();
    }
   } //end of primary

  void print_stmt()
   {
   
    
       if(nextToken().tokenName=="IDENTIFIER")
      {
       backToken();
       intercode<<"LOD"<<" "<<nextToken().tokenWord<<" "<<",0"<<"\n";
       pc++;
      }

    
       if(nextToken().tokenWord==";")
       {
        intercode<<"OPR 21,0"<<"\n";
        
        pc++;
        
         
       }
   }  //end of print_Stmt   

  void expr()
   {
     
     primary();
     op();
     primary();
   intercode<<"OPR"<<" "<<opcode<<" ,0"<<"\n";
    
     pc++;
    
     

   }//end of expr

  void op()
  {

    if(tokens[currentToken].tokenWord=="+") 
      opcode=2;
    else if(tokens[currentToken].tokenWord=="-")
     opcode=3;
    else if(tokens[currentToken].tokenWord=="*")
     opcode=4;
    else if(tokens[currentToken].tokenWord=="/")
     opcode=5;
    else cout<<"\nError";
   } //end of op


  void while_stmt()       //for while statement 
  {
    int jump;
    cout<<pc;
    whileflag=1;
    labelcounter++;
    c++;
    std::string jmc = std::to_string(labelcounter);    
    string label = "#e" + jmc;
    
    backToken();   
    jump=pc; //for JMC
    condition();
    intercode<<"JMC"<<" "<<label<<",false"<<"\n";
    pc++;
    body();
    pc++;
    symTable.insertSymbol(label,pc); //jmc values get inserted 
    labelcounter++;
    c++;
    std::string jmd = std::to_string(labelcounter);    
    string label1 = "#e" + jmd; 
    symTable.insertSymbol(label1,jump); //jmp labels get inserted
    std::string jmp = std::to_string(labelcounter);    
    label = "#e" + jmp;
    intercode<<"JMP"<<" "<<label<<", 0"<<"\n";
    whileflag=0;
  }

  void condition()    //checks if and while condition
  {
   primary();
   relop();
   primary();
   intercode<<"OPR"<<" "<<relopcode<<",0"<<"\n";
   pc++;
   
  }

  void relop()
   {
     
      if(tokens[currentToken].tokenWord==">") 
        relopcode=11;
      else if(tokens[currentToken].tokenWord=="<")
        relopcode=12;
      else if(tokens[currentToken].tokenWord=="!=")
         relopcode=16;
      else
        cout<<"\nError";
    }

  void if_stmt()
  {
    whileflag=1;
    backToken();  
    labelcounter++;
    c++;
    std::string jmc = std::to_string(labelcounter);    
    string label = "#e" + jmc;
    condition();
    intercode<<"JMC"<<" "<<label<<",false"<<"\n";
    pc++;
    body();
    symTable.insertSymbol(label,pc);
    whileflag=0;
  }


  void switch_stmt()
   {
   string id, switchEnd;
   switch_flag=1;
   if(tokens[currentToken].tokenName=="IDENTIFIER")
   {  
      id = tokens[currentToken].tokenWord;	
                        
   }
   else
   {
      cout<<"Switch identifier error\n";
   }
   currentToken++;
   if(tokens[currentToken].tokenWord == "{")
   {
	stringstream st1;
	st1<<"#e"<<c++;
        labelcounter++;
        switchEnd = st1.str();
        case_list(id, switchEnd);
	if(tokens[currentToken+1].tokenName == "KEYWORD" && tokens[currentToken+1].tokenWord == "DEFAULT")
	{
	   default_case();
	}
   }
   else
   {
	cout<<"Switch { error\n";
   }
   currentToken++;
   if(tokens[currentToken].tokenWord == "}")
   {   pc++;
	intercode<<"OPR 1, 0\n";
	symTable.insertSymbol(switchEnd, pc);
   }
   else
  {
	cout<<"Switch } error\n";
  }
 switch_flag=0;
 nextToken();
}   


  void case_stmt(string id, string switchEnd)
 {
        
	string num, switchLabel;
	currentToken++;
	
	if(tokens[currentToken].tokenName == "KEYWORD" && tokens[currentToken].tokenWord == "CASE")
	{
		
		currentToken++;
		if(tokens[currentToken].tokenName == "INTEGER")
		{
			
			num = tokens[currentToken].tokenWord;
			pc++;
			intercode<<"LOD "<<id<<", 0\n";
			pc++;
			intercode<<"LIT "<<num<<", 0\n";
			currentToken++;
			if(tokens[currentToken].tokenName == "DELIMITER" && tokens[currentToken].tokenWord == ":")
			{
				
				pc++;
				intercode<<"OPR 15, 0\n";
				stringstream st;
				st<<"#e"<<c++;
                                labelcounter++;                                 
				switchLabel = st.str();
				pc++;
				intercode<<"JMC "<<switchLabel<<", false\n";
				body();
				symTable.insertSymbol(switchLabel, pc+1);
				pc++;
				intercode<<"JMP "<<switchEnd<<", 0\n";
			}
			else
			{
				cout<<"case : error\n";
			}
		}
		else
		{
			cout<<"case integer error\n";
		}
	}
	else
		cout<<"case error\n";
}

void case_list(string id, string switchEnd)
{
        
	case_stmt(id, switchEnd);
	if((tokens[currentToken+1].tokenName == "KEYWORD" && tokens[currentToken+1].tokenWord == "DEFAULT") || (tokens[currentToken+1].tokenWord == "}"))
	{
		return;		
	}
	else
	{
		case_list(id, switchEnd);
	}	
}



void default_case()
{
	currentToken++;
	if(tokens[currentToken].tokenName == "KEYWORD" && tokens[currentToken].tokenWord == "DEFAULT")
	{
		
		currentToken++;
		if(tokens[currentToken].tokenName == "DELIMITER" && tokens[currentToken].tokenWord == ":")
		{
						
			body();
		}
		else
		{
			cout<<"default : error\n";
		}
	}
	else
	{
		cout<<"default error\n";
	}
}








