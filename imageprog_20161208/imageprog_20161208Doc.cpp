
// imageprog_20161208Doc.cpp: Cimageprog20161208Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imageprog_20161208.h"
#endif

#include "imageprog_20161208Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cimageprog20161208Doc

IMPLEMENT_DYNCREATE(Cimageprog20161208Doc, CDocument)

BEGIN_MESSAGE_MAP(Cimageprog20161208Doc, CDocument)
END_MESSAGE_MAP()


// Cimageprog20161208Doc 생성/소멸

Cimageprog20161208Doc::Cimageprog20161208Doc() noexcept
{
	inputdata = NULL;
	inputdata2 = NULL;
	resultimg = NULL;

	gResultimg = NULL;


}

Cimageprog20161208Doc::~Cimageprog20161208Doc()
{
	int i;

	if (inputdata != NULL) {
		for (i = 0; i < imageHeight; i++) {
			free(inputdata[i]);
		}
		free(inputdata);
	}

	if (inputdata2 != NULL) {
		for (i = 0; i < imageHeight; i++) {
			free(inputdata2[i]);
		}
		free(inputdata2);
	}

	if (resultimg != NULL) {
		for (i = 0; i < imageHeight; i++) {
			free(resultimg[i]);
		}
		free(resultimg);
	}

	if (gResultimg != NULL) {
		for (i = 0; i < gimageHeight; i++) {
			free(gResultimg[i]);
		}
		free(gResultimg);
	}
}

BOOL Cimageprog20161208Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


// Cimageprog20161208Doc serialization

void Cimageprog20161208Doc::Serialize(CArchive& ar) // 파일을 불러와 처리하는 부분
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
		LoadImageFile(ar);
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void Cimageprog20161208Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void Cimageprog20161208Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Cimageprog20161208Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Cimageprog20161208Doc 진단

#ifdef _DEBUG
void Cimageprog20161208Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cimageprog20161208Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cimageprog20161208Doc 명령


int Cimageprog20161208Doc::LoadImageFile(CArchive& ar)
{
	int maxValue, i;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)	depth = 1;
		else                            depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B'))		return 0;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		imageWidth = bih.biWidth;
		imageHeight = bih.biHeight;
		depth = bih.biBitCount / 8;

		//palette처리
		if (depth == 1)
		{	//palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	// 메모리 할당
	inputdata = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	resultimg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++)
	{
		inputdata[i] = (unsigned char*)malloc(imageWidth * depth);
		resultimg[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	if (!isbmp)
	{
		// 파일에서 읽어서 저장
		for (i = 0; i < imageHeight; i++)
			ar.Read(inputdata[i], imageWidth * depth);
	}
	else
	{
		// 파일에서 읽어서 저장
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < imageHeight; i++)
		{
			if (depth == 1)
				ar.Read(inputdata[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				// BGR=>RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1);	ar.Read(&g, 1);	ar.Read(&r, 1);
					inputdata[imageHeight - 1 - i][3 * j + 0] = r;
					inputdata[imageHeight - 1 - i][3 * j + 1] = g;
					inputdata[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}

	return 0;
}	


int Cimageprog20161208Doc::LoadSecondImageFile(CArchive& ar)
{
	int w, h, d;
	int maxValue, i;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d %d", &w, &h);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0)	d = 1;
		else                            d = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B'))		return 0;

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		w = bih.biWidth;
		h = bih.biHeight;
		d = bih.biBitCount / 8;

		//palette처리
		if (d == 1)
		{	//palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		w = 256;
		h = 256;
		d = 1;
	}

	if (imageWidth != w || imageHeight != h || depth != d)
	{
		AfxMessageBox("가로,세로,색상수가 같아야만 처리할수 있습니다.");
		return 0;
	}

	// 메모리 할당
	inputdata2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (i = 0; i < imageHeight; i++)
	{
		inputdata2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	if (!isbmp)
	{
		// 파일에서 읽어서 저장
		for (i = 0; i < imageHeight; i++)
			ar.Read(inputdata2[i], imageWidth * depth);
	}
	else
	{
		// 파일에서 읽어서 저장
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (i = 0; i < imageHeight; i++)
		{
			if (depth == 1)
				ar.Read(inputdata2[imageHeight - 1 - i], imageWidth * depth);
			else
			{
				// BGR=>RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1);	ar.Read(&g, 1);	ar.Read(&r, 1);
					inputdata2[imageHeight - 1 - i][3 * j + 0] = r;
					inputdata2[imageHeight - 1 - i][3 * j + 1] = g;
					inputdata2[imageHeight - 1 - i][3 * j + 2] = b;
				}
			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}

	return 0;
}

