#include "stdafx.h"
#include "FGlobalData.h"
#include "SelectionManager.h"
#include "RModelingManager.h"
#include "RMaterialManager.h"
#include "RLightsManager.h"
FGlobalData::FGlobalData()
{
	m_filesystemManager = new FileSystemsManager(this);
	m_dragger = new DraggerManager(this);

	m_modelingmanager = new RModelingManager(this);
	m_materialmanager = new RMaterialManager(this);
	m_LightsManager = new RLightsManager(this);
	//LoadViewerDBDll();

}


FGlobalData::~FGlobalData()
{
	delete m_filesystemManager;
	delete m_modelingmanager;
	delete m_dragger;
	delete m_materialmanager;
	delete m_LightsManager;
}

void FGlobalData::SetSelectedNode(osg::MatrixTransform* selNOde)
{
	//if nothing selected
	if (!selNOde)
	{
		m_selectednode = selNOde;
		return;
	}

	//if previously nothing is selected
	if (!m_selectednode)
	{
		m_selectednode = selNOde;
		m_dragger->SetDraggerToRendererParent();
		return;
	}

	//if already a node is selected
	if (m_selectednode != selNOde)
	{
		m_selectednode = selNOde;
		m_dragger->SetDraggerToRendererParent();
	}

}







DraggerManager * FGlobalData::GetDragger()
{
	if (!m_dragger.valid())
	{
		m_dragger = new DraggerManager(this);
	}
	return m_dragger;
}

osg::Matrix FGlobalData::GetModelViewProjectionMatrix()
{
		osg::Matrix MVPW(GetCamera()->getViewMatrix() *
			GetCamera()->getProjectionMatrix() *
			GetCamera()->getViewport()->computeWindowMatrix());

		return MVPW;

	/*
		osg::Matrixd inverseMVPW = osg::Matrixd::inverse(MVPW);
	
		osg::Vec3 posIn2D = osg::Vec3(0, 0, 0) * MVPW;
	
		osg::Vec3 near_point = osg::Vec3(x, y, 0.0f) * inverseMVPW;
		osg::Vec3 far_point = osg::Vec3(x, y, 1.0f) * inverseMVPW;
		osg::Vec3 ss = osg::Vec3(x, y, -1.0f) * inverseMVPW;*/
}


