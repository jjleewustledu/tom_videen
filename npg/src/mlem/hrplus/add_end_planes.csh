#! /bin/csh
#
source $proc_dir/PATHS.INC

# Ask directory
#--------------
start:
  set c1 = "Working directory     "
    set d1 = `pwd`
  set c2 = "Input image           "
    set d2 = ""
    set t2 = "FILE"
    set ext2 = ".v"
    set h2 = "File naam "
  set c3 = "Output image          "
    set d3 = ""
    set t3 = "FILE"
    set ext3 = ".v"
    set h3 = "File naam"
  set c4 = "Tussen image          "
    set d4 = ""
    set t4 = "FILE"
    set ext4 = ".v"
    set h4 = "File naam (blanco voor weggooien tussen beeld"
  set c5 = "Beeld orde [a],r,n    "
    set d5 = "a"
    set h5 = "a ,automatisch (computer beslist), r = reverse, n = no reverse"
  set c6 = "Start execution"
    set d6 = ".NA."
  set mvm_num   = 6
  set mvm_title = "ADD_END_PLANES"
# !
# ! Call mvm_menu in a loop
# ! =======================
 
askagain:
cd $d1
source $GENERAL_scripts/mvm_menu

# Laat alles terug zien, indien niet het eind commando gegeven is
#----------------------------------------------------------------
if ($mvm_select != $mvm_num) then 
  goto askagain
endif

set def_dir     = $d1
set input       = $d1/$d2$ext2
set output      = $d1/$d3$ext3
if ($d4 != "") then
  set tmp_image = $d1/$d4$ext4
else
  set tmp_image = $d1/$d2"_raw"$ext4
endif
set bed_orde = 2
if ($d5 == "r") then
  set bed_orde = 1
endif
if ($d5 == "n") then
  set bed_orde = 0
endif


cd $def_dir

$HRP_dir/add_end_planes  -i $input -o $tmp_image -rb $bed_orde
$HRP_dir/raw_file2volume -i $tmp_image -o $output
if ($d4 == "") then
  \rm $tmp_image
endif
exit

  





















































