#!/bin/sh

# ./test_run.sh orig_main ../orig
# ./test_run.sh new_main ../new
# ./test_run.sh orig_main ../new

cd $1

java -cp $2:../libs/GMC.jar:./ -Djava.library.path=../libs/ TCMain

exit 0
