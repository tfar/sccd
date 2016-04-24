#!/usr/bin/env sh
set -e


# List of variables used in this script
PROJECT_ROOT=$PWD
CC=""
SCCD_SOURCES="sccd/core/ec.c sccd/core/fp.c sccd/core/util.c sccd/core/clock.c sccd/ibc/vbnn_ibs.c"
SCCD_INCLUDE_FLAGS="-I."
BACKEND_SOURCES=""
BLAKE_FLAGS="-I3rdParty/BLAKE2/ref 3rdparty/BLAKE2/ref/blake2s-ref.c"

if [ "$1" = "native" ]
then
	echo "Building for native."
	CC=clang
elif [ "$1" = "pi" ]
then
	echo "Building for pi."
	#export PATH=/Users/tobias/Downloads/gcc-arm-none-eabi-5_2-2015q4/bin:$PATH
	export PATH=/usr/local/linaro/arm-linux-gnueabihf-raspbian/bin:$PATH
	PI_ROOT=/tmp/cryptopi_root
	CC=arm-linux-gnueabihf-gcc
	SYSTEM_FLAGS=""

	echo "Mount Pi root to /tmp/cryptopi_root"
	mkdir -p /tmp/cryptopi_root
	sshpass -p"$RASPBERRYPI_PW" sshfs pi@cryptopi:/ /tmp/cryptopi_root -ocache=no -onolocalcaches -ovolname=ssh
elif [ "$1" = "riot" ]
then
	echo "Building for RIOT."
	export PATH=/Users/tobias/Downloads/gcc-arm-none-eabi-5_2-2015q4/bin:$PATH
	CC=arm-none-eabi-gcc
	SYSTEM_FLAGS="-mcpu=cortex-m0plus -mlittle-endian -mthumb -mfloat-abi=soft -mno-thumb-interwork -ffunction-sections -fdata-sections -fno-builtin -fshort-enums -g3"
	#export PATH=/usr/local/linaro/arm-linux-gnueabihf-raspbian/bin:$PATH
	#PI_ROOT=/tmp/cryptopi_root
	#CC=arm-none-eabi-gcc
	#SYSTEM_FLAGS="-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -fno-toplevel-reorder"
elif [ "$1" = "ctgrind" ]
then
	echo "Building for ctgrind."
	CC=gcc
else
	echo "Usage: ./build.sh (native|pi|ctgrind|riot) (c25519|relic)"
	exit 0
fi

if [ "$2" = "c25519" ]
then
	BACKEND_CC_FLAGS="-DSCCD_BACKEND_C25519 -I3rdParty"
	BACKEND_SOURCES="3rdparty/c25519/src/f25519.c 3rdparty/c25519/src/ed25519.c"
elif [ "$2" = "relic" ]
then
	BACKEND_CC_FLAGS="-DSCCD_BACKEND_RELIC -I3rdparty/relic_prefix/include -L3rdparty/relic_prefix/lib -lrelic_s"
	# build relic
	if [ "$1" = "native" ] || [ "$1" = "ctgrind" ]
	then
		rm -rfd 3rdparty/relic_prefix
		rm -rfd 3rdparty/relic_build
		mkdir -p 3rdparty/relic_build
		cd 3rdparty/relic_build
		COMP="-g" cmake -DCMAKE_INSTALL_PREFIX:PATH=../relic_prefix -DDOCUM=off -DSHLIB=off -DSTBIN=off \
			-DTESTS=0 -DBENCH=0 \
			-DALIGN=1 \
			-DWITH="ED;EC;MD;FP;BN;DV;CP;BC;EP" \
			-DBN_METHD="COMBA;COMBA;BASIC;SLIDE;BASIC;BASIC" -DBN_PRECI=512 \
			-DFP_PRIME=255 \
			-DEC_METHD=EDWARD \
			-DMD_METHD=SHONE \
			-DED_METHD="PROJC;LWNAF;LWNAF;BASIC" -DED_PRECO=off \
			../relic
		make install -j 4
		cd ../..
	elif [ "$1" = "pi" ]
	then
		rm -rfd 3rdparty/relic_prefix
		rm -rfd 3rdparty/relic_build
		mkdir -p 3rdparty/relic_build
		cd 3rdparty/relic_build
		cmake -DCMAKE_INSTALL_PREFIX:PATH=../relic_prefix -DDOCUM=off -DSHLIB=off -DSTBIN=off \
			-DTESTS=0 -DBENCH=0 \
			-DALIGN=1 \
			-DWITH="ED;EC;MD;FP;BN;DV;CP;BC;EP" \
			-DBN_METHD="COMBA;COMBA;BASIC;SLIDE;BASIC;BASIC" -DBN_PRECI=512 \
			-DFP_PRIME=255 \
			-DEC_METHD=EDWARD \
			-DED_METHD="PROJC;LWNAF;LWNAF;BASIC" -DED_PRECO=off \
			../relic
		make install -j 4
		cd ../..
	else
		exit 0
	fi
else
	echo "Please specify the crypto backend as second argument. Possible values: c25519, relic."
	exit 0
fi


if [ "$1" = "ctgrind" ]
then
	export LD_LIBRARY_PATH=/home/test/agl_ctgrind
	$CC $SYSTEM_FLAGS -g -O3 -std=c11 \
		$SCCD_INCLUDE_FLAGS \
		$SCCD_SOURCES sccd/qa/ctgrind_test.c $BACKEND_CC_FLAGS \
		$BACKEND_SOURCES \
		$BLAKE_FLAGS \
		-I/home/test/agl_ctgrind -L/home/test/agl_ctgrind -lctgrind -o ctgrind_test
	~/ctgrind/bin/valgrind ./ctgrind_test
	exit 0
fi

echo "\nRun tests"
if [ "$1" = "riot" ]
then
	#set -x
	$CC $SYSTEM_FLAGS -Os -std=c11 $BACKEND_CC_FLAGS \
		$SCCD_INCLUDE_FLAGS \
		$source_file \
		$BLAKE_FLAGS \
		-c  $SCCD_SOURCES $BACKEND_SOURCES
	arm-none-eabi-ar rcs sccd/qa/riot-test/libsccd_all.a *.o
	cd sccd/qa/riot-test
	make clean
	make -j 4
	make flash 2> /dev/null
	export RIOTBASE="$PROJECT_ROOT/3rdparty/RIOT/"
	../runner.py
	cd $PROJECT_ROOT
else
	$CC $SYSTEM_FLAGS -g -O3 -std=c11 $BACKEND_CC_FLAGS \
		$SCCD_INCLUDE_FLAGS \
		$SCCD_SOURCES sccd/qa/test.c \
		$BACKEND_SOURCES \
		$BLAKE_FLAGS \
		-o test
fi

if [ "$1" = "native" ]
then
	./test
elif [ "$1" = "pi" ]
then
	sshpass -p"$RASPBERRYPI_PW" scp ./test pi@cryptopi:/tmp/test
	sshpass -p"$RASPBERRYPI_PW" ssh pi@cryptopi 'sudo /tmp/test'
fi

now=$(date +"%Y-%m-%d_%H-%M-%S")

echo "\nRun benchmark"
if [ "$1" = "riot" ]
then
	#set -x
	$CC $SYSTEM_FLAGS -Os -std=c11 $BACKEND_CC_FLAGS \
		$SCCD_INCLUDE_FLAGS \
		$source_file \
		$BLAKE_FLAGS \
		-DSCCD_RIOT_TIMER \
		-c  $SCCD_SOURCES $BACKEND_SOURCES
	arm-none-eabi-ar rcs sccd/qa/riot-bench/libsccd_all.a *.o
	cd sccd/qa/riot-bench
	make clean
	make -j 4
	make flash 2> /dev/null
	export RIOTBASE="$PROJECT_ROOT/3rdparty/RIOT/"
	../runner.py > "$PROJECT_ROOT/benchmark_$1_$now.log"
	cd $PROJECT_ROOT
else
	$CC $SYSTEM_FLAGS -fno-omit-frame-pointer -O3 -std=c11 $BACKEND_CC_FLAGS \
		$SCCD_INCLUDE_FLAGS \
		$SCCD_SOURCES sccd/qa/bench.c \
		$BACKEND_SOURCES \
		$BLAKE_FLAGS \
		-o bench
fi

if [ "$1" = "native" ]
then
	sudo nice -n -20 ./bench 12 > "./benchmark_$1_$now.log"
elif [ "$1" = "pi" ]
then
	sshpass -p"$RASPBERRYPI_PW" scp ./bench pi@cryptopi:/tmp/bench
	sshpass -p"$RASPBERRYPI_PW" ssh pi@cryptopi "sudo nice -n -20 chrt -f 99 /tmp/bench 12 > /tmp/benchmark_$1_$now.log" #| ./analyze_bench.py
	sshpass -p"$RASPBERRYPI_PW" scp pi@cryptopi:/tmp/benchmark_$1_$now.log ./
fi

cat benchmark_$1_$now.log | ./analyze_bench.py
# ./bench 10 | ./analyze_bench.py
#now=$(date +"%Y-%m-%d_%H-%M-%S")
#./bench 10000 > ./benchmark_$now.log
