/* File: elTimer.i */
%import "elDefs.hpp"

%{
#define SWIG_FILE_WITH_INIT
#include "elTimer.hpp"
%}

namespace EL {
class Timer
{
public:
			Timer	(void);

	void	clear	(void);
	double	get		(void) const;

private:
	int64_t m_timer;
};

class CPUTimer
{
public:
	EL_FORCE_INLINE			CPUTimer	(void)			{ m_timer = 0; }
	
	EL_FORCE_INLINE	void	clear		(void)			{ m_timer = 0; }
	EL_FORCE_INLINE void	toggle		(void)			{ m_timer = rdtsc() - m_timer; }
	EL_FORCE_INLINE int64_t	getI		(void) const	{ return m_timer; }
	EL_FORCE_INLINE double	get			(void) const	{ return (double)getI() * s_msec; }

	static int64_t			rdtsc		(void);
	void					calibrate	(const Timer& referenceTimer);

private:
	int64_t					m_timer;
	static double			s_msec;		// calibration counter
};
} // namespace EL
