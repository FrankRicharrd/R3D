#include "stdafx.h"
#include "RDrawingEntity.h"

void RGeomInstance::SetShader(osg::ref_ptr<osg::Shader> prgm)
{
	if (!m_shaderprogram.valid())
	{
		m_shaderprogram = new osg::Program;
		getOrCreateStateSet()->setAttributeAndModes(m_shaderprogram.get(), osg::StateAttribute::ON);
	}

	if (prgm->getType() == osg::Shader::Type::VERTEX)
	{
		if (m_vertexShader.valid())
		{
			m_shaderprogram->removeShader(m_vertexShader);
			m_vertexShader = prgm;
			m_shaderprogram->addShader(m_vertexShader);
		}
		else
		{
			m_vertexShader = prgm;
			m_shaderprogram->addShader(m_vertexShader);
		}
	}

	if (prgm->getType() == osg::Shader::Type::FRAGMENT)
	{
		if (m_fragmentShader.valid())
		{
			m_shaderprogram->removeShader(m_fragmentShader);
			m_fragmentShader = prgm;
			m_shaderprogram->addShader(m_fragmentShader);
		}
		else
		{
			m_fragmentShader = prgm;
			m_shaderprogram->addShader(m_fragmentShader);
		}
	}
}

RGeomInstance::RGeomInstance()
{
}

RGeomInstance::RGeomInstance(RGeomDefinitionRef def) : m_definition(def)
{
	bool status = addChild(m_definition);

	setMatrix(osg::Matrix::identity());

}

RGeomInstance::RGeomInstance(const RGeomInstance & copy, const osg::CopyOp & copyop)
	:osg::MatrixTransform(copy, copyop)
{
	m_definition = copy.m_definition;
	m_instanceName = copy.m_instanceName;

}

RGeomInstance::~RGeomInstance()
{
}
