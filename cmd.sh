DATA_DIR="./data"
EXTRACT_DIR=${DATA_DIR}"/tmp/AllSetting"

# list=`find ${DATA_DIR} | grep -e kas5 -e kas4`
# echo ${list}

list=${DATA_DIR}/"2022.kas5"

for item in ${list[@]};
do
	echo ${item}
	# TODO:tar gz
	target=`find ${EXTRACT_DIR} | grep -e kas5`
	# echo ${target}
	mv ${item} ${item}.bak
	fname=`basename ${item}`
	echo ${target}
	mv ${target} ${DATA_DOR}/${fname}
done
