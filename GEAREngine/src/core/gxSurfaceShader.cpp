#include "gxSurfaceShader.h"

bool gxSurfaceShader::findQuat(std::string::const_iterator& start, std::string::const_iterator& end, bool bStrict)
{
	while(start!=end)
	{
		if(bStrict)
		{
			bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
			if(!bWS && *start!='"')
				return false;
		}
		if(*start=='"')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findOpeningRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!='(')
			return false;

		if(*start=='(')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findClosingRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!=')')
			return false;

		if(*start==')')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findOpeningCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!='{')
			return false;

		if(*start=='{')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findClosingCurlyBrace(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!='}')
			return false;

		if(*start=='}')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}
bool gxSurfaceShader::findComma(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!=',')
			return false;

		if(*start==',')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findEqualTo(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && *start!='=')
			return false;

		if(*start=='=')
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findAnyValidCharForName(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bValidChar=((*start>=65 && *start<=90) || (*start>=97 && *start<=122) || *start=='_');
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && !bValidChar)
			return false;

		if(bValidChar)
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findAnyValidAlphaNumeric(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bValidChar=((*start>=65 && *start<=90) || (*start>=97 && *start<=122) || (*start>=48 && *start<=57));
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && !bValidChar)
			return false;

		if(bValidChar)
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::findAnyValidNumeric(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bValidChar=(*start>=48 && *start<=57);
		bool bWS=(*start=='\n' || *start=='\t' || *start==0 || *start==' ');
		if(!bWS && !bValidChar)
			return false;

		if(bValidChar)
		{
			start++;
			return true;
		}
		start++;
	}

	return false;
}

bool gxSurfaceShader::parseKeyWord(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name)
{
	std::string::const_iterator startPos;
	if(findAnyValidCharForName(start, end))
		startPos=--start;
	else return false;

	while(findAnyValidAlphaNumeric(start, end))
	{
		//start++;
	}

	name.assign(startPos, start);
	return true;
}

bool gxSurfaceShader::parseArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount)
{
	int cnt=argCount-1;

	if(findOpeningRoundBrace(start, end))	//find the first "
	{
		while(start!=end)
		{
			if(findAnyValidNumeric(start, end))
			{
				std::string::const_iterator name_start=start-1;
				bool valid=false;
				if(cnt)
					valid=findComma(start, end);
				else
					valid=findClosingRoundBrace(start, end);	//last arg
				if(valid)
				{
					std::string str;
					str.assign(name_start, start-1);
					args.push_back(str);
					cnt--;
					if(cnt==-1)
					{
						return true;
					}
				}
				else return false;
			}
			//start++;
		}
	}

	return false;
}

bool gxSurfaceShader::parseNameInsideQuats(std::string::const_iterator& start, std::string::const_iterator& end, std::string& name)
{
	if(findQuat(start, end))	//find the first "
	{
		std::string::const_iterator second_it=start;
		if(findQuat(second_it, end, false))	//find the second "
		{
			name.assign(start, --second_it);
			start=++second_it;
			return true;
		}
	}

	return false;
}

bool gxSurfaceShader::parseColorProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		switch(depth)
		{
		case -1:
			{
				if(findOpeningRoundBrace(it, end))
				{
					if(parseNameInsideQuats(it, end, str))	//find the first "
					{
						if(findComma(it, end))
						{
							if(parseKeyWord(it, end, str) && findClosingRoundBrace(it, end))
							{
								if(findEqualTo(it, end))
								{
									std::vector<std::string> args;
									if(parseArgInsideRoundBrace(it, end, args, 4))
									{
										start=it;
										return true;
									}
									else return false;
								}
								else return false;
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return false;
}

bool gxSurfaceShader::parseMainTexProperty(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		switch(depth)
		{
		case -1:
			{
				if(findOpeningRoundBrace(it, end))
				{
					if(parseNameInsideQuats(it, end, str))	//find the first "
					{
						if(findComma(it, end))
						{
							if(parseKeyWord(it, end, str) && findClosingRoundBrace(it, end))
							{
								if(findEqualTo(it, end))
								{
									std::vector<std::string> args;
									if(parseNameInsideQuats(it, end, str))
									{
										if(findOpeningCurlyBrace(it, end))
										{
											if(findClosingCurlyBrace(it, end))
											{
												start=it;
												return true;
											}
											else return false;
										}
										else return false;
									}
									else return false;
								}
								else return false;
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return false;
}

bool gxSurfaceShader::parseProperties(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		char ch=*it;
		if(findClosingCurlyBrace(it, end))
		{
			start=it;
			return true;
		}

		switch(depth)
		{
		case -1:
			if(findOpeningCurlyBrace(it, end))
			{
				//_Color
				str.assign(it, end);
				int pos=str.find("_Color");
				if(pos>=0)
				{
					it=it+pos+strlen("_Color");
					int colorpropertydepth=-1;
					if(!parseColorProperty(it, end, colorpropertydepth))
						return false;
				}

				//_MainTex
				str.assign(it, end);
				pos=str.find("_MainTex");
				if(pos>=0)
				{
					it=it+pos+strlen("_MainTex");
					int maintexpropertydepth=-1;
					if(!parseMainTexProperty(it, end, maintexpropertydepth))
						return false;
				}

				//_Decal
				str.assign(it, end);
				pos=str.find("_DecalTex");
				if(pos>=0)
				{
					it=it+pos+strlen("_DecalTex");
					int maintexpropertydepth=-1;
					if(!parseMainTexProperty(it, end, maintexpropertydepth))
						return false;
				}
			}
			break;
		}
		
		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return true;
}

bool gxSurfaceShader::parseSubShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		if(*it=='{')
		{
			depth++;
		}

		if(depth>=0)
		{
			if(*it=='}')
			{
				depth--;
				if(depth<0)
				{
					start=++it;
					return true;
				}
			}
		}

		if(std::distance(it, end)<=0) return false;
			it++;
	}

	return false;
}

bool gxSurfaceShader::parseShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		if(findClosingCurlyBrace(it, end))
		{
			--depth;
			start=it;
			return true;
		}

		switch(depth)
		{
		case -1:
			{
				str.assign(it, end);
				int pos=str.find("Properties");
				if(pos>=0)
				{
					it=it+pos+strlen("Properties");
					int properties_depth=-1;
					if(!parseProperties(it, end, properties_depth))
						return false;
				}

				str.assign(it, end);
				pos=str.find("SubShader");
				if(pos>=0)
				{
					it=it+pos+strlen("SubShader");
					int subshader_depth=-1;
					if(!parseSubShader(it, end, subshader_depth))
						return false;
				}
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	//std::cout << str;

	return true;
}

bool gxSurfaceShader::parse(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		if(*it=='}')
		{
			--depth;
			start=it;
			return true;
		}

		switch(depth)
		{
		case -1:
			{
				str.assign(it, end);
				int pos=str.find("Shader");
				if(pos>=0)
				{
					it=it+pos+strlen("Shader");

					if(parseNameInsideQuats(it, end, str))
					{
						if(findOpeningCurlyBrace(it, end))
						{
							int shader_depth=-1;
							if(parseShader(it, end, shader_depth))
							{
							}
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	//std::cout << str;

	return true;
}

bool gxSurfaceShader::load(const char* filename)
{
	int fileSz=0;
	FILE* fp=fopen(filename, "r");
	if(fp==NULL) return false;

	fseek(fp, 0, SEEK_END);
	fileSz=ftell(fp);
	fclose(fp);


	//shader source
	fp=fopen(filename, "r");
	if(fp==NULL) return false;
	char* vsource=new char[fileSz];
	memset((void*)vsource, 0, fileSz);
	fread((void*)vsource, 1, fileSz, fp);
	fclose(fp);

	std::string str;
	str.assign(vsource);
	std::string::const_iterator iter = str.begin();
    std::string::const_iterator end  = str.end();

	int depth=-1;
	bool bParseRetVal=parse(iter, end, depth);
	if(bParseRetVal)
	{
		std::cout << filename << " Parse Success";
	}
	else
	{
		std::cout << filename << " Parse Failed";
	}
	delete [] vsource;

	return bParseRetVal;
}