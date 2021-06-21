#pragma once
#include "dbmain.h"

class __declspec(dllexport) SampleReactor : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(SampleReactor);
	SampleReactor();
	virtual ~SampleReactor();
	void eLinkage(AcDbObjectId, double = 1.0);
	void modified(const AcDbObject*);
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
private:
	AcDbObjectId mId; //������id
	double mFactor; //�ƶ��ı���
};

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleReactor)
#endif
