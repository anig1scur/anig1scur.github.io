#!/bin/bash

# 指定要处理的文件夹路径
FOLDER_PATH="./"

# 循环处理文件夹中所有 jpg 和 png 图片
for file in "$FOLDER_PATH"/*.{jpg,png}; do
  # 获取文件大小（单位为字节）
  filesize=$(stat -c%s "$file")

  # 转换字节到 MB, 1MB = 1048576字节
  filesizeMB=$(echo "scale=2; $filesize / 1048576" | bc)

  # 如果文件大小 >= 2MB
  if (( $(echo "$filesizeMB >= 1.5" | bc -l) )); then
    # 使用convert命令压缩图片并保留文件名
    convert -resize 80% "$file" "$file"
    echo "Converted: $file ($filesizeMB MB)"
  fi
done
