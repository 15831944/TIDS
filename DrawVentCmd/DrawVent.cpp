#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// ͨ��ͼԪ
#include "../DefGE/Tunnel.h"
#include "../DefGE/Fan.h"
#include "../DefGE/WindDirection.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/TTunnel.h"
#include "../DefGE/Joint.h"
#include "../DefGE/WindLibrary.h"
#include "../DefGE/FanTagGE.h"

#include "../DefGE/Chimney.h"
#include "../DefGE/QTagGE.h"
#include "../DefGE/EffectRanTag.h"

#include "PolyLineJig.h"

/* ȫ�ֺ���(ʵ����PromptTool.cpp) */
extern bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle );
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool PromptInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt );
extern bool PromptArcPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt );

// ���Ʒ�������ʱ���������С����
#define MIN_LENGTH 150

static void CreateDirection( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    WindDirection* pDir = new WindDirection( pt, angle );
    pDir->setRelatedGE( host ); // ����ͼԪ

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pDir ) ) delete pDir;
}

void DrawCmd::DrawArcTunnel( void )
{
	AcGePoint3d startPt, endPt, thirdPt;
	if( !PromptArcPt( _T( "�������" ), startPt, endPt, thirdPt ) ) return;

	ArcTunnel* pArcTunnel = new ArcTunnel( startPt, endPt, thirdPt );  // �������

	if( !ArxUtilHelper::PostToModelSpace( pArcTunnel ) ) delete pArcTunnel;
}

void DrawCmd::DrawTunnel( void )
{
    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "���" ), startPt, endPt, angle ) ) return;

    // 3������ͼԪ
    Tunnel* pTunnel = new Tunnel( startPt, endPt );  // ���

    // 4����ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pTunnel ) ) delete pTunnel;
	//bool ret = ArxUtilHelper::PostToModelSpace( pTunnel );
	//acutPrintf(_T("\n�ύ״̬��%s"),ret ? _T("�ɹ�"):_T("ʧ��"));
}


void DrawCmd::DrawLocalFan()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;
	//if( !ArxUtilHelper::IsEqualType( _T( "Chimney" ), objId ) ) return;

    AcDbObjectIdArray objIds;
    DrawHelper::GetTagGEById2( objId, _T( "LocalFan" ), objIds );
    //if( !objIds.isEmpty() )
    //{
    //    AfxMessageBox( _T( "�þ���������������˾ֲ�ͨ���!" ) );
    //    return;
    //}

    AcGePoint3d pt,insertPt;

	double angle;
	//if( !GetClosePtAndAngle( objId, pt, angle ) ) return;
	
	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����ȵĲ��������:" ), pt ) ) return;

	insertPt = pt;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;
    Fan* pFan = new LocalFan( insertPt, angle ); 
    if( pFan == 0 ) return;

    pFan->setRelatedGE( objId ); // �������

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pFan ) ) delete pFan;
}

static void FanTagGEDrawed(AcDbObjectId fanId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,fanId,AcDb::kForRead)) return;
	LocalFan *pFan = LocalFan::cast(pObj);
	if ( 0 == pFan)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d insertPt = 	pFan->getInsertPt();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray fanTags;
	DrawHelper::GetTagGEById2( fanId, _T( "FanTagGE" ), fanTags );
	if (!fanTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( fanTags, true );
	}
	
	CString name,qStr,hStr,way;
	DataHelper::GetPropertyData(fanId,_T("����ͺ�"),name);
	DataHelper::GetPropertyData(fanId,_T("��������(m3/s)"),qStr);
	DataHelper::GetPropertyData(fanId,_T("������ѹ(Pa)"),hStr);
	DataHelper::GetPropertyData(fanId,_T("������ʽ"),way);

	FanTagGE *pFanTag = new FanTagGE(insertPt,name,way,_tstof(qStr),_tstof(hStr));
	if( pFanTag == 0 ) return;

	//acutPrintf(_T("\ninsertPt:(%lf,%lf),angle:%lf"),insertPt.x,insertPt.y,angle);

	pFanTag->setRelatedGE( fanId ); // ��������

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pFanTag ) ) delete pFanTag;

}

void DrawCmd::DrawFanTag()
{
	AcDbObjectIdArray fanIds;
	DrawHelper::FindMineGEs(_T("LocalFan"),fanIds);
	int len = fanIds.length();
	for (int i = 0; i < len; i++)
	{
		FanTagGEDrawed(fanIds[i]);
	}
}

void DrawCmd::DrawDirection( void )
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

	AcDbObject* pObj;
	acdbOpenObject( pObj, objId, AcDb::kForRead );
    
	AcGePoint3d pt;
    if( !PromptInsertPt( objId, pt ) ) return;

	//ArcTunnel *pArcTunnel = ArcTunnel::cast(pObj);
	LinkedGE* pEdge = LinkedGE::cast( pObj );
	//TTunnel* pTTunel = TTunnel::cast(pObj);

	pObj->close();

	AcGePoint3d spt,ept;
	pEdge->getSEPoint( spt, ept );
	AcGeVector3d v = ept - spt;
	double angle = v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis);

    if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	//bool isOppositeDir = false;
	//CString method;

	//CString name;
	//DataHelper::GetPropertyData( objId, _T( "����" ), name );

	//if (ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ))
	//{
	//	DataHelper::GetPropertyData( objId, _T( "ͨ�緽��" ), method );
	//}
	//else
	//{
	//	AcDbObjectId tWorkId = GetRelatedTW(objId);
	//	DataHelper::GetPropertyData( tWorkId, _T( "ͨ�緽��" ), method );
	//}

	//if ( _T("������") != name && _T("�ط���") != name)
	//{
	//	if (_T("ѹ��ʽ") == method || _T("��ѹ�̳�") == method)
	//	{
	//		isOppositeDir = true;
	//	}
	//}

	//if (true == isOppositeDir)
	//{
	//	angle = PI + angle;
	//}

	angle = ControlDirByMethods(objId,angle);
    CreateDirection( objId, pt, angle );
}

static void CreatJoint( const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
	Joint* pStation = new Joint( pt, angle );
	// �����������
	pStation->setRelatedGE( host );

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pStation ) ) delete pStation;
}

//static void getLinkedGEObj(LinkedGE* pEdge,AcDbObjectId objId)
//{
//	AcTransaction* pTrans = actrTransactionManager->startTransaction();
//	if( pTrans == 0 ) return;
//
//	AcDbObject* pObj;
//	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
//	{
//		actrTransactionManager->abortTransaction();
//		return;
//	}
//
//	pEdge = LinkedGE::cast( pObj );
//	if( pEdge == 0 )
//	{
//		actrTransactionManager->abortTransaction();
//		return;
//	}
//
//
//}
void DrawCmd::DrawJoint()
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����������������:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	LinkedGE* pEdge = LinkedGE::cast( pObj );
	if( pEdge == 0 )
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	actrTransactionManager->endTransaction();
	AcGePoint3d spt,ept;
	pEdge->getSEPoint(spt,ept);
	AcGePoint3d pt;
	if( !PromptInsertPt( objId, pt ) ) return;

	double angle;
	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	SplitTunnel(objId,spt,ept,pt);
	CreatJoint( objId, pt, angle );
}

void DrawCmd::DrawChimney( void )
{
    //acutPrintf( _T( "\n���Ʒ�Ͳ����..." ) );

    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ���Ͳ����Ӧ�ľ���:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "LocalFan" ), objId ) ) return;

    //if( !objIds.isEmpty() )
    //{
    //    AfxMessageBox( _T( "�þ���������������˷�Ͳ!" ) );
    //    return;
    //}

    AcGePoint3dArray pts;
    PolyLineJig jig;
    if( !jig.doJig( pts ) ) return;

    int len = pts.length();
    //acutPrintf( _T( "\n�����:%d" ), len );
    if( len < 2 ) return;

    Chimney* pChimney = new Chimney();
    pChimney->setRelatedGE( objId );

    for( int i = 0; i < len; i++ ) pChimney->addControlPoint( pts[i] );

    // ��ʼ�����ύ�����ݿ�
    if( !ArxUtilHelper::PostToModelSpace( pChimney ) ) delete pChimney;

	AcDbObjectIdArray objIds;
	DrawHelper::GetTagGEById2( objId, _T( "Chimney" ), objIds );

	//��ͬһ���������ķ�Ͳ�ϲ���һ��
	//int numChim = objIds.length();
	//for(int i = 0; i < numChim-1; i++)
	//{
	//	Merging(objIds[i],objIds[i+1]);
	//}
}

void DrawCmd::DrawWindLibrary( void )
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;


	AcGePoint3d pt,insertPt;

	double angle;

	if( !ArxUtilHelper::PromptPt( _T( "\n��ָ�����Ĳ��������:" ), pt ) ) return;

	if( !GetClosePtAndAngle( objId, pt, angle ) ) return;

	AcGeVector3d v = AcGeVector3d(AcGeVector3d::kXAxis);
	v.rotateBy(angle - PI/2,AcGeVector3d::kZAxis);
	v.normalize();

	insertPt = pt + v * 60;

	WindLibrary* pWindLib = new WindLibrary( insertPt, angle ); 	
	if( pWindLib == 0 ) return;


	pWindLib->setRelatedGE( objId ); // �������

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pWindLib ) ) delete pWindLib;

}


static void QTagGEDrawed(const AcDbObjectId& chimId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,chimId,AcDb::kForRead)) return;
	Chimney *pChim = Chimney::cast(pObj);
	if ( 0 == pChim)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3dArray ctlPts = 	pChim->getControlPoint();

	AcDbObjectId fanId;
	if(!DrawHelper::GetHostGE(chimId,fanId)) return;

	if (Acad::eOk != pTrans->getObject(pObj,fanId,AcDb::kForRead)) return;
	LocalFan *pFan = LocalFan::cast(pObj);
	if ( 0 == pFan)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d insertPt = 	pFan->getInsertPt();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray QTags;
	DrawHelper::GetTagGEById2( chimId, _T( "QTagGE" ), QTags );
	if (!QTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( QTags, true );
	}

	CString inQStr,outQStr;
	DataHelper::GetPropertyData(chimId,_T("��ڷ���(m3/s)"),inQStr);
	DataHelper::GetPropertyData(chimId,_T("���ڷ���(m3/s)"),outQStr);

	CString way;
	DataHelper::GetPropertyData(fanId,_T("������ʽ"),way);
	if(way.IsEmpty() || inQStr.IsEmpty() || outQStr.IsEmpty()) return;

	QTagGE *pInQTag,*pOutQTag;
	if( _T("ѹ��ʽ") == way )
	{
		pInQTag = new QTagGE(ctlPts[0],_tstof(inQStr));
		if( pInQTag == 0 ) return;
		pOutQTag = new QTagGE(ctlPts[ctlPts.length()-1],_tstof(outQStr));
		if( pOutQTag == 0 ) return;
	}

	if( _T("���ʽ") == way )
	{
		pOutQTag = new QTagGE(ctlPts[0],_tstof(outQStr));
		if( pOutQTag == 0 ) return;
		pInQTag = new QTagGE(ctlPts[ctlPts.length()-1],_tstof(inQStr));
		if( pInQTag == 0 ) return;
	}
	pOutQTag->setRelatedGE( chimId ); // ������Ͳ
	pInQTag->setRelatedGE( chimId ); // ������Ͳ

	// ��ʼ�����ύ�����ݿ�
	if( !ArxUtilHelper::PostToModelSpace( pOutQTag ) ) delete pOutQTag;
	if( !ArxUtilHelper::PostToModelSpace( pInQTag ) ) delete pInQTag;
}

void DrawCmd::DrawQTagGE()
{
	AcDbObjectIdArray chims;
	DrawHelper::FindMineGEs(_T("Chimney"),chims);
	int len = chims.length();
	//acutPrintf(_T("\n��Ͳ��:%d"),len);
	if(chims.isEmpty()) return;
	for (int i = 0; i < len; i++)
	{
		QTagGEDrawed(chims[i]);
	}

}

static void EffectRanDrawed(AcDbObjectId ttunel)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if ( 0 == pTrans ) return;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,ttunel,AcDb::kForRead)) return;
	TTunnel *pTTunnel = TTunnel::cast(pObj);
	if ( 0 == pTTunnel)
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	AcGePoint3d spt,ept;
	pTTunnel->getSEPoint(spt,ept);
	double angle = pTTunnel->getAngle();
	actrTransactionManager->endTransaction();

	AcDbObjectIdArray eTags;
	DrawHelper::GetTagGEById2( ttunel, _T( "EffectRanTagGE" ), eTags );
	if (!eTags.isEmpty())
	{
		ArxEntityHelper::EraseObjects( eTags, true );
	}

	AcGeVector3d v = ept - spt;
	double diatance = v.length();
	CString area,way;
	if(!DataHelper::GetPropertyData(ttunel,_T("�������"),area)) return;
	if(!DataHelper::GetPropertyData(ttunel,_T("ͨ�緽��"),way)) return;

	double minDistan,maxDistan;
	if(way.IsEmpty()) return;
	if(area.IsEmpty()) return;

	if (_T("ѹ��ʽ") == way || _T("��ѹ�̳�") == way)
	{
		minDistan = 4*sqrtf(_tstof(area));
		maxDistan = 5*sqrtf(_tstof(area));
	}

	else
	{
		minDistan = 0;
		maxDistan = 1.5*sqrtf(_tstof(area));
	}
	EffectRanTagGE *pTag = new EffectRanTagGE(ept,angle,minDistan,maxDistan,diatance*0.1);
	if (0 == pTag) return;

	pTag->setRelatedGE(ttunel);
	if( !ArxUtilHelper::PostToModelSpace( pTag ) ) delete pTag;
}

void DrawCmd::DrawEffectRanGE()
{
	AcDbObjectIdArray ttunels;
	DrawHelper::FindMineGEs(_T("TTunnel"),ttunels);
	int len = ttunels.length();
	if(ttunels.isEmpty()) return;
	for (int i = 0; i < len; i++)
	{
		EffectRanDrawed(ttunels[i]);
	}

}
