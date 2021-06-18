#pragma once
//���Զ���ʵ��̳���AcDbEntity��������õ��Զ���ʵ�常��
class __declspec(dllexport) SampleCustEnt : public AcDbEntity {
public:
	AcGePoint3d m_center = AcGePoint3d::kOrigin;
	AcGePoint3d m_left;
	AcGePoint3d m_right;
	AcGePoint3d m_up;
	AcGePoint3d m_down;
	double m_radius = 0;
	ACRX_DECLARE_MEMBERS(SampleCustEnt);//������һЩ����ʱ����ʶ���ת����������cast, desc, isA��
protected:
	static Adesk::UInt32 kCurrentVersionNumber;//��¼�Զ���ʵ��İ汾���������汾����
public:
	SampleCustEnt();
	SampleCustEnt(AcGePoint3d m_center, double m_radius);
	virtual ~SampleCustEnt();
	void setCenter(AcGePoint3d center);
	void setRadius(double radius);
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//�ӿ��޹ص���ʾ
	virtual void subViewportDraw(AcGiViewportDraw* mode);//�ӿ���ص���ʾ
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits *traits);
	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);
	Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler * pFiler) const;
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler * pFiler);
	//������ʾ��ص�����
};

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleCustEnt)
#endif