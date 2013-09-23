## -*- mode: autoconf -*-

## 
## This file is part of the Score-P software (http://www.score-p.org)
##
## Copyright (c) 2009-2012, 
##    RWTH Aachen University, Germany
##    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
##    Technische Universitaet Dresden, Germany
##    University of Oregon, Eugene, USA
##    Forschungszentrum Juelich GmbH, Germany
##    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
##    Technische Universitaet Muenchen, Germany
##
## See the COPYING file in the package base directory for details.
##


## individual timer tests

AC_DEFUN([AC_SCOREP_TIMER_BGQ_MFTB_AVAILABLE],[
ac_scorep_timer_bgq_mftb_available="no"
AC_MSG_CHECKING([for bgq_mftb timer])
ac_scorep_timer_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[[
/* DEFAULT_FREQ_MHZ in firmware/include/personality.h
 * __mftb is build-in, see
 * http://publib.boulder.ibm.com/infocenter/comphelp/v101v121/index.jsp?topic=/com.ibm.xlcpp101.aix.doc/compiler_ref/bif_mftb.html
 */
#include <firmware/include/personality.h>
#ifndef DEFAULT_FREQ_MHZ
#error "DEFAULT_FREQ_MHZ not defined. Check your includes."
#endif
]], [[
double clockspeed      = 1.0e-6 / DEFAULT_FREQ_MHZ;
double time_in_seconds = ( __mftb() * clockspeed );
]])],
                  [ac_scorep_timer_bgq_mftb_available="yes"],
                  [])
CPPFLAGS="$ac_scorep_timer_save_CPPFLAGS"
AC_MSG_RESULT([$ac_scorep_timer_bgq_get_timebase_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_BGP_GET_TIMEBASE_AVAILABLE],[
ac_scorep_timer_bgp_get_timebase_available="no"
AC_MSG_CHECKING([for bgp_get_timebase timer])
ac_scorep_timer_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/arch/include"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <common/bgp_personality.h>
#include <common/bgp_personality_inlines.h>
#include <spi/kernel_interface.h>]],
                                [[_BGP_Personality_t mybgp;
    Kernel_GetPersonality( &mybgp, sizeof( _BGP_Personality_t ) );
    long ticks_per_sec = ( long )BGP_Personality_clockMHz( &mybgp ) * 1e6;
    _bgp_GetTimeBase();]])],
               [ac_scorep_timer_bgp_get_timebase_available="yes"
                AC_SUBST([TIMER_LIBDIR], ["/bgsys/drivers/ppcfloor/runtime/SPI"])], [])
CPPFLAGS="$ac_scorep_timer_save_CPPFLAGS"
AC_MSG_RESULT([$ac_scorep_timer_bgp_get_timebase_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_CLOCK_GETTIME_AVAILABLE],[
ac_scorep_timer_clock_gettime_available="no"
ac_scorep_timer_clock_gettime_clock=""

ac_scorep_timer_save_LIBS="$LIBS"
AC_SEARCH_LIBS([clock_gettime], [rt],
               [AS_IF([test "x${ac_cv_search_clock_gettime}" != "xnone required"],
                      [ac_scorep_timer_librt="$ac_cv_search_clock_gettime"])])

m4_foreach([clock],
          [[CLOCK_REALTIME], [CLOCK_MONOTONIC], [CLOCK_MONOTONIC_RAW]],
          [AC_MSG_CHECKING([for clock])
           AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
#ifdef _POSIX_C_SOURCE
#  if _POSIX_C_SOURCE < 199309L
#    undef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 199309L
#  endif
#else
#  define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>
                                                      ], 
                                                      [
    struct timespec tp;
    clock_getres(  clock, &tp );
    clock_gettime( clock, &tp );
                                                      ])], 
                                     [ac_scorep_timer_clock_gettime_clock="clock"
                                      AC_MSG_RESULT([yes])], 
                                     [AC_MSG_RESULT([no])])
])dnl

# perform a final link test
AS_IF([test "x${ac_scorep_timer_clock_gettime_clock}" != "x"],
      [AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#ifdef _POSIX_C_SOURCE
#  if _POSIX_C_SOURCE < 199309L
#    undef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 199309L
#  endif
#else
#  define _POSIX_C_SOURCE 199309L
#endif
#include <time.h>]], [[
    struct timespec tp;
    clock_getres( $ac_scorep_timer_clock_gettime_clock , &tp );
    clock_gettime( $ac_scorep_timer_clock_gettime_clock, &tp );
]])], [ac_scorep_timer_clock_gettime_available="yes"
       AC_DEFINE([HAVE_CLOCK_GETTIME], [1], 
                 [Defined to 1 if the clock_gettime() function is available.])
       AC_DEFINE_UNQUOTED([SCOREP_CLOCK_GETTIME_CLOCK], 
                          [${ac_scorep_timer_clock_gettime_clock}],
                          [The clock used in clock_gettime calls.])])])
AC_MSG_CHECKING([for clock_gettime timer])
AC_MSG_RESULT([$ac_scorep_timer_clock_gettime_available])

LIBS="$ac_scorep_timer_save_LIBS"

])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_CRAY_RTCLOCK_AVAILABLE],[
# not thouroghly tested yet. Do we need to link a particular library?
ac_scorep_timer_cray_rtclock_available="no"
AC_MSG_CHECKING([for cray_rtclock timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <time.h>
#include <unistd.h>]],
                                [[double clockspeed = 1.0 / sysconf( _SC_CLK_TCK );
double wtime = rtclock() * clockspeed;]])],
               [ac_scorep_timer_cray_rtclock_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_cray_rtclock_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_CYCLE_COUNTER_ITC_AVAILABLE],[
# Check for the itc cycle conter on __ia64__ platforms. See also the tsc
# timer:AC_SCOREP_TIMER_CYCLE_COUNTER_TSC_AVAILABLE
ac_scorep_timer_cycle_counter_itc_available="no"
AC_MSG_CHECKING([for cycle_counter_itc timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <asm/intrinsics.h>]],
                                [[#ifdef __ia64__
volatile long long r = (long long) __getReg(_IA64_REG_AR_ITC);
#else
#error "This test makes sense only on __ia64__"
#endif]])],
               [ac_scorep_timer_cycle_counter_itc_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_cycle_counter_itc_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_CYCLE_COUNTER_TSC_AVAILABLE],[
# According to http://en.wikipedia.org/wiki/Time_Stamp_Counter the TSC timer
# is no longer recommended, at least for x86 processors.
# We can't really check for TSC without running a program on the
# backend. A pragmatic solution is to check the cpu type.
# See also the itc timer: AC_SCOREP_TIMER_CYCLE_COUNTER_ITC_AVAILABLE
ac_scorep_timer_cycle_counter_tsc_available="yes"
AC_MSG_CHECKING([for cycle_counter_tsc timer])
AS_CASE([$host_cpu],
        [i*86 | x86* | powerpc*], [AS_IF([test -e /proc/cpuinfo], 
                                         [${GREP} constant_tsc /proc/cpuinfo | ${GREP} -v nonstop_tsc > /dev/null
                                          AS_IF([test $? -eq 0], 
                                                [ac_scorep_timer_cycle_counter_tsc_available="no"])],
                                         [#no /proc/cpuinfo
                                          ac_scorep_timer_cycle_counter_tsc_available="no"])],
                                  [ac_scorep_timer_cycle_counter_tsc_available="no"])

AC_MSG_RESULT([$ac_scorep_timer_cycle_counter_tsc_available])

AH_TEMPLATE([HAVE_USLEEP],
            [Define to 1 if the usleep() function is available.])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <unistd.h>]],
                                   [[useconds_t secs = 100; usleep(secs);]])],
                  [AC_DEFINE([HAVE_USLEEP])], [])
])

############################################################################### 

AC_DEFUN([AC_SCOREP_TIMER_CRAY_DCLOCK_AVAILABLE],[
# not tested yet as I don't have access to a cray xt
ac_scorep_timer_cray_dclock_available="no"
AC_MSG_CHECKING([for cray_dclock timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#ifndef __LIBCATAMOUNT__
#error "__LIBCATAMOUNT__ not defined."
#endif
#include <catamount/dclock.h>
]],
                                [[double wtime = dclock();]])],
               [ac_scorep_timer_cray_dclock_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_cray_dclock_available])
])

############################################################################### 

AC_DEFUN([AC_SCOREP_TIMER_SUN_GETHRTIME_AVAILABLE],[
## The gethrtime() function returns the current high-resolution real
## time. Time is expressed as nanoseconds since some arbitrary time in the
## past; it is not correlated in any way to the time of day, and thus is not
## subject to resetting or drifting by way of adjtime(2) or
## settimeofday(3C). The hi-res timer is ideally suited to performance
## measurement tasks, where cheap, accurate interval timing is required.
ac_scorep_timer_sun_gethrtime_available="no"
AC_MSG_CHECKING([for sun_gethrtime timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/time.h>]],
                                [[hrtime_t wtime = gethrtime();]])],
               [ac_scorep_timer_sun_gethrtime_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_sun_gethrtime_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_GETTIMEOFDAY_AVAILABLE],[
ac_scorep_timer_gettimeofday_available="no"
AH_TEMPLATE([HAVE_GETTIMEOFDAY],
            [Define to 1 if the gettimeofday() function is available.])
AC_MSG_CHECKING([for gettimeofday timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include <sys/time.h>
                                ]], [[
struct timeval tp; gettimeofday( &tp, 0 );
                                ]])], [
ac_scorep_timer_gettimeofday_available="yes"
AC_DEFINE([HAVE_GETTIMEOFDAY])
                                ], [])
AC_MSG_RESULT([$ac_scorep_timer_gettimeofday_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_INTEL_MMTIMER_AVAILABLE],[
ac_scorep_timer_intel_mmtimer_available="no"
# cannot check for file existence when cross compiling
AS_IF([test "x${ac_scorep_cross_compiling}" = "xno"],
      [mmtimer_header_available="no"
       AC_CHECK_HEADERS([mmtimer.h], [mmtimer_header_available="yes"],
                        [AC_CHECK_HEADERS([linux/mmtimer.h], [mmtimer_header_available="yes"],
                                          [AC_CHECK_HEADERS([sn/mmtimer.h], [mmtimer_header_available="yes"])])])
       AS_IF([test "x${mmtimer_header_available}" = "xyes"],
             [AS_IF([test -r /dev/mmtimer], [ac_scorep_timer_intel_mmtimer_available="yes"])])])

AC_MSG_CHECKING([for intel_mmtimer timer])
AC_MSG_RESULT([$ac_scorep_timer_intel_mmtimer_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_PAPI_REAL_CYC_AVAILABLE],[
ac_scorep_timer_papi_real_cyc_available="no"
AC_MSG_CHECKING([for papi_real_cyc timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#error "not yet implemented, define SCOREP_PAPI first"]],
                                [[]])],
               [ac_scorep_timer_papi_real_cyc_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_papi_real_cyc_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_PAPI_REAL_USEC_AVAILABLE],[
ac_scorep_timer_papi_real_usec_available="no"
AC_MSG_CHECKING([for papi_real_usec timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#error "not yet implemented, define SCOREP_PAPI first"]],
                                [[]])],
               [ac_scorep_timer_papi_real_usec_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_papi_real_usec_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_IBM_REALTIME_AVAILABLE],[
## The read_real_time() subroutine and the time_base_to_time() conversion
## subroutine are part of the Standard C Library on IBM AIX systems. They are
## probably not found on non-AIX systems.
## They are designed to measure elapsed time using the processor real time
## clock, or time base registers.
## On IBM SP systems, the resolution is one nanosecond.
ac_scorep_timer_ibm_realtime_available="no"
AC_MSG_CHECKING([for ibm_realtime timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/time.h>]],
                                [[timebasestruct_t t;
read_real_time( &t, TIMEBASE_SZ );
time_base_to_time( &t, TIMEBASE_SZ );]])],
               [ac_scorep_timer_ibm_realtime_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_ibm_realtime_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_CRAY_RTC_AVAILABLE],[
# not tested yet as I don't have access to a cray x1
ac_scorep_timer_cray_rtc_available="no"
AC_MSG_CHECKING([for cray_rtc timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <intrinsics.h>]],
                                [[double clockspeed = 1.0 / sysconf( _SC_SV2_USER_TIME_RATE );
unsigned long long wtime = _rtc();]])],
               [ac_scorep_timer_cray_rtc_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_cray_rtc_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_BGL_RTS_GET_TIMEBASE_AVAILABLE],[
# not tested yet as I don't have access to a bgl
ac_scorep_timer_bgl_rts_get_timebase_available="no"
ac_scorep_timer_save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -I/bgl/BlueLight/ppcfloor/bglsys/include"
AC_MSG_CHECKING([for bgl_rts_get_timebase timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <bglpersonality.h>
#include <rts.h>]],
                                [[BGLPersonality mybgl;
rts_get_personality(&mybgl, sizeof(BGLPersonality));
double clockspeed = 1.0/(double)BGLPersonality_clockHz(&mybgl);
double wtime = rts_get_timebase() * clockspeed;]])],
               [ac_scorep_timer_bgl_rts_get_timebase_available="yes"], [])
CPPFLAGS="$ac_scorep_timer_save_CPPFLAGS"
AC_MSG_RESULT([$ac_scorep_timer_bgl_rts_get_timebase_available])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_IBM_SWITCH_CLOCK_AVAILABLE],[
ac_scorep_timer_ibm_switch_clock_available="no"

ac_scorep_timer_save_LIBS="$LIBS"
AC_SEARCH_LIBS([swclockInit], [swclock], [ac_scorep_timer_have_libswclock="yes"])
LIBS="$ac_scorep_timer_save_LIBS"

if test "x${ac_scorep_timer_have_libswclock}" = "xyes"; then
    ac_scorep_timer_libswclock="$ac_cv_search_swclockInit"

    AC_MSG_CHECKING([for ibm_switch_clock timer])

    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <swclock.h>]],
                                    [[swclock_handle_t handle = swclockInit();
swclockRead(handle);]])],
                   [ac_scorep_timer_ibm_switch_clock_available="yes"], [])
    AC_MSG_RESULT([$ac_scorep_timer_ibm_switch_clock_available])
fi
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_NEC_SYSSX_HGTIME_AVAILABLE],[
# not tested yet as I don't have access to a nec sx
ac_scorep_timer_nec_syssx_hgtime_available="no"
AC_MSG_CHECKING([for nec_syssx_hgtime timer])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/syssx.h>]],
                                [[unsigned long long val;
syssx(HGTIME, &val);]])],
               [ac_scorep_timer_nec_syssx_hgtime_available="yes"], [])
AC_MSG_RESULT([$ac_scorep_timer_nec_syssx_hgtime_available])
])

###############################################################################

#using this macro exceeds the autoreconfs recursion limit ...
AC_DEFUN([AC_SCOREP_TIMER_CHECK], [
ac_scorep_timer_$1="no"
m4_expand(m4_join([], [AC_SCOREP_TIMER_], m4_toupper($1), [_AVAILABLE]))
])


###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_USE_IF_AVAILABLE], [[test "x$ac_scorep_timer_$1_available" = "xyes"],[
ac_scorep_timer_given="yes"
ac_scorep_timer_$1="yes"
ac_scorep_timer="$1"
AC_MSG_NOTICE([Selecting platform default $1 timer])]
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_PLATFORM_DEFAULTS], [
AS_IF([test "x${ac_scorep_user_timer_given}" = "xno"], [
    AS_CASE([${ac_scorep_platform}], 
            ["altix"],    [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([intel_mmtimer]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([clock_gettime]))],
            ["bgl"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([bgl_rts_get_timebase]))],
            ["bgp"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([bgp_get_timebase]))],
            ["bgq"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([bgq_mftb]))],
            ["crayxt"],   [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([cray_dclock]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([cycle_counter_itc]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([clock_gettime]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([gettimeofday]))],
            ["solaris"],  [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([sun_gethrtime]))],
            ["mac"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([cycle_counter_itc]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([gettimeofday]))],
            ["aix"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([ibm_realtime]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([ibm_switch_clock]))],
            ["necsx"],    [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([nec_syssx_hgtime]))],
            ["linux"],    [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([cycle_counter_itc]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([clock_gettime]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([gettimeofday]))],
            ["arm"],      [AS_IF(AC_SCOREP_TIMER_USE_IF_AVAILABLE([cycle_counter_itc]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([clock_gettime]),
                                 AC_SCOREP_TIMER_USE_IF_AVAILABLE([gettimeofday]))],
            [AC_MSG_ERROR([Encountered unsupported platform "$ac_scorep_platform" during timer checks.])])
])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_ARG_ENABLE], [
AC_ARG_ENABLE([timer-$1],
              [AS_HELP_STRING([--enable-timer-$1],
                              [enable $1 timer if available instead of platform default])],
              [if test "x${ac_scorep_user_timer_given}" = "xyes"; then
                   AC_MSG_WARN([Several timers provided, using $ac_scorep_timer])
               else
                   if test "x${ac_scorep_timer_$1_available}" = "xno"; then
                       AC_MSG_WARN([User selected $1 timer not available])
                   else
                       AC_MSG_NOTICE([User selected $1 timer available])
                       ac_scorep_user_timer_given="yes"
                       ac_scorep_timer="$1"
                       ac_scorep_timer_$1="yes"
                   fi
               fi])
])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER_ENABLE_SPECIFIC], [

AC_SCOREP_TIMER_ARG_ENABLE([bgl_rts_get_timebase])
AC_SCOREP_TIMER_ARG_ENABLE([bgq_mftb])
AC_SCOREP_TIMER_ARG_ENABLE([bgp_get_timebase])
AC_SCOREP_TIMER_ARG_ENABLE([clock_gettime])
AC_SCOREP_TIMER_ARG_ENABLE([cray_dclock])
AC_SCOREP_TIMER_ARG_ENABLE([cray_rtc])
AC_SCOREP_TIMER_ARG_ENABLE([cray_rtclock])
AC_SCOREP_TIMER_ARG_ENABLE([cycle_counter_itc])
AC_SCOREP_TIMER_ARG_ENABLE([cycle_counter_tsc])
AC_SCOREP_TIMER_ARG_ENABLE([gettimeofday])
AC_SCOREP_TIMER_ARG_ENABLE([ibm_realtime])
AC_SCOREP_TIMER_ARG_ENABLE([ibm_switch_clock])
AC_SCOREP_TIMER_ARG_ENABLE([intel_mmtimer])
AC_SCOREP_TIMER_ARG_ENABLE([nec_syssx_hgtime])
AC_SCOREP_TIMER_ARG_ENABLE([papi_real_cyc])
AC_SCOREP_TIMER_ARG_ENABLE([papi_real_usec])
AC_SCOREP_TIMER_ARG_ENABLE([sun_gethrtime])

])

###############################################################################

AC_DEFUN([AC_SCOREP_TIMER], [
# init all timers to "no". then evaluate user arguments and availability 
# and set one of them to "yes"
ac_scorep_timer_bgl_rts_get_timebase="no"
ac_scorep_timer_bgq_mftb="no"
ac_scorep_timer_bgp_get_timebase="no"
ac_scorep_timer_clock_gettime="no"
ac_scorep_timer_cray_dclock="no"
ac_scorep_timer_cray_rtc="no"
ac_scorep_timer_cray_rtclock="no"
ac_scorep_timer_cycle_counter_itc="no"
ac_scorep_timer_cycle_counter_tsc="no"
ac_scorep_timer_gettimeofday="no"
ac_scorep_timer_ibm_realtime="no"
ac_scorep_timer_ibm_switch_clock="no"
ac_scorep_timer_intel_mmtimer="no"
ac_scorep_timer_nec_syssx_hgtime="no"
ac_scorep_timer_papi_real_cyc="no"
ac_scorep_timer_papi_real_usec="no"
ac_scorep_timer_sun_gethrtime="no"

AC_LANG_PUSH([C])
AC_SCOREP_TIMER_BGL_RTS_GET_TIMEBASE_AVAILABLE
AC_SCOREP_TIMER_BGQ_MFTB_AVAILABLE
AC_SCOREP_TIMER_BGP_GET_TIMEBASE_AVAILABLE
AC_SCOREP_TIMER_CLOCK_GETTIME_AVAILABLE
AC_SCOREP_TIMER_CRAY_DCLOCK_AVAILABLE
AC_SCOREP_TIMER_CRAY_RTCLOCK_AVAILABLE
AC_SCOREP_TIMER_CRAY_RTC_AVAILABLE
AC_SCOREP_TIMER_CYCLE_COUNTER_ITC_AVAILABLE
AC_SCOREP_TIMER_CYCLE_COUNTER_TSC_AVAILABLE
AC_SCOREP_TIMER_GETTIMEOFDAY_AVAILABLE
AC_SCOREP_TIMER_IBM_REALTIME_AVAILABLE
AC_SCOREP_TIMER_IBM_SWITCH_CLOCK_AVAILABLE
AC_SCOREP_TIMER_INTEL_MMTIMER_AVAILABLE
AC_SCOREP_TIMER_NEC_SYSSX_HGTIME_AVAILABLE
AC_SCOREP_TIMER_PAPI_REAL_CYC_AVAILABLE
AC_SCOREP_TIMER_PAPI_REAL_USEC_AVAILABLE
AC_SCOREP_TIMER_SUN_GETHRTIME_AVAILABLE
AC_LANG_POP([C])
# now all ac_scorep_timer_*_available variables are set

ac_scorep_user_timer_given="no"
AC_SCOREP_TIMER_ENABLE_SPECIFIC

ac_scorep_timer_given="no"
AC_SCOREP_TIMER_PLATFORM_DEFAULTS

AS_IF([test "x${ac_scorep_user_timer_given}" = "xno" && test "x${ac_scorep_timer_given}" = "xno"],
      [AC_MSG_ERROR([No suitable timer found. You may specify one with the --enable-timer options])],
      [AC_MSG_NOTICE([Using ${ac_scorep_timer} timer])])


AM_CONDITIONAL([SCOREP_TIMER_BGL_RTS_GET_TIMEBASE], [test "x${ac_scorep_timer_bgl_rts_get_timebase}" = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_BGQ_MFTB],             [test "x${ac_scorep_timer_bgq_mftb}"             = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_BGP_GET_TIMEBASE],     [test "x${ac_scorep_timer_bgp_get_timebase}"     = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CLOCK_GETTIME],        [test "x${ac_scorep_timer_clock_gettime}"        = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CRAY_DCLOCK],          [test "x${ac_scorep_timer_cray_dclock}"          = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CRAY_RTCLOCK],         [test "x${ac_scorep_timer_cray_rtclock}"         = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CRAY_RTC],             [test "x${ac_scorep_timer_cray_rtc}"             = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CYCLE_COUNTER_ITC],    [test "x${ac_scorep_timer_cycle_counter_itc}"    = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_CYCLE_COUNTER_TSC],    [test "x${ac_scorep_timer_cycle_counter_tsc}"    = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_GETTIMEOFDAY],         [test "x${ac_scorep_timer_gettimeofday}"         = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_IBM_REALTIME],         [test "x${ac_scorep_timer_ibm_realtime}"         = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_IBM_SWITCH_CLOCK],     [test "x${ac_scorep_timer_ibm_switch_clock}"     = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_INTEL_MMTIMER],        [test "x${ac_scorep_timer_intel_mmtimer}"        = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_NEC_SYSSX_HGTIME],     [test "x${ac_scorep_timer_nec_syssx_hgtime}"     = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_PAPI_REAL_CYC],        [test "x${ac_scorep_timer_papi_real_cyc}"        = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_PAPI_REAL_USEC],       [test "x${ac_scorep_timer_papi_real_usec}"       = "xyes"])
AM_CONDITIONAL([SCOREP_TIMER_SUN_GETHRTIME],        [test "x${ac_scorep_timer_sun_gethrtime}"        = "xyes"])

ac_scorep_timer_summary="${ac_scorep_timer}"
# specific libs are defined during the checks in the SCOREP_TIMER_*_AVAILABLE macros
ac_scorep_timer_lib=""
AS_IF([test "x${ac_scorep_timer_clock_gettime}"    = "xyes"], 
          [ac_scorep_timer_lib=${ac_scorep_timer_librt}
           ac_scorep_timer_summary="${ac_scorep_timer_summary}, using ${ac_scorep_timer_clock_gettime_clock} ${ac_scorep_timer_librt}"],
      [test "x${ac_scorep_timer_ibm_switch_clock}" = "xyes"], 
          [ac_scorep_timer_lib=${ac_scorep_timer_libswclock}
           ac_scorep_timer_summary="${ac_scorep_timer_summary}, using ${ac_scorep_timer_libswclock}"],
      [test "x${ac_scorep_timer_bgp_get_timebase}" = "xyes"], 
          [ac_scorep_timer_lib="-lSPI.cna -lrt"
           ac_scorep_timer_summary="${ac_scorep_timer_summary}, using -lSPI.cna -lrt"])
AC_SUBST([TIMER_LIB], ["$ac_scorep_timer_lib"])

AC_SCOREP_SUMMARY([Clock used for measurement], [${ac_scorep_timer_summary}])
])
