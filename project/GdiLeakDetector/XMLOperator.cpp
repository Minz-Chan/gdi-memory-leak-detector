#include "StdAfx.h"
#include "xmloperator.h"
#include "Markup.h"


typedef string::size_type (string::*find_t)(const string& delim, 
		string::size_type offset) const;

xmloperator::xmloperator(){
	xml = new CMarkup();
	filename = "E:\\InfoOfLeakObject.xml";	//若不进行设置, 则初始文件名为InfoOfLeakObject.xml
}

void xmloperator::AddRegisters(unsigned long p,char *name)//添加寄存器信息
{	
	char reg[20];
	_ultoa(p,reg,10);
	xml->AddElem("reg",reg);
	xml->AddAttrib("Type",name);
}

void xmloperator::AddModules(PHANDLE_INFORMATION p)//添加模板信息
{
	xml->AddElem("Module");
	xml->IntoElem();
	MODULE_INFORMATION mInfo;
	std::vector<MODULE_INFORMATION>::iterator it;
	for(it=p->moduleInfo.begin()
		; it != p->moduleInfo.end()
		; it++){
	mInfo = *it;
		
	DWORD v4 = (DWORD)	mInfo.FileVersion & 0xFFFF;
	DWORD v3 = (DWORD) (mInfo.FileVersion>>16) & 0xFFFF;
	DWORD v2 = (DWORD) (mInfo.FileVersion>>32) & 0xFFFF;
	DWORD v1 = (DWORD) (mInfo.FileVersion>>48) & 0xFFFF;
	
	char b[30];

	xml->AddElem(mInfo.ModuleName);
	xml->IntoElem();
	_ultoa(mInfo.BaseOfImage,b,16);
	xml->AddElem("Address",b);	
	xml->AddElem("FilePath",mInfo.ImageName);
	_ultoa(mInfo.ImageSize,b,10);
	xml->AddElem("Size",b);
	_ultoa(mInfo.TimeDateStamp,b,10);
	xml->AddElem("Time",b);
	char f[30];
	sprintf(f,"[FileVersion:%d.%d.%d.%d]",v1, v2, v3,v4);
	xml->AddElem("FileVersion",f);
	xml->OutOfElem();
	}
	xml->OutOfElem();
}

void xmloperator::AddCallStacks(PHANDLE_INFORMATION p)//添加调用栈信息
{
	
	xml->AddElem("CallStack");
	xml->IntoElem();
	string Type;
	vector<string>::iterator iter=p->callStack.callStack.begin();
	for(int i=0;iter!=p->callStack.callStack.end();iter++,i++) 
	{
		Type="Stack";
		xml->AddElem(Type.c_str(),iter->c_str());
	}	
	xml->OutOfElem();
}

void xmloperator::Createxml()//创建xml文件
{
	xml->AddElem("Objecthandle");
	xml->Save(filename.c_str());
}


void  xmloperator::AddHandleValue(PHANDLE_INFORMATION p)//添加句柄值
{

	char HValue[20];
//	sprintf(HValue, "0x%0*X",8, p->handle);
	_ultoa(p->handle,HValue,16);//转为16进制
	xml->AddElem("Handle");
	xml->AddAttrib("Value",HValue);
	xml->IntoElem();
	xml->AddElem("Handle",HValue);


}

void xmloperator::AddHandleType(PHANDLE_INFORMATION p)//添加句柄类型
{
	char TValue[20];
	_ultoa(p->uType,TValue,16);
	xml->AddElem("Type",TValue);

}

void xmloperator::Add(PHANDLE_INFORMATION p)//将结构体PHANDLE_INFORMATION 加入xml文件中
{
	
	xml->FindElem("Objecthandle");
	xml->IntoElem();
	AddHandleValue(p);
	AddHandleType(p);
	AddParameterinfo(p);
	xml->AddElem("Register");
	xml->IntoElem();
	AddRegisters(p->registers.Eax,"Eax");
	AddRegisters(p->registers.Ebp,"Ebp");
	AddRegisters(p->registers.Ebx,"Ebx");
	AddRegisters(p->registers.Ecx,"Ecx");
	AddRegisters(p->registers.Edi,"Edi");
	AddRegisters(p->registers.Edx,"Edx");
	AddRegisters(p->registers.EFlags,"EFlags");
	AddRegisters(p->registers.Eip,"Eip");
	AddRegisters(p->registers.Esi,"Esi");
	AddRegisters(p->registers.Esp,"Esp");
	AddRegisters(p->registers.SegCs,"SegCs");
	AddRegisters(p->registers.SegSs,"SegSs");
	xml->OutOfElem();
	AddCallStacks(p);
	AddModules(p);
	xml->AddElem("End");
	xml->OutOfElem();
	xml->ResetMainPos();
}
void xmloperator::AddParameterinfo(PHANDLE_INFORMATION p)//添加参数信息
{
	xml->AddElem("Parameterinfo");
	xml->IntoElem();
	vector<string> iter;
	vector<string>::iterator it;
	string TValue,Content,Type;
	iter=Split(p->szParaInfo,"|",true,false);
	for(it=iter.begin();it!=iter.end();it++)
	{
	 Type="Para";
	 TValue=it->substr(0,it->find(":"));
	 Content=it->substr(it->find(":")+1,it->length()-1);
	 xml->AddElem(Type.c_str(),Content.c_str());
	 xml->AddAttrib("Type",TValue.c_str());
	}
	xml->OutOfElem();
}


void xmloperator::Start(string name)//载入文件
{
	xml->Load(name.c_str());
}

void xmloperator::End(string name)//保存并结束xml文件
{
	xml->Save(name.c_str());
}


void xmloperator::Read(PHANDLE_INFORMATION p)//从XML文件中读取出数据
{
	vector<string> iter;
	vector<string> iterContent;
	vector<string> iterTemp,iterTemp2,iterFileVer;
	unsigned __int64 Version=0,Ver1,Ver2,Ver3,Ver4;
	int Type=0;
	char *Temp;
	string ParaInfo,modulename;
	int Order=0;
	MODULE_INFORMATION Moduleinfo;
	vector<string>::iterator it,itContent,itFileVer,itTemp2,itTemp;
	iter=Split(test,"|!",true,false);//将字符串test以"|!"分割
	for (it=iter.begin();it!=iter.end();it++)
	{
		
		
		switch (Type)
		{
		case 1:  iterContent=Split((*it),"?",true,false);
					for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
					{					
						unsigned long a=strtoul(itContent->c_str(),&Temp,16);
//						unsigned long a=strtoul(itContent->c_str(),&Temp,10);
						p->handle=a;	
					
					}	Type=0;continue;
					break;
		case 2:  iterContent=Split((*it),"?",true,false);
			for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
			{
			p->uType=strtoul(itContent->c_str(),&Temp,10);	
			
			}	Type=0;continue;break;
		case 3:  iterContent=Split((*it),"?",true,false);
			for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
			{
				ParaInfo+=(*itContent);
					ParaInfo+=":";	
				itContent++;
				ParaInfo+=(*itContent);
					ParaInfo+="|";
							
			}Type=0;continue;break;
		case 4:  iterContent=Split((*it),"?",true,false);
			for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
			{
				if (strcmp(itContent->c_str(),"Edi")==0)
				{
					itContent++;
					p->registers.Edi=strtoul(itContent->c_str(),&Temp,10);	
				} 
				else if(strcmp(itContent->c_str(),"Esi")==0)
				{
					itContent++;
					p->registers.Esi=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Eax")==0)
				{
					itContent++;
					p->registers.Eax=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Ebx")==0)
				{
					itContent++;
					p->registers.Ebx=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Ecx")==0)
				{
					itContent++;
					p->registers.Ecx=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Edx")==0)
				{
					itContent++;
					p->registers.Edx=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Eip")==0)
				{
					itContent++;
					p->registers.Eip=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Ebp")==0)
				{
					itContent++;
					p->registers.Ebp=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"SegCs")==0)
				{
					itContent++;
					p->registers.SegCs=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"EFlags")==0)
				{
					itContent++;
					p->registers.EFlags=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"Esp")==0)
				{	
					itContent++;
					p->registers.Esp=strtoul(itContent->c_str(),&Temp,10);	
				}
				else if(strcmp(itContent->c_str(),"SegSs")==0)
				{
					itContent++;
					p->registers.SegSs=strtoul(itContent->c_str(),&Temp,10);	
				}
				else;		
			}
			Type=0;continue;
			break;
		case 5:  
			iterContent=Split((*it),"?",true,false);
			for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
			{
					p->callStack.callStack.push_back((*itContent));					
			}
			Type=0;
			continue;
			break;
		case 6:  iterContent=Split((*it),"?",true,false);
			for (itContent=iterContent.begin();itContent!=iterContent.end();itContent++)
			{
				if (Order==1)
				{
					Moduleinfo.BaseOfImage=strtoul(itContent->c_str(),&Temp,16);	
					Type=0;
						continue;
				} 
				else if	(Order==2)
				{
															
						strcpy(Moduleinfo.ImageName,itContent->c_str());
						modulename=(*itContent).substr((*itContent).rfind("\\")+1,(*itContent).length()-(*itContent).rfind("\\"));
						strcpy(Moduleinfo.ModuleName,modulename.c_str());
						Type=0;
						continue;
					
				}
				else if	(Order==3)
				{
					Moduleinfo.ImageSize=strtoul(itContent->c_str(),&Temp,10);
					Type=0;
				continue;
				}
				else if	(Order==4)
				{
					Moduleinfo.TimeDateStamp=strtoul(itContent->c_str(),&Temp,10);
					Type=0;
						continue;
				}
				else if (Order==5)
				{
					iterTemp=Split((*itContent),":",true,false);
					for (itTemp=iterTemp.begin();itTemp!=iterTemp.end();itTemp++)
					{
						itTemp++;
						iterTemp2=Split((*itTemp),"]",true,false);
							for (itTemp2=iterTemp2.begin();itTemp2!=iterTemp2.end();itTemp2++)
							{
								iterFileVer=Split((*itTemp2),".",true,false);
									for (itFileVer=iterFileVer.begin();itFileVer!=iterFileVer.end();itFileVer++)
									{
										Ver1=strtoul(itFileVer->c_str(),&Temp,10);
										Version = Version + (Ver1 <<48);	
										itFileVer++;
										Ver2=strtoul(itFileVer->c_str(),&Temp,10);
										Version = Version + (Ver2 << 32);
										itFileVer++;
										Ver3=strtoul(itFileVer->c_str(),&Temp,10);
										Version = Version + (Ver3 << 16);
										itFileVer++;
										Ver4=strtoul(itFileVer->c_str(),&Temp,10);
										Version = Version + Ver4 ;												
									}
							}
					}
					Moduleinfo.FileVersion=Version;
					Version=0;
					Order=0;
					Type=0;
					p->moduleInfo.push_back(Moduleinfo);
					strcpy(p->szParaInfo,ParaInfo.c_str());
				} 
				else ;
			}
			continue;
			break;
		default: break;
		}
		//判断类型
		if (strcmp(it->c_str(),"Handle")==0)
		{
			Type=1;
			ParaInfo="";
			p->callStack.callStack.clear();
			p->moduleInfo.clear();
		} 
		else if	(strcmp(it->c_str(),"Type")==0)
		{
			Type=2;
		}
		else if	(strcmp(it->c_str(),"Para")==0)
		{
			Type=3;
		}
		else if	(strcmp(it->c_str(),"reg")==0)
		{
			Type=4;
		}
		else if	(strcmp(it->c_str(),"Stack")==0)
		{
			Type=5;
		}
		else
		{
			Type=6;
			Order++;
		}
	}	
}

void xmloperator::Find_All_Data(string &Tag, string &Value)//遍历XMl文件将所有信息赋予字符串test
{
	char *Temp,t[20];
	 xml->FindChildElem();
		xml->IntoElem(); 
		Tag = xml->GetTagName();
		Tag =xml->GetAttrib("Value");
		unsigned long a=strtoul(Tag.c_str(),&Temp,16);
		sprintf(t, "0x%0*X",8, a);
		Value+=t;
	
	while ( xml->FindElem()) 
	{	
		Value+="@";
		Tag = xml->GetTagName();
		if (strcmp(Tag.c_str(),"Handle")==0)
		{
			Tag =xml->GetAttrib("Value");
			a=strtoul(Tag.c_str(),&Temp,16);
			sprintf(t, "0x%0*X",8, a);
			Value+=t;
			Value+="@";
		}
	}
}

vector<string> xmloperator::Split(const string& s,const string& match,bool removeEmpty,bool fullMatch)
{
/*	removeEmpty=false;
	fullMatch=false;*/
	vector<string> result;                 // return container for tokens
	string::size_type start = 0,           // starting position for searches
		skip = 1;            // positions to skip after a match
	find_t pfind = &string::find_first_of; // search algorithm for matches
	if (fullMatch)
	{
		// use the whole match string as a key
		// instead of individual characters
		// skip might be 0. see search loop comments
		skip = match.length();
		pfind = &string::find;
	}
	while (start != string::npos)
	{
		// get a complete range [start..end)
		string::size_type end = (s.*pfind)(match, start);
		// null strings always match in string::find, but
		// a skip of 0 causes infinite loops. pretend that
		// no tokens were found and extract the whole string
		if (skip == 0) end = string::npos;
		string token = s.substr(start, end - start);
		if (!(removeEmpty && token.empty()))
		{
			// extract the token and add it to the result list
			result.push_back(token);
		}
		// start the next range
		if ((start = end) != string::npos) start += skip;
	}
	return result;
}

void xmloperator::ShowHandle(string Handle, string &Value)
{
	char *Temp,t[20];
	string Tag,Val;
	xml->FindChildElem();
	xml->IntoElem(); 
	Tag =xml->GetAttrib("Value");
	unsigned long a=strtoul(Tag.c_str(),&Temp,16);
	sprintf(t, "0x%0*X",8, a);
	if (strcmp(t,Handle.c_str())==0)
	{
		Find_Data( Tag, Val);
	}

while ( xml->FindElem()) 
{
	Tag =xml->GetAttrib("Value");
	a=strtoul(Tag.c_str(),&Temp,16);
	sprintf(t, "0x%0*X",8, a);
	if (strcmp(t,Handle.c_str())==0)
	{
		Find_Data(Tag, Val);
	}
	}
}


void   xmloperator::Find_Data(string &a, string &Value)
{
	string Tag;
	if ( xml->FindChildElem()) 
	{   
		
		xml->IntoElem(); 
		Find_Data( Tag, Value); 	
	} 
	else                     // 没有下层结构了 
	{ 
		Value+="|";
		Value+="!";
		Tag = xml->GetTagName();
		if (strcmp(Tag.c_str(),"End")==0)
		{
			Stack.push_back(Value);
		}
		Value+=Tag;	
		Value+="!";
		Tag =xml->GetAttrib("Type");
		Value+=Tag;
		Value+="?";
		Tag = xml->GetData(); 
		Value+=Tag;
		
	} 
    while ( xml->FindElem()) 
	{ 
		// 还有下层结构
        if ( xml->FindChildElem()) 
		{   			
            xml->IntoElem(); 
            Find_Data( Tag, Value); 			
		} 
		else                     // 没有下层结构了 
		{ 
			Value+="|";
			Value+="!";
            Tag = xml->GetTagName(); 
			if (strcmp(Tag.c_str(),"End")==0)
			{
				Stack.push_back(Value);
			}
			Value+=Tag;			
			Value+="!";
			Tag =xml->GetAttrib("Type");
			Value+=Tag;
			Value+="?";
            Tag = xml->GetData(); 
			Value+=Tag;			
		} 
	} 
    xml->OutOfElem();
}

string xmloperator::Show_Data(PHANDLE_INFORMATION p)
{
	char t[256];
	int Order=0;   
	sprintf(t, "0x%0*X",8, p->handle);
	string s="Handle :";
	s=s+t+"\r\nType :";
	if ( p->uType==1)
	{
		sprintf(t, "BITMAP");
	}
	if ( p->uType==2)
	{
		sprintf(t, "BRUSH");
	}
	if ( p->uType==3)
	{
		sprintf(t, "COLORSPACE");
	}
	if ( p->uType==4)
	{
		sprintf(t, "DC");
	}
	if ( p->uType==5)
	{
		sprintf(t, "ENHMETADC");
	}
	if ( p->uType==6)
	{
		sprintf(t, "ENHMETAFILE");
	}	
	if ( p->uType==7)
	{
		sprintf(t, "EXTPEN");
	}
	if ( p->uType==8)
	{
		sprintf(t, "FONT");
	}
	if ( p->uType==9)
	{
		sprintf(t, "MEMDC");
	}
	if ( p->uType==10)
	{
		sprintf(t, "METAFILE");
	}
	if ( p->uType==11)
	{
		sprintf(t, "METADC");
	}
	if ( p->uType==12)
	{
		sprintf(t, "PAL");
	}
	if ( p->uType==13)
	{
		sprintf(t, "PEN");
	}
	if ( p->uType==14)
	{
		sprintf(t, "REGION");
	}
	if ( p->uType==15)
	{
		sprintf(t, "UNKNOWN");
	}

	vector<string> iter;
	vector<string>::iterator it;
	string TValue,Content;
	iter=Split(p->szParaInfo,"|",true,false);
	s=s+t+"\r\nParameterInfo  :\r\n";
	for(Order=0,it=iter.begin();it!=iter.end();it++,Order++)
	{
		 sprintf(t, "%d", Order);
	 TValue=it->substr(0,it->find(":"));
	 Content=it->substr(it->find(":")+1,it->length()-1);
	s=s+"!@参数类型"+t+"  "+TValue+" :"+Content+"\r\n";
	}

	s=s+"Registers :\r\n";
	sprintf(t, "0x%0*X",8,p->registers.Eax);
	s=s+"!@Eax:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Ebp);
	s=s+"  Ebp:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Ebx);
	s=s+"  Ebx:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Ecx);
	s=s+"\r\n!@Ecx:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Edi);
	s=s+"  Edi:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Edx);
	s=s+"  Edx:  "+t;
	sprintf(t,"0x%0*X",8,p->registers.EFlags);	
	s=s+"\r\n!@Eax:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Eip);
	s=s+"  Eip:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Esi);
	s=s+"  Esi:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.Esp);
	s=s+"\r\n!@Esp:  "+t;	
	sprintf(t, "0x%0*X",8,p->registers.SegCs);
	s=s+"  SegCs:  "+t;
	sprintf(t, "0x%0*X",8,p->registers.SegSs);
	s=s+"  SegSs:  "+t;

	s=s+"\r\nCallStack  : \r\n";
	vector<string>::iterator ff;
	for(ff=p->callStack.callStack.begin();ff!=p->callStack.callStack.end();ff++)
	{
		s=s+"!@"+(*ff)+"\r\n";
	}
	s=s+"Modules  :\r\n";
	vector<MODULE_INFORMATION>::iterator f;
	for(f=p->moduleInfo.begin();f!=p->moduleInfo.end();f++)
	{
		s=s+"Name :"+(*f).ModuleName+"\r\n";
		 sprintf(t, "!@Address :%u\r\n!@Size :%u \r\n!@TimeStamp :%u\r\n!@FileVersion :%u\r\n", (*f).BaseOfImage ,(*f).ImageSize,(*f).TimeDateStamp,(*f).FileVersion);

		s+=t;
		s=s+"!@"+"FilePath  :"+(*f).ImageName+"\r\n";
	}
	return s;
	
}



