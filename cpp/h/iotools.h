#ifndef IOTOOLS_H

#define IOTOOLS_H

#include <fstream>
#include <string>

#include <sys/stat.h>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
#include <deque>

#include <dirent.h> //opendir, listdir()
using namespace std;

#include <unistd.h> //unlink

// color for konsole, use: printf("%sRED\n",KRED);
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define KGreen KGRN
/*
 ATTENTION,WARNING:
 La couleur ne s'affichera pas dans la console intégrée de KDevelop !
 */
//////------------------------ Console, Terminal functions
void printErr(string greenText)
{
  string green="\x1B[31m", white="\x1B[37m";
  printf("%s%s%s",green.c_str(),greenText.c_str(),white.c_str());
}

void printCool(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s",green.c_str(),greenText.c_str(),white.c_str());
}

void printlnErr(string greenText)
{
  string green="\x1B[31m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}
void printErrLn(string redText)
{
  printlnErr(redText);
}

void println(string greenText, string color=KNRM)
{
  string green=color, white=KWHT;
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void printLn(string greenText, string color)
{
  string green=color, white=KWHT;
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void printlnCool(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}
void printCoolLn(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void print(string whiteText)
{
  string green="\x1B[37m", white="\x1B[0m";
  printf("%s%s%s",green.c_str(),whiteText.c_str(),white.c_str());
}

void printLn(string whiteText)
{
  string green="\x1B[37m", white="\x1B[0m";
  printf("%s%s%s\n",green.c_str(),whiteText.c_str(),white.c_str());
}


//Run a command and return the result into a string
//modifiée le 28/08/2017, ajout de trimRight
string execute(string command, bool trimRight=true, bool redirectErr=false, bool waitResponse=true)
{  
    if(redirectErr)command+=" 2>&1";
    FILE *cmd = popen(command.c_str(), "r");
    char result[1024];
    string response = "";    
    if(waitResponse)while (fgets(result, sizeof(result), cmd) != NULL)
    {
       string ret = string(result, strlen(result) );//Enregistre : «exec.2»
       response+= ret;      
    }
    pclose(cmd);
    
    
    //trim in case of necessary
    if(trimRight)
    {     
      const char* chars2remove=" \r\n";
      if (!response.empty())
	{
	  string::size_type pos = response.find_last_not_of(chars2remove);

	  if (pos != string::npos)
	  response.erase(pos+1);
	  else
	  response.erase( response.begin() , response.end() ); // make empty
	}
    }
  return response;
}//end execute

string executeEx(string command)
{  
  return execute(command, true, true);
}//end execute


bool commandExists(string command)
{
  //if (WEXITSTATUS(result) == 127)    printf("That shell command is not found");    else        printf("system call return succesfull with  %d",WEXITSTATUS(result));
  return false;
}//commandExists

//reads text file and return it's content
string getFileContent(string filename)
{  
  ifstream ifs( filename.c_str());
  string content( (istreambuf_iterator<char>(ifs) ),
		  (istreambuf_iterator<char>()    ) );  
  return content;
}//getFileContent
#define readTextFile getFileContent


//Inserts text in a file
//exemple:  insertInFile(cfilename, "hello:hello:","COOL",true);
int insertInFile (string filename,string search, string toAppend, bool replace=false)
{

    int len=0;
    ifstream ifs( filename.c_str());
    string content( (istreambuf_iterator<char>(ifs) ),
		  (istreambuf_iterator<char>()    ) );  

    int where=content.find(search.c_str());
    if(where==-1) return -1; //search pattern not found
    if(replace) len=search.length();
    string rest = content.substr(where+len);
 
    fstream file;
    file.open(filename.c_str(),  ios_base::in | ios_base::out| ios_base::binary);
    file.seekp(where);
    file.write(toAppend.c_str(),toAppend.length());
    //write the rest of the file
    file.write(rest.c_str(),(int) rest.length());
    file.close();
    return 0;
}//insertInFile



bool removeFile(string fullname)
{
  int r = unlink(fullname.c_str());  
  (r==0)?true:false;
  
  return false;
}

bool fileExists(string fullname)
{
  
    ifstream f(fullname.c_str());
    return f.good();
}

long int getFileSize(string filename)
{  
  struct stat statbuf;

  if (stat(filename.c_str(), &statbuf) == -1) {
    /* check the value of errno */
  }
  
  long int len = (long int) statbuf.st_size;
  //printf("%9jd", () statbuf.st_size);
  return len;
}//getFileContent

long int getfilesize(string filename){return getFileSize(filename);}

string escape_quotes(const string &before)
{
    string after;
    after.reserve(before.length() + 4);

   for (string::size_type i = 0; i < before.length(); ++i) {
        switch (before[i]) {
            case '"':
            case '\\':
                after += '\\';
                // Fall through.

            default:
                after += before[i];
        }
    }

    return after;
}


//convert string to double
double strToDouble(string str)
{
	return strtod(str.c_str(),NULL);
}

long double strToLongDouble(string str)
{
	return stold(str.c_str(),NULL);
}

// cout<<strToFloat("9.43");
float strToFloat(string str)
{
	return (float) strToDouble(str);
}
 
int strToInt(string str)
{
   return atoi(str.c_str());
}

long strToLong(string str)
{
   return atol(str.c_str());
}

long long strToLongLong(string str)
{
   return atoll(str.c_str());
}

//convert hexadecimal to integer
int strToHex(string str)
{    
    int x;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return x;
}

//convert string to int
bool strToInt(string str, int *i, int base=10)
{
 char *end;
 long lnum = strtol(str.c_str(), &end,base);
 *i=lnum;
 bool success=false;
 if(strlen(end)==0)success = true;
 
 return success; 
}

string intToStr(int n)
{ 
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

string floatToStr(float f,  int precision = 18)
{ 
    std::ostringstream oss;
    oss.precision(precision);
    oss << f;
    return oss.str();
}


#ifndef boolToStr

string boolToStr(bool val)
{
  if(val) return "true"; else return "false";
}

#endif 


string longToStr(long long l)
{
  std::ostringstream oss;
    oss << l;
    return oss.str();
}

string intToStr(long l)
{
   std::ostringstream oss;
    oss << l;
    return oss.str();
}

string doubleToStr(double n)
{ 
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
/*
 #include <sstream>

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
 */


//HERE renvoie le numéro de ligne dans le programme
//#define HERE (std::string(__FILE__) + "(" + itos(__LINE__) + "): ")

//détect si une chaine débute avec une autre.
bool strStartsWith(string str, string pattern)
{
	bool result= false;
	if( str.length()>=pattern.length())
	{
		if(! str.substr(0,pattern.length()).compare(pattern) )
		{
			result = true;
		}
	}
	return result;
}


//to lowerCase
string strToLower(string s)
{
	string result="";
	for(int i=0; i<(int)s.length(); i++)
	{
		result+=tolower( s.at(i) );	
	}
	return result;
}

string stdStrToLower(string str)
{
  string output="";
  std::locale loc;  
  for (std::string::size_type i=0; i<str.length(); ++i)
    output+=std::tolower(str[i],loc);
  return output;
}


bool startsWith(string str, string pattern)
{
	return strStartsWith(str,pattern);
}


string lastLeftStr(string line,string pattern, bool includePattern=false)
{
	int l=0,r,len;
	if(line.empty())return "";
	string str;	
	r = line.length()+line.find_last_of(pattern,l);    
	len=pattern.length();
	if(includePattern)
	{
		str=line.substr(0,r+len);		
	}else
	{
		str=line.substr(0,r);	
	}
	return str;
}


string leftStr(string line,string pattern, bool includePattern=false)
{
	int l=0,r,len;
	if(line.empty())return "";
	string str;	
	r=line.find(pattern,l);
	len=pattern.length();
	if(includePattern)
	{
		str=line.substr(0,r+len);		
	}else
	{
		str=line.substr(0,r);	
	}
	return str;
}

string rightStr(string line,string pattern, bool includePattern=false)
{
	int l,len;
	if(line.empty())return "";
	string str;	
	l=line.find(pattern);
	len=pattern.length();

	if(includePattern)
	{
		str=line.substr(l);		
	}else
	{
		str=line.substr(l+len);	
	}
	return str;
}


//does a string end with ....?
bool endsWith(string s,string pattern)
{
	int plen=pattern.length();
	int slen=s.length();
	if( (plen>slen)|| pattern.empty() )return false;
	string str=s.substr(slen-plen);
	if(!str.compare(pattern)) return true; else return false;
}



//si une chaine est présente dans une autre
bool inStr(string line, string toFind, bool ignoreCase=false)
{
	if(ignoreCase)
	{
		line=strToLower(line);
		toFind=strToLower(toFind);
	}
	if(line.find(toFind)!=-1)return true;
	else return false;
}



void trimLeft(std::string& str, const char* chars2remove=" \r\n")
{
	if (!str.empty())
	{
	std::string::size_type pos = str.find_first_not_of(chars2remove);

	if (pos != std::string::npos)
	str.erase(0,pos);
	else
	str.erase( str.begin() , str.end() );
	}
}

void trimRight(std::string& str, const char* chars2remove=" \r\n")
{
	if (!str.empty())
	{
	std::string::size_type pos = str.find_last_not_of(chars2remove);

	if (pos != std::string::npos)
	str.erase(pos+1);
	else
	str.erase( str.begin() , str.end() ); // make empty
	}
}

void trim(string &str,const char* chars2remove=" \r\n")
{
	trimLeft(str,chars2remove);
	trimRight(str,chars2remove);
}

string trimStr(string line)
{
     trim(line);
	return line;
}

///verified:
//  string innerBrack = between_last( trimStr( " !{int:20}" ),"!{","}");
//    cout<<"["<<innerBrack<<"]"<<endl; //[int:20]
string between_last2(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string r,str="";
	if( line.find(leftPattern) <0 ){ return str; }
	if( line.find(rightPattern) <0 ){ return str; }	
	//if( line.find(leftPattern) >= line.find(rightPattern)  ){ return str; }  //line = "<w f="10">sous</w>" recherche de "sous"

	if(leftPattern.compare(rightPattern))
	r=rightStr(line,leftPattern,includePattern);
	else
	r=rightStr(line,leftPattern,false);
         
	if(leftPattern.compare(rightPattern))
    {  //pattern different
        str=lastLeftStr(r,rightPattern,includePattern);              
           int rn = str.find_last_of(rightPattern) - rightPattern.length();
           str = str.substr(0,rn+1);           
    }
	else
	{
        str=lastLeftStr(r,rightPattern,includePattern);
        
        int rn = str.find_last_of(rightPattern) - rightPattern.length();
        str = str.substr(0,rn+1);      
           
        string str2=str;
        if(includePattern)
        {   str2=leftPattern;
            str2.append(str);
            str2.append(rightPattern);
        }                
        return str2;
	}
	
	return str;
}//between_last2

string between_last(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string str="";
	if( line.find(leftPattern) <0 ){ return str; }
	if( line.find(rightPattern) <0 ){ return str; }	
	
	int l = line.find(leftPattern)+leftPattern.length();    
    int r = line.length() - line.find_last_of(rightPattern) + rightPattern.length()-1;
 
    int len = line.length() -l - r;
	str = line.substr(l, len );
    
    if(includePattern)
    {
        str=leftPattern + str + rightPattern;        
    }
    
	return str;
}//between_last

//changé le 13:48 31/03/2009
//non vérifiée voir between_last
string between_manual(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string r,str="";
	if( line.find(leftPattern) <0 ){ return str; }
	if( line.find(rightPattern) <0 ){ return str; }	
	//if( line.find(leftPattern) >= line.find(rightPattern)  ){ return str; }  //line = "<w f="10">sous</w>" recherche de "sous"

	if(leftPattern.compare(rightPattern))
	r=rightStr(line,leftPattern,includePattern);
	else
	r=rightStr(line,leftPattern,false);

	if(leftPattern.compare(rightPattern))
	str=leftStr(r,rightPattern,includePattern);
	else
	{
	str=leftStr(r,rightPattern,includePattern);
	string str2="";
	if(includePattern) str2=leftPattern;
	str2.append(str);
	return str2;
	}
	return str;
}

//Création 23/03/2018
string between_escape(string line,string left, string right, int &rightPos)
{
    int l, r; string s = "";
    l=line.find(left,0);    
    if (l < 0) return s;
    r = line.find(right, l+1);
    while( line.at(r-1)=='\\' )    
    {   //find the next ending
        r=line.find(right, r+1);          
        if (r < 0) return s;
    }     
    rightPos=r; //save right position
    if (r < 0) return s;        
    int bigl=l+left.length();
    int bigr = r-l - left.length();	  
    
    s=line.substr(bigl, bigr);
    return s;
}//end between_escape
      
      
      
      
//création 16/08/2017 //messageBox(between("<<<1234567]]fin>","<<<","]]") );return 0;
string between(string line,string left, string right)
        {
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);
            if (r < 0) return s;
	    int bigl=l+left.length();
	    int bigr = r-l - left.length();	  
            s=line.substr(bigl, bigr);
            return s;
        }//endbetween (verified)
            
            
//création 11/10/2017 //messageBox(between("<<<1234567]]fin>","<<<","]]") );return 0;
string between(string line,string left, string right, int &posLastRight)
        {
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);
	    posLastRight=r;
            if (r < 0) return s;
	    int bigl=l+left.length();
	    int bigr = r-l - left.length();	  
            s=line.substr(bigl, bigr);	    
            return s;
        }//endbetween (verified)
            

            
//string clean = removeStrBetween("12[*ABasourdi*]4bonjour","[*","*]");
//returns 124bonjour
string removeStrBetween(string line, string left, string right)
{        
    //code from function between
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);            
            if (r < 0) return s;
            int bigl=l+left.length();
            int bigr = r-l - left.length();	  
            s=line.substr(bigl, bigr);	                
            //cout<<"r:"<<r<<endl; cout<<"l:"<<l<<endl;            cout<<"length:"<<len<<endl;            
            string clean = line.erase(l, r+right.length() - l );      
    return clean;
}//removeStrBetween

            
//string.split            
vector<string> splitStr(string str, char sep)
{
    vector<string> strings;
    istringstream f(str);
    string s;    
    while (getline(f, s, sep)) {
        //cout << s << endl;
        strings.push_back(s);
    }      
	return strings;      
}//end split


// return number of occurences replaced, and updates the &whole variable
//   utilisation:
//   string ident = "";
//   string somefile = "/home/boony/dev/c++/myheaders/myfile";
//   string tofind ="/";  
//   int occurences =  replaceStr(somefile,"/","(!)");
//   disp(occurences);
//   box(ident+"\nfirstvar:"+somefile);

//2020-07-13 00:18:30 - ReplaceAll fonctionne correctement
string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
#define replaceString ReplaceAll

void replaceStr(string &whole,  const std::string& from, const std::string& to)
{  
 	whole = replaceString(whole, from, to );
}
 
 

/*************************   exemple map pour stockage hashtable de long?    ************************************/

//utilisation de map comme hastable avec pointer (exemple pour stocker des long)
// map<string, long long*>timeMap2;
//  // timeMap2["test inexistant"] = new long long;
//   //*(timeMap2["test inexistant"])=132;
//  long long *res =  timeMap2["test inexistant"];
//  if(res == NULL)
//  {
//    box("NULL");
// }else
// {
//   box("ok");
//   disp(*res);
// }
// 


/*************Ma classe stringmap********************/
/*Utilisation:
StringMap sm;
sm.add("text1");
sm.add("text2");
*/
class StringMap
{
public:
	map <int,string> m;	
	void init()
	{
		text="";
	}
public:
	StringMap(){init();}
	StringMap(vector<string> arr){init(); addRange(arr);}
	~StringMap(){}
	string text;
	
	int count(){ return m.size(); }
	int size(){ return m.size(); }

	void add(string s)
	{
		m[count()]=s;
		if(m.size()-1>0)//si ya une ligne avant:
			text.append("\r\n");
		text.append(s);
	}
	
	void add(char cstr[])
	{
		string s=cstr;
		add(s);
	}

	void add(int i)
	{		
		add(intToStr(i));
	}
	void add(string before,int i)
	{
		string s=before;
		s.append(intToStr(i));
		add(s);
	}

	void add(bool b)
	{		
		add((b)?"true":"false");
	}
	void add(string before,bool b)
	{
		string s=before;
		s.append((b)?"true":"false");
		add(s);
	}

	void add(string before,string str)
	{
		string s=before;
		s.append(str);
		add(s);
	}
	
	void addRange(vector<string> arr)
	{	    
		for(int i=0; i<arr.size(); i++)
		{
			add(arr[i]);
		} 
	}

	/*
	void add(WCHAR wstr[])
	{
		char cstr[sizeof(wstr)];
		wcharToChar(cstr,wstr);
		string s=cstr;
		add(s);
	}*/	

	void add(StringMap sm0)
	{
		for(int i=0; sm0.count(); i++)
		{
			add(sm0.item(i));
		}
	}

	void add(string before,StringMap sm0)
	{string str;
		int countFix=sm0.count();
		for(int i=0; i<countFix; i++)
		{
			str=before;
			str.append(sm0.item(i));
			add(str);
		}
	}

	int findItem(string strEq)
	{string str;
		int c=count();//gain de vitesse
		for(int i=0; i<c; i++)
		{
			str=item(i);
			if(!str.compare(strEq))return i;
		}
		return -1;
	}

	string item(int i){return m[i];}
	string last(){return m[m.size()-1];}
	
		
	string &operator[](int i) {         
	  return this->m[i];
	}
	
	//virtual string toString()	{	}
	
	
	//retourner une seule chaine avec toutes les lignes séparées par "\r\n"
	string all()
	{
		string result="";
		int c=count();//gain de vitesse
		for(int i=0; i<c; i++)
		{
			result.append(item(i));
			if(i+1<c)result.append("\r\n");			
		}
		return result;
	}
	
	bool contains(bool (*compareFn)(string item, string item2), string value, int *ifound=NULL)
	{	    
	      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {
		//  (*it).first
		  string vv =(*it).second;
		  if( compareFn(vv,value) )
		  {
		    key=(*it).first;
		    if(ifound!= NULL)(*ifound)=key;
		    return true; //break; 
		  }
	      }
	}//contains overload
			
	bool contains(string value, int *ifound=NULL)
	{
	 
	   // map<int, string> m = {  make_pair(0, "zero"), make_pair(1, "one"), make_pair(2, "two") };
/*
	      int key = -1;
	      for (auto &i : m) {
		  if (i.second == value) {
		    key = i.first;
		    break; // to stop searching
		  }
	      }
	     
	     ifound=key;
	     printf("key found:%d\n",key);
	     */
	      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {
		//  (*it).first
		  string vv =(*it).second;
		  if(vv==value)
		  {
		    key=(*it).first;
		   if(ifound!= NULL)(*ifound)=key;
		   return true; //break; 
		  }
	      }
	      
	      
		    /*  
		      if(std::find(v.begin(), v.end(), x) != v.end()) {
			// v contains x 
			} else {
			    // v does not contain x 
			}
		      */
		    
		    return false;
	}//contains

  
  //operator std::string() const { return "Hi"; }
  
    string toString() 
    {
      string s="";
      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {		
		  string vv =(*it).second;
		  key=(*it).first;
		  ostringstream oss;
		  oss<<key;
		  s+="["+oss.str()+"]=\""+vv+"\"";	
	      }
	      return s;
  }//toString
};
#define StringList StringMap

//----------------------    vector helper functions     -------------------------------------
bool vectorContains( std::vector<int>& vec, int element ) 
{
    if (std::find(vec.begin(), vec.end(), element) != vec.end())
        return true;

    return false;
}
bool vectorContains( std::vector<string>& vec, string element ) 
{
    if (std::find(vec.begin(), vec.end(), element) != vec.end())
        return true;

    return false;
}

void vectorRemove(vector<string> &vect, string value)
{    
  vect.erase(std::remove(vect.begin(), vect.end(), value), vect.end());
}


void vectorRemove(vector<int> &vect, int value)
{    
  vect.erase(std::remove(vect.begin(), vect.end(), value), vect.end());
}


string vectorFind(vector<string> vect, string tofind, bool (*compareFn)(string item, string item2))
{
  string vline="";
  for(int i=0; i<vect.size(); i++)
	 {
	   if(compareFn!=NULL)
	   {
	     if(  (*compareFn)(vect[i], tofind) )
	     {
	       vline = vect[i];
	       return vline;
	       break;
	     }
	   }	   
	 }//next  
	 return vline;
}//vectorFind

int vectorFind(vector<int> vect, int tofind, bool (*compareFn)(int item, int item2))
{  
  for(int i=0; i<vect.size(); i++)
	 {
	   if(compareFn!=NULL)
	   {
	     if(  (*compareFn)(vect[i], tofind) )
	     {
	        return vect[i];	     
	       break;
	     }
	   }	   
	 }//next  
	 
	 return -1;
}//vectorFind

string vectorContains(vector<string> vect, string tofind, bool (*compareFn)(string item, string item2))
{
  return vectorFind(vect,tofind,compareFn);
}//vectorFind

int vectorContains(vector<int> vect, int tofind, bool (*compareFn)(int item, int item2))
{
  return vectorFind(vect,tofind,compareFn);
}//vectorFind

string vectorJoin(vector<string> vect, string join_sep=",")
{
  string output="";
  for(int i=0; i<vect.size(); i++)
  {
    if(output.length()>0) output+=join_sep;      
    output+=vect[i];    
  }
  return output;
}

//-----------------------------------------------------------

/*
 //énum map key and value
	map <int,string> m;
	m[9]="lundi";
	m[10]="mardi";
m[11]="mercredi";

	map<int,string>::iterator  it;
	
	it=m.begin();
  for (; it != m.end(); it++ )
  {
	  disp( (*it).first, (char*) (*it).second.c_str() );
  }
box("exit");exit(9);
*/

template <typename tKey, typename tValue>
class HashTable
{
	
private:


public:
map<tKey,tValue>m;
void put(tKey key,tValue value)
{
	m[key]=value;
}

void get(tKey key)
{
	return m[key];
}

//surcharge d'opérateur [
	tValue& operator[](tKey key)
	{			
		return m[key];
	}

	int size()
	{		
		return m.size();
	}
	//enum and get key list

/*
	deque <tKey> enumKeys()
	{
		deque <tKey> d;
		 map<tKey,tValue>::iterator  it;
	
		  it=m.begin();
		  for (; it != m.end(); it++ )
		  {
			  d.push_back( (*it).first);
			  //d.push_back((*it).second);
		  }
		return d;
	}

	deque <tValue> enumValues()
	{
		deque <tValue> d;
		 map<tKey,tValue>::iterator it;
	
		  it=m.begin();
		  for (; it != m.end(); it++ )
		  {
			  //d.push_back( (*it).first);
			  d.push_back((*it).second);
		  }
		  return d;
	}
	*/
};

bool compareDirectories(string d,string d2)
{
  trim(d);
  trim(d2);
  if(endsWith(d,"/"))
  {
    if(!endsWith(d2,"/"))
    {
      d2+="/";
    }    
  }else
  {
    if(endsWith(d2,"/"))
    {
      d2=d2.substr(0,d2.length()-1);
    }  
  }
  return (d==d2);
}

//TODO: Type de retour en cas d'erreur de création du répertoire
void createDirectory(string path, bool recursv=true)
{
  string command="mkdir ";
  if(recursv)command+="-p ";
  execute(command+"\""+path+"\"");
}

#define makeDir createDirectory

//copy directory with content
void copyDirectory(string path, string dest)
{
  string command="cp -R \""+path+ "\" \""+dest+"\"";
  execute(command);
}

//copy binary file to destination
void copyFile(string srcfile, string destfilename)
{
  std::ifstream  src(srcfile.c_str(), std::ios::binary);
  std::ofstream  dst(destfilename.c_str(),   std::ios::binary);
  dst << src.rdbuf();
}
	
bool directoryExists( string dir )
{
  

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (dir.c_str());

    if (pDir != NULL)
    {
        bExists = true;    
        (void) closedir (pDir);
    }

    return bExists;
}

void listdir(string name, vector<string> &dirs)
{   
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name.c_str())))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) 	
	{
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name.c_str(), entry->d_name);	    
	    dirs.push_back(path);            
	    printf("[%s]\n", path);
            //recursive call
	    listdir(path, dirs);
        } else {
	  //here files
           // printf("%s\n",entry->d_name);
        }
    }
    closedir(dir);
    
    return;
}//end listdir recursive


//# mise à jour le 2020-07-11 15:56:39
//write file to disk !
bool writeToFile(string outputFileName, string content)
{
  ofstream outputfile;
  outputfile.open(outputFileName.c_str()); 
	if( !outputfile.is_open() )
	{
		cerr << "writeToFile() '"<<outputFileName<<"' Error: " << strerror(errno)<<endl;
		return false;
	}
  outputfile<<content; 
  outputfile.close();
  return true;
}//writeToFile
#define writeTextFile writeToFile

bool appendToFile( string outputFileName, string content )
{
	ofstream outputfile;
	outputfile.open(outputFileName.c_str(),std::ios_base::app); 
		if( !outputfile.is_open() )
		{
			cerr << "writeToFile() '"<<outputFileName<<"' Error: " << strerror(errno)<<endl;		
			return false;
		}
	outputfile<<content; 
	outputfile.close();
	return true;
}//appendToFile



// // Usage example: filePutContents("./yourfile.txt", "content", true);
// void filePutContents(const std::string& name, const std::string& content, bool append = false) {
//     std::ofstream outfile;
//     if (append)
//         outfile.open(name, std::ios_base::app);
//     else
//         outfile.open(name);
//     outfile << content;
// }


// remove directory and just return the filename
string getBasename(string dir_and_basename)
{
  return basename(dir_and_basename.c_str()); 
}

//compatible windows
//extract/gives directory from a file path
string getDirectory(string fullname)
{
 string directory="";
  int l=fullname.find_last_of("\\");
  if(l<0) l=fullname.find_last_of("/");
  directory=fullname.substr(0,l);
  return directory;   
}//getDirectory


string getDirectoryBasename(string fullname)
{
    string dir = getDirectory( fullname );
    int l=-1;
    if(l<0) l=dir.find_last_of("/");
    string directory=dir.substr(l+1);     
    return directory;   
}//getDirectoryBasename



//lire et écrire un fichier de conf
//config file class
/*
////////config.txt
top=10
left=100
startServer=true
serverPort=6891
*/
//config file ------------utilisation
/*
ConfigFile cfile("config.txt");
	if( cfile.read() ==0) 
	{
	int port=cfile.getiVar("serverPort");//le port server du client	
	bool servSockActivated=cfile.getbVar("startServer");
	string servername=cfile.getVar("server");
	}else{
	  cfile.writeVar("currentEdit",tracktionEditFile);
	  cfile.writeVar("tracktionEdits","elem1;el2");
	  printCool("Nouveau fichier de config créé");
      }

*/
//penser à mettre à jour dans iotools.h
//mis à jour le 11 fev 09
class ConfigFile
{
private:

	FILE *f;	
	int lineBufferSize;
	char *buffer;

	void init()
	{
		f=NULL;
		filename="";
		setLineBufferSize(1024);
	}

public:
	string filename;
	map<string/*first: name*/,string/*second: value*/>variables;


	ConfigFile()
	{
		init();
	}

	ConfigFile(string filename)
	{
		init();
		this->filename=filename;
	}

	void setLineBufferSize(int value)
	{
		lineBufferSize=value;
		buffer = new char [lineBufferSize];
	}
	bool hasVar(string varname)
	{
	 	 
	      map<string, string>::iterator it;
		    it=variables.begin();
	      for (; it != variables.end(); it++ )
	      {
		 string kv=  (*it).first;
		  string vv =(*it).second;
		  if(kv==varname)
		  {
		    return true;
		  }
		  /*if(vv==value)
		  {
		    key=(*it).first;
		   if(ifound!= NULL)(*ifound)=key;
		   return true; //break; 
		  }*/
	      }
	      
	  return false;
	}
	//privé------
	void addVar(string line)
	{
		string varname,varval;
		int r,l=line.find_first_of("=");
		if(l>=0)
		{
			varname=line.substr(0,l);
			varval=line.substr(l+1);			
			trim(varname); trim(varval);			
			variables[varname]=varval;
		}
		//cas ou on a la longueur à lire:
		l=line.find("[");
		varname=line.substr(0,l);
		trim(varname);
		if(l>0)
		{
			r=line.find("]");
			if(r>l)
			{
				string slen = between(line,"[","]");
				int len = strToInt( slen );
				//lire len caractère:
				char *data = new char[len+1];
				int nRead=fread( data,1, len, f);
				data[nRead]=0;
				varval=data;
				variables[varname]=varval;
			}
		}
	}//addVar


/*
	static void extractVar(string line,string &varname,string &varval)
	{	
		int l=line.find_first_of("=");
		if(l>=0)
		{
			varname=line.substr(0,l);
			varval=line.substr(l+1);
			trim(varname); trim(varval);						
		}
	}
*/

	//pour lire une variable binaire, on utilise extractVar
	static void extractVar(FILE *f,string line,string &varname,string &varval, int &len, int &rest)
	{	
		
		int l=line.find_first_of("=");
		string slen="";
		len=-1; rest=0;
		if(l>=0)
		{
			varname=line.substr(0,l);
			//check for '[' ']' in var name
			slen=between(varname,"[","]");
			if( !slen.empty() )
			{
				len=strToInt(slen); 
				//retreive varval bytes:
				varval=line.substr(l+1); //premiere ligne.				
				int lu=varval.length();
				rest = len-lu;
				char * buf = new char[len+1];
				strcpy(buf, varval.c_str()) ;
				fread(buf+varval.length(),1,rest,f);				
				buf[len]=0;
				varval=buf;		
				//remove length from varname
				int lp=varname.find_first_of("[");
				varname=varname.substr(0,lp);
				
			}else
			{
			//retreive varval
			varval=line.substr(l+1);
			trim(varval);						
			}
			
			trim(varname);
		}
	}

	string getVar(string varname)
	{
		return variables[varname];
	}

	int getiVar(string varname)
	{
		return strToInt(getVar(varname) );
	}
	
	float getfVar(string varname)
	{
		return strToFloat( getVar(varname) );
	}

	bool getbVar(string varname)
	{
		if(! strToLower( getVar(varname) ).compare("true") )
		{
			return true;
		}else if(! strToLower( getVar(varname) ).compare("1")  )
		{
			return true;
		}
		else return false;
	}
	
	//à implémenter:

	int removeVar(string varname)
	{
		//box(varname,"removing var!");
		int r=read() ;
		if( r!= 0 )return r;
		//read and find
		f=fopen(filename.c_str(),"r");
		string filename2=filename+"~";
		FILE *f2=fopen(filename2.c_str(),"w+"); 
		if(!f2){ printErrLn("ConfigFile Erreur d'écriture"); return -1; }
		if(!f){ printErrLn("ConfigFile error file not Found :'"+filename+"'");   }
		string varnameRead,varvalRead;
		int pos=0,len,rest;
		while(true)
		{					
			fgets(buffer,lineBufferSize,f);	

			if( feof(f) ){//ya rien à lire, le fichier est vide
				break;}
			string line = buffer;
			//box("["+line+"]","line");		
			ConfigFile::extractVar(f,line,varnameRead,varvalRead, len, rest);
		
			//box(varvalRead, varnameRead);
			//on trouve ce qu'on cherche
			//box(" varnameRead: ("+varnameRead+")"); box(" varnameRead: ("+varname+")");

			
			if(!!varnameRead.compare(varname) )
			{				
				//réécrire la ligne dans le fichier tamporaire:		
				line+="\n";
				if(len>0)
				{//réécrire les ligne
					
					fwrite(line.c_str(),1,line.length()-1,f2);
					if(rest>0)
					{
						varvalRead+="\n";
					   fwrite(varvalRead.substr((len-rest) ).c_str() ,1,rest,f2);
					}
				}else
				{			
					fwrite(line.c_str(),1,line.length()-1,f2);
				}
			}else
			{	//c'est la variable que l'on veut supprimer				
				//ne rien faire (soit effacer la ligne)
				if(len>0)
				{/*
					char *trashBuff = new char [rest];
					disp(rest,"rest");
					fread(trashBuff,1,rest,f);// lire le reste de la valeur, pour que cela n'interfère pas avec le reste;
					box(trashBuff,"trashbuffer");
					*/
				
				}
			}

			if( feof(f) )
			{
				break;
			}
		}
		fclose(f2);
		fclose(f);
		//remove f
		this->remove();
		//rename f2
		rename(filename2.c_str(),filename.c_str());	
		return 0;
		//removeline
	}

	//public rempli le map de var
	int read()
	{
		f=fopen(filename.c_str(),"r");	
		if(!f){ printErrLn("ConfigFile error file not Found :'"+filename+"'");  return -1;  }
		
		while(true)
		{
			fgets(buffer,lineBufferSize,f);				
			//addVar into map
			addVar(buffer);
			if( feof(f) )
			{
				break;
			}
		}
		fclose(f);
		return 0;
	}

	bool exists()
	{
		return fileExists(filename);		
	}

	bool remove()
	{
		 return removeFile(filename );
	}

	void create()
	{
		FILE * f;
		// //Check if path exists :
		// string parentDir = getDirectory(filename);
		// if(!directoryExists(parentDir))
		// {
		// 	createDirectory(parentDir, true); //ensure directories exist
		// }
		f=fopen(filename.c_str(),"w+");
		fclose(f);
	}

	void writeVar(string varName, string varValue)
	{
		removeVar(varName);
		FILE *f;
		f=fopen(filename.c_str(),"a+");	
		fprintf(f,"%s=%s\n",(char*)varName.c_str(),(char*)varValue.c_str() );
		fclose(f);
	}
	
	void writeaVar(string varName, string varValue)
	{		
		writeVar(varName,varValue);
	}

	void writeVarBytes(string varName,char * buff,int len)
	{
		string varName2=varName+"["+intToStr(len)+"]";
		
		removeVar(varName);

		FILE *f;
		f=fopen(filename.c_str(),"a+");	
		fprintf(f,"%s\n%s\n",(char*)varName2.c_str(),buff );
		fclose(f);
	}

	void writeVarBytes(string varName,string multilineStr)
	{
		writeVarBytes(varName,(char*) multilineStr.c_str(), multilineStr.length() );
	}

	void writeiVar(int i,string varname)
	{
		writeVar(varname, intToStr(i) );	
	}
	void writefVar(string varname, float f)
	{
		writeVar(varname, floatToStr(f) );	
	}
	void writebVar(bool b,string varname)
	{
		writeVar(varname,boolToStr(b) );
	}
}; //end ConfigFile



void msSleep(int ms) {
usleep(ms*1000); //convert to microseconds
return;
}

void sleepMs(int ms) {
usleep(ms*1000); //convert to microseconds
return;
}

//clock sleep or delay
void csleep(unsigned int msecs) {
clock_t goal = msecs*CLOCKS_PER_SEC/1000 + clock();  //convert msecs to clock count  
while ( goal > clock() );  
}


//not tested
void lsleep(int value)
{
    
    clock_t goal = value*CLOCKS_PER_SEC/1000 + clock();  //convert msecs to clock count  
    while ( goal > clock() )
    {
            usleep(10);
    }  
    
}



#endif
 