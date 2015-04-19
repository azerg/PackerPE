# parameter format is: 'gtest-1.7.0'

if [[ -z "$1" ]]; then
	echo $0" Error: Parameter expected but not found!";
else
	echo "var is set to '$param'";
fi

GTEST_BUILD_NAME=$1

wget 'http://googletest.googlecode.com/files/'$GTEST_BUILD_NAME'.zip'
unzip $GTEST_BUILD_NAME.zip
rm $GTEST_BUILD_NAME.zip
cd $GTEST_BUILD_NAME/cmake
cmake -DBUILD_SHARED_LIBS=ON -Dgtest_build_samples=ON -G"Unix Makefiles" ..
make
cd ../../ #todo(azerg): check this
echo '-----gtest compiled-----'
