#!/bin/csh

# Initialisations
# ===============

#find the current path
#-----
set filename = $0
set pathname = `echo $filename:h`
if ($pathname == $filename) set pathname = $cwd


if (-f $pathname/mlempetv7) then
  set MLEM_bin     = "$pathname"
  set HRP_dir      = "$pathname"
  set GENERAL_bin  = "$pathname"
else
  set MLEM_bin = "$pathname/../bin"
  set test = `echo $pathname | grep CURRENT_VERSION`
  if (($test == "") && (-d $proc_dir/hrplus)) then
    echo "Using the development version"
    set HRP_dir = "$proc_dir/hrplus"
  else
    echo "Using the CURRENT_VERSION"
    set HRP_dir = "$pathname/../hrplus"
  endif
  
  if (-f $proc_dir/GENERAL/bin/NCmaxval) then
    set GENERAL_bin = "$proc_dir/GENERAL/bin"
  else
    set GENERAL_bin = "$MLEM_bin"
  endif
endif
echo $MLEM_bin
echo $HRP_dir
echo $GENERAL_bin

if (((`uname -r` == "5.4")  || (`uname -r` == "5.6")) && (-f $MLEM_bin/mlempetv7_solaris)) then
  echo "Solaris"
  set mlempet  = "$MLEM_bin/mlempetv7_solaris"
  set copypln  = "$HRP_dir/copy_imagev7_solaris"
  set gminmax  = "$HRP_dir/get_minmaxv7_solaris"
  set convolg  = "$HRP_dir/convolgaussv7_solaris"
else
  echo "SunOS"
  set mlempet  = "$MLEM_bin/mlempetv7"
  set copypln  = "$HRP_dir/copy_imagev7"
  set gminmax  = "$HRP_dir/get_minmaxv7"
  set convolg  = "$HRP_dir/convolgaussv7"
endif
set sinoname = ""
set imgname  = ""
set nrmname  = ""
set atnname  = ""

set plane    = 1
set frame    = 1
set gate     = 1
set bed      = 0
set zoom     = 1
set imsize   = 128
set xoffset  = 0
set yoffset  = 0
set rotation = 0
set default_iters = "-i 1 -j 16 -i 1 -j 12 -i 1 -j 4 -i 2 -j 1"
#set default_iters = "-i 1 -j 8"
set iters = ""
set version = 70
set arcradius = 0
set decay = "y"
set action = 1
set gauss  = ""
set verb = ""
set lik  = ""
set sumd = 1
set lsrelax = 0

# check all arguments
#====================
@ i = 1
set nargs = $#argv
while ($i <= $nargs)
  switch ($argv[$i])
    case "-s":
      @ i++; set sinoname = $argv[$i]
      breaksw

    case "-o":
      @ i++;  set imgname = $argv[$i]
      breaksw

    case "-n":
      @ i++;  set nrmname = $argv[$i]
      breaksw

    case "-a":
      @ i++;  set atnname = $argv[$i]
      breaksw

    case "-p":
      @ i++;  set plane = $argv[$i]
      breaksw

    case "-f":
      @ i++;  set frame = $argv[$i]
      breaksw

    case "-g":
      @ i++;  set gate = $argv[$i]
      breaksw

    case "-b":
      @ i++;  set bed = $argv[$i]
      breaksw

    case "-z":
      @ i++;  set zoom = $argv[$i]
      breaksw

    case "-X":
      @ i++;  set imsize = $argv[$i]
      breaksw

    case "-x":
      @ i++;  set xoffset = $argv[$i]
      breaksw

    case "-y":
      @ i++;  set yoffset = $argv[$i]
      breaksw

    case "-r":     
      @ i++;  set rotation = $argv[$i]
      breaksw

    case "-i":
      @ i++;  set iters = "$iters -i $argv[$i]"
      breaksw

     case "-j":
      @ i++;  set iters = "$iters -j $argv[$i]"
      breaksw

     case "-V":
      @ i++;  set version = $argv[$i]
      breaksw

     case "-d":
      @ i++;  set decay = $argv[$i]
      breaksw

      case "-sumd":
      @ i++;  set sumd = $argv[$i]
      breaksw

      case "-ls":
      @ i++;  set lsrelax = $argv[$i]
      breaksw

    case "-R":
      @ i++;  set arcradius = $argv[$i]
      breaksw

     case "-test":
      set action = 0
      breaksw

     case "-v":
      set verb = "-v"
      set lik = "-lik"
      breaksw

     case "-lik":
      set lik = "-lik"
      breaksw

# gauss-smoothing

     case "-sx":
      @ i++;  set gauss = "$gauss -sx $argv[$i]"
      breaksw
     case "-fx":
      @ i++;  set gauss = "$gauss -fx $argv[$i]"
      breaksw
     case "-fz":
      @ i++;  set gauss = "$gauss -fz $argv[$i]"
      breaksw
     case "-sz":
      @ i++;  set gauss = "$gauss -sz $argv[$i]"
      breaksw
     case "-ns":
      @ i++;  set gauss = "$gauss -ns $argv[$i]"
      breaksw

    case "-h":
     echo "Syntax: mlempetv7.com -s sinogram -o outputfile"
     echo "          [-a attenuation]  [-n normalization]"
     echo "          [-i nriter -j nrsubiter] [-i nriter -j nrsubiter] ..."
     echo "          [-p plane(s)] [-f frame(s)] [-g gate(s)] [-b bed(s)]"
     echo "          [-z zoom] [-X imagesize] [-x xoffset] [-y yoffset]"
     echo "          [-r rotation] [-d decay] [-V version] [-R arc_radius]"
     echo "          [-sx sigma] [-fx fwhm] [-sz sigma_z] [-fz fwhm_z]"
     echo "          [-ns nsigma] [-v] [-sumd sumd] [-ls lsrelax]"
     echo "For plane, frame, gate, bed: supply single value (x) or range (x-y)"
     echo "Multiple <-i nriter -j subiter> may be supplied and are executed"
     echo "in the same order."
     echo "Offsets in cm, rotation in degrees."
     echo "When version == 70, a set of intermediate float files is created"
     echo "     version == 6, output is directly written into the final file"
     echo "Arcradius [cm] should include 1 cm depth of interaction:"
     echo "   for Ecat931: 52, Ecat962: 42.2, no arc-correction: 0."
     echo "Decay correction: <-d y> apply,  <-d n> do not apply"
     echo "-v: verb"
     echo ""
     echo "Post filtering with 3D Gaussian smooth: "
     echo "  sigma:   transaxial standard deviation in cm."
     echo "  fwhm:    transaxial fwhm in cm (use either sigma or fwhm\!) "
     echo "  sigma_z: axial standard deviation in cm."
     echo "  fwhm_z:  axial fwhm in cm (use either sigma_z or fwhm_z\!) "
     echo "     when  axial width not specified, same as transaxial is used"
     echo "  nsigma:  truncate mask at nsigma st.devs. Default 2"
     echo "  sumd:    number of detectors to be added before reconstruction"
     echo "  lsrelax: when not zero, MLEM is replaced by a least squares "
     echo "           gradient ascent method. This allows negative values."
     echo "           Default = 0 ==> MLEM. Use value of 1 for Least-squares."
     echo "           Only interesting for non-attenuation corrected studies,"
     echo "           because then, negative values may contain information;"
     echo "           do not use <lsrelax 1> in other cases."
     echo ""
     echo "Default: no atten, no norm, -p 1 -f 1 -g 1 -b 0 -X 128 -V 70 -R 0"
     echo "         -xoffset 0 -yoffset 0 -r 0 -d y -sumd 1"
     echo "         $default_iters"
     echo "         no Gaussian smoothing"
     goto stopermee
     breaksw

   default:
      echo "Error! Illegal argument. Use -h to get some help."
      goto stopermee
      breaksw
  endsw
  @ i++
end

set nsimgname = "$imgname"_ns  # un-smoothed image.

set firstplane = `echo $plane | cut -f1 -d-`
set lastplane  = `echo $plane | cut -f2 -d-`
set firstframe = `echo $frame | cut -f1 -d-`
set lastframe  = `echo $frame | cut -f2 -d-`
set firstgate  = `echo $gate | cut -f1 -d-`
set lastgate   = `echo $gate | cut -f2 -d-`
set firstbed   = `echo $bed | cut -f1 -d-`
set lastbed    = `echo $bed | cut -f2 -d-`

if (("$sinoname" == "") || ("$imgname" == "")) then
  echo "Error! Both sinogram and output file should be specified."
  echo "Use -h to get some help."
  goto stopermee
endif

if (("$version" != "6") && ("$version" != "70")) then
  echo "Version should be 70 or 6. Use -h to get some help."
  goto stopermee
endif

if ("$iters" == "") set iters = "$default_iters"

if ("$version" == "6") goto version_6

##############
version_70:  #
##############

set curgate  = $firstgate
while ($curgate <= $lastgate)
 set curbed   = $firstbed
 while ($curbed <= $lastbed)
  set curframe = $firstframe
  while ($curframe <= $lastframe)

   # Apply MLEM for every plane...
   ###############################
   set curplane = $firstplane
   while ($curplane <= $lastplane)
    set curimgname = "$imgname"_"$curplane"_"$curframe"_"$curgate"_"$curbed"
    set cmdstring = "$mlempet -s $sinoname -o $curimgname"
    if ($atnname != "") set cmdstring = "$cmdstring -a $atnname"
    if ($nrmname != "") set cmdstring = "$cmdstring -n $nrmname"
    set cmdstring = "$cmdstring $iters"

    set cmdstring = "$cmdstring -p $curplane -f $curframe"
    set cmdstring = "$cmdstring -g $curgate -b $curbed"
    set cmdstring = "$cmdstring -SP -V 70 -R $arcradius -d $decay -ls $lsrelax"
    set cmdstring = "$cmdstring -z $zoom -X $imsize -sumd $sumd $lik"
    set cmdstring = "$cmdstring -x $xoffset -y $yoffset -r $rotation $verb"

    echo $cmdstring

    if ($action) $cmdstring

    @ curplane++
   end

   # Look at all the planes and find the global maximum
   ####################################################
   if ($action) then
#    set minval = 99999
    set maxval = 0
    set curplane = $firstplane
    while ($curplane <= $lastplane)
 
     set curimgname = "$imgname"_"$curplane"_"$curframe"_"$curgate"_"$curbed"
     set minmax = `$gminmax -i $curimgname -min -max`
     set curmin = `echo $minmax | cut -f1 -d,`
     set curmax = `echo $minmax | cut -f2 -d,`
 
#     if (`echo "if ($curmin < $minval) 1" | bc -l`) set minval = $curmin
     set maxval = `$GENERAL_bin/NCmaxval $maxval $curmax`
#     if (`echo "if ($curmax > $maxval) 1" | bc -l`) set maxval = $curmax
#     echo "$curmin, $curmax ==> $minval, $maxval"
     echo "$curmin, $curmax ==> $maxval"
 
     @ curplane++
    end
 
#    if (`echo "if (-($minval) > $maxval) 1" | bc -l`) then
#      set maxval = `echo "-1 * $minval" | bc -l`
#    endif
    echo $maxval
   else
     set minval = -123
     set maxval =  123
   endif
   

   # Copy the planes from the individual
   #####################################
   set curplane = $firstplane
   while ($curplane <= $lastplane)

    set cpystr = "$copypln"
    set curimgname = "$imgname"_"$curplane"_"$curframe"_"$curgate"_"$curbed"

    set cpystr = "$cpystr -i $curimgname -o $nsimgname -msi 1,1,1,0,0"
    set cpystr = "$cpystr -mso $curframe,$curplane,$curgate,0,$curbed"
    set cpystr = "$cpystr -dt s -V 70 -np $lastplane"
    set cpystr = "$cpystr -rm $maxval $verb -rev 2"
    echo $cpystr
    if ($action) then
      $cpystr
      \rm $curimgname
    endif
    @ curplane++
   end


   @ curframe++
  end
  @ curbed++
 end
 @ curgate++
end

goto gauss_smooth

############
version_6: #
############
set curgate  = $firstgate
while ($curgate <= $lastgate)
 set curbed   = $firstbed
 while ($curbed <= $lastbed)
  set curframe = $firstframe
  while ($curframe <= $lastframe)

   # Apply MLEM for every plane...
   ###############################
   set curplane = $firstplane
   while ($curplane <= $lastplane)
    set cmdstring = "$mlempet -s $sinoname -o $nsimgname"
    if ($atnname != "") set cmdstring = "$cmdstring -a $atnname"
    if ($nrmname != "") set cmdstring = "$cmdstring -n $nrmname"
    set cmdstring = "$cmdstring $iters"

    set cmdstring = "$cmdstring -p $curplane -f $curframe"
    set cmdstring = "$cmdstring -g $curgate -b $curbed -V 6 -R $arcradius"
    set cmdstring = "$cmdstring -d $decay -ls $lsrelax"
    set cmdstring = "$cmdstring -z $zoom -X $imsize -sumd $sumd $lik"
    set cmdstring = "$cmdstring -x $xoffset -y $yoffset -r $rotation $verb"

    echo $cmdstring

    if ($action) $cmdstring

    @ curplane++
   end
   @ curframe++
  end
  @ curbed++
 end
 @ curgate++
end

goto gauss_smooth

###############
gauss_smooth: #
###############
if ("$gauss" == "") then
  set cmdstring = "mv $nsimgname $imgname"
  echo $cmdstring
  if ($action) $cmdstring
else
  set curgate  = $firstgate
  while ($curgate <= $lastgate)
    set curbed   = $firstbed
    while ($curbed <= $lastbed)
      set curframe = $firstframe
      while ($curframe <= $lastframe)
        set cmdstring = "$convolg -i $nsimgname -o $imgname"
        set cmdstring = "$cmdstring $gauss -cm -p0 $firstplane -p1 $lastplane"
        set cmdstring = "$cmdstring -f $curframe -b $curbed -g $curgate"
        set cmdstring = "$cmdstring -V $version $verb"
        echo $cmdstring
        if ($action) then
          $cmdstring
        endif
        @ curframe++
      end
      @ curbed++
    end
    @ curgate++
  end  
  \rm $nsimgname
endif

############
stopermee: #
############

