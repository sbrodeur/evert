#ifndef __ELBEAM_HPP
#define __ELBEAM_HPP
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

#if !defined (__ELPOLYGON_HPP)
#	include "elPolygon.hpp"
#endif
#if !defined (__ELVECTOR_HPP)
#	include "elVector.hpp"
#endif

namespace EL
{

//------------------------------------------------------------------------

class Beam
{
public:
							Beam			(void);
							Beam			(const Vector3& top, const Polygon& polygon);
							Beam			(const Beam& beam);
							~Beam			(void);

	const Beam&				operator=		(const Beam& beam);

	const Vector3&			getTop			(void) const	{ return m_top; }
	const Polygon&			getPolygon		(void) const	{ return m_polygon; }
	int						numPleqs		(void) const	{ return (int)m_pleqs.size(); }
	const Vector4&			getPleq			(int i) const	{ EL_ASSERT(i >= 0 && i < numPleqs()); return m_pleqs[i]; }

	void					render			(const Vector3& color) const;
	EL_FORCE_INLINE bool	contains		(const Vector3& p) const
	{
		for (int i=0; i < numPleqs(); i++)
			if (dot(p, getPleq(i)) < 0.f)
				return false;
		return true;
	}

private:
	void					calculatePleqs	(void);

	Vector3					m_top;
	Polygon					m_polygon;
	std::vector<Vector4>	m_pleqs;
};

//------------------------------------------------------------------------
} // namespace EL

#endif // __ELBEAM_HPP
