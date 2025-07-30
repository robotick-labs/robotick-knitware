clear

export WORKLOAD_PRESET=spine-workloads-barr-e
export ROBOTICK_PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/.."

echo -e "\033[1m🔨 Building project...\033[0m" && \
idf.py build
