 	#include <iostream>
    #include <stdio.h>
    #include <stdlib.h>
    #include <fstream>
    #include <string>
	#include <sstream>
    #include <cstring>
    #include <time.h>
    
    using namespace std;
string username,password,confirmpassword,clearance,hashString;
string fileOwner,fileName,fileClearance;
ofstream writeSaltFile ("salt.txt",ios::app);
ifstream readSaltFile ("salt.txt");
ofstream writeShadowFile ("shadow.txt",ios::app);
ifstream readShadowFile ("shadow.txt");
ofstream writeFileStore ("Files.store",ios::app);
ifstream readFileStore ("Files.store");

void WriteInFileStore(string line)
{
if (writeFileStore.is_open())
  {
    writeFileStore.seekp (0, ios::end);
    writeFileStore << line << "\n";
  }
  else
  {
  	cout<<"Error on writing file"<<endl;
  }
}
bool IsFileNameAlreadyExists(string fName)
{
string line;
string temp;
string substr1;
int i=0;
bool ok=false;
if (readFileStore.is_open())
  {
    while ( getline (readFileStore,line))
    {
      stringstream s_stream(line); //create string stream from the string
	  i=0;
   while(s_stream.good() && i<3) {
      
      getline(s_stream, substr1, ':'); //get first string delimited by comma
	  cout<<substr1;
      if(i==0)
	  {
		  fileName=substr1;
		  if(fName.compare(substr1)==0){
			   ok=true;
		  }
	  }
	  else if(i==1)
	  {
		  fileOwner=substr1;
		  
	  }
	  else if(i==2)
	  {
		  fileClearance=substr1;
	  }
	  i++;
    }
	if(ok) return ok;
  }
}
  return ok;
}
void ShowAllFileEntries()
{
string line;
int i=0;
if (readFileStore.is_open())
  {
    while ( getline (readFileStore,line))
    {
		cout<<"File Entry #"<<i<<" "<<line<<endl;
		i++;
	}
  }
}
string random_string(int n)
{
char letters[11] = {'0','1','2','3','4','5','6','7','8','9','0'};
string ran = "";
 srand(time(0)); 
for (int i=0;i<n;i++) 
ran=ran + letters[rand() % 10];
return ran;
}
void WriteLineToSaltFile(string user,string key)
{
if (writeSaltFile.is_open())
  {
    writeSaltFile.seekp (0, ios::end);
    writeSaltFile << user << ":"<<key<<"\n";
  }
  else
  {
  	cout<<"Error on writing file"<<endl;
  }
}
void WriteLineToShadowFile(string user,string hashPass,string num)
{
if (writeShadowFile.is_open())
  {
    writeShadowFile.seekp (0, ios::end);
    writeShadowFile << user << ":"<<hashPass<<":"<<num<<"\n";
  }
  else
  {
  	cout<<"Error on writing file"<<endl;
  }
}
void InitializeShadowDataOf(string user)
{
string line;
string temp;
int i=0;
if (readShadowFile.is_open())
  {
   while ( getline (readShadowFile,line))
    {
	 i=0;
     int pos = line.find(":");
     temp=line.substr(0,pos); 
     if(user.compare(temp)==0)
	 {
	stringstream s_stream(line); //create string stream from the string
   while(s_stream.good() && i<3) {
      string substr;
      getline(s_stream, substr, ':'); //get first string delimited by comma
      if(i==0)
	  {
		  username=substr;
	  }
	  else if(i==1)
	  {
		  hashString=substr;
	  }
	  else if(i==2)
	  {
		  clearance=substr;
	  }
	  i++;
   }
		 
		 break;
	 }
    }
  }
  else
  {
  	cout<<"Error on Reading file"<<endl;
  }
}
bool IsUserNameAlreadyExists(string user)
{
string line;
string temp;
if (readSaltFile.is_open())
  {
    while ( getline (readSaltFile,line))
    {
    //  cout << line << '\n';
       int pos = line.find(":");
     temp=line.substr(0,pos); 
     if(user.compare(temp)==0)
     return true;
    }
  }
  return false;
}

string GetSaltOf(string user)
{
string line;
string temp;
if (readSaltFile.is_open())
  {
	   readSaltFile.seekg(0, ios::beg);
    while ( getline (readSaltFile,line))
    {
      //cout << line << '\n';
      int pos = line.find(":");
     temp=line.substr(0,pos); 
     string salt=line.replace(0,pos+1,""); 
     if(user.compare(temp)==0)
     return salt;
    }
  }
  return "";
}
void CheckRequiredFiles()
{
cout<<"Checking Required Files..."<<endl;
  if (writeSaltFile.is_open())
  {
    writeSaltFile.seekp (0, ios::end);
  }
  else cout << "Unable to open file";
}
    string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
    }

void TryUserCreation()
{

    CheckRequiredFiles();
    
    string salt=random_string(8);
    string encPass="";
    //reading username
    cout<<"Username : ";
    cin>>username;
    cout<<endl;
    if(IsUserNameAlreadyExists(username))
    {
      cout<<"Username already Exists! Exiting..."<<endl;
      exit(0);
    }
    //check for valid or invalid username here
    cout<<"Password : ";
    cin>>password;
    cout<<endl<<"Confirm Password : ";
    cin>>confirmpassword;
    //check for password validation
    if(password.compare(confirmpassword)!=0)
    	{
    	cout<<"Password doesn't match. Exiting..."<<endl;
    	exit(0);
    	}
    cout<<endl;
    cout<<"User clearance (0 or 1 or 2 or 3):";
    cin>>clearance;
    if(clearance.compare("0")!='0' || clearance.compare("1")!='0' || 
    clearance.compare("2")!='0' || clearance.compare("3")!='0')
    {
    
    }
	else
	{
		 cout<<"Invalid Clearance! Exiting..."<<endl;
     exit(0);
	}
    string m(salt);
    string hashCmd="echo -n ";
	hashCmd.append(password);
	hashCmd.append(salt);
	hashCmd.append(" | md5sum");
    encPass=GetStdoutFromCommand(hashCmd);
     int pos = encPass.find(" ");
     encPass=encPass.substr(0,pos); 
    cout<<encPass<<endl;
    cout<<"Writing to the file"<<endl;
    WriteLineToSaltFile(username,salt);
    WriteLineToShadowFile(username,encPass,clearance);
    cout<<"Successfully Done!"<<endl;
    exit(0);
}
bool TryUserLogin()
{
bool stat=false;
string username;
string password;
cout<<endl<<"Username : ";
cin>>username;
cout<<endl;
if(!IsUserNameAlreadyExists(username))
{
  cout<<"Username Doesn't exists! Exiting..."<<endl;
  exit(0);
}
cout<<username<<" Found in salt.txt"<<endl;
cout<<"Password : ";
cin>>password;
cout<<endl;
string saltStr=GetSaltOf(username);
cout<<"Salt retrived : "<<saltStr<<endl;
cout<<"Hashing... "<<endl;
string hashCmd="echo -n "+password+saltStr+" | md5sum";
string encPass=GetStdoutFromCommand(hashCmd);
int pos = encPass.find(" ");
encPass=encPass.substr(0,pos); 
cout<<"Hash value : "<<encPass<<endl;
InitializeShadowDataOf(username);
if(encPass.compare(hashString)!=0)
{
	cout<<"Password Not Matched ! Exiting..."<<endl;
	exit(0);
}
stat=true;
cout<<"Authentication for user "<<username<<" complete."<<endl;
cout<<"The Clearance for "<<username<<" is "<<clearance<<"."<<endl;
return stat;
}
void ShowHashDemo()
{
	string hashCmd="echo -n ";
	hashCmd.append("This is a test");
	hashCmd.append(" | md5sum");
string encStr=GetStdoutFromCommand(hashCmd);
int pos = encStr.find(" ");
encStr=encStr.substr(0,pos); 
cout <<"MD5 ('This is a test') = "<<encStr<<endl;
}
void ShowMenu()
{
	char selection='f';
    while(selection!='e' || selection!='E')
	{
		cout<<"\n Menu";
cout<<"\n==================================================";
cout<<"\n (C)reate, (A)ppend, (R)ead, (W)rite, (L)ist, (S)ave or (E)xit : ";
cin>>selection;
switch(selection)
{
case 'C' :
case 'c' :{
	string fname;
	cout<<"Filename : ";
	cin>>fname;
	if(IsFileNameAlreadyExists(fname))
	{
		cout<<"Filename Already Exists."<<endl;
		break;
	}
	string line="";
	line=fname+":"+username+":"+clearance;
	WriteInFileStore(line);
	cout<<"Writing success..."<<endl;
}
break;
case 'A' :
case 'a' :{
	string fname;
	cout<<"Filename : ";
	cin>>fname;
	if(!IsFileNameAlreadyExists(fname))
	{
		cout<<"Filename Not Exists."<<endl;
		break;
	}
	if(clearance.compare(fileClearance)<0)
	{
		cout<<"Failure. No Permission!"<<endl;
		break;
	}
	cout<<"Success"<<endl;
}
break;
case 'R' :
case 'r' :{
	string fname;
	cout<<"Filename : ";
	cin>>fname;
	if(!IsFileNameAlreadyExists(fname))
	{
		cout<<"Filename Not Exists."<<endl;
		break;
	}
	if(clearance.compare(fileClearance)<0)
	{
		cout<<"Failure. No Permission!"<<endl;
		break;
	}
	cout<<"Success"<<endl;
}
break;
case 'W' :
case 'w' :{
	string fname;
	cout<<"Filename : ";
	cin>>fname;
	if(!IsFileNameAlreadyExists(fname))
	{
		cout<<"Filename Not Exists."<<endl;
		break;
	}
	if(clearance.compare(fileClearance)<0)
	{
		cout<<"Failure. No Permission!"<<endl;
		break;
	}
	cout<<"Success"<<endl;
}
break;
case 'L' :
case 'l' :{
	ShowAllFileEntries();
}
break;
case 'S' :
case 's' :{
	ShowAllFileEntries();
}
break;
case 'E' :
case 'e' :{
	cout<<"Shut down the FileSystem? (Y)es or (N)o";
	cin>>selection;
	if(selection=='Y' || selection=='y')
	{
		exit(0);
	}
}
break;
default : continue;
}
	}
}
    int main (int argc, char *argv[]) 
    {
    string arg1="";
	int i=0;
	ShowHashDemo();
 	if(argc>1)
 	{
 		arg1=argv[1];
    	if(arg1.compare("-i")==0)
      		TryUserCreation();
	}
  	bool stat=TryUserLogin();
    if(!stat)
     	{
     	 	cout<<"Authentication Failed! Exiting..."<<endl;
     	 	exit(0);
     	}
	
	ShowMenu();
	return 0;
    }

