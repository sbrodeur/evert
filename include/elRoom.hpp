#ifndef __ELROOM_HPP
#define __ELROOM_HPP
/******************************************************************************
 *
 * Copyright (c) 2004-2005, Samuli Laine
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  - Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined (__ELLISTENER_HPP)
#	include "elListener.hpp"
#endif
#if !defined (__ELSOURCE_HPP)
#	include "elSource.hpp"
#endif
#if !defined (__ELPOLYGON_HPP)
#	include "elPolygon.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

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

	bool					import				(const char* filename);
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

#endif // __ELROOM_HPP
