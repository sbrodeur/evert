/* File: elSource.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elSource.hpp"
%}

namespace EL
{

//------------------------------------------------------------------------

class Source : public OrientedPoint
{
public:
						Source		(void);
						Source		(const Source& s);
						~Source		(void);

	const Source&		operator=	(const Source& s);
};

//------------------------------------------------------------------------
} // namespace EL
