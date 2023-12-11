
// imageprog_20161208View.cpp: Cimageprog20161208View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imageprog_20161208.h"
#endif

#include "imageprog_20161208Doc.h"
#include "imageprog_20161208View.h"

#include "CAngleinputDialog.h"
//#include "CAngleinputDialog"
#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MORPHING

// Cimageprog20161208View

IMPLEMENT_DYNCREATE(Cimageprog20161208View, CScrollView)

BEGIN_MESSAGE_MAP(Cimageprog20161208View, CScrollView)
	ON_COMMAND(ID_REGION_SHARPENINING, &Cimageprog20161208View::OnRegionSharpenining)
	ON_COMMAND(ID_REGION_SMOOTHING, &Cimageprog20161208View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &Cimageprog20161208View::OnRegionEmbossing)
	ON_COMMAND(ID_REGION_PREWITT, &Cimageprog20161208View::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_ROBERT, &Cimageprog20161208View::OnRegionRobert)
	ON_COMMAND(ID_REGION_SOBEL, &Cimageprog20161208View::OnRegionSobel)
	ON_COMMAND(ID_REGION_AVERAGE_FILLTERING, &Cimageprog20161208View::OnRegionAverageFilltering)
	ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &Cimageprog20161208View::OnRegionMedianFiltering)
	ON_COMMAND(ID_PIXEL_ADD, &Cimageprog20161208View::OnPixelAdd2)
	ON_COMMAND(ID_PIXEL_MUL, &Cimageprog20161208View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &Cimageprog20161208View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONTRAST_STRETCHING, &Cimageprog20161208View::OnPixelContrastStretching)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &Cimageprog20161208View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &Cimageprog20161208View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &Cimageprog20161208View::OnPixelTwoImageSub)
	ON_COMMAND(ID_EROSION, &Cimageprog20161208View::OnErosion)
	ON_COMMAND(ID_MOPOLOGY_COLOR_TO_GRAY, &Cimageprog20161208View::OnMopologyColorToGray)
	ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &Cimageprog20161208View::OnMopologyBinarization)
	ON_COMMAND(ID_MOPOLOGY_DILATION, &Cimageprog20161208View::OnMopologyDilation)
	ON_COMMAND(ID_MOPOLOGY_OPENING, &Cimageprog20161208View::OnMopologyOpening)
	ON_COMMAND(ID_MOPOLOGY_CLOSEING, &Cimageprog20161208View::OnMopologyCloseing)
	ON_COMMAND(ID_GIOMETRY_ZOOMIN_PIXEL_COPY, &Cimageprog20161208View::OnGiometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &Cimageprog20161208View::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &Cimageprog20161208View::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &Cimageprog20161208View::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &Cimageprog20161208View::OnGeometryWarping)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AVI_VIEW, &Cimageprog20161208View::OnAviView)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &Cimageprog20161208View::OnGeometryZoomoutMeanSub)
	ON_COMMAND(ID_GEOMETRY_AVG_SAMPLING, &Cimageprog20161208View::OnGeometryAvgSampling)
	ON_COMMAND(ID_GEOMETRY_ROTATION, &Cimageprog20161208View::OnGeometryRotation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &Cimageprog20161208View::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_PIXEL_SUB, &Cimageprog20161208View::OnPixelSub)
	ON_COMMAND(ID_PIXEL_DIV, &Cimageprog20161208View::OnPixelDiv)
	ON_COMMAND(ID_GEOMETRY_MORPHING, &Cimageprog20161208View::OnGeometryMorphing)
END_MESSAGE_MAP()

// Cimageprog20161208View 생성/소멸

Cimageprog20161208View::Cimageprog20161208View() noexcept
{
	bAviMode = false;

}

Cimageprog20161208View::~Cimageprog20161208View()
{
}

BOOL Cimageprog20161208View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// Cimageprog20161208View 그리기

void Cimageprog20161208View::OnDraw(CDC* pDC)
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
	int x,y;

	if (bAviMode) {
		//avi화일 재생
		LoadAviFile(pDC);

		bAviMode = false;
		return;
	}

	if (pDoc->inputdata != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputdata[y][x], pDoc->inputdata[y][x], pDoc->inputdata[y][x]));
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputdata[y][x * 3 + 0], pDoc->inputdata[y][x * 3 + 1], pDoc->inputdata[y][x * 3 + 2]));
		}
	}

	if (pDoc->resultimg != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->resultimg[y][x], pDoc->resultimg[y][x], pDoc->resultimg[y][x]));
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->resultimg[y][x * 3 + 0], pDoc->resultimg[y][x * 3 + 1], pDoc->resultimg[y][x * 3 + 2]));
		}
	}

	if (pDoc->inputdata2 != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y, RGB(pDoc->inputdata2[y][x], pDoc->inputdata2[y][x], pDoc->inputdata2[y][x]));
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y, RGB(pDoc->inputdata2[y][x * 3 + 0], pDoc->inputdata2[y][x * 3 + 1], pDoc->inputdata2[y][x * 3 + 2]));
		}
	}

	if (pDoc->gResultimg != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
					pDC->SetPixel(x, pDoc->imageWidth + 20 + y, RGB(pDoc->gResultimg[y][x], pDoc->gResultimg[y][x], pDoc->gResultimg[y][x]));
		}
		else {
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
					pDC->SetPixel( x, pDoc->gimageWidth + 20 + y, RGB(pDoc->gResultimg[y][x * 3 + 0], pDoc->gResultimg[y][x * 3 + 1], pDoc->gResultimg[y][x * 3 + 2]));
		}
	}

}

void Cimageprog20161208View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = 2048;
	sizeTotal.cy = 2048;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// Cimageprog20161208View 진단

#ifdef _DEBUG
void Cimageprog20161208View::AssertValid() const
{
	CScrollView::AssertValid();
}

void Cimageprog20161208View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

Cimageprog20161208Doc* Cimageprog20161208View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cimageprog20161208Doc)));
	return (Cimageprog20161208Doc*)m_pDocument;
}
#endif //_DEBUG


// Cimageprog20161208View 메시지 처리기

void Cimageprog20161208View::OnPixelAdd2()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	if (pDoc->inputdata == NULL)
		return;

	int x, y, value;

	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageHeight; x++) {
			if (pDoc->depth == 1) {

				value = pDoc->inputdata[y][x] + 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x] = value;
			}
			else {

				value = pDoc->inputdata[y][x * 3 + 0] + 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 0] = value;

				value = pDoc->inputdata[y][x * 3 + 1] + 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 1] = value;

				value = pDoc->inputdata[y][x * 3 + 2] + 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 2] = value;

			}

		}
		Invalidate();
	}

}




void Cimageprog20161208View::OnRegionSharpenining()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	
	float kernel[3][3] = { { 0, -1,  0},
							{-1,  5, -1},
							{ 0, -1,  0} };

	convolve(pDoc->inputdata, pDoc->resultimg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}
void Cimageprog20161208View::OnRegionSmoothing()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	float kernel[3][3] = { {1.0 / 9.0,1.0 / 9.0,1.0 / 9.0},
							{1.0 / 9.0,1.0 / 9.0,1.0 / 9.0},
							{1.0 / 9.0,1.0 / 9.0,1.0 / 9.0} };

	convolve(pDoc->inputdata, pDoc->resultimg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();

}


void Cimageprog20161208View::OnRegionEmbossing()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	float kernel[3][3] = { {-1, 0, 0}, 
							{0, 0, 0}, 
							{0, 0, 1} };

	convolve(pDoc->inputdata, pDoc->resultimg, pDoc->imageWidth, pDoc->imageHeight, kernel, 128, pDoc->depth);

	Invalidate();
}



void Cimageprog20161208View::convolve(unsigned char** indata, unsigned char** outdata, int clos, int rows, float mask[][3], int bias, int depth)
{
	int x, y;
	int i, j;
	int sum, rsum, gsum, bsum;

	for (y = 1; y < rows-1; y++) {
		for (x = 1; x < clos-1; x++) {

			if (depth == 1) {
				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++) {
						sum += (indata[y + j - 1][x + i - 1] * mask[j][i]);
					}
				sum += bias;
				if (sum > 255)
					sum = 255;
				else if (sum < 0)
					sum = 0;

				outdata[y][x] = sum;
			}
			else {
				rsum = 0;
				gsum = 0;
				bsum = 0;

				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++) {
						rsum += (indata[y + j - 1][3 * (x + i - 1) + 0] * mask[j][i]);
						gsum += (indata[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (indata[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);
					}

				rsum += bias;
				gsum += bias;
				bsum += bias;

				if (rsum > 255)
					rsum = 255;
				else if (rsum < 0)
					rsum = 0;
				if (gsum > 255)
					gsum = 255;
				else if (gsum < 0)
					gsum = 0;
				if (bsum > 255)
					bsum = 255;
				else if (bsum < 0)
					bsum = 0;

				outdata[y][3 * x + 0] = rsum;
				outdata[y][3 * x + 1] = gsum;
				outdata[y][3 * x + 2] = bsum;
			}
		}
	}

}


void Cimageprog20161208View::OnRegionPrewitt()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	float kernel_h[3][3] = { { -1, -1,  -1},
							 {  0,  0,   0},
							 {  1,   1,   1} };
	float kernel_v[3][3] = { { -1,  0,  1},
								{ -1,  0,  1},
								{ -1,  0,  1} };

	int x, y, i;
	int value, rvalue, gvalue, bvalue;

	unsigned char** Er, ** Ec;
	// 메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++)
	{
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	convolve(pDoc->inputdata, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputdata, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)		value = 255;
				else if (value < 0)	value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else
			{
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
				if (value > 255)		value = 255;
				else if (value < 0)	value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}

	//메모리 삭제
	for (i = 0; i < pDoc->imageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimageprog20161208View::OnRegionRobert()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	float kernel_h[3][3] = {{-1, 0, 0},
							 {0, 1, 0},
							 {0, 0, 0} };

	float kernel_v[3][3] = { {0, 0, -1},
							 {0, 1, 0},
							 {0, 0, 0} };

	int x, y;
	int value, rvalue, gvalue, bvalue;

	//메모리 할당
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}


	convolve(pDoc->inputdata, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputdata, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageHeight; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255) {
					value = 255;
				}
				else if (value < 0) {
					value = 0;
				}
				pDoc->resultimg[y][x] = value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
				if (value > 255) {
					value = 255;
				}
				else if (value < 0) {
					value = 0;
				}
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	//메모리 삭제
	for (int i = 0; i < pDoc->imageHeight; i++) {
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimageprog20161208View::OnRegionSobel()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	float kernel_h[3][3] = { {-1, -2, -1},
							 {0, 0, 0},
							 {1, 2, 1} };

	float kernel_v[3][3] = { {1, 0, -1},
							 {2, 0, -2},
							 {1, 0, -1} };

	int x, y;
	int value, rvalue, gvalue, bvalue;

	//메모리 할당
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}


	convolve(pDoc->inputdata, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputdata, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageHeight; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255) {
					value = 255;
				}
				else if (value < 0) {
					value = 0;
				}
				pDoc->resultimg[y][x] = value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
				if (value > 255) {
					value = 255;
				}
				else if (value < 0) {
					value = 0;
				}
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	//메모리 삭제
	for (int i = 0; i < pDoc->imageHeight; i++) {
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void Cimageprog20161208View::OnRegionAverageFilltering()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	int x, y, i, j;
	int xpos, ypos;
	int sum, rsum, gsum, bsum;
	int pixelcount;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			sum = 0;		pixelcount = 0;		rsum = gsum = bsum = 0;
			for (j = -2; j <= 2; j++)
				for (i = -2; i <= 2; i++)
				{
					xpos = x + i;
					ypos = y + j;
					if (xpos >= 0 && xpos <= pDoc->imageWidth - 1 && ypos >= 0 && ypos <= pDoc->imageHeight - 1)
					{
						if (pDoc->depth == 1)
							sum += pDoc->inputdata[y + j][x + i];
						else
						{
							rsum += pDoc->inputdata[y + j][3 * (x + i) + 0];
							gsum += pDoc->inputdata[y + j][3 * (x + i) + 1];
							bsum += pDoc->inputdata[y + j][3 * (x + i) + 2];
						}
						pixelcount++;
					}
				}

			if (pDoc->depth == 1)
				pDoc->resultimg[y][x] = sum / pixelcount;
			else
			{
				pDoc->resultimg[y][3 * x + 0] = rsum / pixelcount;
				pDoc->resultimg[y][3 * x + 1] = gsum / pixelcount;
				pDoc->resultimg[y][3 * x + 2] = bsum / pixelcount;
			}
		}

	Invalidate();
}


void Cimageprog20161208View::OnRegionMedianFiltering()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	int x, y, i, j;
	int n[9], temp;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			n[0] = pDoc->inputdata[y - 1][x - 1];
			n[1] = pDoc->inputdata[y - 1][x - 0];
			n[2] = pDoc->inputdata[y - 1][x + 1];
			n[3] = pDoc->inputdata[y - 0][x - 1];
			n[4] = pDoc->inputdata[y - 0][x - 0];
			n[5] = pDoc->inputdata[y - 0][x + 1];
			n[6] = pDoc->inputdata[y + 1][x - 1];
			n[7] = pDoc->inputdata[y + 1][x - 0];
			n[8] = pDoc->inputdata[y + 1][x + 1];

			// 버블소팅(오름차순)
			for (i = 8; i > 0; i--)
				for (j = 0; j < i; j++)
				{
					if (n[j] > n[j + 1])
					{
						temp = n[j + 1];
						n[j + 1] = n[j];
						n[j] = temp;
					}
				}

			pDoc->resultimg[y][x] = n[4];
		}
	Invalidate();

}


void Cimageprog20161208View::OnPixelMul()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;
	int value;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			value = pDoc->inputdata[y][x] * 1.5;
			if (value > 255)	value = 255;
			else if (value < 0)	value = 0;
			pDoc->resultimg[y][x] = value;
		}

	Invalidate();
}


void Cimageprog20161208View::OnPixelHistoEq()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y, k;
	int hist[256], sum[256];
	int acc_hist = 0;
	int N = 256 * 256;

	for (k = 0; k < 256; k++)	hist[k] = 0;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
			hist[pDoc->inputdata[y][x]]++;

	for (k = 0; k < 256; k++)
	{
		acc_hist += hist[k];
		sum[k] = acc_hist;
	}

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			k = pDoc->inputdata[y][x];
			pDoc->resultimg[y][x] = (float)sum[k] / N * 255;
		}

	Invalidate();
}


void Cimageprog20161208View::OnPixelContrastStretching()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;
	int low = 255;
	int high = 0;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->inputdata[x][y] < low)
				low = pDoc->inputdata[x][y];
		}

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->inputdata[x][y] > high)
				high = pDoc->inputdata[x][y];
		}

	float mult = 255.0f / (float)(high - low);
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			pDoc->resultimg[x][y] = (unsigned char)((pDoc->inputdata[x][y] - low) * mult);
		}
	Invalidate();
}


void Cimageprog20161208View::OnPixelBinarization()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;
	int threshold = 150;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			if (pDoc->inputdata[y][x] >= threshold)	pDoc->resultimg[y][x] = 255;
			else	
				pDoc->resultimg[y][x] = 0;
		}
	Invalidate();
}


void Cimageprog20161208View::OnPixelTwoImageAdd()
{
	Cimageprog20161208Doc* pDoc = GetDocument();


	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK)
	{
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					pDoc->resultimg[y][x] = 0.5 * pDoc->inputdata[y][x] + 0.5 * pDoc->inputdata2[y][x];
				}
				else
				{
					pDoc->resultimg[y][3 * x + 0] = 0.5 * pDoc->inputdata[y][3 * x + 0] + 0.5 * pDoc->inputdata2[y][3 * x + 0];
					pDoc->resultimg[y][3 * x + 1] = 0.5 * pDoc->inputdata[y][3 * x + 1] + 0.5 * pDoc->inputdata2[y][3 * x + 1];
					pDoc->resultimg[y][3 * x + 2] = 0.5 * pDoc->inputdata[y][3 * x + 2] + 0.5 * pDoc->inputdata2[y][3 * x + 2];
				}
			}

		Invalidate();
	}
}


void Cimageprog20161208View::OnPixelTwoImageSub()
{
	Cimageprog20161208Doc* pDoc = GetDocument();


	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK)
	{
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					pDoc->resultimg[y][x] = 0.5 * pDoc->inputdata[y][x] - 0.5 * pDoc->inputdata2[y][x];
				}
				else
				{
					pDoc->resultimg[y][3 * x + 0] = 0.5 * pDoc->inputdata[y][3 * x + 0] - 0.5 * pDoc->inputdata2[y][3 * x + 0];
					pDoc->resultimg[y][3 * x + 1] = 0.5 * pDoc->inputdata[y][3 * x + 1] - 0.5 * pDoc->inputdata2[y][3 * x + 1];
					pDoc->resultimg[y][3 * x + 2] = 0.5 * pDoc->inputdata[y][3 * x + 2] - 0.5 * pDoc->inputdata2[y][3 * x + 2];
				}
			}

		Invalidate();
	}
}


void Cimageprog20161208View::LoadTwoImage()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CFile file;
		file.Open(dlg.GetPathName(), CFile::modeRead);		// 화일열기
		CArchive ar(&file, CArchive::load);
		pDoc->LoadImageFile(ar);

		file.Close();
	}
}


void Cimageprog20161208View::OnMopologyColorToGray()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	if (pDoc->depth == 1)
		return;
	int x, y;
	int gray;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			gray = (pDoc->inputdata[y][3 * x + 0] + pDoc->inputdata[y][3 * x + 1] + pDoc->inputdata[y][3 * x + 2]) / 3;
			pDoc->inputdata[y][3 * x + 0] = gray;
			pDoc->inputdata[y][3 * x + 1] = gray;
			pDoc->inputdata[y][3 * x + 2] = gray;
		}

	Invalidate();
}

void Cimageprog20161208View::OnMopologyBinarization()
{
	int x, y;
	int gray;
	int threshold = 100;

	Cimageprog20161208Doc* pDoc = GetDocument();
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			if (pDoc->depth == 1) {
				if (pDoc->inputdata[y][x] > threshold)
					pDoc->inputdata[y][x] = 255;
				else
					pDoc->inputdata[y][x] = 0;
			}
			else {
				if ((pDoc->inputdata[y][3 * x + 0] + pDoc->inputdata[y][3 * x + 1] + pDoc->inputdata[y][3 * x + 2]) / 3 > threshold) {
					pDoc->inputdata[y][3 * x + 0] = 255;
					pDoc->inputdata[y][3 * x + 1] = 255;
					pDoc->inputdata[y][3 * x + 2] = 255;
				}
				else {
					pDoc->inputdata[y][3 * x + 0] = 0;
					pDoc->inputdata[y][3 * x + 1] = 0;
					pDoc->inputdata[y][3 * x + 2] = 0;
				}
			}
		}
	Invalidate();
}


void Cimageprog20161208View::OnErosion()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	
	int x, y;
	int min = 255;
	int rmin, gmin, bmin;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++) {
			min = 255;
			rmin = gmin = bmin = 255;
			for (int j = -1; j < 1; j++)
				for (int i = -1; i < 1; i++) {
					if (pDoc->depth == 1){
						if (pDoc->inputdata[y + j][x + i] < min)
							min = pDoc->inputdata[y + j][x + i];
					}
					else {
						if (pDoc->inputdata[y + j][3 * (x + i) + 0] < min)
							rmin = pDoc->inputdata[y + j][3 * (x + i) + 0];
						if (pDoc->inputdata[y + j][3 * (x + i) + 1] < min)
							gmin = pDoc->inputdata[y + j][3 * (x + i) + 1];
						if (pDoc->inputdata[y + j][3 * (x + i) + 2] < min)
							bmin = pDoc->inputdata[y + j][3 * (x + i) + 2];
					}
				}
			if (pDoc->depth == 1)
			pDoc->resultimg[y][x] = min;
			else {
				pDoc->resultimg[y][3 * x + 0] = rmin;
				pDoc->resultimg[y][3 * x + 1] = gmin;
				pDoc->resultimg[y][3 * x + 2] = bmin;
			}
		}
	Invalidate();
}


void Cimageprog20161208View::OnMopologyDilation()
{
	Cimageprog20161208Doc* pDoc = GetDocument();


	int x, y;
	int max;
	int rmax, gmax, bmax;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++) {
			max = 0;
			rmax = gmax = bmax = 0;
			for (int j = -1; j < 1; j++)
				for (int i = -1; i < 1; i++) {
					if (pDoc->depth == 1) {
						if (pDoc->inputdata[y + j][x + i] > max)
							max = pDoc->inputdata[y + j][x + i];
					}
					else {
						if (pDoc->inputdata[y + j][3 * (x + i) + 0] > max)
							rmax = pDoc->inputdata[y + j][3 * (x + i) + 0];
						if (pDoc->inputdata[y + j][3 * (x + i) + 1] > max)
							gmax = pDoc->inputdata[y + j][3 * (x + i) + 1];
						if (pDoc->inputdata[y + j][3 * (x + i) + 2] > max)
							bmax = pDoc->inputdata[y + j][3 * (x + i) + 2];
					}
				}
			if (pDoc->depth == 1)
				pDoc->resultimg[y][x] = max;
			else {
				pDoc->resultimg[y][3 * x + 0] = rmax;
				pDoc->resultimg[y][3 * x + 1] = gmax;
				pDoc->resultimg[y][3 * x + 2] = bmax;
			}
		}
	Invalidate();
}


void Cimageprog20161208View::OnMopologyOpening()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	OnErosion();
	CopyResultToInput();
	OnErosion();
	CopyResultToInput();
	OnErosion();

	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();

}


void Cimageprog20161208View::CopyResultToInput()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++)
		{
			pDoc->inputdata[y][x] = pDoc->resultimg[y][x];
		}
}


void Cimageprog20161208View::OnMopologyCloseing()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();

	CopyResultToInput();
	OnErosion();
	CopyResultToInput();
	OnErosion();
	CopyResultToInput();
	OnErosion();
}


void Cimageprog20161208View::OnGiometryZoominPixelCopy()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	int x, y, i;

	int xscale = 3;
	int yscale = 2;

	if (pDoc->gResultimg != NULL)
	{
		for (i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gResultimg[i]);
		free(pDoc->gResultimg);
	}

	pDoc->gimageWidth = pDoc->imageWidth * xscale;
	pDoc->gimageHeight = pDoc->imageHeight * yscale;
	// 메모리 할당
	pDoc->gResultimg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gimageHeight; i++)
	{
		pDoc->gResultimg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}
	/*
		// 전방향 사상
		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				pDoc->gResultImg[y* yscale][x* xscale] = pDoc->inputImg[y][x];
			}
	*/

	// 역방향 사상
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc->gimageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gResultimg[y][x] = pDoc->inputdata[y / yscale][x / xscale];
			else
			{
				pDoc->gResultimg[y][3 * x + 0] = pDoc->inputdata[y / yscale][3 * (x / xscale) + 0];
				pDoc->gResultimg[y][3 * x + 1] = pDoc->inputdata[y / yscale][3 * (x / xscale) + 1];
				pDoc->gResultimg[y][3 * x + 2] = pDoc->inputdata[y / yscale][3 * (x / xscale) + 2];
			}
		}

	Invalidate();
}


void Cimageprog20161208View::OnGeometryZoominInterpolation()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y, i;

	float xscale = 2.1;
	float yscale = 1.5;
	float src_x, src_y;
	float alpha, beta;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	int E, F;

	if (pDoc->gResultimg != NULL)
	{
		for (i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gResultimg[i]);
		free(pDoc->gResultimg);
	}

	pDoc->gimageWidth = (pDoc->imageWidth) * xscale;
	pDoc->gimageHeight = (pDoc->imageHeight) * yscale;
	// 메모리 할당
	pDoc->gResultimg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gimageHeight; i++)
	{
		pDoc->gResultimg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	// 역방향 사상
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc->gimageWidth; x++)
		{
			src_x = x / xscale;
			src_y = y / yscale;
			alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;

			Ax = (int)src_x;
			Ay = (int)src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > pDoc->imageWidth - 1)		Bx = pDoc->imageWidth - 1;
			if (Cy > pDoc->imageHeight - 1)		Cy = pDoc->imageHeight - 1;
			if (Dx > pDoc->imageWidth - 1)		Dx = pDoc->imageWidth - 1;
			if (Dy > pDoc->imageHeight - 1)		Dy = pDoc->imageHeight - 1;

			if (pDoc->depth == 1)
			{
				E = (1 - alpha) * pDoc->inputdata[Ay][Ax] + alpha * pDoc->inputdata[By][Bx];
				F = (1 - alpha) * pDoc->inputdata[Cy][Cx] + alpha * pDoc->inputdata[Dy][Dx];

				pDoc->gResultimg[y][x] = (1 - beta) * E + beta * F;
			}
			else
			{
				E = (1 - alpha) * pDoc->inputdata[Ay][3 * Ax + 0] + alpha * pDoc->inputdata[By][3 * Bx + 0];
				F = (1 - alpha) * pDoc->inputdata[Cy][3 * Cx + 0] + alpha * pDoc->inputdata[Dy][3 * Dx + 0];
				pDoc->gResultimg[y][3 * x + 0] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputdata[Ay][3 * Ax + 1] + alpha * pDoc->inputdata[By][3 * Bx + 1];
				F = (1 - alpha) * pDoc->inputdata[Cy][3 * Cx + 1] + alpha * pDoc->inputdata[Dy][3 * Dx + 1];
				pDoc->gResultimg[y][3 * x + 1] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputdata[Ay][3 * Ax + 2] + alpha * pDoc->inputdata[By][3 * Bx + 2];
				F = (1 - alpha) * pDoc->inputdata[Cy][3 * Cx + 2] + alpha * pDoc->inputdata[Dy][3 * Dx + 2];
				pDoc->gResultimg[y][3 * x + 2] = (1 - beta) * E + beta * F;
			}
		}

	Invalidate();
}


void Cimageprog20161208View::OnGeometryZoomoutSubsampling()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y, i;

	int xscale = 3;		//1/3
	int yscale = 2;		//1/2

	if (pDoc->gResultimg != NULL)
	{
		for (i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gResultimg[i]);
		free(pDoc->gResultimg);
	}

	pDoc->gimageWidth = (pDoc->imageWidth) / xscale;
	pDoc->gimageHeight = (pDoc->imageHeight) / yscale;
	// 메모리 할당
	pDoc->gResultimg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gimageHeight; i++)
	{
		pDoc->gResultimg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	// 역방향 사상
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc->gimageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gResultimg[y][x] = pDoc->inputdata[y * yscale][x * xscale];
			else
			{
				pDoc->gResultimg[y][3 * x + 0] = pDoc->inputdata[y * yscale][3 * (x * xscale) + 0];
				pDoc->gResultimg[y][3 * x + 1] = pDoc->inputdata[y * yscale][3 * (x * xscale) + 1];
				pDoc->gResultimg[y][3 * x + 2] = pDoc->inputdata[y * yscale][3 * (x * xscale) + 2];
			}
		}
	Invalidate();
}


void Cimageprog20161208View::OnGeometryMirror()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;

	for (int y = 0; y < pDoc->imageHeight; y++)
		for (int x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {

			if(pDoc->depth == 1)
				pDoc->resultimg[y][x] = pDoc->inputdata[y][pDoc->imageWidth - 1 - x];
			else {
				pDoc->resultimg[y][3 * x + 0] = pDoc->inputdata[y][3 * (pDoc->imageWidth - 1 - x) + 0];
				pDoc->resultimg[y][3 * x + 1] = pDoc->inputdata[y][3 * (pDoc->imageWidth - 1 - x) + 1];
				pDoc->resultimg[y][3 * x + 2] = pDoc->inputdata[y][3 * (pDoc->imageWidth - 1 - x) + 2];
			}
		}
	Invalidate();
}


void Cimageprog20161208View::OnGeometryFlip()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;

	for (int y = 0; y < pDoc->imageHeight; y++)
		for (int x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			if(pDoc->depth == 1)
				pDoc->resultimg[pDoc->imageHeight-1-y][x] = pDoc->inputdata[y][x];
			else {
				pDoc->resultimg[pDoc->imageHeight - 1 - y][3 * x + 0] = pDoc->inputdata[y][3 * x + 0];
				pDoc->resultimg[pDoc->imageHeight - 1 - y][3 * x + 1] = pDoc->inputdata[y][3 * x + 1];
				pDoc->resultimg[pDoc->imageHeight - 1 - y][3 * x + 2] = pDoc->inputdata[y][3 * x + 2];
			}
		}
	Invalidate();
}

typedef struct {
	int Px;
	int Py;
	int Qx;
	int Qy;
}control_line;

control_line mctrl_source = { 100,100,150,150 };
control_line mctrl_dest = { 100,100,200,200 };

void Cimageprog20161208View::OnGeometryWarping()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	control_line source_lines[5] = { {100,100,150,150},
		{0,0,pDoc->imageWidth - 1,0} ,{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1} ,
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1} ,{0,pDoc->imageHeight - 1,0,0} };
	control_line dest_lines[5] = { {100,100,200,200},
		{0,0,pDoc->imageWidth - 1,0} ,{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1} ,
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1} ,{0,pDoc->imageHeight - 1,0,0} };

	source_lines[0] = mctrl_source;
	dest_lines[0] = mctrl_dest;

	int x, y;

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalweight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length;

	int num_lines = 5;
	int line;
	int source_x, source_y;
	int last_row, last_col;

	last_row = pDoc->imageHeight - 1;
	last_col = pDoc->imageWidth - 1;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			tx = 0.0;
			ty = 0.0;
			totalweight = 0.0;
			//각제어선의 영향을 계산
			for (line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0)		d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1)	d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
				else				d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;

				src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) +
					(src_y2 - src_y1) * (src_y2 - src_y1));

				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

				weight = pow(pow(dest_line_length, p) / (a + d), b);

				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalweight += weight;
			}
			source_x = x + (tx / totalweight);
			source_y = y + (ty / totalweight);

			if (source_x < 0)		source_x = 0;
			if (source_x > last_col)	source_x = last_col;
			if (source_y < 0)		source_y = 0;
			if (source_y > last_row)	source_y = last_row;

			if (pDoc->depth == 1)
				pDoc->resultimg[y][x] = pDoc->inputdata[source_y][source_x];
			else
			{
				pDoc->resultimg[y][3 * x + 0] = pDoc->inputdata[source_y][3 * source_x + 0];
				pDoc->resultimg[y][3 * x + 1] = pDoc->inputdata[source_y][3 * source_x + 1];
				pDoc->resultimg[y][3 * x + 2] = pDoc->inputdata[source_y][3 * source_x + 2];
			}
		}
	Invalidate();
}

CPoint mpos_st, mpos_end;

void Cimageprog20161208View::OnLButtonDown(UINT nFlags, CPoint point)
{
	mpos_st = point;

	CScrollView::OnLButtonDown(nFlags, point);
}

//교재 12장 내용
void Cimageprog20161208View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mpos_end = point;

	CDC* pDC = GetDC();
	CPen rpen;
	rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	pDC->SelectObject(&rpen);

	pDC->MoveTo(mpos_st);
	pDC->LineTo(mpos_end);

	ReleaseDC(pDC);

	int Ax, Ay, Bx, By;
	Ax = mpos_st.x;
	Ay = mpos_st.y;
	Bx = mpos_end.x;
	By = mpos_end.y;

	if (Ax < Bx)
		mctrl_source.Px = Ax - (Bx - Ax) / 2;
	else
		mctrl_source.Px = Ax + (Ax - Bx) / 2;

	if (Ay < By)
		mctrl_source.Py = Ay - (By - Ay) / 2;
	else
		mctrl_source.Py = Ay + (Ay - By) / 2;

	mctrl_dest.Px = mctrl_source.Px;
	mctrl_dest.Py = mctrl_source.Py;

	mctrl_source.Qx = mpos_st.x;
	mctrl_source.Qy = mpos_st.y;
	mctrl_dest.Qx = mpos_end.x;
	mctrl_dest.Qy = mpos_end.y;

	CScrollView::OnLButtonUp(nFlags, point);
}


void Cimageprog20161208View::OnAviView()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"AVI화일(*.avi)|*.avi|모든화일|*.*");

	if (dlg.DoModal() == IDOK); {
		AVIFileName = dlg.GetPathName();
		bAviMode = true;
		Invalidate();
	}

	
}


void Cimageprog20161208View::LoadAviFile(CDC* pDC)
{
	PAVIFILE pavi;
	AVIFILEINFO fi;
	int stm;
	PAVISTREAM pstm = NULL;
	AVISTREAMINFO si;
	PGETFRAME pfrm = NULL;
	int frame;
	LPBITMAPINFOHEADER pbmpih;
	unsigned char* image;
	int x, y;

	AVIFileInit();
	AVIFileOpen(&pavi, AVIFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

	for (stm = 0; stm < fi.dwStreams; stm++) {
		AVIFileGetStream(pavi, &pstm, 0, stm);
		AVIStreamInfo(pstm, &si, sizeof(si));

		if (si.fccType == streamtypeVIDEO) {
			pfrm = AVIStreamGetFrameOpen(pstm, NULL);
			for (frame = 0; frame < si.dwLength; frame++) {
				pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
				if (!pbmpih)
					continue;

				image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
				
				/*for (y = 0; y < fi.dwHeight; y++)
					for (x = 0; x < fi.dwWidth; x++) {
						pDC->SetPixel(x, fi.dwHeight - 1 - y,
							RGB(image[(y * fi.dwWidth + x) * 3 + 2],
								image[(y * fi.dwWidth + x) * 3 + 1],
								image[(y * fi.dwWidth + x) * 3 + 0]));
					}
					*/
				::SetDIBitsToDevice(pDC->GetSafeHdc(), 
									0, 0, fi.dwWidth, fi.dwHeight, 
									0, 0, 0, fi.dwWidth,
									image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
				Sleep(30);
			}
		}

	}


}


void Cimageprog20161208View::OnGeometryZoomoutMeanSub()
{
	OnRegionSmoothing();
	CopyResultToInput();
	OnGeometryZoomoutSubsampling();

}


void Cimageprog20161208View::OnGeometryAvgSampling()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	int x, y, i, j;

	int xscale = 3;		//1/3
	int yscale = 2;		//1/2
	int src_x, src_y;
	int sum, rsum, gsum, bsum;

	if (pDoc->gResultimg != NULL)
	{
		for (i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gResultimg[i]);
		free(pDoc->gResultimg);
	}

	pDoc->gimageWidth = (pDoc->imageWidth) / xscale;
	pDoc->gimageHeight = (pDoc->imageHeight) / yscale;
	// 메모리 할당
	pDoc->gResultimg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gimageHeight; i++)
	{
		pDoc->gResultimg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	// 전방향 사상
	for (y = 0; y < pDoc->imageHeight; y += yscale)
		for (x = 0; x < pDoc->imageWidth; x += xscale)
		{
			sum = 0;	rsum = 0; gsum = 0; bsum = 0;
			for (j = 0; j < yscale; j++)
				for (i = 0; i < xscale; i++)
				{
					src_x = x + i;
					src_y = y + j;
					if (src_x > pDoc->imageWidth - 1)		src_x = pDoc->imageWidth - 1;
					if (src_y > pDoc->imageHeight - 1)		src_y = pDoc->imageHeight - 1;

					if (pDoc->depth == 1)
						sum += pDoc->inputdata[src_y][src_x];
					else
					{
						rsum += pDoc->inputdata[src_y][3 * src_x + 0];
						gsum += pDoc->inputdata[src_y][3 * src_x + 1];
						bsum += pDoc->inputdata[src_y][3 * src_x + 2];
					}
				}
			if (pDoc->depth == 1)
				pDoc->gResultimg[y / yscale][x / xscale] = sum / (xscale * yscale);
			else
			{
				pDoc->gResultimg[y / yscale][3 * (x / xscale) + 0] = rsum / (xscale * yscale);
				pDoc->gResultimg[y / yscale][3 * (x / xscale) + 1] = gsum / (xscale * yscale);
				pDoc->gResultimg[y / yscale][3 * (x / xscale) + 2] = bsum / (xscale * yscale);
			}
		}
	Invalidate();
}

#define PI 3.1415926521

void Cimageprog20161208View::OnGeometryRotation()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	CAngleinputDialog dlg;

	int x, y, i, j;
	int angle = 30;		//degree
	float radian;
	int Cx, Cy, Oy;
	int x_source, y_source, xdiff, ydiff;

	dlg.m_iAngle = angle;
	if (dlg.DoModal() == IDCANCEL)		return;
	angle = dlg.m_iAngle;

	if (pDoc->gResultimg != NULL)
	{
		for (i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gResultimg[i]);
		free(pDoc->gResultimg);
	}

	radian = 2 * PI / 360 * angle;
	pDoc->gimageWidth = pDoc->imageHeight * fabs(cos(PI / 2 - radian)) + pDoc->imageWidth * fabs(cos(radian));
	pDoc->gimageHeight = pDoc->imageHeight * fabs(cos(radian)) + pDoc->imageWidth * fabs(cos(PI / 2 - radian));
	// 메모리 할당
	pDoc->gResultimg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gimageHeight; i++)
	{
		pDoc->gResultimg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	// 중심점
	Cx = pDoc->imageWidth / 2;	Cy = pDoc->imageHeight / 2;
	//y의 마지막 좌표
	Oy = pDoc->imageHeight - 1;
	xdiff = (pDoc->gimageWidth - pDoc->imageWidth) / 2;
	ydiff = (pDoc->gimageHeight - pDoc->imageHeight) / 2;

	for (y = -ydiff; y < pDoc->gimageHeight - ydiff; y++)
		for (x = -xdiff; x < pDoc->gimageWidth - xdiff; x++)
		{
			x_source = (Oy - y - Cy) * sin(radian) + (x - Cx) * cos(radian) + Cx;
			y_source = Oy - ((Oy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

			if (pDoc->depth == 1)
			{
				if (x_source<0 || x_source>pDoc->imageWidth - 1 ||
					y_source<0 || y_source>pDoc->imageHeight - 1)
					pDoc->gResultimg[y + ydiff][x + xdiff] = 255;
				else
					pDoc->gResultimg[y + ydiff][x + xdiff] = pDoc->inputdata[y_source][x_source];
			}
			else
			{
				if (x_source<0 || x_source>pDoc->imageWidth - 1 ||
					y_source<0 || y_source>pDoc->imageHeight - 1)
				{
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 0] = 255;
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 1] = 255;
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 2] = 255;
				}
				else
				{
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->inputdata[y_source][3 * x_source + 0];
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->inputdata[y_source][3 * x_source + 1];
					pDoc->gResultimg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->inputdata[y_source][3 * x_source + 2];
				}
			}

		}
	Invalidate();
}


void Cimageprog20161208View::OnPixelSub()
{
	Cimageprog20161208Doc* pDoc = GetDocument();
	if (pDoc->inputdata == NULL)
		return;

	int x, y, value;

	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageHeight; x++) {
			if (pDoc->depth == 1) {

				value = pDoc->inputdata[y][x] - 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x] = value;
			}
			else {

				value = pDoc->inputdata[y][x * 3 + 0] - 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 0] = value;

				value = pDoc->inputdata[y][x * 3 + 1] - 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 1] = value;

				value = pDoc->inputdata[y][x * 3 + 2] - 100;
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				pDoc->resultimg[y][x * 3 + 2] = value;

			}

		}
		Invalidate();
	}

}


void Cimageprog20161208View::OnPixelDiv()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	int x, y;
	int value;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
		{
			value = pDoc->inputdata[y][x] / 1.5;
			if (value > 255)	value = 255;
			else if (value < 0)	value = 0;
			pDoc->resultimg[y][x] = value;
		}

	Invalidate();
}

#define NUM_FRAMES 10

void Cimageprog20161208View::OnGeometryMorphing()
{
	Cimageprog20161208Doc* pDoc = GetDocument();

	unsigned char** morphedImg[10];

	control_line source_lines[23] = {
		{116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
		{123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
		{78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
		{108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
		{80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
		{114,225,186,257},{109,258,143,277},{152,278,190,262}
	};

	control_line dest_lines[23] = {
		{120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
		{142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
		{96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
		{131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
		{91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
		{121,280,205,284},{112,286,160,301},{166,301,214,287}
	};

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;

	double weight;
	double totalWeight;
	double a = 0.001;
	double b = 2.0;
	double p = 0.75;

	unsigned char** warpedImg;
	unsigned char** warpedImg2;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2;
	double xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2;
	int source_x2, source_y2;

	int x1, x2, y1, y2;
	int src_x1, src_y1, src_x2, src_y2;
	double src_lines_length, dest_line_length;

	int i, j;
	int num_lines = 23;
	int line;
	int x, y;
	int source_x, source_y;
	int last_row, last_col;

	warpedImg = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++) {
		warpedImg[i] = (unsigned char*)malloc(pDoc->imageHeight * pDoc->depth);
	}

	warpedImg2 = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++) {
		warpedImg2[i] = (unsigned char*)malloc(pDoc->imageHeight * pDoc->depth);
	}

	for (i = 0; i < NUM_FRAMES; i++) {
		morphedImg[i] = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		for (j = 0; j < pDoc->imageHeight; j++) {
			morphedImg[i][j] = (unsigned char*)malloc(pDoc->imageHeight * pDoc->depth);
		}
	}

	last_row = pDoc->imageHeight - 1;
	last_col = pDoc->imageWidth - 1;

	for (frame = 1; frame <= NUM_FRAMES; frame++) {
		fweight = (double)(frame) / NUM_FRAMES;
		
		for (line = 0; line < num_lines; line++) {
			warp_lines[line].Px = source_lines[line].Px + (dest_lines[line].Px - source_lines[line].Px) * fweight;
			warp_lines[line].Py = source_lines[line].Py + (dest_lines[line].Py - source_lines[line].Py) * fweight;
			warp_lines[line].Qx = source_lines[line].Qx + (dest_lines[line].Qx - source_lines[line].Qx) * fweight;
			warp_lines[line].Qy = source_lines[line].Qy + (dest_lines[line].Qy - source_lines[line].Qy) * fweight;
		}

		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageWidth; x++) {
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;

				for (line = 0; line < num_lines; line++) {
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;

					dest_line_length = sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / (double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0)
						d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
					else if (u > 1)
						d = sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2)));
					else
						d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_lines_length = sqrt((double)((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1)));

					dest_x1 = dest_lines[line].Px;
					dest_y1 = dest_lines[line].Py;
					dest_x2 = dest_lines[line].Qx;
					dest_y2 = dest_lines[line].Qy;
					dest_line_length = sqrt((double)((dest_x2 - dest_x1) * (dest_x2 - dest_x1) + (dest_y2 - dest_y1) * (dest_y2 - dest_y1)));

					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_lines_length;
					yp = src_y1 + u * (src_y2 - src_y1) - h * (src_x2 - src_x1) / src_lines_length;
					xp2 = dest_x1 + u * (dest_x2 - dest_x1) - h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) - h * (dest_x2 - dest_x1) / dest_line_length;

					weight = pow((pow((double)(dest_line_length), p) / (a + b)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}

				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);
				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				if (source_x < 0)
					source_x = 0;
				if (source_x > last_col)
					source_x = last_col;
				if (source_y < 0)
					source_y = 0;
				if (source_y > last_row)
					source_y = last_row;
				if (source_x2 < 0)
					source_x2 = 0;
				if (source_x2 > last_col)
					source_x2 = last_col;
				if (source_y2 < 0)
					source_y2 = 0;
				if (source_y2 > last_row)
					source_y2 = last_row;

				warpedImg[y][x] = pDoc->inputdata[source_y][source_x];
				warpedImg2[y][x] = pDoc->inputdata2[source_y2][source_x2];
			}
		}

		for(y=0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] + fweight * warpedImg2[y][x]);

				if (val < 0)
					val = 0;
				if (val > 255)
					val = 255;

				morphedImg[frame - 1][y][x] = val;
			}
	}

	Invalidate();
}
