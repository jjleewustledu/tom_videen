#! /bin/csh




#
source $proc_dir/PATHS.INC

# Ask directory
#--------------
start:
  set c1 = "Working directory     "
    set d1 = `pwd`
  set c2 = "hrplus image file               "
    set d2 = ""
    set t2 = "FILE"
    set ext2 = ".v"
    set h2 = "File name "
  set c3 = "Scan start time                 "
    set d3 = ""
    set h3 = "only information:"
  set c4 = "gewicht (kg)                    "
    set d4 = ""
    set h4 = "float"
  set c5 = "tijd injectie                   "
    set d5 = ""
    set h5 = "hh:mm:ss"
  set c6 = "netto dosis indien bekend (mCi) "
    set d6 = ""
    set h6 = "dose in mCi"
  set c7 = "tijd meting spuit               "
    set d7 = ""
    set h7 = "hh:mm:ss"
  set c8 = "activiteit in spuit (mCi)       "
    set d8 = ""
    set h8 = "dose in mCi"
  set c9 = "tijd rest dosis                 "
    set d9 = ""
    set h9 = "hh:mm:ss"
  set c10 = "rest dosis (mCi)                "
    set d10 = ""
    set h10 = "rest dose in mCi" 
  set c11 = "calibratie factor hrplus 2d     "
    set d11 = "18000000"
    set h11 = "calibratie factor"
  set c12 = "Start execution"
    set d12 = ".NA."
  set mvm_num   = 12
  set mvm_title = "FIX CAL HEADER"
# !
# ! Call mvm_menu in a loop
# ! =======================
 
askagain:
cd $d1
source $GENERAL_scripts/mvm_menu

if ($d2 != "") then
  if ($d3 == "") then
#    setenv TZ GMT
    set test = `$HRP_dir/show_header $d2$ext2 | grep TOD | awk '{print $6" "$5" "$8" "$7 }'`
    set hour = `echo $test[4] | cut -f1 -d":"`
#    @ hour = $hour + 1
    set d3 = $test[1]"_"$test[2]"_"$test[3]"_"$hour":"`echo $test[4] | cut -f2-3 -d":"`
#    setenv TZ MET
  endif

  if ($d4 == "") then
     set d4 = `$HRP_dir/show_header $d2$ext2 | grep Weight | cut -f2 -d":"`
  endif

  if ($d6 != "") then 
    set c7 = "Niet meer nodig"
    set c8 = "Niet meer nodig"
    set c9 = "Niet meer nodig"
    set c10 = "Niet meer nodig"
  endif

  if (($d5 != "")&&($d7 != "")&&($d8 != "")&&($d9 != "")&&($d10 != "")) then 
    set halfleven = `$HRP_dir/show_header $d2$ext2 | grep Half-life | cut -f2 -d":" | cut -f1 -d "s"`
    set d6 = `$PET_bin/compute_dose_f18 $d7 $d8 $d5 $d9 $d10 $halfleven`
  endif
endif
  
# Laat alles terug zien, indien niet het eind commando gegeven is
#----------------------------------------------------------------
if ($mvm_select != $mvm_num) then 
  goto askagain
endif


set hrp_file      = $d1/$d2$ext2
set scan_start_h  = `echo $d3 | cut -f4 -d"_" | cut -f1 -d":"`
set scan_start_m  = `echo $d3 | cut -f4 -d"_" | cut -f2 -d":"`
set scan_start_s  = `echo $d3 | cut -f4 -d"_" | cut -f3 -d":"`
set weight        = $d4
set inj_time_h    = `echo $d5 | cut -f1 -d":"`
set inj_time_m    = `echo $d5 | cut -f2 -d":"`
set inj_time_s    = `echo $d5 | cut -f3 -d":"`
set dose          = $d6
set ecf           = $d11
@ diff_time = ($scan_start_h - $inj_time_h) * 3600 + ($scan_start_m - $inj_time_m) * 60 + ($scan_start_s - $inj_time_s)

set dose = `echo "$dose * 37000000" | bc -l`

echo $hrp_file      
echo $scan_start_h  
echo $scan_start_m  
echo $scan_start_s  
echo $weight        
echo $inj_time_h    
echo $inj_time_m    
echo $inj_time_s    
echo $dose          
echo $ecf           
echo $diff_time     

$HRP_dir/fix_header -i $hrp_file -w $weight -d $dose -t $diff_time -e $ecf


















































