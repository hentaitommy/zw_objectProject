#pragma once
//���Զ���ʵ��̳���AcDbEntity��������õ��Զ���ʵ�常��
#include<vector>
using std::vector;
class __declspec(dllexport) SampleCustEnt : public AcDbEntity {
public:
	ACRX_DECLARE_MEMBERS(SampleCustEnt);//������һЩ����ʱ����ʶ���ת����������cast, desc, isA��

	AcGePoint3d m_center = AcGePoint3d::kOrigin;
	double m_radius = 0;
	AcGeVector3d m_trans = AcGeVector3d(100, 100, 0);
	double m_factor = 2;
	AcGiClipBoundary cb; // ���ڴ���Ŵ󾵵ı߽磬�������ִ���
	

	vector<AcDbObjectId> zoomE; // ������Ŵ�Ȧ����ʵ��

	SampleCustEnt();
	SampleCustEnt(AcGePoint3d m_center, double m_radius);
	virtual ~SampleCustEnt();
	Acad::ErrorStatus setCenter(AcGePoint3d);
	Acad::ErrorStatus setRadius(double);
	Acad::ErrorStatus setTrans(AcGeVector3d);
	Acad::ErrorStatus setFactor(double);

	Acad::ErrorStatus applyPartialUndo(AcDbDwgFiler*, AcRxClass*);

	void zoom(); // ˢ�·Ŵ󾵵�ѡ��Χ
	
	enum MyEnum{ kRadius, kCenter, kTrans, kFactor };
protected:
	static Adesk::UInt32 kCurrentVersionNumber;//��¼�Զ���ʵ��İ汾���������汾����
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//�ӿ��޹ص���ʾ
	virtual void subViewportDraw(AcGiViewportDraw* mode);//�ӿ���ص���ʾ
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits *traits);

	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);
	Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler * pFiler) const;
	Acad::ErrorStatus dxfOutFields(AcDbDxfFiler * pFiler) const;
	Acad::ErrorStatus dxfInFields(AcDbDxfFiler * pFiler);
	Acad::ErrorStatus subDeepClone(AcDbObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;
	Acad::ErrorStatus subWblockClone(AcRxObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler * pFiler);
	//������ʾ��ص�����
};

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleCustEnt)
#endif