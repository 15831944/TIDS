#include "stdafx.h"
#include "DrawCmd.h"

// ����ģ��MineGE������
#include "../MineGE/LinkedGE.h"
#include "../MineGE/TagGE.h"
#include "../DefGE/WindDirection.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/TTunnel.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include <cmath>

static void ReadWriteData( const AcDbObjectId& sObjId, const CString& sField, const AcDbObjectId& tObjId, const CString& tField )
{
    CString v;
    DataHelper::GetPropertyData( sObjId, sField, v );
    DataHelper::SetPropertyData( tObjId, tField, v );
}

void DrawCmd::UpdateAllWindStationData()
{
    // �������еĲ��վ
    // �����������ϵ���������١��������ݶ�ȡ�����վ��
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "Joint" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObjectIdArray geObjIds;
    int len = objIds.length();
    bool ret = true;
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            ret = false;
            break;
        }

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 )
        {
            ret = false;
            break;
        }

        geObjIds.append( pTag->getRelatedGE() );
    }

    actrTransactionManager->endTransaction();

    if( !ret )
    {
        geObjIds.removeAll();
    }
    else
    {
        //assert(objIds.length() == geObjIds.length());
        int len = objIds.length();
        for( int i = 0; i < len; i++ )
        {
            ReadWriteData( geObjIds[i], _T( "�������" ), objIds[i], _T( "���Զ������" ) );
            ReadWriteData( geObjIds[i], _T( "����" ), objIds[i], _T( "���Է���" ) );
            ReadWriteData( geObjIds[i], _T( "����" ), objIds[i], _T( "���Է���" ) );
        }
    }
}

static void GetSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pObj );
        if( pEdge == 0 ) continue;

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

static void EraseAllDirections()
{
    AcDbObjectIdArray dirs;
    DrawHelper::FindMineGEs( _T( "WindDirection" ), dirs );
    ArxEntityHelper::EraseObjects( dirs, true );
}

static void AddDirection( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcGeVector3d v = ept - spt;
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	AcDbObject* pObj;
	if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForRead ) ) 
	{
		acutPrintf(_T("\n����ʧ��!"));
		return;
	}

	//TTunnel* pTTunel = TTunnel::cast( pObj );
	//�Ѷ���ǿ��ת����Բ���������
	ArcTunnel* pArcTunnel = ArcTunnel::cast( pObj );
	pObj->close();

	AcGePoint3d insertPt;
	
	//bool isOppositeDir = false;
	
	if( pArcTunnel == 0 )//����Բ�����
	{
		insertPt = spt + v * 0.5;
	}

	else//Բ�����
	{
		insertPt = pArcTunnel->getArcCntPt();
	}

	//CString name;
	//CString method;
	//
	//DataHelper::GetPropertyData( objId, _T( "����" ), name );
	//if (ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ))
	//{
	//	DataHelper::GetPropertyData( objId, _T( "ͨ�緽��" ), method );
	//}
	//else
	//{
	//	AcDbObjectId tWorkId = DrawCmd::GetRelatedTW(objId);		
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

	angle = DrawCmd::ControlDirByMethods(objId,angle);
	WindDirection* pDir = new WindDirection( insertPt, angle ); // �������λ��
	pDir->setRelatedGE( objId );
	ArxUtilHelper::PostToModelSpace( pDir );	
}

void DrawCmd::AutoDirection()
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );
    if( objIds.isEmpty() ) return;

    // ɾ�����еķ�������
    EraseAllDirections();

    AcGePoint3dArray spts, epts;
    GetSEPts( objIds, spts, epts );

    int len = objIds.length();

	for (int i = 0;i < len;i++)
	{
		AddDirection(objIds[i],spts[i],epts[i]);
	}
}

