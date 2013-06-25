#!/bin/sh
#############################################################################
#
#
#   ###               ##
#  ## ##  ###   ###  #### ## ##  ###  ####
#  ##    ## ## ## ##  ##  ##### ## ## ## ##
#  ##    ##### #####  ##  ###   ## ## ## ##
#  ## ## ##    ##     ##  ##    ## ## ## ##
#   ###   ###   ###    ## ##     ###  ## ## -- understanding by visualization
#
#
# CeeVizFramework script to rebuild and run unit-tests
#
#############################################################################


# Check for required build configuration argument
if [ "$1" != "debug" ] && [ "$1" != "release" ]; then
   echo "ERROR: Invalid build configuration [$1]!"
   echo " Valid options: <debug|release>"
   exit
fi

# Check for required platform argument
if [ "$2" != "linux32" ] && [ "$2" != "linux64" ]; then
   echo "ERROR: Invalid build platform [$2]!"
   echo " Valid options: <linux32|linux64>"
   exit
fi

check_errs()
{
  # Function. Parameter 1 is the return code
  # Para. 2 is text to display on failure.
  if [ "${1}" -ne "0" ]; then
    echo "ERROR # ${1} : ${2}"
    # as a bonus, make our script exit with the right error code.
    exit ${1}
  fi
}

check_gtest_xml_file_exists()
{
if [ ! -f "${1}" ];
then
   echo "ERROR gtest XML file ${1} does not exists"
   exit 1
fi
}
 

# Rebuild CeeViz libraries
echo "\n\n"
make -C LibCore CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibCore"

echo "\n\n"
make -C LibIo CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibIo"

echo "\n\n"
make -C LibGeometry CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibGeometry"

echo "\n\n"
make -C LibRender CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibRender"

echo "\n\n"
make -C LibViewing CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibViewing"

echo "\n\n"
make -C LibUtilities CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibUtilities"

echo "\n\n"
make -C LibRegGrid2D CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibRegGrid2D"

echo "\n\n"
make -C LibStructGrid CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibStructGrid"

echo "\n\n"
make -C LibUnstructGrid CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibUnstructGrid"

echo "\n\n"
make -C LibUnstructDisplay CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build LibUnstructDisplay"



# Rebuild CeeViz unit tests
echo "\n\n"
make -C Tests/LibCore_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibCore_UnitTests"

echo "\n\n"
make -C Tests/LibIo_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibIo_UnitTests"

echo "\n\n"
make -C Tests/LibGeometry_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibGeometry_UnitTests"

echo "\n\n"
make -C Tests/LibRender_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibRender_UnitTests"

echo "\n\n"
make -C Tests/LibViewing_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibViewing_UnitTests"

echo "\n\n"
make -C Tests/LibUtilities_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibUtilities_UnitTests"

echo "\n\n"
make -C Tests/LibRegGrid2D_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibRegGrid2D_UnitTests"

echo "\n\n"
make -C Tests/LibStructGrid_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibStructGrid_UnitTests"

echo "\n\n"
make -C Tests/LibUnstructGrid_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibunstructGrid_UnitTests"

echo "\n\n"
make -C Tests/LibUnstructDisplay_UnitTests CFG=$1 PLATFORM=$2 clean all
check_errs $? "Failed to build Tests/LibunstructDisplay_UnitTests"


# Set up platform folder
if [ "$2" = "linux32" ]; then
   PLATFORM_FOLDER=LINUX
fi
if [ "$2" = "linux64" ]; then
   PLATFORM_FOLDER=LINUX64
fi

# Keep current (root) path
CeeVizRoot=$PWD

# Recreate testreport folder
rm -rf testreports
mkdir testreports

# Run unit tests
export GTEST_OUTPUT="xml:./testreports/"
if [ "$1" = "debug" ]; then
   ./Tests/LibCore_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibCore_UnitTestd
   ./Tests/LibIo_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibIo_UnitTestd
   ./Tests/LibGeometry_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibGeometry_UnitTestd
   ./Tests/LibRender_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibRender_UnitTestd
   ./Tests/LibViewing_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibViewing_UnitTestd
   ./Tests/LibUtilities_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUtilities_UnitTestd
   ./Tests/LibRegGrid2D_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibRegGrid2D_UnitTestd
   ./Tests/LibStructGrid_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibStructGrid_UnitTestd
   ./Tests/LibUnstructGrid_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUnstructGrid_UnitTestd
   ./Tests/LibUnstructDisplay_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUnstructDisplay_UnitTestd
else
   ./Tests/LibCore_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibCore_UnitTest
   ./Tests/LibIo_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibIo_UnitTest
   ./Tests/LibGeometry_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibGeometry_UnitTest
   ./Tests/LibRender_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibRender_UnitTest
   ./Tests/LibViewing_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibViewing_UnitTest
   ./Tests/LibUtilities_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUtilities_UnitTest
   ./Tests/LibRegGrid2D_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibRegGrid2D_UnitTest
   ./Tests/LibStructGrid_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibStructGrid_UnitTest
   ./Tests/LibUnstructGrid_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUnstructGrid_UnitTest
   ./Tests/LibUnstructDisplay_UnitTests/bin/$PLATFORM_FOLDER/CeeVizLibUnstructDisplay_UnitTest
fi

# Check that all unit tests were run completely by checking if the gtest XML report exists
if [ "$1" = "debug" ]; then
    check_gtest_xml_file_exists "testreports/CeeVizLibCore_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibIo_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibGeometry_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibRender_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibViewing_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUtilities_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibRegGrid2D_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibStructGrid_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUnstructGrid_UnitTestd.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUnstructDisplay_UnitTestd.xml"
else
    check_gtest_xml_file_exists "testreports/CeeVizLibCore_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibIo_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibGeometry_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibRender_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibViewing_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUtilities_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibRegGrid2D_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibStructGrid_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUnstructGrid_UnitTest.xml"
    check_gtest_xml_file_exists "testreports/CeeVizLibUnstructDisplay_UnitTest.xml"
fi

