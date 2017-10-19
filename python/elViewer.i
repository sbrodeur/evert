/* File: elViewer.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elViewer.hpp"
%}

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
