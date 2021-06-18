#pragma once
#include "dbmain.h"

class __declspec(dllexport) SampleReactor : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(SampleReactor);
	SampleReactor() {};
	void eLinkage(AcDbObjectId i, double f = 1.0) { mId = i; mFactor = f; };
	void modified(const AcDbObject*);
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
private:
	AcDbObjectId mId; //������ֱ��id
	double mFactor; //�ƶ��ı���
};

#ifdef ARXTEMPLATE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleReactor)
#endif
