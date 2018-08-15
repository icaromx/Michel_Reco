if [[ "$HOSTNAME" == *"uboonegpvm"* ]]; then
	source /grid/fermiapp/products/uboone/setup_uboone.sh
	setup cmake v3_10_1
	setup root v6_12_06a -q e15:prof
fi

mkdir ../build_Michel_Reco/
cd ../build_Michel_Reco/
cmake ../Michel_Reco/
make

export MICHELRECOPATH=`pwd`
export PATH=${MICHELRECOPATH}/bin:$PATH
