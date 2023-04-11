#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release_x64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'../GcmAes/dist/Release_x64/GNU-Linux' -L../GcmAes/dist/Release_x64/GNU-Linux -lGcmAes

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gcmaestest

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gcmaestest: ../GcmAes/dist/Release_x64/GNU-Linux/libGcmAes.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gcmaestest: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gcmaestest ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../GcmAes && ${MAKE}  -f Makefile CONF=Release_x64
	cd ../GcmAes && ${MAKE}  -f Makefile CONF=Release_x64

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libGcmAes.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gcmaestest

# Subprojects
.clean-subprojects:
	cd ../GcmAes && ${MAKE}  -f Makefile CONF=Release_x64 clean
	cd ../GcmAes && ${MAKE}  -f Makefile CONF=Release_x64 clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
