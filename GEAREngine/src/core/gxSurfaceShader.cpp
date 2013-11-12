#include "gxSurfaceShader.h"
#include "../GEAREngine.h"

gxSubMap::gxSubMap()
{
	memset(m_szTextureName, 0, sizeof(m_szTextureName));
	m_iTextureCRC=0;
	m_pTexture=NULL;
	m_pShaderPropertyVar=NULL;
}

gxSubMap::~gxSubMap()
{
	GX_DELETE(m_pTexture);
}

gxTexture* gxSubMap::load(CTextureManager& textureManager, const char* filename)
{
	char metaInfoFileName[256];
	const char* onlyFilename = gxUtil::getFileNameFromPath(filename);
	
	setTextureName(onlyFilename);
	sprintf(metaInfoFileName, "%s.meta", filename);

	gxFile metaInfoFile;
	if(metaInfoFile.OpenFile(metaInfoFileName))
	{
		int crc=0;
		metaInfoFile.Read(crc);
		metaInfoFile.CloseFile();
		return loadTextureFromMeta(textureManager, crc);
	}

	return NULL;
}

gxTexture* gxSubMap::loadTextureFromMeta(CTextureManager& textureManager, int crc)
{
	char metaDataFileName[256];
	sprintf(metaDataFileName, "%x", crc);

	stTexturePacket* texturePack=textureManager.LoadTexture(metaDataFileName);
	if(texturePack)
	{
		//delete old texture instance
		GX_DELETE(m_pTexture);

		m_pTexture = new gxTexture();
		m_pTexture->setTexture(texturePack);
		m_pTexture->setFileCRC(crc);
		if(texturePack->bAlphaTex)
		{
			m_pTexture->setTextureType(gxTexture::TEX_ALPHA);
		}
		else
		{
			m_pTexture->setTextureType(gxTexture::TEX_NORMAL);
		}
	}
	return m_pTexture;
}

//============================================================================================================================================
//============================================================================================================================================
//============================================================================================================================================
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
		if(*start=='\n' || *start==' ' || *start=='\t')
		{
			//start++;
			break;
		}
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
	std::string _keyword;
	std::string _name;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		switch(depth)
		{
		case -1:
			{
				if(findOpeningRoundBrace(it, end))
				{
					if(parseNameInsideQuats(it, end, _name))	//find the first "
					{
						if(findComma(it, end))
						{
							if(parseKeyWord(it, end, _keyword) && findClosingRoundBrace(it, end))
							{
								if(findEqualTo(it, end))
								{
									std::vector<std::string> args;
									if(parseArgInsideRoundBrace(it, end, args, 4))
									{
										stShaderProperty_Color* newcolor = new stShaderProperty_Color();
										newcolor->name=_name;
										newcolor->color.set(atof(args.at(0).c_str()), atof(args.at(1).c_str()), atof(args.at(2).c_str()), atof(args.at(3).c_str()));
										m_vColor_Properties.push_back(newcolor);

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

bool gxSurfaceShader::parseTexProperty(std::string::const_iterator& start, std::string::const_iterator& end, const char* _texname, int& depth)
{
	std::string _temp;
	std::string _keyword;
	std::string _name;
	std::string::const_iterator it=start;
	while(it!=end)
	{
		switch(depth)
		{
		case -1:
			{
				if(findOpeningRoundBrace(it, end))
				{
					if(parseNameInsideQuats(it, end, _name))	//find the first "
					{
						if(findComma(it, end))
						{
							if(parseKeyWord(it, end, _keyword) && findClosingRoundBrace(it, end))
							{
								if(findEqualTo(it, end))
								{
									std::vector<std::string> args;
									if(parseNameInsideQuats(it, end, _temp))
									{
										if(findOpeningCurlyBrace(it, end))
										{
											if(findClosingCurlyBrace(it, end))
											{
												stShaderProperty_Texture2D* newtex2D = new stShaderProperty_Texture2D();
												newtex2D->name=_name;
												newtex2D->texture_uv_in_name= "uv_in";
												newtex2D->texture_uv_in_name+=_texname;
												newtex2D->texture_uv_out_name= "uv_out";
												newtex2D->texture_uv_out_name+=_texname;
												newtex2D->texture_sampler2d_name= "sampler2d";
												newtex2D->texture_sampler2d_name+= _texname;
												//
												m_vTex2D_Properties.push_back(newtex2D);
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
					if(!parseTexProperty(it, end, "_MainTex", maintexpropertydepth))
						return false;
				}

				//_Decal
				str.assign(it, end);
				pos=str.find("_DecalTex");
				if(pos>=0)
				{
					it=it+pos+strlen("_DecalTex");
					int maintexpropertydepth=-1;
					if(!parseTexProperty(it, end, "_DecalTex", maintexpropertydepth))
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

bool gxSurfaceShader::parseSubShader_vertex(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;

	std::string::const_iterator shader_start;
	std::string::const_iterator shader_end;

	while(it!=end)
	{
		if(*it=='{')
		{
			if(depth==-1)
			{
				shader_start=(it+1);
			}
			depth++;
		}

		if(depth>=0)
		{
			if(*it=='}')
			{
				depth--;
				if(depth<0)
				{
					shader_end = (it-1);
					m_cSubShader.vertex_buffer.assign(shader_start, shader_end);
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

bool gxSurfaceShader::parseSubShader_fragment(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;

	std::string::const_iterator shader_start;
	std::string::const_iterator shader_end;

	while(it!=end)
	{
		if(*it=='{')
		{
			if(depth==-1)
			{
				shader_start=(it+1);
			}
			depth++;
		}

		if(depth>=0)
		{
			if(*it=='}')
			{
				depth--;
				if(depth<0)
				{
					shader_end = (it-1);
					m_cSubShader.fragment_buffer.assign(shader_start, shader_end);
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

bool gxSurfaceShader::parseSubShader(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
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
				int pos=0;
				do
				{
					pos=str.find("__Pass");
					if(pos>=0)
					{
						it=it+pos+strlen("__Pass");
						int pass_depth=-1;
						if(!parseSubShaderPass(it, end, pass_depth))
							return false;
					}
					str.assign(it, end);
					pos=str.find("__Pass");
				}while(pos>=0);
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return true;
}

bool gxSurfaceShader::parseSubShaderPass(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
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
				int pos=str.find("__includeModule");
				if(pos>=0)
				{
					it=it+pos+strlen("__includeModule");
					int properties_depth=-1;
					if(findOpeningCurlyBrace(it, end))
					{
						std::string key;
						it++;
						while(parseKeyWord(it, end, key))
						{
							m_vIncludeModule.push_back(key);
							if(findClosingCurlyBrace(it, end) && it!=end)
								break;
						}
					}
					else
						return false;
				}

				str.assign(it, end);
				pos=str.find("__vertex");
				if(pos>=0)
				{
					it=it+pos+strlen("__vertex");
					int properties_depth=-1;
					if(!parseSubShader_vertex(it, end, properties_depth))
						return false;
				}

				str.assign(it, end);
				pos=str.find("__fragment");
				if(pos>=0)
				{
					it=it+pos+strlen("__fragment");
					int subshader_depth=-1;
					if(!parseSubShader_fragment(it, end, subshader_depth))
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
				do
				{
					pos=str.find("SubShader");
					if(pos>=0)
					{
						it=it+pos+strlen("SubShader");
						int subshader_depth=-1;
						if(!parseSubShader(it, end, subshader_depth))
							return false;
					}
					str.assign(it, end);
					pos=str.find("SubShader");
				}while(pos>=0);
			}
			break;
		}

		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return true;
}

bool gxSurfaceShader::parse(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
{
	std::string str;
	std::string::const_iterator it=start;

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
		break;
	}

	return false;
}

gxSurfaceShader::gxSurfaceShader()
{
	m_pMainShader=NULL;
}

gxSurfaceShader::~gxSurfaceShader()
{
	for(std::vector<gxSubMap*>::iterator it = m_vSubMap.begin(); it != m_vSubMap.end(); ++it)
	{
		gxSubMap* map = *it;
		GX_DELETE(map);
	}
	m_vSubMap.clear();

	for(std::vector<stShaderProperty_Texture2D*>::iterator it = m_vTex2D_Properties.begin(); it != m_vTex2D_Properties.end(); ++it)
	{
		stShaderProperty_Texture2D* obj = *it;
		GX_DELETE(obj);
	}
	m_vTex2D_Properties.clear();

	for(std::vector<stShaderProperty_Color*>::iterator it = m_vColor_Properties.begin(); it != m_vColor_Properties.end(); ++it)
	{
		stShaderProperty_Color* obj = *it;
		GX_DELETE(obj);
	}
	m_vColor_Properties.clear();

	for(std::vector<stTextureMap*>::iterator it = m_vTextureMap.begin(); it != m_vTextureMap.end(); ++it)
	{
		stTextureMap* obj = *it;
		GX_DELETE(obj);
	}
	m_vTextureMap.clear();

	m_cMainShaderSource.clear();

	m_vIncludeModule.clear();
}

bool gxSurfaceShader::loadSurfaceShader(const char* filename)
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
		for(std::vector<stShaderProperty_Texture2D*>::iterator it = m_vTex2D_Properties.begin(); it != m_vTex2D_Properties.end(); ++it)
		{
			stShaderProperty_Texture2D* tex2d = *it;
			gxSubMap* submap = new gxSubMap();
			submap->setShaderTextureProperty(tex2d);
			appendSubMap(submap);
		}

		HWShaderManager* hwShaderManager = engine_getHWShaderManager();
		m_pMainShader=hwShaderManager->LoadShaderFromBuffer(filename, NULL, 0);
		if(m_pMainShader)
		{
			std::cout << filename << "\n" << "Shader already loaded" << "\n Parse Success.\n";
			GX_DELETE_ARY(vsource);
			return true;
		}
		//construct the glsl shader
		m_cMainShaderSource = "#version 120\n";
		m_cMainShaderSource += "#ifdef GEAR_VERTEX_SHADER\n";
		m_cMainShaderSource+=hwShaderManager->getShaderSnippet(0)->snippet;
		m_cMainShaderSource+=hwShaderManager->getShaderSnippet(1)->snippet;
		for(std::vector<std::string>::iterator it = m_vIncludeModule.begin(); it != m_vIncludeModule.end(); ++it)
		{
			std::string module = *it;
			if(module=="PointLightStruct")
			{
				m_cMainShaderSource+=hwShaderManager->getShaderSnippet(4)->snippet;
			}
			else if(module=="MaterialStruct")
			{
				m_cMainShaderSource+=hwShaderManager->getShaderSnippet(5)->snippet;
			}
		}
		m_cMainShaderSource+=m_cSubShader.vertex_buffer+"\n";
		m_cMainShaderSource+=hwShaderManager->getShaderSnippet(2)->snippet;
		m_cMainShaderSource += "#elif defined (GEAR_FRAGMENT_SHADER)\n";
		m_cMainShaderSource+=m_cSubShader.fragment_buffer+"\n";
		m_cMainShaderSource+=hwShaderManager->getShaderSnippet(3)->snippet;
		m_cMainShaderSource += "#endif\n";
		//

		//create the tmp glsl file
		char temp[1024];
		sprintf(temp, "%s.tmp.glsl", filename);
		FILE* fp=fopen(temp, "w");
		if(fp)
		{
			fwrite(m_cMainShaderSource.c_str(), 1, m_cMainShaderSource.size(), fp);
			fclose(fp);
		}
		//

		m_pMainShader=hwShaderManager->LoadShaderFromBuffer(filename, m_cMainShaderSource.c_str(), m_cMainShaderSource.size());
		if(!m_pMainShader)
			std::cout << filename << "\n" << m_cMainShaderSource << "\n Parse Success but GLSL compiler failed.\n";
		else
			std::cout << filename << "\n" << m_cMainShaderSource << "\n Parse Success.\n";

		m_cMainShaderSource.clear();
	}
	else
	{
		std::cout << filename << " Parse Failed.\n";
	}

	GX_DELETE_ARY(vsource);

	return bParseRetVal;
}

void gxSurfaceShader::appendTextureMap(stTextureMap* texmap)
{
	m_vTextureMap.push_back(texmap);
}

void gxSurfaceShader::appendSubMap(gxSubMap* map)
{
	m_vSubMap.push_back(map);
}

gxSubMap* gxSurfaceShader::getSubMap(int index)
{
	if(index>=(int)m_vSubMap.size()) return NULL;

	return m_vSubMap[index];
}

gxTexture* gxSurfaceShader::loadTextureFromFile(CTextureManager& textureManager, const char* filename, int submap)
{
	gxSubMap* map=NULL;
	if(submap==-1)
	{
		map = new gxSubMap();
		appendSubMap(map);
	}
	else
	{
		map = m_vSubMap[submap];
	}

	return map->load(textureManager, filename);
}