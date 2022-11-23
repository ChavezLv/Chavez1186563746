#!/bin/bash
read -n1 -p "Please make sure your local changes have been saved. Do you want to continue [Y/N]?" answer
case $answer in
Y | y)
  find . -type f -iname '*.tar.gz' -execdir tar -zxvf {} \;
  sh desdk/release_packages.sh
  echo "release complete.";;
*)
  echo "release canceled.";;
esac