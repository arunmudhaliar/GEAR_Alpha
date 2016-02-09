//
//  geFontManager.cpp
//  waves2
//
//  Created by arun on 03/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "geFontManager.h"

#if defined (USE_ProgrammablePipeLine_test)
geFont::geFont(gxHWShader* pFontShaderPtr, gxRenderer* renderer)
{
    fontShader=pFontShaderPtr;
	this->renderer=renderer;
#else
    geFont::geFont()
    {
#endif
    bitmapWidth=bitmapHeight=baseChar=totalChar=0;
    textureCoordUArray=textureCoordVArray=textureCoordUArray_width=textureCoordUArray_height=NULL;
    charWidthArray=charHeightArray=NULL;
    charDistanceArray=NULL;
    charOffsetXArray=charOffsetYArray=NULL;
    fontTextureID=0;
    spaceWidth=0;
    lineHeight=0;
    deleteFontTexture=true;
    setRGBA(1.0f, 1.0f, 1.0f, 1.0f);
    setYOffset(0.0f);
}

geFont::~geFont()
{
    reset();
}

void geFont::reset()
{
    if(fontTextureID>0 && deleteFontTexture)
    {
        glDeleteTextures(1, &fontTextureID);
        fontTextureID=0;
    }
    
    GE_DELETE_ARY(charOffsetXArray);
    GE_DELETE_ARY(charOffsetYArray);
    GE_DELETE_ARY(charWidthArray);
    GE_DELETE_ARY(charHeightArray);
    GE_DELETE_ARY(charDistanceArray);
    
    GE_DELETE_ARY(textureCoordUArray);
    GE_DELETE_ARY(textureCoordVArray);
    GE_DELETE_ARY(textureCoordUArray_width);
    GE_DELETE_ARY(textureCoordUArray_height);
#if defined (USE_ProgrammablePipeLine_test)
    fontShader=NULL;
#endif
    
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("geFont Destroyed...");
#endif 
}

bool geFont::load(gxFile& file)
{
    file.Read(bitmapWidth);
    file.Read(bitmapHeight);
    file.Read(totalChar);
    file.Read(baseChar);
    file.Read(spaceWidth);    
    
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("font width=%d, height=%d, buffersize=%d", bitmapWidth, bitmapHeight, bitmapHeight*bitmapWidth*2);
#endif 
    if(totalChar==0) return false;
    
    unsigned char* buffer=new unsigned char[bitmapHeight*bitmapWidth*2];
    
    textureCoordUArray=new float[totalChar];
    textureCoordVArray=new float[totalChar];
    textureCoordUArray_width=new float[totalChar];
    textureCoordUArray_height=new float[totalChar];
    charWidthArray=new unsigned short[totalChar];
    charHeightArray=new unsigned short[totalChar];
    charDistanceArray=new short[totalChar];
    charOffsetXArray=new short[totalChar];
    charOffsetYArray=new short[totalChar];
    
    for(int x=0;x<totalChar;x++)
    {
        file.Read(charOffsetXArray[x]);
        file.Read(charOffsetYArray[x]);

        file.Read(charWidthArray[x]);
        file.Read(charHeightArray[x]);
        file.Read(charDistanceArray[x]);
        file.Read(textureCoordUArray[x]);
        file.Read(textureCoordVArray[x]);
        file.Read(textureCoordUArray_width[x]);
        file.Read(textureCoordUArray_height[x]);
    }
    
    file.Read(lineHeight);
    file.ReadBuffer(buffer, bitmapHeight*bitmapWidth*2);
    fontTextureID=loadBuffer(buffer, true, bitmapWidth, bitmapHeight, 16);
    GE_DELETE_ARY(buffer);
    
    return true;
}

unsigned int geFont::loadBuffer(unsigned char* buffer, bool bAlpha, unsigned int width, unsigned int height, unsigned int bpp)
{
    unsigned int texID=0;
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
        
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    switch (bpp) {
        case 16:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buffer);
            break;
        case 32:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            break;
        default:
            break;
    }
        
        
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
        
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("texture loaded from buffer - w=%d, h=%d", width, height);
#endif 	
        
        
    return texID;
}

void geFont::draw()
{
#if defined (USE_ProgrammablePipeLine_test)
#else
#if ARUN_COMMENTED
    const int vertLst[8] =
	{
		bitmapWidth*65536,	0,
		0,                      0,
		bitmapWidth*65536,	bitmapHeight*65536,
		0,                      bitmapHeight*65536
	};
    
    const int tex[8] =
	{
		65536,  0,
		0,      0,
		65536,  65536,
		0,      65536,
	};

    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FIXED, 0, vertLst);

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FIXED, 0, tex);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //arun_commenting_alphatest glEnable(GL_ALPHA_TEST);
    //arun_commenting_alphatest glAlphaFuncx(GL_GREATER, 6553); //0.1f
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable(GL_BLEND);
    //arun_commenting_alphatest glDisable(GL_ALPHA_TEST);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
#endif
}

float geFont::calculateStringWidthInPixelTillNewLine(const char* str, int nChar, int iCurIndex)
{
    float kerning=GE_DEBUG_KERNING;
    float width=0.0f;
    int diff=nChar-iCurIndex;
    int cntr=0;
    while(diff--)
    {
        int actual_char=(unsigned char)str[iCurIndex+cntr];
        int ch=(unsigned char)str[iCurIndex+cntr]-baseChar;

        if(actual_char=='\n')
        {
            break;
        }
        else if(actual_char=='\t')
        {
            width+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        if(ch<0 || ch>=totalChar)
        {
            cntr++;
            continue;
        }
        width+=((float)charDistanceArray[ch]+kerning);
        cntr++;
    }
    
    return width;
}

float geFont::getCharWidth(char charValue)
{
	float kerning=GE_DEBUG_KERNING;
	int ch=(unsigned char)charValue-baseChar;
	return ((float)charDistanceArray[ch]+kerning);
}

#define CHAR_DELAY  0.05f
int geFont::drawString(const char* str, int x, int y, int width_limit, bool bCentered, bool bShadowed, float elapsetime)
{
    int retVal=1;
    int actual_len=(int)strlen(str);
    int len=(elapsetime==0.0f)?actual_len:(int)(elapsetime/CHAR_DELAY);
    if(len>actual_len){len=actual_len;}
    else
    { retVal=0; }
    
    int cntr=0;
    
    float raster_pos_x=0.0f;
    float raster_pos_y=0.0f;
    float kerning=GE_DEBUG_KERNING;

    float width=0.0f;
    if(bCentered)
    {
        width=calculateStringWidthInPixelTillNewLine(str, len, 0);
        raster_pos_x=-width*0.5f;
    }
    
    for(int xx=0;xx<len;xx++)
    {
        int actual_char=(unsigned char)str[xx];
        int ch=(unsigned char)str[xx]-baseChar;
        
        if(actual_char=='\n')
        {
            raster_pos_y+=(float)lineHeight;
            raster_pos_x=0.0f;
            if(bCentered)
            {
                width=calculateStringWidthInPixelTillNewLine(str, len, xx+1);
                raster_pos_x=-width*0.5f;
            }
            continue;
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }

        int offx=charOffsetXArray[ch];
        int offy=charOffsetYArray[ch];
        
        float left=raster_pos_x+offx;
        float right=raster_pos_x+charWidthArray[ch]+offx;
        float top=raster_pos_y+offy;
        float bottom=raster_pos_y+charHeightArray[ch]+offy;
        
        vertexCoordinateArray[cntr*12+0]=(short)right;     vertexCoordinateArray[cntr*12+1]=(short)top;
        vertexCoordinateArray[cntr*12+2]=(short)left;      vertexCoordinateArray[cntr*12+3]=(short)top;
        vertexCoordinateArray[cntr*12+4]=(short)right;     vertexCoordinateArray[cntr*12+5]=(short)bottom;
        vertexCoordinateArray[cntr*12+6]=(short)left;      vertexCoordinateArray[cntr*12+7]=(short)bottom;
        vertexCoordinateArray[cntr*12+8]=(short)right;     vertexCoordinateArray[cntr*12+9]=(short)bottom;
        vertexCoordinateArray[cntr*12+10]=(short)left;     vertexCoordinateArray[cntr*12+11]=(short)top;


        textureCoordinateArray[cntr*12+0]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordinateArray[cntr*12+1]=textureCoordVArray[ch];
        textureCoordinateArray[cntr*12+2]=textureCoordUArray[ch];                      textureCoordinateArray[cntr*12+3]=textureCoordVArray[ch];
        textureCoordinateArray[cntr*12+4]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordinateArray[cntr*12+5]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordinateArray[cntr*12+6]=textureCoordUArray[ch];                      textureCoordinateArray[cntr*12+7]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordinateArray[cntr*12+8]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordinateArray[cntr*12+9]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordinateArray[cntr*12+10]=textureCoordUArray[ch];                     textureCoordinateArray[cntr*12+11]=textureCoordVArray[ch];

        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
		if(raster_pos_x>(float)width_limit)
		{
			retVal=9;	//window bound
			break;
		}
        cntr++;
    }
    
#if defined (USE_ProgrammablePipeLine_test)
    fontShader->enableProgram();
    //fontShader->resetAllFlags();
    
    glVertexAttribPointer(fontShader->getAttribLoc("a_vertex_coord_v4"), 2, GL_SHORT, GL_FALSE, 0, vertexCoordinateArray);
    glEnableVertexAttribArray(fontShader->getAttribLoc("a_vertex_coord_v4"));
    
    glVertexAttribPointer(fontShader->getAttribLoc("a_uv_coord0_v2"), 2, GL_FLOAT, GL_FALSE, 0, textureCoordinateArray);
    glEnableVertexAttribArray(fontShader->getAttribLoc("a_uv_coord0_v2"));
    
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);	
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    matrix4x4f transformTM;
    
    if(bShadowed)
    {
        transformTM.setPosition(x+1, y+1+fontYOffset, -1.0f);
        transformTM= *renderer->getOrthoProjectionMatrix() * transformTM;
        const float* u_mvp_m4x4_shadow=transformTM.getMatrix();
        fontShader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4_shadow, false, 4);
        float shadow_color[]={0.0f, 0.0f, 0.0f, 0.7f};
        fontShader->sendUniform4fv("u_color_v4", shadow_color);
        glDrawArrays(GL_TRIANGLES, 0, cntr*6);
        
        transformTM.identity(); //reset the matrix
    }
    
    transformTM.setPosition(x, y+fontYOffset, -1.0f);
    transformTM= *renderer->getOrthoProjectionMatrix() * transformTM;
    const float* u_mvp_m4x4=transformTM.getMatrix();
    fontShader->sendUniformTMfv("u_mvp_m4x4", u_mvp_m4x4, false, 4);
    fontShader->sendUniform4fv("u_color_v4", colorRGBA);
    glDrawArrays(GL_TRIANGLES, 0, cntr*6);
    
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(fontShader->getAttribLoc("a_uv_coord0_v2"));
    //glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(fontShader->getAttribLoc("a_vertex_coord_v4"));
    
    fontShader->disableProgram();
#else
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_SHORT, 0, vertexCoordinateArray);
    
    //glActiveTexture(GL_TEXTURE0);
    //glClientActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinateArray);
    
    //arun_commenting_alphatest glEnable(GL_ALPHA_TEST);
    //arun_commenting_alphatest glAlphaFuncx(GL_GREATER, 6553); //0.1f
	glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_ONE, GL_ONE);

    glPushMatrix();
    if(bShadowed)
    {   
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glTranslatef(x+1, y+1+fontYOffset, 0);
        glDrawArrays(GL_TRIANGLES, 0, cntr*6);
        glTranslatef(-1, -1, 0);
    }
    else
    {
        glTranslatef(x, y+fontYOffset, 0);
    }
    glColor4f(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]);
    glDrawArrays(GL_TRIANGLES, 0, cntr*6);
    glPopMatrix();
    
    glDisable(GL_BLEND);
    //arun_commenting_alphatest glDisable(GL_ALPHA_TEST);
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
#endif
    
    return retVal;
}

void geFont::drawOnBuffer(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer, ALIGNMENT alignment) const
{
    switch (alignment)
    {
        case ALIGN_LEFT:
            drawOnBuffer_left(str, cx, cy, nChar, vertexCoordBuffer, textureCoordBuffer, nLines, linebuffer);
            break;
        case ALIGN_RIGHT:
            drawOnBuffer_left(str, cx, cy, nChar, vertexCoordBuffer, textureCoordBuffer, nLines, linebuffer);
            break;
        case ALIGN_CENTER:
            drawOnBuffer_center(str, cx, cy, nChar, vertexCoordBuffer, textureCoordBuffer, nLines, linebuffer);
            break;
        case ALIGN_JUSTIFY:
            drawOnBuffer_justify(str, cx, cy, nChar, vertexCoordBuffer, textureCoordBuffer, nLines, linebuffer);
            break;
    }
}

void geFont::drawOnBuffer_justify(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const
{
    nLines=0;
    int len=nChar;
    int cntr=0;
    
    float raster_pos_x=0.0f;
    float raster_pos_y=0.0f;
    float kerning=GE_DEBUG_KERNING;
    bool bLineStarted=false;
    float extraspace=0.0f;
    float extra_pixels_added_to_space=0.0f;
    int nWords=0;
    if(calculateExtraSpaceOnRightSide(str, cx, nChar, 0, extraspace, nWords))
    {
        if(nWords)
        {
            extra_pixels_added_to_space=extraspace/nWords;
        }
    }
    else
    {
        extraspace=0.0f;
    }
    
    unsigned int nCharsOnLine=0;
    bool bCalculateNextLine=false;
    for(int x=0;x<len;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;
        
        if(actual_char=='\n')
        {
            if(bCalculateNextLine)
            {
                if(nLines==0)
                    linebuffer[nLines]=nCharsOnLine;
                else
                    linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;
                nCharsOnLine=0;
                nLines++;
            }
            
            bCalculateNextLine=true;
            raster_pos_y+=(float)lineHeight;

            continue;
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        
        if(bCalculateNextLine)
        {
            if(calculateExtraSpaceOnRightSide(str, cx, nChar, x, extraspace, nWords))
            {
                if(nWords)
                {
                    extra_pixels_added_to_space=extraspace/nWords;
                }
            }
            else
            {
                extra_pixels_added_to_space=0.0f;
                extraspace=0.0f;
            }
            raster_pos_x=0.0f;
            if(nLines==0)
                linebuffer[nLines]=nCharsOnLine;
            else
                linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;
            bLineStarted=false;
            nCharsOnLine=0;
            nLines++;
            bCalculateNextLine=false;
        }
        
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }
        
        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            bCalculateNextLine=true;
            raster_pos_y+=(float)lineHeight;
            x--;
            continue;
        }
        
        if(!bLineStarted)
        {
            if(actual_char==32)
                continue;
            else
                bLineStarted=true;
        }
        
        int offx=charOffsetXArray[ch];
        int offy=charOffsetYArray[ch];
        
        float left=raster_pos_x+offx;
        float right=raster_pos_x+charWidthArray[ch]+offx;
        float top=raster_pos_y+offy;
        float bottom=raster_pos_y+charHeightArray[ch]+offy;
        
        vertexCoordBuffer[cntr*12+0]=(short)right;     vertexCoordBuffer[cntr*12+1]=(short)top;
        vertexCoordBuffer[cntr*12+2]=(short)left;      vertexCoordBuffer[cntr*12+3]=(short)top;
        vertexCoordBuffer[cntr*12+4]=(short)right;     vertexCoordBuffer[cntr*12+5]=(short)bottom;
        vertexCoordBuffer[cntr*12+6]=(short)left;      vertexCoordBuffer[cntr*12+7]=(short)bottom;
        vertexCoordBuffer[cntr*12+8]=(short)right;     vertexCoordBuffer[cntr*12+9]=(short)bottom;
        vertexCoordBuffer[cntr*12+10]=(short)left;     vertexCoordBuffer[cntr*12+11]=(short)top;
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
        if(actual_char==32)
            raster_pos_x+=extra_pixels_added_to_space;
        
        textureCoordBuffer[cntr*12+0]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+1]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+2]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+3]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+4]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+5]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+6]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+7]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+8]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+9]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+10]=textureCoordUArray[ch];                     textureCoordBuffer[cntr*12+11]=textureCoordVArray[ch];
        
        cntr++;
        nCharsOnLine++;
    }
    nChar=cntr;
    if(nLines==0)
        linebuffer[nLines]=nChar;
    else
        linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;
    nLines++;
}

bool geFont::calculateExtraSpaceOnRightSide(const char* str, int cx, int nChar, int iCurIndex, float& extraspace, int& nWords) const
{   
    nWords=0;
    float raster_pos_x=0.0f;
    float kerning=GE_DEBUG_KERNING;
    float last_space_charXPos=0.0f;
    bool bLineStarted=false;
    bool bCarriageReturnEncountered=false;
    bool bNeglectLastWord=true;
    bool bEndReached=false;
    
    int x=0;
    for(x=iCurIndex;x<nChar;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;
//        const char* current_char=&str[x];
        
        if(!bLineStarted)
        {
            if(actual_char==32)
                continue;
            else
                bLineStarted=true;
        }
        
        if(actual_char==32) //space
        {
            nWords++;
            last_space_charXPos=raster_pos_x;
        }
        else if(actual_char=='\n')
        {
            raster_pos_x=0.0f;
            bCarriageReturnEncountered=true;
            break;  //found eol
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }

        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            if(actual_char==32 || actual_char=='\n' || actual_char=='\t')
            {
                bNeglectLastWord=false;
            }
            raster_pos_x=0.0f;
            break; //found eol
        }
        
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
    }
    
    if(x==nChar)
        bEndReached=true;
    
    extraspace=cx-last_space_charXPos;
    
    if(bNeglectLastWord)
        nWords--;
    
    return (bLineStarted && !bCarriageReturnEncountered && bNeglectLastWord && !bEndReached);
}

void geFont::drawOnBuffer_center(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const
{
    nLines=0;
    int len=nChar;
    int cntr=0;
    
    float raster_pos_x=0.0f;
    float raster_pos_y=0.0f;
    float kerning=GE_DEBUG_KERNING;
    bool bLineStarted=false;
    float extraspace=0.0f;
    int nWords=0;
    if(calculateExtraSpaceOnBothSide(str, cx, nChar, 0, extraspace, nWords))
    {
        if(nWords)
        {
            raster_pos_x=(extraspace/2);
        }
    }
    
    unsigned int nCharsOnLine=0;
    bool bCalculateNextLine=false;
    for(int x=0;x<len;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;
        //const char* temp=&str[x];
        
        if( actual_char=='\n')
        {
            raster_pos_y+=(float)lineHeight;
            bCalculateNextLine=true;
            continue;
        }
        
        if(bCalculateNextLine)
        {
            raster_pos_x=0.0f;
            if(calculateExtraSpaceOnBothSide(str, cx, nChar, x, extraspace, nWords))
            {
                if(nWords)
                {
                    raster_pos_x=(extraspace/2);
                }
            }

            if(nLines==0)
                linebuffer[nLines]=nCharsOnLine;
            else
                linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;

            bLineStarted=false;
            nCharsOnLine=0;
            nLines++;
            bCalculateNextLine=false;
        }

        if(canNeglectNextWord(str, cx, nChar, x, raster_pos_x))
        {
            bCalculateNextLine=true;
            raster_pos_y+=(float)lineHeight;
            continue;
        }
        

        if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }
        
        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            bCalculateNextLine=true;
        }
        
        if(!bLineStarted)
        {
            if(actual_char==32)
                continue;
            else
                bLineStarted=true;
        }
        
        int offx=charOffsetXArray[ch];
        int offy=charOffsetYArray[ch];
        
        float left=raster_pos_x+offx;
        float right=raster_pos_x+charWidthArray[ch]+offx;
        float top=raster_pos_y+offy;
        float bottom=raster_pos_y+charHeightArray[ch]+offy;
        
        vertexCoordBuffer[cntr*12+0]=(short)right;     vertexCoordBuffer[cntr*12+1]=(short)top;
        vertexCoordBuffer[cntr*12+2]=(short)left;      vertexCoordBuffer[cntr*12+3]=(short)top;
        vertexCoordBuffer[cntr*12+4]=(short)right;     vertexCoordBuffer[cntr*12+5]=(short)bottom;
        vertexCoordBuffer[cntr*12+6]=(short)left;      vertexCoordBuffer[cntr*12+7]=(short)bottom;
        vertexCoordBuffer[cntr*12+8]=(short)right;     vertexCoordBuffer[cntr*12+9]=(short)bottom;
        vertexCoordBuffer[cntr*12+10]=(short)left;     vertexCoordBuffer[cntr*12+11]=(short)top;
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
        
        textureCoordBuffer[cntr*12+0]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+1]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+2]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+3]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+4]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+5]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+6]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+7]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+8]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+9]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+10]=textureCoordUArray[ch];                     textureCoordBuffer[cntr*12+11]=textureCoordVArray[ch];
        
        cntr++;
        nCharsOnLine++;
    }
    nChar=cntr;
    nLines++;
}

bool geFont::calculateExtraSpaceOnBothSide(const char* str, int cx, int nChar, int iCurIndex, float& extraspace, int& nWords) const
{
    nWords=0;
    float raster_pos_x=0.0f;
    float kerning=GE_DEBUG_KERNING;
    float last_space_charXPos=0.0f;
    bool bLineStarted=false;
    bool bCarriageReturnEncountered=false;
    bool bNeglectLastWord=true;
    bool bEndReached=false;
    
    int x=0;
    for(x=iCurIndex;x<nChar;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;
        //        const char* current_char=&str[x];
        
        if(!bLineStarted)
        {
            if(actual_char==32)
            {
                if(raster_pos_x+(float)spaceWidth+kerning>cx)
                {
                    raster_pos_x=0.0f;
                    return false;
                }
                else
                    raster_pos_x+=(spaceWidth+kerning);
                continue;
            }
            else
                bLineStarted=true;
        }
        
        if(actual_char==32) //space
        {
            nWords++;
            last_space_charXPos=raster_pos_x;
        }
        else if(actual_char=='\n')
        {
            if(bLineStarted)
                nWords++;
            last_space_charXPos=raster_pos_x;   //chk
            raster_pos_x=0.0f;
            bCarriageReturnEncountered=true;
            bNeglectLastWord=false;
            break;  //found eol
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }
        
        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            if(actual_char==32 || actual_char=='\n' || actual_char=='\t')
            {
                bNeglectLastWord=false;
            }
            raster_pos_x=0.0f;
            break; //found eol
        }
        
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
    }
    
    if(x==nChar)
        bEndReached=true;
    
    extraspace=cx-last_space_charXPos;
    
    if(bNeglectLastWord)
        nWords--;
    
    return (bLineStarted);
}

void geFont::drawOnBuffer_left(const char* str, int cx, int cy, unsigned int& nChar, short* vertexCoordBuffer, float* textureCoordBuffer, unsigned int& nLines, unsigned short* linebuffer) const
{
    nLines=0;
    int len=nChar;
    int cntr=0;
    
    float raster_pos_x=0.0f;
    float raster_pos_y=0.0f;
    float kerning=GE_DEBUG_KERNING;
    bool bLineStarted=false;
//    float extraspace=0.0f;
//    float extra_pixels_added_to_space=0.0f;
//    int nWords=0;
//    if(calculateExtraSpaceOnLine(str, cx, nChar, 0, extraspace, nWords))
//    {
//        if(nWords)
//        {
//            extra_pixels_added_to_space=extraspace/nWords;
//        }
//    }
//    else
//    {
//        extraspace=0.0f;
//    }
    
    unsigned int nCharsOnLine=0;
    for(int x=0;x<len;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;
        
        if(actual_char==32 && bLineStarted)
        {
            if(canNeglectNextWord(str, cx, nChar, x, raster_pos_x))
            {
                raster_pos_x=0.0f;
                raster_pos_y+=(float)lineHeight;
                if(nLines==0)
                    linebuffer[nLines]=nCharsOnLine;
                else
                    linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;

                bLineStarted=false;
                nCharsOnLine=0;
                nLines++;
                continue;
            }
        }
        else if(actual_char=='\n')
        {
            raster_pos_x=0.0f;
            raster_pos_y+=(float)lineHeight;
            if(nLines==0)
                linebuffer[nLines]=nCharsOnLine;
            else
                linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;

            bLineStarted=false;
            nCharsOnLine=0;
            nLines++;
            continue;
        }
        else if(actual_char=='\t')
        {
            raster_pos_x+=(charWidthArray[32-baseChar]*5);
            continue;
        }
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }
        
        /*
        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            raster_pos_x=0.0f;
            raster_pos_y+=(float)lineHeight;
            if(nLines==0)
                linebuffer[nLines]=nCharsOnLine;
            else
                linebuffer[nLines]=linebuffer[nLines-1]+nCharsOnLine;

            bLineStarted=false;
            nCharsOnLine=0;
            nLines++;
        }
        */
        
        if(!bLineStarted)
        {
            if(actual_char!=32)
                bLineStarted=true;
        }
        
        int offx=charOffsetXArray[ch];
        int offy=charOffsetYArray[ch];
        
        float left=raster_pos_x+offx;
        float right=raster_pos_x+charWidthArray[ch]+offx;
        float top=raster_pos_y+offy;
        float bottom=raster_pos_y+charHeightArray[ch]+offy;
        
        vertexCoordBuffer[cntr*12+0]=(short)right;     vertexCoordBuffer[cntr*12+1]=(short)top;
        vertexCoordBuffer[cntr*12+2]=(short)left;      vertexCoordBuffer[cntr*12+3]=(short)top;
        vertexCoordBuffer[cntr*12+4]=(short)right;     vertexCoordBuffer[cntr*12+5]=(short)bottom;
        vertexCoordBuffer[cntr*12+6]=(short)left;      vertexCoordBuffer[cntr*12+7]=(short)bottom;
        vertexCoordBuffer[cntr*12+8]=(short)right;     vertexCoordBuffer[cntr*12+9]=(short)bottom;
        vertexCoordBuffer[cntr*12+10]=(short)left;     vertexCoordBuffer[cntr*12+11]=(short)top;
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
        
        textureCoordBuffer[cntr*12+0]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+1]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+2]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+3]=textureCoordVArray[ch];
        textureCoordBuffer[cntr*12+4]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+5]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+6]=textureCoordUArray[ch];                      textureCoordBuffer[cntr*12+7]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+8]=(textureCoordUArray[ch]+textureCoordUArray_width[ch]);   textureCoordBuffer[cntr*12+9]=(textureCoordVArray[ch]+textureCoordUArray_height[ch]);
        textureCoordBuffer[cntr*12+10]=textureCoordUArray[ch];                     textureCoordBuffer[cntr*12+11]=textureCoordVArray[ch];
        
        cntr++;
        nCharsOnLine++;
    }
    nChar=cntr;
    if(nCharsOnLine>0)
        nLines++;
}

bool geFont::canNeglectNextWord(const char* str, int cx, int nChar, int iCurIndex, float raster_pos_x) const
{
    float kerning=GE_DEBUG_KERNING;
    //float last_space_charXPos=0.0f;
    bool bWordStarted=false;
    
    int x=0;
    for(x=iCurIndex;x<nChar;x++)
    {
        int actual_char=(unsigned char)str[x];
        int ch=actual_char-baseChar;

        if(!bWordStarted)
        {
            if(actual_char==32)
                continue;
            else
                bWordStarted=true;
        }
        
        if(actual_char==32) //space
        {
            return false;
        }
        
        if(actual_char=='\n')
        {
            return false;
        }
        if(actual_char=='\t')
        {
            return false;
        }
//        if(actual_char=='.')
//        {
//            return false;
//        }
        if(ch<0 || ch>=totalChar)
        {
            continue;
        }
        
        if(raster_pos_x+(float)charDistanceArray[ch]+kerning>cx)
        {
            if(actual_char==32 || actual_char=='\n' || actual_char=='\t')
            {
                return false;
            }
            raster_pos_x=0.0f;
            break; //found eol
        }
        
        raster_pos_x+=((float)charDistanceArray[ch]+kerning);
    }
    
    if(x==nChar)
        return false;
    
    return true;
}

//========================================================================================

geFontManager geFontManager::g_cFontManager;
geFont* geFontManager::g_pFontArial10_84Ptr=NULL;
geFont* geFontManager::g_pFontArial10_80Ptr=NULL;

void geFontManager::InitializeFonts()
{
    g_cFontManager.init();
    g_pFontArial10_84Ptr=g_cFontManager.loadFont("res//fonts//arial_iphone10_84.ecf");
    g_pFontArial10_80Ptr=g_cFontManager.loadFont("res//fonts//arial_iphone10_80.ecf");
    
    g_pFontArial10_84Ptr->setRGBA(0.7f, 0.7f, 0.7f);
    g_pFontArial10_80Ptr->setRGBA(0.5f, 0.5f, 0.5f);
}
    
geFontManager::geFontManager()
{
#if defined (USE_ProgrammablePipeLine_test)
	renderer=NULL;
#endif
}

geFontManager::~geFontManager()
{
	reset(false);
}

void geFontManager::init()
{
#if defined (USE_ProgrammablePipeLine_test)
    //if(technique==rendererBase::gl_programmable_pipeline)
    {
#if defined(WIN32)
        m_cFontShader.loadShader("res/shadersWin32/hwshader/fontshader.glsl");
#else
        m_cFontShader.loadShader("/storage/emulated/0/gear/shadersAndroid/hwshader/fontshader.glsl");
#endif
    }
#endif
}

void geFontManager::reset(bool reload)
{
    for(int x=0;x<m_cvFontList.size();x++)
    {
        geFont* font=m_cvFontList[x];
        if(reload)font->setDeleteGLTexture(!reload);
        GE_DELETE(font);
    }
    m_cvFontList.clear();
}

#if defined (USE_ProgrammablePipeLine_test)
void geFontManager::setRenderer(gxRenderer* renderer)
{
	this->renderer=renderer;
	for(int x=0;x<m_cvFontList.size();x++)
    {
        geFont* font=m_cvFontList[x];
		font->setRenderer(this->renderer);
    }
}
#endif

geFont* geFontManager::loadFont(const char* filename)
{
#if defined (USE_ProgrammablePipeLine_test)
    geFont* newFont=new geFont(&m_cFontShader, renderer);
#else
    geFont* newFont=new geFont();
#endif
    gxFile file;
    file.OpenFile(filename, gxFile::FILE_r);
        
#if defined (LOG_DEBUG_ENGINE)
    DEBUG_PRINT("font %s loading...", filename);
#endif 
    newFont->load(file);
    file.CloseFile();
    
    m_cvFontList.push_back(newFont);
    
    return newFont;
}

