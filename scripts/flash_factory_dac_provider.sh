#! /usr/sh
# flash_factory.sh
# author: seung hee, lee
# purpose: flash manufacture binaries

port=/dev/ttyUSB0
baudrate=115200

cur_path=${PWD}
if [[ "$OSTYPE" == "darwin"* ]]; then
    project_path=$(dirname $(dirname $(realpath $0)))
else 
    project_path=$(dirname $(dirname $(realpath $BASH_SOURCE)))
fi
# sdk_path=${project_path}/sdk
sdk_path=~/tools  # change to your own sdk path
esp_idf_path=${sdk_path}/esp-idf

port_arg=0
idf_path_arg=0
baud_arg=0
for arg in "$@"
do
    if [[ "$arg" == "-p" ]]; then
        port_arg=1
    elif [[ "$arg" == "-b" ]]; then
        baud_arg=1
    elif [[ "$arg" == "-idf" ]]; then
        idf_path_arg=1
    else
        if [[ $port_arg == 1 ]]; then
            port=$arg
            port_arg=0
        elif [[ $baud_arg == 1 ]]; then
            baudrate=$arg
            baud_arg=0
        elif [[ $idf_path_arg == 1 ]]; then
            esp_idf_path=$arg
            idf_path_arg=0
        fi
    fi
done

nvs_address_label=0x10000  # "nvs"
binary_path=${project_path}/resource/DACProvider/partition.bin

# prepare environment (python, idf.py, esptool.py)
if [ -z "$IDF_PATH" ]; then
  source ${esp_idf_path}/export.sh
fi

# flash manufacturing binary
esptool.py -p ${port} -b ${baudrate} write_flash ${nvs_address_label} ${binary_path}

cd ${cur_path}
