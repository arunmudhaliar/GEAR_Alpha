#ifndef CURSORUTIL_H
#define CURSORUTIL_H

class cursorUtil
{
public:

	static void init()
	{
		HCURSOR cursors[3];

		g_cszCursors[0] = LoadCursor(NULL, IDC_ARROW);    // default cursor
		g_cszCursors[1] = LoadCursor(NULL, IDC_SIZENS);    // other cursor
		g_cszCursors[2] = LoadCursor(NULL, IDC_SIZEWE);     // wait cursor
	}

	static HCURSOR getCursor(int index)
	{
		return g_cszCursors[index];
	}

	static void changeCursor(int index)
	{
		SetCursor(g_cszCursors[index]);
	}

	static HCURSOR g_cszCursors[3];
};

HCURSOR cursorUtil::g_cszCursors[3];

#endif