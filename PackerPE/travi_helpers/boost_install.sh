# - expected version in definite format only: '1.46.1'
# - script returns boost folder name
# Exec it like boost_folder=$(./boost_install.sh '1.46.1') to obtain folder name in soirce script
# BOOST_VERSION_PARAM=$(readlink -f .). todo(azerg): remove this line

if [[ -z "$1" ]]; then
	echo $0" Error: Parameter expected but not found!";
else
	echo "var is set to '$param'";
fi

boost_version=$1
boost_underline_vera=$(echo ${boost_version//./_})

boost_dl_url='http://sourceforge.net/projects/boost/files/boost/'$boost_version'/boost_'$boost_underline_vera'.tar.bz2/download'

wget -c $boost_dl_url
tar xf download
rm download
#echo '-----boost downloaded & extracted-----'
echo 'boost'$boost_underline_vera
