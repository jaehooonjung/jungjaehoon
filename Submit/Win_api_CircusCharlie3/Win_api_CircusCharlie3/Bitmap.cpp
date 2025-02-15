#include "Bitmap.h"

Bitmap::Bitmap()
{
}

void Bitmap::Init(HDC hdc, LPCTSTR bitmapname, POINT pos)
{
	m_MemDC = CreateCompatibleDC(hdc);
	m_phImage = (HBITMAP)LoadImage(NULL, bitmapname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_phOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_phImage);

	BITMAP bitmap;
	GetObject(m_phImage, sizeof(BITMAP), &bitmap);
	m_Size.cx = bitmap.bmWidth;
	m_Size.cy = bitmap.bmHeight;
	PosChange(pos);
	m_strBitmapName = bitmapname;
}

void Bitmap::PosChange(POINT pos)
{
	m_BitmapPos.x = pos.x;
	m_BitmapPos.y = pos.y;
}

void Bitmap::Draw(HDC hdc)
{
	TransparentBlt(hdc, m_BitmapPos.x, m_BitmapPos.y, m_Size.cx, m_Size.cy, m_MemDC, 0, 0, m_Size.cx, m_Size.cy, RGB(255, 0, 255));
}

Bitmap::~Bitmap()
{
}


DubleBufferingBitmap::DubleBufferingBitmap()
{

}

void DubleBufferingBitmap::Init(HDC hdc, LPCTSTR backgroundbitmapname, LPCTSTR bitmapname, POINT pos)
{
	m_MemDC[0] = CreateCompatibleDC(hdc);
	m_phImage[0] = CreateCompatibleBitmap(hdc, 1024, 768);
	m_phOldBitmap[0] = (HBITMAP)SelectObject(m_MemDC[0], m_phImage[0]);

	m_MemDC[1] = CreateCompatibleDC(m_MemDC[0]);
	m_phImage[1] = (HBITMAP)LoadImage(NULL, backgroundbitmapname, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_phOldBitmap[1] = (HBITMAP)SelectObject(m_MemDC[1], m_phImage[1]);
	
	m_MemDC[2] = CreateCompatibleDC(m_MemDC[0]);
	m_phImage[2] = (HBITMAP)LoadImage(NULL, bitmapname, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_phOldBitmap[2] = (HBITMAP)SelectObject(m_MemDC[2], m_phImage[2]);

	BITMAP bitmaptmp;
	GetObject(m_phImage[2], sizeof(BITMAP), &bitmaptmp);
	m_Size.cx = bitmaptmp.bmWidth;
	m_Size.cy = bitmaptmp.bmHeight;
	PosChange(pos);
	m_strBitmapName = bitmapname;

}


void DubleBufferingBitmap::Draw(HDC hdc, int endline)
{
	int sizetmp;

	if (endline != NULL && (m_BitmapPos.x + m_Size.cx > endline))
	{
			sizetmp = endline - m_BitmapPos.x;
			if (sizetmp < 0)
				sizetmp = 0;
	}
	else
		sizetmp = m_Size.cx;
	BitBlt(m_MemDC[0], 0, 0, 1024, 768, m_MemDC[1], 0, 0, SRCCOPY);
	BitBlt(m_MemDC[0], 0, 0, 1024, 768, m_MemDC[0], 0, 0, SRCCOPY);
	TransparentBlt(m_MemDC[0], m_BitmapPos.x, m_BitmapPos.y, m_Size.cx, m_Size.cy, m_MemDC[2], 0, 0, sizetmp, m_Size.cy, RGB(255, 0, 255));
	BitBlt(m_MemDC[0], 0, 0, 1024, 768, m_MemDC[0], 0, 0, SRCCOPY);
	TransparentBlt(hdc, m_BitmapPos.x, m_BitmapPos.y, sizetmp, m_Size.cy, m_MemDC[0], 0, 0, sizetmp, m_Size.cy, RGB(255, 0, 255));
}

void DubleBufferingBitmap::Move(int keyflag)
{
	switch (keyflag)
	{
	case VK_LEFT:
		m_BitmapPos.x -= MOVE_DISTANCE;
		break;
	case VK_RIGHT:
		m_BitmapPos.x += MOVE_DISTANCE;
		break;
	}
}

DubleBufferingBitmap::~DubleBufferingBitmap()
{

}

AniBitmap::AniBitmap()
{

}

void AniBitmap::Init(HDC hdc, LPCTSTR backgroundbitmapname, LPCTSTR bitmapname1, LPCTSTR bitmapname2)
{
	m_MemDC[0] = CreateCompatibleDC(hdc);
	m_phImage[0] = CreateCompatibleBitmap(hdc, 1024, 768);
	m_phOldBitmap[0] = (HBITMAP)SelectObject(m_MemDC[0], m_phImage[0]);

	m_MemDC[1] = CreateCompatibleDC(m_MemDC[0]);
	m_phImage[1] = (HBITMAP)LoadImage(NULL, backgroundbitmapname, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_phOldBitmap[1] = (HBITMAP)SelectObject(m_MemDC[1], m_phImage[1]);

	m_MemDC[2] = CreateCompatibleDC(m_MemDC[0]);
	m_phImage[2] = (HBITMAP)LoadImage(NULL, bitmapname1, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_phOldBitmap[2] = (HBITMAP)SelectObject(m_MemDC[2], m_phImage[2]);

	m_MemDC[3] = CreateCompatibleDC(m_MemDC[0]);
	m_phImage[3] = (HBITMAP)LoadImage(NULL, bitmapname2, IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_phOldBitmap[3] = (HBITMAP)SelectObject(m_MemDC[3], m_phImage[3]);

	
	BITMAP bitmaptmp;
	GetObject(m_phImage[2], sizeof(BITMAP), &bitmaptmp);
	m_Size.cx = bitmaptmp.bmWidth;
	m_Size.cy = bitmaptmp.bmHeight;
	POINT nullpoint;
	nullpoint.x = NULL;
	nullpoint.y = NULL;
	PosChange(nullpoint);
	m_BitmapChangeNum = BITMAP_CHANGE_1;
	m_bCreateCheckFlag = false;
	m_ObjectArea = { m_BitmapPos.x, m_BitmapPos.y, m_BitmapPos.x+m_Size.cx, m_BitmapPos.y+m_Size.cy };
}

void AniBitmap::Draw(HDC hdc, int endline)
{
	if (m_BitmapPos.x == NULL || m_BitmapPos.y == NULL)
		return;

	int sizetmp;

	if (endline != NULL && (m_BitmapPos.x + m_Size.cx > endline))
	{
		sizetmp = endline - m_BitmapPos.x;
		if (sizetmp < 0)
			sizetmp = 0;
	}
	else
		sizetmp = m_Size.cx;

	BitBlt(m_MemDC[0], 0, 0, 1024, 768, m_MemDC[m_BitmapChangeNum], 0, 0, SRCCOPY);
	TransparentBlt(m_MemDC[0], m_BitmapPos.x, m_BitmapPos.y, m_Size.cx, m_Size.cy, m_MemDC[3], 0, 0, sizetmp, m_Size.cy, RGB(255, 0, 255));
	TransparentBlt(hdc, m_BitmapPos.x, m_BitmapPos.y, sizetmp, m_Size.cy, m_MemDC[0], 0, 0, sizetmp, m_Size.cy, RGB(255, 0, 255));

	switch (m_BitmapChangeNum)
	{
	case BITMAP_CHANGE_1:
		m_BitmapChangeNum = BITMAP_CHANGE_2;
		break;
	case BITMAP_CHANGE_2:
		m_BitmapChangeNum = BITMAP_CHANGE_1;
		break;
	}
	m_ObjectArea = { m_BitmapPos.x, m_BitmapPos.y, m_BitmapPos.x+m_Size.cx, m_BitmapPos.y+m_Size.cy };
	m_ObjectArea.top = m_BitmapPos.y + (m_Size.cy/2); // 충돌범위 조정단위 50%
}

void AniBitmap::DistancePostionSet(int pos_x)
{
	m_iDistancePostion = pos_x;
}

void AniBitmap::CreateCheckFlagChange()
{
	switch (m_bCreateCheckFlag)
	{
	case true:
		m_bCreateCheckFlag = false;
		break;
	case false:
		m_bCreateCheckFlag = true;
		break;
	}
}

void AniBitmap::ObjectAreaChange(int left, int top, int right, int bottom)
{
	m_ObjectArea = { left, top,right,bottom };

}

AniBitmap::~AniBitmap()
{

}
