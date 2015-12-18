#pragma once

class DrawCmd
{
public:
	/* ʵ����DrawVent.cpp�� */
	//static void DrawJoint(void);
	static void DrawTunnel(void);
	static void DrawArcTunnel(void);
	static void DrawWS(void);

	/* ʵ����DrawTTunnel.cpp�� */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	static void DrawMainFan();
	static void DrawLocalFan();
	static void DrawFanTag();
	static void DrawGate(int flag);
	static void DrawCasement(int flag);
	static void DrawWindBridge();
	static void DrawJoint();
	static void DrawWall(int flag);
	static void DrawStorage(int flag);
	static void DrawDirection(void);
	static void DrawWindLibrary(void);


	static void DrawChimney(void);
	//static void DrawCompass();

	/* ʵ����AutoDraw.cpp�� */
	static void AutoDraw();
	static void AutoDraw2();

	/* �Զ���ע�������� */
	static void AutoDirection();

	static double ControlDirByMethods(AcDbObjectId objId,double angle);

	/* �Զ����²��վ���� */
	static void UpdateAllWindStationData();

	// ͨ�ñ༭����
	static void JunctionEnclose(void);
	static void ReverseDirection();
	static void ReBindEdge();
	static void ZoomMineGE();
	static void SplitTunnel(AcDbObjectId objId,AcGePoint3d spt,AcGePoint3d ept,AcGePoint3d pt);
	static void SplitDeal();
	static void MergeChimney();
	static void Merging(AcDbObjectId sourceId,AcDbObjectId mergeId);
	static void DrawQTagGE();
	static void DrawEffectRanGE();

	static void DeleteDirection();
	static void DeleteFanTag();
	static void DeleteQTag();
	static void DeleteEDTag();

	//static AcDbObjectId GetRelatedTW(AcDbObjectId objId);

	static void testHD();
};