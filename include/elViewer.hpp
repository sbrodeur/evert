#ifndef __ELVIEWER_HPP
#define __ELVIEWER_HPP
/******************************************************************************
 *
 * Copyright (c) 2004-2005, Samuli Laine
 *                    2017, Simon Brodeur
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

#if !defined (__ELROOM_HPP)
#	include "elRoom.hpp"
#endif
#if !defined (__ELPATHSOLUTION_HPP)
#	include "elPathSolution.hpp"
#endif

//------------------------------------------------------------------------
namespace EL {
//--------------------------------------------------------------------

class Viewer;
static Viewer* currentInstance;

class Viewer
{
public:

	//--------------------------------------------------------------------

	Viewer				(Room* room, const int maxorder);
	~Viewer				(void);

	void show(void);
	void display(void);

private:

	Room*			g_room;
	PathSolution*	g_solution;
	int				g_maxorder;

	static void displayCallback(void){ currentInstance->display(); }

};

//--------------------------------------------------------------------
} // namespace EL

#endif // __ELVIEWER_HPP
