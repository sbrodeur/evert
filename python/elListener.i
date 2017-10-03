/* File: elListener.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elListener.hpp"
%}

namespace EL
{

//------------------------------------------------------------------------

class Listener : public OrientedPoint
{
public:
						Listener	(void);
						Listener	(const Listener& s);
						~Listener	(void);

	const Listener&		operator=	(const Listener& s);
};

//------------------------------------------------------------------------
} // namespace EL
