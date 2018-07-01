#pragma once

class RDrawingElementVisitor;
class ConstRDrawingElementVisitor;


/** META_StateAttribute macro define the standard clone, isSameKindAs,
* className and getType methods.
* Use when subclassing from Object to make it more convenient to define
* the standard pure virtual methods which are required for all Object
* subclasses.*/
#define META_RDrawingEntity(library,name) \
        virtual osg::Object* cloneType() const { return new name(); } \
        virtual osg::Object* clone(const osg::CopyOp& copyop) const { return new name (*this,copyop); } \
        virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const name *>(obj)!=NULL; } \
        virtual const char* libraryName() const { return #library; } \
        virtual const char* className() const { return #name; } \
        virtual void accept(RDrawingElementVisitor& sv) { sv.apply(*this); } \
        virtual void accept(ConstRDrawingElementVisitor& csv) const { csv.apply(*this); }


class RPoint3D;
class RVertex3D;
class REdge;
class REdgeloop;
class RFace;
class RDrawableDefinition;
class RDrawableInstance;
class RGeomDefinition;
class RGeomInstance;
class RGeomEntity;
class RMesh;
class RGeomEdgeLoop;


typedef osg::ref_ptr<RPoint3D> RPoint3DRef;
typedef osg::ref_ptr<RVertex3D> RVertex3DRef;
typedef osg::ref_ptr<REdge> REdgeRef;
typedef osg::ref_ptr<REdgeloop> REdgeloopRef;
typedef osg::ref_ptr<RFace> RFaceRef;
typedef osg::ref_ptr<RDrawableDefinition> RDrawableDefinitionRef;
typedef osg::ref_ptr<RDrawableInstance> RDrawableInstanceRef;
typedef osg::ref_ptr<RGeomDefinition> RGeomDefinitionRef;
typedef osg::ref_ptr<RGeomInstance> RGeomInstanceRef;
typedef osg::ref_ptr<RGeomEntity> RGeomEntityRef;
typedef osg::ref_ptr<RMesh> RMeshRef;
typedef osg::ref_ptr<RGeomEdgeLoop> RGeomEdgeLoopRef;



typedef std::vector<RVertex3DRef> RVertex3DCollection;
typedef std::vector<REdgeRef> REdgeCollection;
typedef std::vector<REdgeloopRef> REdgeLoopCollection;
typedef std::vector<RFaceRef> RFaceCollection;
typedef std::vector<RDrawableInstanceRef> RDrawableInstanceCollection;
typedef std::vector<RDrawableDefinitionRef> RDrawableDefinitionCollection;
typedef std::vector<RGeomDefinitionRef> RGeomDefinitionCollection;
typedef std::vector<RGeomInstanceRef> RGeomInstanceCollection;
typedef std::vector<RGeomEntityRef> RGeomEntityCollection;
typedef std::vector<RMeshRef> RMeshCollection;
typedef std::vector<RGeomEdgeLoopRef> RGeomEdgeLoopCollection;



class R3DAPI RDrawingEntity : public osg::Object
{
public:
	RDrawingEntity();
	RDrawingEntity(const RDrawingEntity& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	/** Clone the type of an attribute, with Object* return type.
	Must be defined by derived classes.*/
	virtual Object* cloneType() const = 0;

	/** Clone an attribute, with Object* return type.
	Must be defined by derived classes.*/
	virtual Object* clone(const osg::CopyOp&) const = 0;


	/** return true if this and obj are of the same kind of object.*/
	virtual bool isSameKindAs(const Object* obj) const { return dynamic_cast<const RDrawingEntity*>(obj) != NULL; }

	/** return the name of the attribute's library.*/
	virtual const char* libraryName() const { return "R3DViewer"; }

	/** return the name of the attribute's class type.*/
	virtual const char* className() const { return "RDrawingEntity"; }

	/** accept a non const shape visitor which can be used on non const shape objects.
	Must be defined by derived classes.*/
	virtual void accept(RDrawingElementVisitor&) = 0;

protected:
	virtual ~RDrawingEntity();

};

class R3DAPI RDrawingElementVisitor
{
public:
	RDrawingElementVisitor();
	virtual ~RDrawingElementVisitor();

	virtual void apply(REdge&) {}
	virtual void apply(REdgeloop&) {}
	virtual void apply(RFace&) {}
	virtual void apply(RVertex3D&) {}


private:

};

class R3DAPI ConstRDrawingElementVisitor
{
public:
	ConstRDrawingElementVisitor();
	virtual ~ConstRDrawingElementVisitor();

	virtual void apply(const REdge&) {}
	virtual void apply(const REdgeloop&) {}
	virtual void apply(const RFace&) {}
	virtual void apply(const RVertex3D&) {}

private:

};




class R3DAPI RVertex3D : public RDrawingEntity
{
public:
	RVertex3D();
	RVertex3D(double x, double y, double z);
	RVertex3D(const RVertex3D& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	META_RDrawingEntity(R3DViewer, RVertex3D);

	REdgeCollection& Getedges() { return m_edges; }

	RFaceCollection& Getfaces() { return m_faces; }

	double x() const { return m_x; }
	double y() const { return m_y; }
	double z() const { return m_z; }
	void setx(double x) { m_x = x; }
	void sety(double y) { m_y = y; }
	void setz(double z) { m_z = z; }


	bool AddEdge(REdge* edge);
	bool Insertedge(unsigned int index, REdge *edge);
	bool containsEdge(const REdge* node) const;

	bool AddFace(RFace* edge);
	bool Insertface(unsigned int index, RFace*edge);
	bool containsFace(const RFace* node) const;

	osg::Vec3 GetOsgVec() { return osg::Vec3(m_x, m_y, m_z); }

	void SetIndex(int i) { m_index = i; }

	int GetIndex() { return m_index; }

	void SetRenderIndex(int i) { m_rIndex = i; }

	int GetRenderIndex() { return m_rIndex; }

	osg::Vec3 GetSmoothNormal();

private:
	REdgeCollection m_edges;
	RFaceCollection m_faces;
	double m_x, m_y, m_z;
	int m_index;
	int m_rIndex;
protected:
	~RVertex3D();
};

class R3DAPI REdge : public RDrawingEntity
{
public:
	REdge(RVertex3D* startpoint, RVertex3D* endpoint);
	REdge(const REdge& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	META_RDrawingEntity(R3DViewer, REdge);

	RVertex3DRef GetStartpoint() const { return m_startpoint; }
	RVertex3DRef GetEndpoint() const { return m_endpoint; }

	bool AddFace(RFace* face);
	bool containsFace(const RFace* face) const;

	bool IsSameEdge(REdgeRef Edge);

	// 0 forward, 1 reversed, -1 vertex not match
	int IsReversed(RVertex3DRef startpoint, RVertex3DRef endpoint);

	void SetIndex(int i) { m_index = i; }
	int GetIndex() { return m_index; }

private:
	RVertex3D* m_startpoint;
	RVertex3D* m_endpoint;

	RFaceCollection m_faces;

	bool Insertface(unsigned int index, RFace*face);

	int m_index;

protected:
	virtual ~REdge();
	REdge();

};

//edge loop should be in CCW direction.
class R3DAPI REdgeloop :public RDrawingEntity
{
public:
	REdgeloop(bool isouteredge = true);
	REdgeloop(const REdgeloop& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	META_RDrawingEntity(R3DViewer, REdgeloop);

	//0 means the edge is forward and 1 means reversed. 
	bool AddEdge(REdge* edge, int direction);
	//0 means the edge is forward and 1 means reversed. 
	bool Insertedge(unsigned int index, REdge *edge, int direction);
	bool containsEdge(const REdge* edge) const;

	bool IsOuterEdge() { return  m_isOuter; }
	void SetIsOuterEdge(bool isOuter) { m_isOuter = isOuter; }

	REdgeCollection& GetEdges() { return m_edges; }

	//0 means the edge is forward and 1 means reversed. this direction is with respect to this loop considering that loops must be in CCW direction.
	//use index of the collection to map edges which you get from GetEdges() method.
	std::vector<int>& GetEdgeDirections() { return m_edgeDirection; }

	void SetFace(RFace* face) { m_face = face; }
	RFaceRef GetFace() { return m_face; }


private:
	REdgeCollection m_edges;
	RFace* m_face;
	bool m_isOuter;
	std::vector<int> m_edgeDirection;

protected:
	virtual ~REdgeloop();


};


class R3DAPI RFace :public RDrawingEntity
{
public:
	RFace();
	RFace(const RFace& p, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
	META_RDrawingEntity(R3DViewer, RFace);

	bool AddEdgeLoop(REdgeloop* edgeloop);
	bool InsertedgeLoop(unsigned int index, REdgeloop *edgeloop);
	bool containsEdgeLoop(const REdgeloop* edgeloop) const;

	//send the transformed matrix, this method will decide whether to move the face and returns true if moved
	virtual bool TranslateFace(osg::Vec3 trans) { return false; }

	//this works only for four points face
	osg::Vec3 GetMidPoint();
	osg::Vec3 GetFaceNormal();

	bool AddVertex(RVertex3D* vertex);
	bool InsertVertex(unsigned int index, RVertex3D*vertex);
	bool containsVertex(const RVertex3D* vertex) const;

	REdgeLoopCollection& GetEdgeLoops() { return m_edgeloops; }
	RVertex3DCollection& GetVertices() { return m_vertices; }

	//RVertex3DCollection GetDistinctPoints(REdgeRef e1, REdgeRef e2);

	osg::Vec4& GetColor() { return m_color; }

	int GetIndex() { return m_index; }
	void SetIndex(int i) { m_index = i; }

	RDrawableDefinitionRef GetDefinition() { return m_definition; }
	void SetDefinition(RDrawableDefinition* definition) { m_definition = definition; }

	std::vector<int>& GetVertexRenderIndex() { return m_vertexRenderIndex; }
	std::vector<int>& GetSmoothenVertexIndex() { return m_smoothenVertexIndices; }

protected:
	virtual ~RFace();
	RDrawableDefinition* m_definition;
	REdgeLoopCollection m_edgeloops;
	RVertex3DCollection m_vertices;
	int m_index;
	osg::Vec4 m_color;

	//the values of these indices will be in geometry getvertices
	std::vector<int> m_vertexRenderIndex;
	
	//the values of the below vector shows wheather a vertex is smoothened. 0 not smooth and 1 if smooth. index refers to the values in m_vertices
	std::vector<int> m_smoothenVertexIndices;


};


class R3DAPI RDrawableDefinition : public osg::Geometry
{
public:
	RDrawableDefinition();

	RDrawableDefinition(const RDrawableDefinition& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RDrawableDefinition);


	bool AddInstance(RDrawableInstance* instance);
	bool containsInstance(const RDrawableInstance* instance) const;
	bool AddEdge(REdge* edge);
	bool Insertedge(unsigned int index, REdge *edge);
	bool containsEdge(const REdge* node) const;
	bool AddEdgeLoop(REdgeloop* edgeloop);
	bool InsertedgeLoop(unsigned int index, REdgeloop *edgeloop);
	bool containsEdgeLoop(const REdgeloop* edgeloop) const;

	bool AddVertex(RVertex3D* vertex);
	bool InsertVertex(unsigned int index, RVertex3D*vertex);
	bool containsVertex(const RVertex3D* vertex) const;

	bool AddFace(RFace* face);
	bool InsertFace(unsigned int index, RFace*face);
	bool containsFace(const RFace* face) const;

	virtual void Selected(bool isSelected);

	bool Redraw(bool initDraw = false);

	RFaceCollection& Getfaces() { return m_faces; }
	REdgeCollection& GetEdges() { return m_edges; }

	osg::ref_ptr<osg::Vec3Array> GetDrawableVertices() { return m_drawVertexCol; }

	RVertex3DCollection& GetVertices() { return m_vertices; }

	osg::Vec4& GetColor() { return m_color; }

	bool HasColor() { if (m_color.x() == 0 && m_color.y() == 0 && m_color.z() == 0) { return false; }return true; }

protected:
	RVertex3DCollection m_vertices;
	REdgeCollection m_edges;
	REdgeLoopCollection m_edgeloops;
	RFaceCollection m_faces;
	RDrawableInstanceCollection m_instances;
	osg::ref_ptr<osg::Vec3Array> m_drawVertexCol;
	osg::ref_ptr<osg::Vec3Array> m_drawNormalArray;

	bool InsertInstance(unsigned int index, RDrawableInstance *instance);

	osg::ref_ptr<osg::Material> m_material;
	osg::Vec4 m_color;
	bool m_smoothenNormals;

protected:
	virtual ~RDrawableDefinition();
};

class R3DAPI RDrawableInstance : public osg::MatrixTransform
{
public:
	RDrawableInstance(RDrawableDefinition* definition);
	RDrawableInstance(const RDrawableInstance& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RDrawableInstance);

	RDrawableDefinitionRef GetDefinition() { return m_definition; }

	osg::Matrix GetLocaltoWorldMatrix();



	bool Redraw(bool initDraw = false);

	int GetIndexInDefinition() { return m_Defindex; }
	void SetIndexInDefinition(int i) { m_Defindex = i; }


protected:
	RDrawableInstance();
	~RDrawableInstance();

private:
	RDrawableDefinition* m_definition;
	osg::ref_ptr<osg::Geometry> m_lineGeom;

	int m_Defindex;

};


class R3DAPI RGeomEntity : public osg::Geometry
{
public:
	RGeomEntity();
	RGeomEntity(const RGeomEntity& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RGeomEntity);



protected:

	~RGeomEntity();

};


class R3DAPI RGeomEdgeLoop : public RGeomEntity
{
public:
	RGeomEdgeLoop(RGeomEntityRef m_parent);
	RGeomEdgeLoop();
	RGeomEdgeLoop(const RGeomEdgeLoop& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RGeomEdgeLoop);

	

private:
	RGeomEntityRef m_parent;

protected:

	~RGeomEdgeLoop();

};

//geometry entity contained inside a geode which holds all of its vertices and other datas for single draw call rendering
class R3DAPI RMesh : public RGeomEntity
{
public:
	RMesh(RGeomDefinitionRef def);
	RMesh();
	RMesh(const RMesh& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RMesh);

	void AddEdgeLoop(RGeomEdgeLoopRef e)
	{
		m_outerEdges.push_back(e);
	}

	RGeomEdgeLoopCollection GetOuterEdgeLoops();

private:
	RGeomEdgeLoopCollection m_outerEdges;
	RGeomDefinitionRef m_definition;

protected:

	~RMesh();

};

//unique edge whose points wont be in contact with the face
class R3DAPI RGeomEdge : public RGeomEntity
{
public:
	RGeomEdge();
	RGeomEdge(const RGeomEdge& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RGeomEdge);



protected:

	~RGeomEdge();

};




class R3DAPI RGeomDefinition : public osg::Geode
{
public:
	RGeomDefinition();
	RGeomDefinition(const RGeomDefinition& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RGeomDefinition);

	RGeomInstanceRef CreateInstance();
	
	bool AddGeomEntity(RGeomEntityRef entity);
	bool AddGeomInstanceChild(RGeomInstanceRef inst);
	//bool AddGeomDefinitionAsChild(RGeomDefinitionRef defasChild);

	
	void SetName(std::string str) { m_name = str; setName(str); }

	RMeshRef GetMesh() { return m_mesh.get(); }

private:
	RGeomInstanceCollection m_instances;

	RGeomEntityCollection m_geomEntities;

	RMeshRef m_mesh;

	RGeomInstanceCollection m_children;
	//RGeomDefinitionCollection m_DefinitionsAschildren;


	std::string m_name;

protected:
	
	~RGeomDefinition();

};



class R3DAPI RGeomInstance : public osg::MatrixTransform
{
public:
	//dont init it. create a RGeomDefinition and use its method createinstance() to get the RGeomInstance
	RGeomInstance(RGeomDefinitionRef def);
	RGeomInstance(const RGeomInstance& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

	META_Object(R3DViewer, RGeomInstance);

	void SetDefinition(RGeomDefinitionRef def) { m_definition = def; }

	std::string GetInstanceName() {
		return m_instanceName;
	}

	void SetInstanceName(std::string name) { m_instanceName = name; }

	std::string GetDefinitionName() {
		auto n = m_definition->getName();
		return n;
	}

	void SetShader(osg::ref_ptr<osg::Shader> prgm);

	RGeomDefinitionRef GetDefinition() { return m_definition; }

private:
	RGeomDefinitionRef m_definition;

	std::string m_instanceName;

	osg::ref_ptr<osg::Program> m_shaderprogram;
	osg::ref_ptr<osg::Shader> m_vertexShader;
	osg::ref_ptr<osg::Shader> m_fragmentShader;

protected:
	RGeomInstance();
	~RGeomInstance();

};


