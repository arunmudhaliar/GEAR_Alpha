BASEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "Script location: ${BASEDIR}"

PREFIX=${BASEDIR}/monoForMACOS
VERSION=2.10.9

if [ -f mono-$VERSION.tar.bz2 ]
then
	echo "mono-$VERSION.tar.bz2 found."
else
	echo "mono-$VERSION.tar.bz2 not found. Try Downloading...."
	curl "http://download.mono-project.com/sources/mono/mono-$VERSION.tar.bz2" -o "mono-$VERSION.tar.bz2"
fi

tar xvf mono-$VERSION.tar.bz2
cd mono-$VERSION
./configure --prefix=$PREFIX --disable-nls --with-tls=pthread
make
make install
cd ..

echo "$PREFIX" > ./macosProj/GEARInternal/mono-prerequisite/monoinstallpath.txt
echo "FINISHED"