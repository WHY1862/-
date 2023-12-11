
// imageprog_20161208View.h: Cimageprog20161208View 클래스의 인터페이스
//

#pragma once


class Cimageprog20161208View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	Cimageprog20161208View() noexcept;
	DECLARE_DYNCREATE(Cimageprog20161208View)

// 특성입니다.
public:
	Cimageprog20161208Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~Cimageprog20161208View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRegionSharpenining();
	void convolve(unsigned char** indata, unsigned char** outdata, int clos, int rows, float mask[][3], int bias, int depth);
	afx_msg void OnRegionSmoothing();
	afx_msg void OnRegionEmbossing();
	afx_msg void OnRegionPrewitt();
	afx_msg void OnRegionRobert();
	afx_msg void OnRegionSobel();
	afx_msg void OnRegionAverageFilltering();
	afx_msg void OnRegionMedianFiltering();
	afx_msg void OnPixelAdd2();
	afx_msg void OnPixelMul();
	afx_msg void OnPixelHistoEq();
	afx_msg void OnPixelContrastStretching();
	afx_msg void OnPixelBinarization();
	afx_msg void OnPixelTwoImageAdd();
	afx_msg void OnPixelTwoImageSub();
	void LoadTwoImage();
	afx_msg void OnErosion();
	afx_msg void OnMopologyColorToGray();
	afx_msg void OnMopologyBinarization();
	afx_msg void OnMopologyDilation();
	afx_msg void OnMopologyOpening();
	void CopyResultToInput();
	afx_msg void OnMopologyCloseing();
	afx_msg void OnGiometryZoominPixelCopy();
	afx_msg void OnGeometryZoominInterpolation();
	afx_msg void OnGeometryZoomoutSubsampling();
	afx_msg void OnGeometryMirror();
	afx_msg void OnGeometryFlip();
	afx_msg void OnGeometryWarping();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAviView();
	bool bAviMode;
	CString AVIFileName;
	void LoadAviFile(CDC* pDC);
	afx_msg void OnGeometryZoomoutMeanSub();
	afx_msg void OnGeometryAvgSampling();
	afx_msg void OnGeometryRotation();
	afx_msg void OnPixelSub();
	afx_msg void OnPixelDiv();
	afx_msg void OnGeometryMorphing();
};

#ifndef _DEBUG  // imageprog_20161208View.cpp의 디버그 버전
inline Cimageprog20161208Doc* Cimageprog20161208View::GetDocument() const
   { return reinterpret_cast<Cimageprog20161208Doc*>(m_pDocument); }
#endif

