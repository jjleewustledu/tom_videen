#!/bin/csh

# Purpose: Generate .sav files required by IDL runtime license

if($#argv < 1) then
    echo Usage: make_runtime.csh output_path
    exit(-1)
endif

set DIR = $1

set dead = `ls *dead.pro`
echo "@stats.h" > tmp.h
echo "@ECATdef.h" >> tmp.h
echo ".run stats" >> tmp.h
echo ".run stats_exec" >> tmp.h
echo ".run get_time_prof" >> tmp.h
echo ".run main" >> tmp.h
echo ".run pickfile" >> tmp.h
echo ".run xmanager" >> tmp.h
echo ".run $dead" >> tmp.h
echo "resolve_all" >> tmp.h
echo "exit" >> tmp.h
idl tmp.h >& tmp_run.txt

echo "@stats.h" > tmp.h
echo "@ECATdef.h" >> tmp.h
tr "[A-Z]" "[a-z]" < tmp_run.txt | gawk '{printf(".run %spro\n",$4)}' >> tmp.h
echo "exit" >> tmp.h
idl tmp.h >& tmp_sav.txt

echo 'echo @stats.h > tmp.h' > tmp1.csh
echo 'echo @ECATdef.h >> tmp.h' >> tmp1.csh
echo 'echo .run stats >> tmp.h' >> tmp1.csh
echo 'echo .run pickfile >> tmp.h' >> tmp1.csh
echo 'echo .run xmanager >> tmp.h' >> tmp1.csh
echo " " >> tmp1.csh
tr "[A-Z]" "[a-z]" < tmp_run.txt | gawk 'BEGIN{printf("foreach file (")} {if($2 == "compiled") printf(" %spro",$4)}END{printf(")\n")}' >> tmp1.csh
echo '    if($file !~ *event.pro) then' >> tmp1.csh
echo '        echo .run $file >> tmp.h' >> tmp1.csh
echo '    endif' >> tmp1.csh
echo 'end' >> tmp1.csh

tr "[A-Z]" "[a-z]" < tmp_sav.txt | gawk 'BEGIN{printf("foreach file (")} {if($2 == "compiled") printf(" %spro",$4)}END{printf(")\n")}' >> tmp1.csh
echo "    make_save $DIR"' $file:r >> tmp.h' >> tmp1.csh
echo 'end' >> tmp1.csh
echo 'echo exit >> tmp.h' >> tmp1.csh

echo 'idl tmp.h' >> tmp1.csh

csh tmp1.csh
