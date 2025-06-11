clear
rm -rf build
echo -e "\033[1m🔧 Setting target to esp32s3...\033[0m" && \
idf.py set-target esp32s3 && \
echo -e "\033[1m🔨 Building project...\033[0m" && \
idf.py build
