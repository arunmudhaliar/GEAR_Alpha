#ifndef CURSORUTIL_H
#define CURSORUTIL_H

class cursorUtil
{
public:


	static void init()
	{
#ifdef _WIN32
        HCURSOR cursors[3];

		g_cszCursors[0] = LoadCursor(NULL, IDC_ARROW);    // default cursor
		g_cszCursors[1] = LoadCursor(NULL, IDC_SIZENS);    // other cursor
		g_cszCursors[2] = LoadCursor(NULL, IDC_SIZEWE);     // wait cursor
#endif
	}

#ifdef _WIN32
	static HCURSOR getCursor(int index)
	{
		return g_cszCursors[index];
	}
#endif
    
	static void changeCursor(int index)
	{
#ifdef _WIN32
		SetCursor(g_cszCursors[index]);
#endif
	}

#ifdef _WIN32
	static HCURSOR g_cszCursors[3];
#endif
};

#ifdef _WIN32
HCURSOR cursorUtil::g_cszCursors[3];
#endif
#endif