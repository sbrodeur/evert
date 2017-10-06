/* File: elPolygon.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elPolygon.hpp"
%}

%include "std_vector.i"

// Instantiate used templates
%template(Vector3Vector) std::vector<EL::Vector3>;
%template(PolygonVector) std::vector<EL::Polygon>;
%template(IntVector) std::vector<int>;
%template(IntVectorVector) std::vector< std::vector<int> >;

%extend EL::Polygon{
    EL_FORCE_INLINE Vector3& __getitem__(int i) {
        return (*self)[i];
    }
    EL_FORCE_INLINE const Vector3& __getitem__(int i) const {
        return (*self)[i];
    }
}

namespace EL
{

//------------------------------------------------------------------------

class AABB;
class Beam;
class Polygon
{
public:
									Polygon		(void);
									Polygon		(const Polygon& p);
									Polygon		(const Vector3* points, int numPoints);
									Polygon		(const Vector3* points, int numPoints, const Vector4& pleq);
									Polygon		(const Vector3* points, int numPoints, const Vector4& pleq, unsigned int materialId);
									Polygon		(const std::vector<Vector3>& points);
									Polygon		(const std::vector<Vector3>& points, unsigned int materialId);
									~Polygon	(void);

	const Polygon&					operator=	(const Polygon& p);

	int								numPoints	(void) const  { return m_points.size(); }
	EL_FORCE_INLINE Vector3&		operator[]	(int i)		  { EL_ASSERT(i >= 0 && i < (int)m_points.size()); return m_points[i]; }
	EL_FORCE_INLINE const Vector3&	operator[]	(int i) const { EL_ASSERT(i >= 0 && i < (int)m_points.size()); return m_points[i]; }
	const Vector4&					getPleq		(void) const  { return m_pleq;   }
	const Vector3&					getNormal	(void) const  { return *((const Vector3*)&m_pleq); }
	bool							isConvex	(void) const;
	float							getNonPlanarity(void) const;
	float							getArea		(void) const;
	AABB							getAABB		(void) const;

	void							triangulate	(std::vector<Polygon>& triangles);
	void							splitConvex	(std::vector<Polygon>& polygons);

	enum ClipResult
	{
		CLIP_CLIPPED,
		CLIP_ORIGINAL,
		CLIP_VANISHED
	};

	ClipResult						clip		(const Vector4& pleq);
	ClipResult						clip		(const AABB& aabb);
	ClipResult						clip		(const Beam& beam);

	void							render		(const Vector3& color) const;

	unsigned int					getMaterialId (void) const;
	void							setMaterialId (unsigned int id);

private:
	void							calculatePleq(void);
	static bool						mergePartials(std::vector<std::vector<int> >& partials, const std::vector<Vector3>& vertices);

	static ClipResult				clipInner	(const Vector3* inPoints, int numInPoints,
												 Vector3* outPoints, int& numOutPoints,
												 const Vector4& pleq);

	static std::vector<Vector3>		s_clipBuffer[2];
	std::vector<Vector3>			m_points;
	Vector4							m_pleq;
	unsigned int					m_materialId;
};

//------------------------------------------------------------------------
} // namespace EL
