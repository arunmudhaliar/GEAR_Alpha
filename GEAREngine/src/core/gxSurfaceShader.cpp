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
		metaInfoFile.Read(m_iTextureCRC);
		metaInfoFile.CloseFile();
		return loadTextureFromMeta(textureManager, m_iTextureCRC);
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
		m_pTexture->setAssetFileCRC(crc, metaDataFileName);
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
			bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bValidChar=((*start>=65 && *start<=90) || (*start>=97 && *start<=122) || (*start>=48 && *start<=57) || *start=='_');
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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

bool gxSurfaceShader::findAnyValidFloat(std::string::const_iterator& start, std::string::const_iterator& end)
{
	while(start!=end)
	{
		bool bValidChar=(*start>=48 && *start<=57);
		if(*start=='.' && (start+1)!=end && *(start+1)>=48 && *(start+1)<=57)
		{
			start++;
			return true;
		}

		bool bWS=(*start=='\r' || *start=='\n' || *start=='\t' || *start==0 || *start==' ');
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
		if(*start=='\r' || *start=='\n' || *start==' ' || *start=='\t')
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
	bool bOnlyOneArgument = (cnt)?false:true;

	if(findOpeningRoundBrace(start, end))	//find the first "
	{
		while(start!=end)
		{
			float floatval=0.0f;
			bool bClosingBraceFound=false;
			while(parseFloat(start, end, floatval))
			{
				char buffer[32];
				sprintf(buffer, "%f", floatval);
				std::string str;
				str.assign(buffer);
				args.push_back(str);
				if(findClosingRoundBrace(start, end))
				{
					bClosingBraceFound=true;
					break;
				}

				if(!findComma(start, end))
				{
					return false;
				}
			}

			return (args.size()==argCount && bClosingBraceFound);
#if 0
				std::string::const_iterator name_start=start-1;
				bool valid=false;
				if(cnt)
					valid=findComma(start, end);
				else
				{
					if(bOnlyOneArgument)
					{
						std::string keyword;
						std::string::const_iterator keyword_it=start-1;
						if(parseKeyWord(keyword_it, end, keyword))
							valid=findClosingRoundBrace(keyword_it, end);	//last arg
						else return false;
						start=keyword_it;
					}
					else
					{
						valid=findClosingRoundBrace(start, end);	//last arg
					}
				}
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
#endif
			//start++;
		}
	}

	return false;
}

bool gxSurfaceShader::parseAlphaNumericArgInsideRoundBrace(std::string::const_iterator& start, std::string::const_iterator& end, std::vector<std::string>& args, int argCount)
{
	int cnt=argCount-1;

	if(findOpeningRoundBrace(start, end))	//find the first "
	{
		while(start!=end)
		{
			if(findAnyValidAlphaNumeric(start, end))
			{
				std::string::const_iterator name_start=start-1;
				bool valid=false;
				if(cnt)
				{
					std::string keyword;
					std::string::const_iterator keyword_it=start-1;
					if(parseKeyWord(keyword_it, end, keyword))
						valid=findComma(keyword_it, end);
					else return false;
					start=keyword_it;
				}
				else
				{
					std::string keyword;
					std::string::const_iterator keyword_it=start-1;
					if(parseKeyWord(keyword_it, end, keyword))
						valid=findClosingRoundBrace(keyword_it, end);	//last arg
					else return false;
					start=keyword_it;
				}

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

bool gxSurfaceShader::parseFloat(std::string::const_iterator& start, std::string::const_iterator& end, float& floatval)
{
	if(findAnyValidFloat(start, end))
	{
		std::string::const_iterator start_float=start-1;
		while(start!=end)
		{
			bool bValidChar=((*start>=48 && *start<=57) || *start=='.');
			if(!bValidChar)
			{
				std::string floatstring;
				floatstring.assign(start_float, start);
				floatval=atof(floatstring.c_str());
				return true;
			}

			start++;
		}
	}

	return false;
}

bool gxSurfaceShader::parseEachProperty(std::string::const_iterator& start, std::string::const_iterator& end, const char* _nameofproperty, int& depth)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								if(strcmp(_keyword.c_str(), "Tex2D")==0)
								{
									//
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
													newtex2D->nameofProperty=_nameofproperty;
													newtex2D->texture_uv_in_name= "uv_in";
													newtex2D->texture_uv_in_name+=_nameofproperty;
													newtex2D->texture_uv_out_name= "uv_out";
													newtex2D->texture_uv_out_name+=_nameofproperty;
													newtex2D->texture_sampler2d_name= "sampler2d";
													newtex2D->texture_sampler2d_name+= _nameofproperty;
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
									//
								}
								else if(strcmp(_keyword.c_str(), "Color")==0)
								{
									if(findEqualTo(it, end))
									{
										std::vector<std::string> args;
										if(parseArgInsideRoundBrace(it, end, args, 4))
										{
											stShaderProperty_Color* newcolor = new stShaderProperty_Color();
											newcolor->name=_name;
											newcolor->nameofProperty=_nameofproperty;
											newcolor->color.set(atof(args.at(0).c_str()), atof(args.at(1).c_str()), atof(args.at(2).c_str()), atof(args.at(3).c_str()));
											m_vColor_Properties.push_back(newcolor);

											start=it;
											return true;
										}
										else return false;
									}
									else return false;
								}
								else if(strcmp(_keyword.c_str(), "Range")==0)
								{
									if(findEqualTo(it, end))
									{
										std::vector<std::string> args;
										if(parseArgInsideRoundBrace(it, end, args, 2))
										{
											if(findEqualTo(it, end))
											{
												float floatval=0.0f;
												if(parseFloat(it, end, floatval))
												{
													stShaderProperty_Range* newrange = new stShaderProperty_Range();
													newrange->name=_name;
													newrange->nameofProperty=_nameofproperty;
													newrange->range_min=atof(args.at(0).c_str());
													newrange->range_max=atof(args.at(1).c_str());
													newrange->range_value=floatval;
													m_vRange_Properties.push_back(newrange);

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
				//parse and get all the property list
				std::string _nameofproperty;
				while(parseKeyWord(it, end, _nameofproperty))
				{
					int propertydepth=-1;
					if(!parseEachProperty(it, end, _nameofproperty.c_str(), propertydepth))
						return false;
				}
				it--;
			}
			break;
		}
		
		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return false;
}

bool gxSurfaceShader::parseSubShader_tag(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth)
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
				//check for CULL
				str.assign(it, end);
				int pos=str.find("CULLFACE");
				if(pos>=0)
				{
					std::vector<std::string> args;
					std::string::const_iterator tmp_it=it+pos+strlen("CULLFACE");
					if(!parseAlphaNumericArgInsideRoundBrace(tmp_it, end, args, 1))
						return false;

					char* tmp=(char*)args.at(0).c_str();
					std::string argVar(gxUtil::trimwhitespace(tmp));
					if(argVar.compare("NONE")==0)
					{
						pass.cull_face=GL_NONE;
					}
					else if(argVar.compare("FRONT")==0)
					{
						pass.cull_face=GL_FRONT;
					}
					else if(argVar.compare("BACK")==0)
					{
						pass.cull_face=GL_BACK;
					}
					else if(argVar.compare("FRONT_AND_BACK")==0)
					{
						pass.cull_face=GL_FRONT_AND_BACK;
					}
				}
			}
			break;
		}
		
		if(std::distance(it, end)<=0) return false;
		it++;
	}

	return false;
}

bool gxSurfaceShader::parseSubShader_vertex(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth)
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
					pass.vertex_buffer.assign(shader_start, shader_end);
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

bool gxSurfaceShader::parseSubShader_fragment(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth)
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
					pass.fragment_buffer.assign(shader_start, shader_end);
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

bool gxSurfaceShader::findEndingCurlyBraseOfModule(std::string::const_iterator& start, std::string::const_iterator& end, int& depth)
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
					end=++it;
					return true;
				}
			}
		}

		if(std::distance(it, end)<=0) return false;
			it++;
	}

	return false;
}

bool gxSurfaceShader::parseSubShaderPass(std::string::const_iterator& start, std::string::const_iterator& end, stPass& pass, int& depth)
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
				pass.GEAR_M = (int)str.find("GEAR_MODEL_MATRIX")>=0;
				pass.GEAR_M_INVERSE = (int)str.find("GEAR_MODEL_INVERSE")>=0;
				pass.GEAR_MV = (int)str.find("GEAR_MODELVIEW")>=0;
				pass.GEAR_MVP = (int)str.find("GEAR_MVP")>=0;
				pass.GEAR_NORMAL_MATRIX = (int)str.find("GEAR_NORMAL_MATRIX")>=0;
				pass.Light = (int)str.find("light.position")>=0;
				pass.Material = (int)str.find("material.diffuse")>=0 || (int)str.find("material.specular")>=0;
				pass.vIN_Position = (int)str.find("vIN_Position")>=0;
				pass.vIN_Normal = (int)str.find("vIN_Normal")>=0;
				pass.vIN_Color = (int)str.find("vIN_Color")>=0;
				pass.Tangent = (int)str.find("Tangent")>=0;
				pass.GEAR_Time = (int)str.find("GEAR_Time")>=0;
				pass.GEAR_ScreenParams = (int)str.find("GEAR_ScreenParams")>=0;

				//properties
				//range
				for(std::vector<stShaderProperty_Range*>::iterator prop_it = m_vRange_Properties.begin(); prop_it != m_vRange_Properties.end(); ++prop_it)
				{
					stShaderProperty_Range* range = *prop_it;
					if((int)str.find(range->nameofProperty)>=0)
					{
						range->passPtr=&pass;
					}
				}

				//color
				for(std::vector<stShaderProperty_Color*>::iterator prop_it = m_vColor_Properties.begin(); prop_it != m_vColor_Properties.end(); ++prop_it)
				{
					stShaderProperty_Color* color = *prop_it;
					if((int)str.find(color->nameofProperty)>=0)
					{
						color->passPtr=&pass;
					}
				}
				//

				//float
				for(std::vector<stShaderProperty_Vector*>::iterator prop_it = m_vVector_Properties.begin(); prop_it != m_vVector_Properties.end(); ++prop_it)
				{
					stShaderProperty_Vector* vector = *prop_it;
					if((int)str.find(vector->nameofProperty)>=0)
					{
						vector->passPtr=&pass;
					}
				}
				//

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
							pass.vIncludeModule.push_back(key);
							if(findClosingCurlyBrace(it, end) && it!=end)
								break;
						}
					}
					else
						return false;
				}

				str.assign(it, end);
				pos=str.find("__tag");
				if(pos>=0)
				{
					it=it+pos+strlen("__tag");
					int tag_depth=-1;
					if(!parseSubShader_tag(it, end, pass, tag_depth))
						return false;
				}

				str.assign(it, end);
				pos=str.find("__vertex");
				if(pos>=0)
				{
					it=it+pos+strlen("__vertex");
					int properties_depth=-1;
					if(!parseSubShader_vertex(it, end, pass, properties_depth))
						return false;
				}

				str.assign(it, end);
				pos=str.find("__fragment");
				if(pos>=0)
				{
					it=it+pos+strlen("__fragment");
					int subshader_depth=-1;
					if(!parseSubShader_fragment(it, end, pass, subshader_depth))
						return false;
				}
			}
			break;
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

						int temp=-1;
						std::string::const_iterator temp_end=end;
						if(findEndingCurlyBraseOfModule(it, temp_end, temp))
						{
							stPass* newPass = new stPass();
							if(!parseSubShaderPass(it, temp_end, *newPass, pass_depth))
							{
								GX_DELETE(newPass);
								return false;
							}
							m_cSubShader.m_vPass.push_back(newPass);
						}
						else return false;
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

	return false;
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

				if(parseNameInsideQuats(it, end, m_cName))
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
}

gxSurfaceShader::~gxSurfaceShader()
{
	//
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

	for(std::vector<stShaderProperty_Range*>::iterator it = m_vRange_Properties.begin(); it != m_vRange_Properties.end(); ++it)
	{
		stShaderProperty_Range* obj = *it;
		GX_DELETE(obj);
	}
	m_vRange_Properties.clear();

	for(std::vector<stShaderProperty_Vector*>::iterator it = m_vVector_Properties.begin(); it != m_vVector_Properties.end(); ++it)
	{
		stShaderProperty_Vector* obj = *it;
		GX_DELETE(obj);
	}
	m_vVector_Properties.clear();
	//


	for(std::vector<stTextureMap*>::iterator it = m_vTextureMap.begin(); it != m_vTextureMap.end(); ++it)
	{
		stTextureMap* obj = *it;
		GX_DELETE(obj);
	}
	m_vTextureMap.clear();

	m_vShaderProgram.clear();
}

bool gxSurfaceShader::loadSurfaceShader(const char* filename)
{
	m_cFileName.assign(gxUtil::getFileNameFromPath(filename));
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
		HWShaderManager* hwShaderManager = engine_getHWShaderManager();

		int cntr=0;
		for(std::vector<stPass*>::iterator it_pass = m_cSubShader.m_vPass.begin(); it_pass != m_cSubShader.m_vPass.end(); ++it_pass, cntr++)
		{
			stPass* currentPass = *it_pass;

			char constructed_glsl_filename[1024];
			sprintf(constructed_glsl_filename, "%s.pass%d.glsl", filename, cntr);
			gxHWShader* pMainShader=hwShaderManager->LoadShaderFromBuffer(constructed_glsl_filename, NULL, 0);
			if(pMainShader)
			{
				m_vShaderProgram.push_back(pMainShader);
				DEBUG_PRINT("%s\nShader already loaded\n Parse Success. Pass(%d)\n", constructed_glsl_filename, cntr);
				continue;
			}

			//construct the glsl shader
			std::string cMainShaderSource = "";//"#version 120\n";
			cMainShaderSource+=hwShaderManager->getShaderSnippet(0)->snippet;
			for(std::vector<std::string>::iterator it = currentPass->vIncludeModule.begin(); it != currentPass->vIncludeModule.end(); ++it)
			{
				std::string module = *it;
				if(module.compare(0, strlen("PointLightStruct"), "PointLightStruct") == 0)
				{
					cMainShaderSource+=hwShaderManager->getShaderSnippet(4)->snippet;
				}
				else if(module.compare(0, strlen("MaterialStruct"), "MaterialStruct") == 0)
				{
					cMainShaderSource+=hwShaderManager->getShaderSnippet(5)->snippet;
				}
			}

			if(currentPass->GEAR_Time)
				cMainShaderSource += "uniform vec4 GEAR_Time;\n";	//hwShaderManager->getShaderSnippet(6)->snippet;
			if(currentPass->GEAR_ScreenParams)
				cMainShaderSource += "uniform vec4 GEAR_ScreenParams;\n";

			//properties
			//range
			for(std::vector<stShaderProperty_Range*>::iterator prop_it = m_vRange_Properties.begin(); prop_it != m_vRange_Properties.end(); ++prop_it)
			{
				stShaderProperty_Range* range = *prop_it;
				if(currentPass==range->passPtr)
				{
					cMainShaderSource += "uniform float "+range->nameofProperty+";\n";
				}
			}

			//color
			for(std::vector<stShaderProperty_Color*>::iterator prop_it = m_vColor_Properties.begin(); prop_it != m_vColor_Properties.end(); ++prop_it)
			{
				stShaderProperty_Color* color = *prop_it;
				if(currentPass==color->passPtr)
				{
					cMainShaderSource += "uniform vec4 "+color->nameofProperty+";\n";
				}
			}
			//

			//float
			for(std::vector<stShaderProperty_Vector*>::iterator prop_it = m_vVector_Properties.begin(); prop_it != m_vVector_Properties.end(); ++prop_it)
			{
				stShaderProperty_Vector* vector = *prop_it;
				if(currentPass==vector->passPtr)
				{
					cMainShaderSource += "uniform vec4 "+vector->nameofProperty+";\n";
				}
			}
			//
			//

			//if(currentPass->Fog)
			//	cMainShaderSource += hwShaderManager->getShaderSnippet(7)->snippet;
			cMainShaderSource += "#ifdef GEAR_VERTEX_SHADER\n";
			cMainShaderSource+=hwShaderManager->getShaderSnippet(1)->snippet;
			cMainShaderSource+=currentPass->vertex_buffer+"\n";
			cMainShaderSource+=hwShaderManager->getShaderSnippet(2)->snippet;
			cMainShaderSource += "#elif defined (GEAR_FRAGMENT_SHADER)\n";
			cMainShaderSource+=currentPass->fragment_buffer+"\n";
			cMainShaderSource+=hwShaderManager->getShaderSnippet(3)->snippet;
			cMainShaderSource += "#endif\n";

			//create the tmp glsl file
			FILE* fp=fopen(constructed_glsl_filename, "w");
			if(fp)
			{
				fwrite(cMainShaderSource.c_str(), 1, cMainShaderSource.size(), fp);
				fclose(fp);
			}
			//

			pMainShader=hwShaderManager->LoadShaderFromBuffer(constructed_glsl_filename, cMainShaderSource.c_str(), cMainShaderSource.size());
			if(!pMainShader)
				DEBUG_PRINT("%s\nParse Success but GLSL compiler failed. Pass(%d)\n", constructed_glsl_filename, cntr);
			else
			{
				currentPass->glslShaderPtr=pMainShader;
				m_vShaderProgram.push_back(pMainShader);
				DEBUG_PRINT("%s\nParse Success. Pass(%d)\n", constructed_glsl_filename, cntr);
			}

			cMainShaderSource.clear();
		}
		//
	}
	else
	{
		DEBUG_PRINT("%s Parse Failed.\n", filename);
	}

	GX_DELETE_ARY(vsource);

	return bParseRetVal;
}

void gxSurfaceShader::appendTextureMap(stTextureMap* texmap)
{
	m_vTextureMap.push_back(texmap);
}
