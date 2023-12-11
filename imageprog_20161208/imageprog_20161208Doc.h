
// imageprog_20161208Doc.h: Cimageprog20161208Doc 클래스의 인터페이스
//


#pragma once


class Cimageprog20161208Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	Cimageprog20161208Doc() noexcept;
	DECLARE_DYNCREATE(Cimageprog20161208Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~Cimageprog20161208Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	unsigned char** inputdata;	//[y],[x]
	unsigned char** inputdata2;	//[y],[x]
	unsigned char** resultimg;	//[y],[x]
	
	int imageWidth;
	int imageHeight;
	int depth;			// 흑백 = 1, 컬러 = 3
	
	int gimageWidth;
	int gimageHeight;

	unsigned char** gResultimg;	//[y],[x]


	int LoadImageFile(CArchive& ar);
	int LoadSecondImageFile(CArchive& ar);
};
