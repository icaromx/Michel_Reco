if [[ "$HOSTNAME" == *"uboonegpvm"* ]]; then
	source /grid/fermiapp/products/uboone/setup_uboone.sh
	setup cmake v3_10_1
	setup root v6_12_06a -q e15:prof
fi

mkdir -p ../test_build_Michel_Reco/
cd ../test_build_Michel_Reco/
cmake ../Michel_Reco/
make

#export PATH=/usr/local/Cellar/eigen/3.3.4/include/eigen3/Eigen:$PATH

export MICHELRECOPATH=`pwd`
export PATH=${MICHELRECOPATH}/bin:$PATH
