#!/bin/bash

log_highlight() {
  echo -e "\033[32m"$1"$(tput sgr0)"
}

cmd_help() {
  log_highlight "repack.sh -u <tar file> : Unpack images.tar.bz2 and rootfs.cpio of it."
  log_highlight "repack.sh -p : Package all images into images.tar.bz2."
  log_highlight "repack.sh -h : Show this message."
}

archive_cpio() {
  input_dir=$1
  output_cpio_file=$2
  log_highlight "\nArchive ${input_dir} to ${output_cpio_file}."
  tree -L 3
  cd ${input_dir}
  sudo find ./ -type f ! -path "./dev/console" ! -path "./THIS_IS_NOT_YOUR_ROOT_FILESYSTEM" ! -path "./md5sum.txt" ! -path "./etc/group" ! -path "./etc/passwd" ! -path "./etc/shadow" -print0 | sort -z | xargs -0 md5sum > md5sum.txt
  sudo find . | cpio --quiet -o -H newc > ${output_cpio_file}
}

unarchive_cpio() {
  input_cpio_file=$1
  output_dir=$2
  log_highlight "\nUnarchive ${input_cpio_file} to ${output_dir}."

  #if [ -d ${output_dir} ]; then
  #  rm -rf ${output_dir}
  #fi
  mkdir -p ${output_dir} && cd ${output_dir}
  sudo cpio -idm < ${input_cpio_file}
}

tar_files() {
  input_dir=$1
  output_tar_file=$2
  log_highlight "\nTar ${input_dir} to ${output_tar_file}."

  cd ${input_dir}
  md5sum output/images/* > ${input_dir}/images.md5

  tar -cjf ${output_tar_file} *
}

untar_files() {
  intput_tar_file=$1
  output_dir=$2
  log_highlight "\nUntar ${intput_tar_file} to ${output_dir}."

  #if [ -d ${output_dir} ]; then
  #  rm -rf ${output_dir}
  #fi
  mkdir -p ${output_dir} && cd ${output_dir}

  tar -xf ${intput_tar_file} -C ${output_dir}
}

work_dir=`pwd`

if [ $# == 0 ]; then
  cmd_help
  exit 0
fi

unpack_enable=false
pack_enable=false

#Get options.
while getopts 'u:ph' OPT; do
  case $OPT in
    u)
      if [ ${OPTARG:0:1} == "/" ]; then
        input_file=$OPTARG
      else
        input_file="${work_dir}/$OPTARG"
      fi
      unpack_enable=true
      ;;
    p)
      pack_enable=true
      ;;
    h)
      cmd_help
      exit 0
      ;;
    ?)
      cmd_help
      exit 0
      ;;
  esac
done

output_dir="${work_dir}/output"

#images var.
input_images_tar=${input_file}
output_images_dir="${work_dir}/output/images"
output_images_tar="${work_dir}/output/images.tar.bz2"

#rootfs var.
input_rootfs_cpio="${work_dir}/output/images/output/images/rootfs.cpio"
output_rootfs_dir="${work_dir}/output/rootfs"
output_rootfs_cpio="${work_dir}/output/images/output/images/rootfs.cpio"

if [ $unpack_enable == true ]; then
  if [ -d ${output_dir} ]; then
    rm -rf ${output_dir}
  fi

  #Untar images.tar.bz2 to output/images.
  untar_files ${input_images_tar} ${output_images_dir}

  #Unarchive output/images/rootfs.cpio to output/rootfs.
  unarchive_cpio ${input_rootfs_cpio} ${output_rootfs_dir}
fi

if [ $pack_enable == true ]; then
  #Archive output/rootfs to output/imaegs/rootfs.cpio.
  archive_cpio ${output_rootfs_dir} ${output_rootfs_cpio}

  #Tar output/images to output/images.tar.bz2.
  tar_files ${output_images_dir} ${output_images_tar}
fi
