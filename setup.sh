# get access to larsoft and uboonecode products
if [[ "$OSTYPE" == "linux-gnu" ]]; then
	echo "using Linux"
	source /grid/fermiapp/products/uboone/setup_uboone.sh
	setup cmake v3_10_1
	setup root v6_12_06a -q e15:prof
elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "Using macOS"
fi
cmake ../Michel_Reco/
make

export MICHELRECOPATH=`pwd`
export PATH=${MICHELRECOPATH}/bin:$PATH
