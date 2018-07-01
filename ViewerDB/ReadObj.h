#pragma once

typedef std::map< std::string, osg::ref_ptr<osg::StateSet> > MaterialToStateSetMap;

class VIEWERDB_EXPORT ReadObj
{
public:
	ReadObj(std::string filePath, std::string basePath);
	~ReadObj();
	RGeomDefinitionRef ReadFile();


private:
	std::string m_filePath;
	std::string m_basePath;

};
