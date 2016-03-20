#pragma once

#include <string.h>

#ifdef GEAR_APPLE
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <OpenGL/glext.h>
    #include <GLUT/glut.h>
#elif defined(GEAR_WINDOWS)
    #include <gl/gl.h>
    #include <gl/glu.h>
    //#include "glext.h>
    #include <gl/glut.h>
#else
#error GL includes not found
#endif

#ifndef GEAR2D
#include "../../../GEAREngine/src/hwShader/gxHWShader.h"
#include "../../../GEAREngine/src/core/vector4.h"
#endif

class geUtil
{
public:
	static float getFloat(const char* str)
	{
		float value=0.0f;
		if(str==NULL)
			return 0.0f;
		if(strlen(str)==0)
			return 0.0f;

		if(EOF==sscanf(str, "%f", &value))
		{
			return 0.0f;
		}

		return value;
	}

#ifndef GEAR2D
	static void drawGizmo(float scale, gxHWShader* shader, int selectedAxiz=0)
	{
		float lines[]=
		{
			0.0f, 0.0f, 0.0f,
			scale, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f,

			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, scale
		};

		float color	[]=
		{
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f
		};

		if(selectedAxiz==1)
		{
			color[1]=1.0f;
			color[4]=1.0f;
		}
		else if(selectedAxiz==2)
		{
			color[6]=1.0f;
			color[9]=1.0f;
		}
		else if(selectedAxiz==3)
		{
			color[12]=1.0f;
			color[13]=1.0f;
			color[14]=0.0f;
			color[15]=1.0f;
			color[16]=1.0f;
			color[17]=0.0f;
		}

		//glDisable(GL_DEPTH_TEST);
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(3, GL_FLOAT, 0, lines);

		//glEnableClientState(GL_COLOR_ARRAY);
		//glColorPointer(3, GL_FLOAT, 0, color);

		//glDrawArrays(GL_LINES, 0, 6);

		//glDisableClientState(GL_COLOR_ARRAY);
		//glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_DEPTH_TEST);
		glEnableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));
		glVertexAttribPointer(shader->getAttribLoc("a_vertex_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, lines);

		glEnableVertexAttribArray(shader->getAttribLoc("a_color_coord_v4"));
		glVertexAttribPointer(shader->getAttribLoc("a_color_coord_v4"), 3, GL_FLOAT, GL_FALSE, 0, color);

		glDrawArrays(GL_LINES, 0, 6);
		glDisableVertexAttribArray(shader->getAttribLoc("a_color_coord_v4"));
		glDisableVertexAttribArray(shader->getAttribLoc("a_vertex_coord_v4"));


		////x-axis cone
		//glPushMatrix();
		//glColor3fv(&color[0]);
		//glTranslatef(scale, 0.0f, 0.0f);
		//glRotatef(90, 0, 1, 0);
		//glutSolidCone(0.1*scale, 0.2*scale, 10, 4);
		//glRotatef(-180, 0, 1, 0);
		//glutSolidCone(0.1*scale, 0.0f, 10, 4);
		//glPopMatrix();

		////y-axis cone
		//glPushMatrix();
		//glColor3fv(&color[6]);
		//glTranslatef(0.0f, scale, 0.0f);
		//glRotatef(-90, 1, 0, 0);
		//glutSolidCone(0.1*scale, 0.2*scale, 10, 4);
		//glRotatef(-180, 1, 0, 0);
		//glutSolidCone(0.1*scale, 0.0f, 10, 4);
		//glPopMatrix();

		////z-axis cone
		//glPushMatrix();
		//glColor3fv(&color[12]);
		//glTranslatef(0.0f, 0.0f, scale);
		//glutSolidCone(0.1*scale, 0.2*scale, 10, 4);
		//glRotatef(-180, 0, 1, 0);
		//glutSolidCone(0.1*scale, 0.0f, 10, 4);
		//glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	}
#endif

	static void drawGizmoCones(float scale, int selectedAxiz=0)
	{
		float color	[]=
		{
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f
		};

		if(selectedAxiz==1)
		{
			color[1]=1.0f;
			color[4]=1.0f;
		}
		else if(selectedAxiz==2)
		{
			color[6]=1.0f;
			color[9]=1.0f;
		}
		else if(selectedAxiz==3)
		{
			color[12]=1.0f;
			color[13]=1.0f;
			color[14]=0.0f;
			color[15]=1.0f;
			color[16]=1.0f;
			color[17]=0.0f;
		}

		//x-axis cone
		glPushMatrix();
		glColor3fv(&color[0]);
		glTranslatef(0.2*scale, 0.0f, 0.0f);
		glRotatef(90, 0, 1, 0);
		glutSolidCone(0.1*scale, 0.3*scale, 10, 4);
		glRotatef(-180, 0, 1, 0);
		glutSolidCone(0.1*scale, 0.0f, 10, 4);
		glPopMatrix();

		//y-axis cone
		glPushMatrix();
		glColor3fv(&color[6]);
		glTranslatef(0.0f, 0.2*scale, 0.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(0.1*scale, 0.3*scale, 10, 4);
		glRotatef(-180, 1, 0, 0);
		glutSolidCone(0.1*scale, 0.0f, 10, 4);
		glPopMatrix();

		//z-axis cone
		glPushMatrix();
		glColor3fv(&color[12]);
		glTranslatef(0.0f, 0.0f, 0.2*scale);
		glutSolidCone(0.1*scale, 0.3*scale, 10, 4);
		glRotatef(-180, 0, 1, 0);
		glutSolidCone(0.1*scale, 0.0f, 10, 4);
		glPopMatrix();
	}

	// Note: This function returns a pointer to a substring of the original string.
	// If the given string was allocated dynamically, the caller must not overwrite
	// that pointer with the returned value, since the original pointer must be
	// deallocated using the same allocator with which it was allocated.  The return
	// value must NOT be deallocated using free() etc.
	static char* trimwhitespace(char *str)
	{
		char *end;

		// Trim leading space
		while(isspace(*str)) str++;

		if(*str == 0)  // All spaces?
		return str;

		// Trim trailing space
		end = str + strlen(str) - 1;
		while(end > str && isspace(*end)) end--;

		// Write new null terminator
		*(end+1) = 0;

		return str;
	}

	static void convertPathToUnixFormat(char* path)
	{
		//win32 to unix style path
		for(int x=0;x<(int)strlen(path);x++)
		{
			if(path[x]=='\\')
				path[x]='/';
		}
		//
	}
	
	static bool isSubString(const char* str, const char* substr)
	{
		const char* fond_str=STRCHR_nocase(str, substr[0]);
		if(fond_str==NULL) return false;

		bool bFound=false;

		while(!bFound)
		{
			int sub_strlen=(int)strlen(substr);
			int str_len=(int)strlen(fond_str);
			if(sub_strlen>str_len) return false;

			bFound=true;
			for(int x=0;x<sub_strlen; x++)
			{
				if(tolower(substr[x])!=tolower(*fond_str))
				{
					bFound=false;
					break;
				}
				fond_str++;
			}

			if(bFound)
				return bFound;

			fond_str=STRCHR_nocase(fond_str, substr[0]);
			if(fond_str==NULL) return false;
		}

		return false;
	}

private:

	static const char* STRCHR_nocase(const char* str, char value)
	{
		for(int x=0;x<(int)strlen(str);x++)
		{
			if(tolower(str[x])==tolower(value))
				return &str[x];
		}

		return NULL;
	}
};