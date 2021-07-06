#include "pch.h"
#include "tchar.h"
#include <aced.h>
#include <rxregsvc.h>
#include <dbproxy.h>
#include "rxboiler.h"
#include "dbapserv.h"
#include "dbidmap.h"
#include "dbcfilrs.h"
#include "acadstrc.h"
#include "AcGraph.h"

#include "SampleCustEnt.h"

//����ʱ����ʶ������ʵ��
ACRX_DXF_DEFINE_MEMBERS(
	SampleCustEnt, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, SAMPLECUSTENT,
	ARXPROJECT1APP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

Adesk::UInt32 SampleCustEnt::kCurrentVersionNumber = 1;

SampleCustEnt::SampleCustEnt() {
	
}

SampleCustEnt::SampleCustEnt(AcGePoint3d center, double radius) {
	m_center = center;
	m_radius = radius;
}
SampleCustEnt::~SampleCustEnt() {

}

Acad::ErrorStatus SampleCustEnt::setCenter(AcGePoint3d center)
{
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(SampleCustEnt::desc());//����ʵ����
		undoFiler()->writeItem((Adesk::Int16)kCenter);//�������Ա��
		undoFiler()->writePoint3d(m_center);
	}
	m_center = center;//�ǵ��ȵ������������µ�ֵ
	this->zoom();
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::setRadius(double rad)
{
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(SampleCustEnt::desc());//����ʵ����
		undoFiler()->writeItem((Adesk::Int16)kRadius);//�������Ա��
		undoFiler()->writeDouble(m_radius);
	}
	m_radius = rad;//�ǵ��ȵ������������µ�ֵ
	this->zoom();
	return Acad::eOk;

}

Acad::ErrorStatus SampleCustEnt::setTrans(AcGeVector3d trans)

{
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(SampleCustEnt::desc());//����ʵ����
		undoFiler()->writeItem((Adesk::Int16)kTrans);//�������Ա��
		undoFiler()->writeVector3d(m_trans);
	}
	m_trans = trans;
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::setFactor(double fac)
{
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(SampleCustEnt::desc());//����ʵ����
		undoFiler()->writeItem((Adesk::Int16)kFactor);//�������Ա��
		undoFiler()->writeDouble(m_factor);
	}
	m_factor = fac;//�ǵ��ȵ������������µ�ֵ
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj)
{
	//���classObj�������ǵ���ʱ��һ��������ʵ����
	if (classObj != SampleCustEnt::desc())
		return AcDbEntity::applyPartialUndo(undoFiler, classObj);
	Adesk::Int16 shortCode;
	undoFiler->readItem(&shortCode);
	AcGraph::PartialUndoCode code = (AcGraph::PartialUndoCode)shortCode;
	double rad = 0;
	AcGePoint3d center = AcGePoint3d::kOrigin;
	double factor = 2;
	AcGeVector3d trans = AcGeVector3d(100, 100, 0);
	switch (code) {//�������Ա�����ò�ͬ������ֵ
	case kRadius:
		//��dwgInFieldsһ��������ж�����ݣ�˳��Ҫ�뵼��ʱһ��
		undoFiler->readDouble(&rad);
		setRadius(rad);//˼����Ϊʲô��ֱ������m_radius��
		break;
	case kCenter:
		//��dwgInFieldsһ��������ж�����ݣ�˳��Ҫ�뵼��ʱһ��
		undoFiler->readPoint3d(&center);
		setCenter(center);//˼����Ϊʲô��ֱ������m_radius��
		break;
	case kFactor:
		undoFiler->readDouble(&factor);
		setFactor(factor);
		break;
	case kTrans:
		undoFiler->readVector3d(&trans);
		setTrans(trans);
		break;
	default:
		assert(Adesk::kFalse);
		break;
	}
	this->zoom();
	return Acad::eOk;
}


//struct resbuf * asBuf( AcGePoint3dArray& pts) {
//	struct resbuf *head;
//	struct resbuf *now;
//
//	now = acutNewRb(RT3DPOINT);
//	head = now;
//	for (int i = 0; i < pts.length(); i++) {
//		now->resval.rpoint[X] = pts[i][X];
//		now->resval.rpoint[Y] = pts[i][Y];
//		now->resval.rpoint[Z] = pts[i][Z];
//		now->rbnext = acutNewRb(RT3DPOINT);
//		now = now->rbnext;
//	}
//	acutRelRb(now->rbnext);
//	now->rbnext = nullptr;
//	return head;
//}

Adesk::Boolean SampleCustEnt::subWorldDraw(AcGiWorldDraw *mode) {
	assertReadEnabled();

	AcGiWorldGeometry * pGeom = &mode->geometry();
	mode->geometry().circle(m_center, m_radius, AcGeVector3d::kZAxis);

	mode->geometry().pushModelTransform(AcGeMatrix3d::translation(m_trans));
	mode->geometry().pushModelTransform(AcGeMatrix3d::scaling(m_factor, m_center));
	
	mode->geometry().circle(m_center, m_radius, AcGeVector3d::kZAxis);
	Adesk::Boolean bPopClipBoundary = pGeom->pushClipBoundary(&cb);

	for (auto i = 0; i < zoomE.size(); i++)
	{
		AcDbEntity* pEnt = nullptr;
		acdbOpenAcDbEntity(pEnt, zoomE[i], AcDb::kForRead);
		if (pEnt)
		{
			mode->geometry().draw(pEnt);
			pEnt->close();
		}
	}
	
	//mode->geometry().popModelTransform();
	mode->geometry().popModelTransform();
	if (bPopClipBoundary)
	{
		pGeom->popClipBoundary();
	}

	return (AcDbEntity::subWorldDraw(mode));
}

void SampleCustEnt::zoom()
{
	assertWriteEnabled();
	zoomE.clear();
	cb.m_aptPoints.removeAll();

	auto pCir = new AcDbCircle(m_center, AcGeVector3d::kZAxis, m_radius);
	AcDbExtents ext;
	pCir->getGeomExtents(ext);

	AcGeCurve3d* pGeCrv = nullptr;
	pCir->getAcGeCurve(pGeCrv);
	pCir->close();
	delete pCir;
	pCir = nullptr;
	AcGePoint3dArray pts;
	pGeCrv->getSamplePoints(50, pts);//������ȡ��
	delete pGeCrv;//����Ҫdelete��
	pGeCrv = nullptr;
	
	ads_name ss;
	//struct resbuf * buf = asBuf(pts);
	// acedSSGet(_T("CP"), buf, NULL, NULL, ss);
	acedSSGet(_T("C"), asDblArray(ext.minPoint()), asDblArray(ext.maxPoint()), NULL, ss);

	Adesk::Int32 len = 0;
	acedSSLength(ss, &len);
	// acutPrintf(_T("ʵ��%s��"), len);
	for (auto i = 0; i < len; i++) {
		ads_name en;
		acedSSName(ss, i, en);
		AcDbObjectId objId;
		acdbGetObjectId(objId, en);
		zoomE.push_back(objId);
	}
	acedSSFree(ss);

	cb.m_bDrawBoundary = true;
	cb.m_vNormal = AcGeVector3d::kZAxis;
	cb.m_ptPoint = m_center;
	
	for (int i = 0; i < pts.logicalLength(); i++) {
		cb.m_aptPoints.append(AcGePoint2d(pts[i][X], pts[i][Y]));
	}

	cb.m_xToClipSpace.setToIdentity();
	cb.m_xInverseBlockRefXForm.setToIdentity();
	cb.m_bClippingBack = cb.m_bClippingFront = false;
	cb.m_dFrontClipZ = cb.m_dBackClipZ = 0;
}
//�ӿ���ص���ʾ
void SampleCustEnt::subViewportDraw(AcGiViewportDraw* pV) {
	
}

//������ʾ��ص�����
Adesk::UInt32 SampleCustEnt::subSetAttributes(AcGiDrawableTraits *traits) {
	assertReadEnabled();
	traits->setColor(0);
	return 0;
}

Acad::ErrorStatus SampleCustEnt::subTransformBy(const AcGeMatrix3d& xform) {
	assertWriteEnabled();
	this->setCenter( m_center.transformBy(xform));
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const {
	assertReadEnabled();
	AcGePoint3d left(m_center.x - m_radius, m_center.y, 0);
	AcGePoint3d right(m_center.x + m_radius, m_center.y, 0);
	AcGePoint3d up(m_center.x, m_center.y + m_radius, 0);
	AcGePoint3d down(m_center.x, m_center.y - m_radius, 0);

	gripPoints.append(m_center);
	gripPoints.append(left);
	gripPoints.append(right);
	gripPoints.append(up);
	gripPoints.append(down);
	gripPoints.append(m_center + m_trans);
	gripPoints.append(AcGePoint3d(m_center.x - m_radius * m_factor, m_center.y, 0) + m_trans);
	gripPoints.append(AcGePoint3d(m_center.x + m_radius * m_factor, m_center.y, 0) + m_trans);
	gripPoints.append(AcGePoint3d(m_center.x , m_center.y + m_radius * m_factor, 0) + m_trans);
	gripPoints.append(AcGePoint3d(m_center.x , m_center.y - m_radius * m_factor, 0) + m_trans);
	
	return Acad::eOk;
}
Acad::ErrorStatus SampleCustEnt::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d& offset) {
	assertWriteEnabled();
	AcGeVector3d original;
	AcGeVector3d selectVector;
	double diff = 0;
	switch (indices[0]) {
	case 0:
		this->setCenter(m_center += offset);
		break;
	case 1:
		selectVector = AcGeVector3d(-m_radius, 0, 0);
		this->setRadius((selectVector + offset).length());
		//m_radius = (selectVector + offset).length();
		break;
	case 2:
		selectVector = AcGeVector3d(m_radius, 0, 0);
		this->setRadius((selectVector + offset).length());
		break;
	case 3:
		selectVector = AcGeVector3d(0, m_radius, 0);
		this->setRadius((selectVector + offset).length());
		break;
	case 4:
		selectVector = AcGeVector3d(0, -m_radius, 0);
		this->setRadius((selectVector + offset).length());
		break;
	case 5:
		this->setTrans(m_trans + offset);
		break;
	case 6:
		selectVector = AcGeVector3d(-m_radius, 0, 0) * m_factor;
		this->setFactor((selectVector + offset).length() / m_radius);
		//m_radius = (selectVector + offset).length();
		break;
	case 7:
		selectVector = AcGeVector3d(m_radius, 0, 0)* m_factor;
		this->setFactor((selectVector + offset).length() / m_radius);
		break;
	case 8:
		selectVector = AcGeVector3d(0, m_radius, 0)* m_factor;
		this->setFactor((selectVector + offset).length() / m_radius);
		break;
	case 9:
		selectVector = AcGeVector3d(0, -m_radius, 0)* m_factor;
		this->setFactor((selectVector + offset).length() / m_radius);
		break;
	}
	
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const
{
	assertReadEnabled();
	AcDbCircle circle;
	circle.setCenter(m_center);
	circle.setRadius(m_radius);
	return circle.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
}


Acad::ErrorStatus SampleCustEnt::dwgInFields(AcDbDwgFiler * pFiler)
{
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	if (version < SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//----- Read params
	pFiler->readPoint3d(&m_center);
	pFiler->readDouble(&m_radius);
	pFiler->readVector3d(&m_trans);
	pFiler->readDouble(&m_factor);
	return (pFiler->filerStatus());

}



Acad::ErrorStatus SampleCustEnt::dwgOutFields(AcDbDwgFiler * pFiler) const
{
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(SampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	pFiler->writePoint3d(m_center);
	pFiler->writeDouble(m_radius);
	pFiler->writeVector3d(m_trans);
	pFiler->writeDouble(m_factor);
	return (pFiler->filerStatus());

}

//- Dxf Filing protocol
Acad::ErrorStatus SampleCustEnt::dxfOutFields(AcDbDxfFiler *pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _T("SampleCustEnt"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, SampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

Acad::ErrorStatus SampleCustEnt::dxfInFields(AcDbDxfFiler *pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_T("SampleCustEnt")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _T("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < SampleCustEnt::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
		case AcDb::kDxfXCoord:
			break;

		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus
SampleCustEnt::subDeepClone(AcDbObject*    pOwner,
	AcDbObject*&   pClonedObject,
	AcDbIdMapping& idMap,
	Adesk::Boolean isPrimary) const
{
	//��������Ӧ�ڴ�������ʱ�����ΪNULL�������Է���һ������Ҫ���ÿ�
	//������һû���Ƴɹ��ó�ȥ����Ұָ��
	pClonedObject = NULL;

	//������ʵ���Ѿ�������Id Map�����Ѿ������ƹ��ˣ���Id Map���Ѿ�������ɶԵ�Id�ˣ���
	//�Ͳ����ٸ����ˣ�ֱ�ӷ���
	//˼����ʲô����»ᷢ�������£�
	bool isPrim = false;
	if (isPrimary)//isPrimary��ʾ��ǰ�����ƵĶ����Ǳ��������ƵĻ�����Ϊ���й�ϵ�������Ƶ�
		isPrim = true;
	AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL,
		false, isPrim);
	if (idMap.compute(idPair) && (idPair.value() != NULL))
		return Acad::eOk;

	//����һ��������ͬ�Ķ���
	auto *pClone = (SampleCustEnt*)isA()->create();
	if (pClone != NULL)
		pClonedObject = pClone;    //������Ǹ��Ƴ����Ķ��󣬿����ȴ浽����ֵ��
	else
		return Acad::eOutOfMemory;

	AcDbDeepCloneFiler filer;
	dwgOut(&filer);//ͨ��dwgOut��ȡ�����������
	filer.seek(0L, AcDb::kSeekFromStart);//���������ļ���ָ�룬���������dwgIn
	pClone->dwgIn(&filer);

	//����Ҫ����owner�����������
	//1. ���owner�ǿ���¼���Ǿ�ֱ�ӵ���appendAcDbEntity�ӽ�ȥ
	//2. ���owner�Ǵʵ䣬��ʹ��setAt�ӵ��ʵ���������û�����֣�
	//3. �����ǰ��������Ϊ���ù�ϵ�������Ƶģ���ʹ��addAcDbObject()
	//�����ӵ�owner�����ݿ��ﲢֱ����������owner
	bool bOwnerXlated = false;//��ʾowner��id�Ƿ��Ѿ�ת��
	if (isPrimary)
	{
		AcDbBlockTableRecord *pBTR =
			AcDbBlockTableRecord::cast(pOwner);
		if (pBTR != NULL)
		{
			pBTR->appendAcDbEntity(pClone);
			bOwnerXlated = true;
		}
		else
		{//����������������֧��ʵ�ǲ����ܽ����ģ�����ֻ���ݴ�
			pOwner->database()->addAcDbObject(pClone);
		}
	}
	else {
		pOwner->database()->addAcDbObject(pClone);
		pClone->setOwnerId(pOwner->objectId());
		bOwnerXlated = true;
	}


	//ÿһ����deepclone�����ﴴ���Ķ���Ҫ�ڼ���Id Mapǰ����setAcDbObjectIdsInFlux()����
	pClone->setAcDbObjectIdsInFlux();
	pClone->disableUndoRecording(true);//pClone�����Ѿ������ݿ��ͨ�������ʽ���Խ��õ�����undo


	//��Դ����͸��ƶ���ճ�һ�ԣ��ŵ�Id Map��
	idPair.setValue(pClonedObject->objectId());
	idPair.setIsCloned(Adesk::kTrue);
	idPair.setIsOwnerXlated(bOwnerXlated);
	idMap.assign(idPair);

	//����Դ��������й�ϵ���������ǵ�deepClone����
	AcDbObjectId id;
	while (filer.getNextOwnedObject(id)) {

		AcDbObject *pSubObject;
		AcDbObject *pClonedSubObject;

		//���й�ϵ�Ķ���id��һ��������Ч�ģ���Ҫ�����
		if (id == NULL)
			continue;

		//ע����Ϊ��Щ�������������й�ϵ�������Ƶģ�����deepClone�����һ������(isPrimary)��Ҫ��Ϊfalse
		acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
		pClonedSubObject = NULL;
		pSubObject->deepClone(pClonedObject,
			pClonedSubObject,
			idMap, Adesk::kFalse);

		//��ĳ���������(����ĳЩ����µ�Ԥ��)������ʵ����ֻ�����ݡ��ƶ�����
		//pSubObject��pClonedSubObject��ָ��ͬһ���������ʱ��Ͳ���close pSubObject��
		if (pSubObject != pClonedSubObject)
			pSubObject->close();

		//����������ʧ��pClonedSubObject�ͻ��ǿ�ָ�룬��Ҫ���
		if (pClonedSubObject != NULL)
			pClonedSubObject->close();
	}

	//ע���ʱpClonedObject��û�б��رյģ����ɵ�����������
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subWblockClone(
		AcRxObject*    pOwner,
		AcDbObject*&   pClonedObject,
		AcDbIdMapping& idMap,
		Adesk::Boolean isPrimary) const
	{
		//�ǵ��Ȱѷ���ֵ�ÿ�
		pClonedObject = NULL;

		//���Դ���ݿ��Ŀ�����ݿ���ͬһ��������ʡ�Դ󲿷ֲ��裬ֱ�ӵ��û����wblockClone�Ϳ�����
		AcDbDatabase *pDest, *pOrig;
		idMap.destDb(pDest);
		idMap.origDb(pOrig);
		if (pDest == pOrig)
			return AcDbEntity::wblockClone(pOwner, pClonedObject,
				idMap, isPrimary);

		//��������ڽ���xbind����������xbindֻ����ģ�Ϳռ�Ķ������Դ������ͼֽ�ռ䣬��ֱ�ӷ���
		AcDbObjectId pspace;
		AcDbBlockTable *pTable;
		database()->getSymbolTable(pTable, AcDb::kForRead);
		pTable->getAt(ACDB_PAPER_SPACE, pspace);
		pTable->close();
		if (idMap.deepCloneContext() == AcDb::kDcXrefBind
			&& ownerId() == pspace)
			return Acad::eOk;

		//���Դ�����Ѿ������ƹ��ˣ�ֱ�ӷ���
		bool isPrim = false;
		if (isPrimary)
			isPrim = true;
		AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL,
			false, isPrim);
		if (idMap.compute(idPair) && (idPair.value() != NULL))
			return Acad::eOk;

		//Դ�����owner�п��������ݿ⣬Ҳ�п�����һ��AcDbObject��
		//���owner�����ݿ⣬��ʾ����������������Ӳָ���ϵ�������Ƶģ�������ʵ��owner����ת��׶α���ȷ����
		//��ʱpOwn��ֵ����NULL����Ϊһ����־
		AcDbObject   *pOwn = AcDbObject::cast(pOwner);
		AcDbDatabase *pDb = AcDbDatabase::cast(pOwner);
		if (pDb == NULL)
			pDb = pOwn->database();

		//����һ��ͬ���͵Ķ���
		auto *pClone = (SampleCustEnt*)isA()->create();
		if (pClone != NULL)
			pClonedObject = pClone;
		else
			return Acad::eOutOfMemory;

		//���owner�Ǹ��������������֪����������ʲô���Ϳ���ֱ�����ø��ƶ����owner
		//����Ͱ�Դ�����owner���ø����ƶ�����ת��׶ν���ת���������Դ������Ŀ�����ownerһ����Ҳ����Ϊת��׶�Ҫ�õ���һ����־
		//Ҫע�������Դ�����ownerҲ����Ҫ�����Ƶ�Ŀ������ݿ���ģ�����ת��׶λ����
		AcDbBlockTableRecord *pBTR = NULL;
		if (pOwn != NULL)
			pBTR = AcDbBlockTableRecord::cast(pOwn);
		if (pBTR != NULL && isPrimary) {
			pBTR->appendAcDbEntity(pClone);
		}
		else {
			pDb->addAcDbObject(pClonedObject);
		}

		//ͨ��dwgOut��dwgIn�������ݵ����ƶ�����
		AcDbWblockCloneFiler filer;
		dwgOut(&filer);
		filer.seek(0L, AcDb::kSeekFromStart);
		pClone->dwgIn(&filer);

		//���Դ�����븴�ƶ���Id Map��ע��AcDbIdPair���캯�������һ������isOwnerXlated�����ƶ����owner�Ƿ���Ҫת��
		//�������������ͨ�������pOwn�Ƿ�Ϊ�����жϵ�
		idMap.assign(AcDbIdPair(objectId(), pClonedObject->objectId(),
			Adesk::kTrue,
			isPrim, (Adesk::Boolean)(pOwn != NULL)));
		pClonedObject->setOwnerId((pOwn != NULL) ?
			pOwn->objectId() : ownerId());

		//��deepcloneһ����Ҫ��ÿ�����ƶ������setAcDbObjectIdsInFlux()
		pClone->setAcDbObjectIdsInFlux();

		//�ݹ鸴�����е�Ӳ���к�Ӳָ��
		AcDbObjectId id;
		while (filer.getNextHardObject(id)) {
			AcDbObject *pSubObject;
			AcDbObject *pClonedSubObject;

			//ĳЩ����»�������õĶ���Ϊ�գ���Ҫ����
			if (id == NULL)
				continue;

			//������ö�����������һ�����ݿ⣬����Ҫ����
			acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
			if (pSubObject->database() != database()) {
				pSubObject->close();
				continue;
			}

			//���������������������������ö����owner���Ͱ����ݿ���Ϊowner����ȥ
			//�����deepcloneһ��������isPrimary����Ҫ��Ϊfalse
			pClonedSubObject = NULL;
			if (pSubObject->ownerId() == objectId()) {
				pSubObject->wblockClone(pClone,
					pClonedSubObject,
					idMap, Adesk::kFalse);
			}
			else {
				pSubObject->wblockClone(
					pClone->database(),
					pClonedSubObject,
					idMap, Adesk::kFalse);
			}
			pSubObject->close();

			//�������ʧ�ܣ�pClonedSubObject���ǿ�ָ�룬��Ҫ���
			if (pClonedSubObject != NULL)
				pClonedSubObject->close();
		}

		//��deepcloneһ�������ƶ����ɵ�������close
		return Acad::eOk;
	}
