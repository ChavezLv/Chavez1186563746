#!/bin/bash
./Exe_DictProducer ../conf/conf en
[ $? = 0 ] && echo -e "en finish\n"
./Exe_DictProducer ../conf/conf cn
[ $? = 0 ] && echo -e "cn finish\n"
