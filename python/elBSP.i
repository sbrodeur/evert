/* File: elBSP.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elBSP.hpp"
%}

%include "std_vector.i"

// Instantiate used templates
%template(ConstPolygonPtrVector) std::vector<const EL::Polygon*>;

namespace EL
{

//------------------------------------------------------------------------

class Beam;
class Polygon;
class Ray;
class BSP
{
public:
					BSP						(void);
					~BSP					(void);

	void			constructHierarchy		(const Polygon** polygons, int numPolygons);

	void			beamCast				(const Beam& beam, std::vector<const Polygon*>& result) const;
	const Polygon*	rayCast					(const Ray& ray) const;
	bool			rayCastAny				(const Ray& ray) const;
	static Vector3	getIntersectionPoint	(void);

	class TempNode;
private:
	TempNode*		m_hierarchy;
	uintptr_t*	m_list;
	AABB			m_aabb;
};

//------------------------------------------------------------------------
} // namespace EL
