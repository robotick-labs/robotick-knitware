rm -rf build

export WORKLOAD_PRESET=spine-workloads-barr-e
export ROBOTICK_PROJECT_ROOT="/workspace/knitware-bots/barr-e"

echo -e "\033[1m🔧 Setting target to esp32s3...\033[0m" && \
idf.py set-target esp32s3 && \