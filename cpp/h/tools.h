#ifndef TOOLS_H
#define TOOLS_H "TOOLS_HEADER_FILE"
/**Util***/
#include "iotools.h"
//#include "maths.h"


#include <iostream>
#include <ctime>

#include <sys/time.h>
#include <unistd.h>
#include <pwd.h> //getHomeDirectory

#include <csignal>
#define DEBUGGER std::raise(SIGINT);      

///javascript: new Date(1503684558848) 
// std::cout << "gettimeMs:"<<getTimeMs() << std::endl; //javascript: new Date(1503684558848)
long long getTimeMs()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds    
    return mslong;  
}//getTimeMs

long long getTime() {return getTimeMs();}
    
      //utilisation:
      // long l = strTimeToMs("2017-10-17 17:23:00");
      long strTimeToSec(string ymdhms)
      {
	
	 struct tm tm;
	 char buf[255];
	 memset(&tm, 0, sizeof(struct tm));
	 string format = "%Y-%m-%d %H:%M:%S";
	 if(ymdhms.length()<=10) format="%Y-%m-%d";
	 if(ymdhms.length()<=16) format="%Y-%m-%d %H:%M";
	 strptime(ymdhms.c_str(),format.c_str(), &tm);
   
	  time_t ts = mktime(&tm);
	  return ts;
      }
 
      //utilisation:
      //long l = strTimeToMs("2017-10-17 17:23:00");
      //printf("ms=%ld\n",l);
      //cout<<"ms="<<l<<endl;
      long strTimeToMs(string ymdhms)
      {   
	  time_t ts = strTimeToSec(ymdhms);
	  
	  return (ts*1000);
      }
      
/*
/// human readabledate time
//results looks like: Fri Dec 15 12:45:11 2017
printf("Last status change:       %s", ctime(&st.st_ctime));
printf("Last file access:         %s", ctime(&st.st_atime));
printf("Last file modification:   %s", ctime(&st.st_mtime));
*/	   
  
//utilisation de msleep (millisec sleep)
void msleep(long long int ms)
{
  usleep(ms*1000);
}


class LengthUnits
    { 
	public:

        static string lengthToString(long long octet)
        {
            string str = "";		 
			double resKo = (((double)octet) /(LengthUnits::GetNum("Ko"))  );		
			double resMo = (((double)octet) / LengthUnits::GetNum("Mo"));
			double resGo = (((double)octet) / ((long)LengthUnits::GetNum("Go"))  );
			char carray[10];
            if (resGo > 1)
            {                
				sprintf(carray,"%1.3f Go", resGo);
				return carray;
            }
            if (resMo > 1)
            {
                sprintf(carray,"%1.2f Mo", resMo);
				return carray;
            }
            if (resKo > 1)
            {
               sprintf(carray,"%1.2f Ko", resKo);
			   return carray;
            }
            if (resKo <= 1)
            {
			
                sprintf(carray,"%u", (unsigned int) octet);
				return carray;
            }

            return str;
        }

		//renvoi le nombre multipliant pour ki mo et go
        static long GetNum(string KoMoGo)
        {
			if (!KoMoGo.compare("Ko") )
            {
                return 1024;
            }
			else if (!KoMoGo.compare("Mo") )
            {
                return 1024 * 1024;
            }
			else if (!KoMoGo.compare("Go") )
            {
                return 1024 * 1024*1024;
            }
            return 0;
        }
    };

/*
//utilisation

		FileW f(L"D:\\Boon",true);				
		sendLog( f.fullname );
		sendLog( f.directory );
		sendLog( f.name );
		sendLog( f.extension );
		sendLogi( f.length ,"length=");
		sendLogb( f.isFile, "isFile=" );
		sendLogb( f.isDirectory,"isdir=" );
*/
//class file unicode

string getFileDirectory(string fullname)
{
  string directory="";
  			int l=fullname.find_last_of("\\");
			if(l<0) l=fullname.find_last_of("/");
			directory=fullname.substr(0,l);			
			return directory;
}

//class file Ansi
class File
{
private: 
	void init()
	{
		length=-1;
		fullname="";
		extension="";
		name="";
		directory="";
		isFile=false;
		this->isDirectory=false;
		exists=false;		
	}
public:
	long length;
	string fullname,directory,name,extension;
	bool isFile,isDirectory,exists;
	FILE *f;

	File()
	{
		init();
	}
	

	File(string filename0,bool retreiveInfo=true)
	{	
		init();
		fullname=filename0;
		
		getInfo();
	}

	void getName()
	{
		getDirectory();		
	}

	string getDirectory()
	{
			int l=fullname.find_last_of("\\");
			if(l<0) l=fullname.find_last_of("/");
			directory=fullname.substr(0,l);
			name=fullname.substr(l+1);
			return directory;
	}

	void getExt()
	{
		int l=fullname.find_last_of(".");
		if(l<0)
		{
			extension="";
		}else
			extension= fullname.substr(l+1);
	}
	
	long int getLength()
	{
		checkIfExists();
		//if(exists && !this->isDirectory )
			length=getFileSize( fullname );
		//else length=-1;
			return length;
	}
	
	void checkIfExists()
	{
		exists=fileExists(fullname);
		
		if(!exists)
		{//check directory case
			exists=directoryExists(fullname);		
			if(exists) this->isDirectory=true; else this->isDirectory=false;
		}else
		{//is file
			this->isFile=true;
		}	
	}
	
	void getInfo()
	{
		checkIfExists();
		getLength();
		getDirectory();
		getExt();
	}
		//-------------------------------
	bool open()
	{				
		f= fopen(fullname.c_str(),"rb");
		if(f)return true; else return false;
	}

	inline FILE* getFilePtr(){return f;}

	inline void close() { if(f)fclose(f);}
	
	
	//copy binary file to destination
	void copyTo(string destfilename)
	{
	  std::ifstream  src(this->fullname.c_str(), std::ios::binary);
	  std::ofstream  dst(destfilename.c_str(),   std::ios::binary);
	  dst << src.rdbuf();
	}
};




StringMap listDir(string dir, bool recur=false)
{
	throw ("utiliser dirent.h, voir iotools.h");
	return StringMap();
}




//overload with filemap
//lister l'arborescence d'un dossier
/*
 * FileMapW listFilesEx(wstring dir,bool recursive,wstring &errorMess, bool getInfo=false)
{	
	wstring subDir=L"",subDirToList=L"",str=dir,filename,filename2;
	FileMapW sm,sm2;
	WIN32_FIND_DATAW finddataa; 
	HANDLE hf=INVALID_HANDLE_VALUE;
	hf=FindFirstFileW(dir.c_str(),&finddataa);
	errorMess=L"";
	if (hf!=INVALID_HANDLE_VALUE)
	{
		if(finddataa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			str.append(L"\\*");
			hf=FindFirstFileW(str.c_str(),&finddataa);
			filename=finddataa.cFileName;//add first file
				sm.add( filename , getInfo  );
			//ici la boucle de parcours des fichiers:
			while( FindNextFileW(hf,&finddataa) )
			{					
				filename=finddataa.cFileName;
				sm.add( filename , getInfo  );
				if(recursive)//si r�cursif,on va en profondeur
				{	//si ce dernier est un dossier diff�rent de ".."
					if( (finddataa.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)&&(filename.compare(L"..")) )
					{
						str=dir; str.append(L"\\"); str.append(filename);
						FileMapW sm2=listFilesEx(str,true,str,false);//don't care for mess
						wstring dir2=filename; dir2.append(L"\\");	
						
						//sendLog(wsToStr( dir2) ,LOG_LRED);
					
						sm.add(dir2,sm2,getInfo);												
					}
				}
			}
		}else
		{//ce n'est pas un r�pertoire:
			errorMess.append(dir);
			errorMess+=L" n'est pas un r�pertoire.";
		}
	}else //fichier n'existe pas
	{
		errorMess.append(dir);
		errorMess+=L" n'existe pas.";
	}
	return sm;
}
*/
//lister l'arborescence d'un dossier
/*
FileMapW listFiles(wstring dir, bool recur=false, bool getinfos=true )
{
	wstring mess;
	FileMapW result=listFilesEx(dir,recur,mess, getinfos);	
	return result;
}
*/



/*
procedure TForm_login.Edit_incrementalChange(Sender: TObject);
var
  I,clNotFound:integer;
  pattern,found:string;
  list:TStringList;
  node:TTreeNode; nodeInfo:TNodeInfo;
begin
  clNotFound:=RGB(255,235,230);
  if Edit_incremental.Text='' then
  begin
    Edit_incremental.Color:=clWindow;
    exit;
  end;
  list:=TStringList.Create;
  pattern:=Edit_incremental.Text;
  for I := 0 to tvContactList.Items.Count - 1 do
  begin
    list.Add(tvContactList.Items[I].Text);
    nodeInfo:=getNodeInfo(tvContactList.Items[I]);
    if nodeInfo<>nil then list.Add(nodeInfo.key);
  end;
  found:=incrementalSearch(list,pattern);
  if found='' then Edit_incremental.Color:=clNotFound
    else Edit_incremental.Color:=clWindow;
node:=getNodeByText(tvContactList,found);
if node=nil then exit;
tvContactList.HideSelection:=False;
node.selected:=true;
end;

//ranger les �l�ments d'une liste de chaine par ordre de taille
 {algorithme 2}
function orderByLength(slist:TStringList):TStringList;
var list:TStringList;
  I:integer; tempStr:string;
begin
  list:=slist;
  Result:=slist;
    I:=0;
    if list.Count<=1 then exit;
    while I<(list.Count-2) do
    begin
      if Length( list[I] )>length(list[I+1]) then
      begin
        tempStr:=list[I];
        list[I]:=list[I+1];
        list[I+1]:=tempStr;
        I:=-1;
      end;
      I:=I+1;
    end;
  Result:=slist;
end;


function inStrIgnoreCase(str,pattern:string):boolean;
begin
  str:=LowerCase(str); pattern:=LowerCase(pattern);
  Result:=inStr(str,pattern);
end;

function incrementalSearch(list:TStringList; search:string):string;
var I:Integer;
begin
  list:=orderByLength(list);
  for I := 0 to list.Count - 1 do
  begin
    if inStrIgnoreCase(list[I],search) then
     begin
       Result:=list[I];
       break;
     end;
  end;
end;
*/

/*
//ranger les �l�ments d'une liste de chaine par ordre de taille
{algorithme 2}
function orderByLength(slist:TStringList):TStringList;
var list:TStringList;
  I:integer; tempStr:string;
begin
  list:=slist;
  Result:=slist;
    I:=0;
    if list.Count<=1 then exit;
    while I<(list.Count-2) do
    begin
      if Length( list[I] )>length(list[I+1]) then
      begin
        tempStr:=list[I];
        list[I]:=list[I+1];
        list[I+1]:=tempStr;
        I:=-1;
      end;
      I:=I+1;
    end;
  Result:=slist;
end;

*/
/**/
StringMap orderByLength(StringMap slist)
{
	StringMap list;
	string tempStr;
	int i;
	list=slist;
    i=0;
	if( list.count()<=1)return list;

    while( i<(list.count()-2) )
	{
		
	  if( list.item(i).length()>list.item(i+1).length() )
	  {
		  tempStr=list.item(i);
		  list.m[i]=list.m[i+1];
		  list.m[i+1]=tempStr;
          i=-2;
	  }
	  i++;
	}
  return list;
}


string incrementalSearch(StringMap list, string search)
{ 
 list = orderByLength(list);
 string retour;
  /*recherche du plus probable*/
  for(int I=0; I<(list.count()-1); I++)
  {	
    if(inStr(list.item(I),search,true))
	{	
		retour=list.item(I);
       break;
	}
  }  
  return retour;
}

/*
int getDiskBusySpace(LPCSTR drive,__int64 &free,__int64 &total)
{
	
   __int64 freeBytes0,totalBytes,totalFreeBytes;
  ::GetDiskFreeSpaceEx(drive,(PULARGE_INTEGER)&freeBytes0,(PULARGE_INTEGER)&totalBytes,(PULARGE_INTEGER)&totalFreeBytes);
 //  printf("total free bytes %d\n", totalFreeBytes/1024/1024);
//printf("total bytes %d\n", totalBytes/1024/1024);
  total=totalBytes/1024/1024;
  free=totalFreeBytes/1024/1024;


  return _round((float) ((double)(total-free)/(double)total)*100 ) ;
}
*/


//DateTime utils:
class DateTime
{
private:

public:

	/*  utilisation de addSystemTime:
		SYSTEMTIME st;
		SYSTEMTIME st1;
		SYSTEMTIME st2;

		GetLocalTime(&st);
		clearSystemTime(&st2);
		st2.wSecond=180;
		st2.wHour=48;
		st1=DateTime::addSystemTime(st,st2);
		//st1=>result
		*/
/*	static SYSTEMTIME addSystemTime(SYSTEMTIME  st1, SYSTEMTIME st2)
	{
		SYSTEMTIME stRes=st1;
		stRes.wMilliseconds+=st2.wMilliseconds;
		stRes.wSecond+=st2.wSecond;
		stRes.wMinute+=st2.wMinute;
		stRes.wHour+=st2.wHour;
		stRes.wDay+=st2.wDay;
		stRes.wYear+=st2.wYear;

		adjustSystemTime(&stRes);
		return stRes;
	}

	static void adjustSystemTime(SYSTEMTIME *st0)
	{
		SYSTEMTIME st=*st0;
		int dividand,diviseur;
		int quotient,rest;
	 
		diviseur=1000;		
		dividand=st.wMilliseconds;
		divE(dividand,diviseur, quotient, rest); 
		//sendLogi(quotient,"q=");		sendLogi(rest,"r=");
		st.wSecond+=quotient;
		st.wMilliseconds=rest;
		//Secondes en minutes
		diviseur=60;
		dividand=st.wSecond;
		divE(dividand,diviseur, quotient, rest);
		st.wMinute+=quotient;
		st.wSecond=rest;
		//minutes en heures
		diviseur=60;
		dividand=st.wMinute;
		divE(dividand,diviseur, quotient, rest);
		st.wHour+=quotient;
		st.wMinute=rest;
		//heures en jour:
				diviseur=24;
		dividand=st.wHour;
		divE(dividand,diviseur, quotient, rest);
		st.wDay+=quotient;
		st.wHour=rest;	
		//conversion jour en ann�e n'est pas prise en compte	
		*st0=st;
	}

	static void clearSystemTime(SYSTEMTIME *outSt)
	{
		outSt->wYear=0;
		outSt->wMonth=0;
		outSt->wDay=0;			
		outSt->wHour=0;
		outSt->wMinute=0;
		outSt->wSecond=0;
		outSt->wMilliseconds=0;	
	}

	//"dd/MM/yyyy HH:mm:ss ms"
	static SYSTEMTIME parse(string frStr)
	{	
		SYSTEMTIME st;
		string sd,sm,sy,sh,smin,ssec;
		sd=frStr.substr(0,2); //box(sd,"day");
		sm=frStr.substr(3,2); //box(sm,"month");
		sy=frStr.substr(6,4);  //box(sy,"year");
		sh=frStr.substr(11,2);  //box(sh,"hour");
		smin=frStr.substr(14,2); //box(smin,"smin");
		ssec=frStr.substr(17,2); //box(ssec,"sec");
	//    string sms=frStr.substr(19); box(sms,"ms");

		st.wMilliseconds=0;

		st.wDay=strToInt(sd);
		st.wHour=strToInt(sh);
		st.wSecond=strToInt(ssec); 
		st.wMinute=strToInt(smin);
		st.wMonth=strToInt(sm);
		st.wYear=strToInt(sy);
			return st;
	}
	
	*/
};


short monthToInt(string str)
{
  short m=-1;
  str = strToLower(str);
  if(startsWith(str, "jan")) m=1;
  if(startsWith(str,"feb")) m=2;
  if(startsWith(str,"mar")) m=3;
  if(startsWith(str,"apr")) m=4;
  if(startsWith(str,"may")) m=5;
  if(startsWith(str,"jun")) m=6;
  if(startsWith(str,"jul")) m=7;  
  if(startsWith(str,"aug")) m=8;
  if(startsWith(str,"sep")) m=9;
  if(startsWith(str,"oct")) m=10;
  if(startsWith(str,"nov")) m=11;
  if(startsWith(str,"dec")) m=12; 
  return m;
}


//convert int<10 to a two digit string; exemple:   9 => 09
string padZero(int m)
{  
  string str ="";
  if((m>=0)&&(m<=9)) str="0";
  str+=intToStr(m);
  return str;
}

string padLeft(string m,string pad="0")
{
  string str;
  if(m.length()==1)str=pad;
  str+=m;
  return str;
}



string padLeft(int m,int len=2, string pad="0")
{
  string str=intToStr(m);
  while(str.length()<len)
  {
      str.insert(0,pad);      
  }
  return str;
}





//convert milliseconds value to hh:mm:ss 
//   int h, m, s,ms;
//   timeSpan( (60000*60)+60000+15 , h, m, s,ms); // 1 heure 1 minute 15 secondes
void timeSpan(long long int ms, int &hours, int &mins, int &secs,int &mill)
{    
  string smin="",ssec="", shour="",sms="";
  long long int s = (ms/1000);
  long long int m=0;
  long long int h=0;
  long long int ms2=0;
  long long int rest=0; 
  
  ms2=ms%1000;
  
  
  if(s>60)
  {
    m = s/60;
    rest=s%60;
    if(m>=1)
    {
      s=rest;
    }
  }    
  if(m>=60)
  {
    h = m/60;
    rest=m%60;
    if(h>=1)
    {
      m=rest;
    }
  }
    hours = h;
  mins= m;
  secs = s;
  mill=ms2;
  if(ms<1000)
  {
    mill=ms;
  }  
  //printf("secs %lld \n",s);
  //printf("minutes %lld \n",m);
  //printf("Hours %lld \n",h); 
  //printf("millisecs %lld \n",ms2);
}//end timeSpan
 
 
string plural(int value, string single, string plur)
{
  if(value>1)
  {
    return plur;
  }else return single;
}
 
 
 
//   cout << strTimeSpan( (60000*60*2)+180000+16000+23).c_str()<<endl;
//   renvoie: 2 heures, 3 minutes, 16 secs, 23 millisecs
string strTimeSpan(long long int duration,bool compact=false, bool displayMs=false)
{  
  int h, m, s,ms;
  timeSpan(duration , h, m, s,ms);
  string str = "";
  if(compact) //always
  {    
    str = padZero(h)+":"+padZero(m)+":"+padZero(s);
  }else
  {   
    str="";
    
    if(h<=0)
    {     
	if(m<=0)
	{
	  	    
	}else
	{
	   str += intToStr(m)+plural(m," minute", " minutes")+", ";
	}
    }else
    {
      str += intToStr(h)+ plural(h," heure"," heures")+ ", "
      + intToStr(m)+plural(m," minute", " minutes")+", ";      
    }        
     str+=intToStr(s)+plural(s," sec"," secs");
     if(displayMs)
       str+=intToStr(ms)+plural(ms,", millisec",", millisecs");
  }
  return str;
} 

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string currentDateTime(string format="%Y-%m-%d %X") {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), format.c_str() , &tstruct);
    return buf;
}

string getCurrentDateTime(string format="%Y-%m-%d %X")
{
    return currentDateTime(format);
}

string dateTimeNow() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}


string timeNow() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}


//---------------------------------------------- End Time Functions --------------------------------------


#pragma region process functions

StringMap findProcesses(string processPattern)
{
  StringMap sm;
  string command="ps aux | grep \""+processPattern+"\"";
  string result=execute(command);
  sm.addRange(splitStr(result,'\n'));
  return sm;
}


string findProcessLine(string processPattern, int commandProcessCount=3)
{
   
  StringMap sm = findProcesses(processPattern);
  //remonter les x derniere lignes:
  int fromLine=commandProcessCount;
  int len = sm.count();
  string lastProcessLine="";
  for(int i=(len-1); i>=0; i--)
  {    
    string line = sm[i];
    //printf("%d==>%s\n",i,line.c_str());     
    if( (len- i) == fromLine)
    {   
      lastProcessLine = line;
      break;    
    }   
    //usleep(1); 
  } 
  
  return lastProcessLine;
}

//return last process id matching pattern
int findProcess(string processPattern, int commandProcessCount=3)
{
  string lastProcessLine=findProcessLine(processPattern, commandProcessCount);
  int pid= -1;
  vector<string> parts = splitStr(lastProcessLine,' ');
  
  int valid = 0;
  StringMap vparts;
  for(int i=0; i<parts.size(); i++)
  {
    string str = trimStr(parts[i]);
    if(str.length()>0)
    {
      vparts.add(parts[valid]);      
    }
    valid++;
  }
  string user=vparts[0];
  string spid=vparts[1];
  pid=strToInt(spid);
  string command=vparts[10]; //ATTENTION WARNING10? commande peut avoir des espaces
  return pid;
}

void killProcess(int pid)
{
  string c = "kill "+intToStr(pid);
  string r = execute(c);  
}

#pragma endregion

/*
 * unicode hexad�cimal caract�res sp�ciaux accents aigu grave
 � : \x81 
� : \x82 
� : \x83 
� : \x84 
� : \x85 
� : \x87 
� : \x88 
� : \x89 
� : \x8A 
� : \x8B 
� : \x8C 
*/

//works with SFR Numéricable Box on Linux uses Curl
string getMyRemoteIP(string gateway="192.168.0.1")
{      
    string response = execute("curl "+gateway);    

    ////#Premiere version de numéricable connue
    // string iphtml = between(response,"Votre adresse IP :","Votre masque de sous r\xc3\xa9s"); //attention aux accents
    // iphtml=between(iphtml,"<span class=\"fontestableau1\">","<");    

    //seconde version de numéricable connue : Version matériel: 	1.0 ;  Version logiciel: 	NCC_5.3.14
    string iphtml = between(response,"Votre adresse IP","</tr>"); //attention aux accents
    iphtml=between(iphtml,"<td align=\"left\">","</td>");    
    return iphtml;
}
        
        
        
        
        
//parsing xml attributes line !
map<string,string> extractAttributes(string inner_attrs)
{
  map<string,string> m;
  
  int l=0, lastRight=0;
  string attvalue="";
  string line= inner_attrs;
  while(true)
  {
    int left = line.find("=");
    if(left<0) break;
    string attname = trimStr( line.substr(l, left));    
    attvalue = line.substr(left+1);
    line=attvalue; //the value and the rest  
    attvalue=between(attvalue,"\"","\"",lastRight);  
    line=line.substr(lastRight+1);//begin by the end
    //printf("attname=[%s] et attvalue[%s]\n",attname.c_str(),attvalue.c_str());
    m[attname]=attvalue;    
  }
  return m;
}//end extractAttributes


//get the home dir

string getHomeDirectory()
{  
  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  return string(homedir);
}

string getHomeDir()
{  
  return getHomeDirectory();
}



//--------------------------------------------------  Some Other Tools -------------------------------------------------
  string urlEncode(string stuff)
  {    
      string output = replaceString(stuff,"&"," %26"); //encode
      output= replaceString(output,"@","%40"); //encode to avoid [java@lang.s< troubles
      return output;
  }
  


#endif
        
