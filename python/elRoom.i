/* File: elRoom.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elRoom.hpp"
%}

%include "std_vector.i"

// Instantiate used templates
%template(ElementVector) std::vector<EL::Element>;
%template(SourceVector) std::vector<EL::Source>;
%template(ListenerVector) std::vector<EL::Listener>;

namespace EL
{

//--------------------------------------------------------------------

struct Element
{
	Polygon	m_polygon;
	Vector3	m_color;
};

class BSP;
class Room
{
public:

	//--------------------------------------------------------------------

							Room				(void);
							~Room				(void);

	bool					importGeometry		(const char* filename);
	void					addPolygon			(Polygon& poly, const Vector3& color);
	void 					constructBSP		(void);
	
	int						numElements			(void) const	{ return m_elements.size(); }
	const Element&			getElement			(int i) const	{ EL_ASSERT(i >= 0 && i < numElements()); return m_elements[i]; }
	Element&				getElement			(int i)			{ EL_ASSERT(i >= 0 && i < numElements()); return m_elements[i]; }

	int						numConvexElements	(void) const	{ return m_convexElements.size(); }
	const Element&			getConvexElement	(int i) const	{ EL_ASSERT(i >= 0 && i < numConvexElements()); return m_convexElements[i]; }
	Element&				getConvexElement	(int i)			{ EL_ASSERT(i >= 0 && i < numConvexElements()); return m_convexElements[i]; }

	int						numSources			(void) const	{ return m_sources.size(); }
	void					addSource			(const Source& s) { m_sources.push_back(s); }
	const Source&			getSource			(int i) const	{ EL_ASSERT(i >= 0 && i < numSources()); return m_sources[i]; }
	Source&					getSource			(int i)			{ EL_ASSERT(i >= 0 && i < numSources()); return m_sources[i]; }

	int						numListeners		(void) const	{ return m_listeners.size(); }
	void					addListener			(const Listener& l) { m_listeners.push_back(l); }
	const Listener&			getListener			(int i) const	{ EL_ASSERT(i >= 0 && i < numListeners()); return m_listeners[i]; }
	Listener&				getListener			(int i)			{ EL_ASSERT(i >= 0 && i < numListeners()); return m_listeners[i]; }

	void					getBoundingBox		(Vector3& mn, Vector3& mx) const;
	float					getMaxLength		(void) const;
	Vector3					getCenter			(void) const;

	const BSP&				getBSP				(void) const	{ return *m_bsp; }
	void					render				(void) const;

private:
	std::vector<Element>	m_elements;
	std::vector<Element>	m_convexElements;
	std::vector<Source>		m_sources;
	std::vector<Listener>	m_listeners;
	BSP*					m_bsp;
};

//------------------------------------------------------------------------
} // namespace EL
