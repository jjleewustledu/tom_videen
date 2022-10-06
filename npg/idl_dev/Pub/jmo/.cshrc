set path=(. /bin /usr/bin /usr/sbin /usr/etc /usr/local/bin/X11  \
/usr/local/bin /usr/ucb /usr/etc /etc /usr/openwin/bin /usr/openwin/bin/xview\
/usr/local/bin/X11 /usr/ccs/bin /usr/lang /euclid/a/wordperfect/wpbin \
/usr/local/purify-2.1 /usr/local/idl/bin /euclid/a/scatter/bin)
#set path=(. /bin /usr/bin /usr/sbin /usr/etc /usr/local/bin/X11  \
#/usr/local/bin /usr/ucb /usr/etc /etc /opt/SUNWspro \
#/opt/SUNWspro/bin /usr/openwin/bin /usr/openwin/bin/xview /usr/ccs/bin)

set history=100
alias cd 'cd \!*;echo `pwd`'
set prompt = "`hostname`% "
set filec
limit coredumpsize 0

setenv ROOT /euclid/a
setenv CPU sun
setenv SINGLE fsingle
setenv EDITOR '/usr/ucb/vi'
setenv MAIL '/var/mail/jmo'
setenv LINK_TIMEOUT 3
setenv SHELL /usr/local/bin/tcsh
setenv EXINIT 'set sw=4 redraw wm=4|map g G|map v ~~~~'
setenv BIN $ROOT/jmo/bin/$CPU
setenv SCATBIN /euclid/a/scatter/bin
setenv SBIN /euclid/a/scatter/bin
setenv LIB $ROOT/jmo/lib/$CPU
setenv DATA IEEE
setenv SRC $ROOT/jmo/src
setenv SCRIPTS $ROOT/jmo/scripts
setenv TABLES $ROOT/jmo/tables/sun
setenv IMG $ROOT/jmo/img
setenv MINUSC -c
setenv OBJ o
setenv FOPTION 
setenv COPTION 
setenv CDEF 
setenv GNUTERM x11
setenv MANPATH /usr/local/man:/usr/lang/man:/usr/man:/usr/openwin/man
setenv OPENWINHOME /usr/openwin
setenv FONTPATH $OPENWINHOME/lib/fonts
#setenv LD_LIBRARY_PATH $OPENWINHOME/lib:/usr/lib
#setenv LD_LIBRARY_PATH /usr/lang/SC2.0.1:/usr/local/lib
setenv WPTERM51 gui_color
setenv PURIFYOPTIONS "-demo=yes"

setenv LM_LICENSE_FILE /usr/local/idl/license.dat
setenv IDL_DIR /euclid/a/local/idl
setenv IDL_PATH +/euclid/a/jmo/idl::+/euclid/a/local/idl/lib
setenv IDL_STARTUP $HOME/idl/idl_startup.pro
setenv IDL_LIB /usr/local/idl/local/lib/sun

#setenv IDL_SRC /usr/local/idl/local/src
#setenv IDL_PATH +/usr/local/idl/local/src:+/usr/local/idl/lib
#setenv IDL_STARTUP idl_startup.pro
#setenv IDL_LIB /usr/local/idl/local/lib/sun

setenv NPG_SUN0 128.252.118.190

set path=($path $SCRIPTS $BIN)

alias rm "rm -i"
alias ls 'ls -F \!*'
alias print "cat \!* | rsh hydra enscript -Pelm"
#alias print "cat \!* | rsh ibc enscript -Poak"
alias print_oak "cat \!* | rsh hydra enscript -Poak"
alias print_land "cat \!* | rsh hydra enscript -r -Pelm"
alias print_dup 'cat \!* | rsh hydra enscript -P oak -S Duplex \!*'
alias h history
alias cdm "cd /midas/b/jmo"
alias cdt "cd /topaz/a/jmo"
alias cdh "cd /hydra/a/jmo"
alias cds "cd /euclid/a/scatter"
alias cdss "cd /euclid/c/scatter"
alias cdds "cd /euclid/d/scatter"
alias cdms "cd /midas/b/jmo/scatter"
alias pds "pushd /euclid/a/scatter"
alias implog $BIN/implog
alias pd pushd
alias mail /usr/ucb/mail
alias opt "setenv COPTION -O2 ;setenv FOPTION -O"
alias opt3 "setenv COPTION -xO3;setenv FOPTION -O3"
alias opt4 "setenv COPTION -xO4;setenv FOPTION -O4"
alias noopt "setenv COPTION " ";setenv FOPTION "" "
alias debug "setenv COPTION -g;setenv FOPTION -g"
alias make "make \!*;echo "
alias manu /usr/ucb/man
alias ncd "setenv DISPLAY wuncd06:0.0"
alias ximp "imp2ncd \!* &"
alias ximp2 "imp2ncd \!* -zoom 200 &"
alias ximp3 "imp2ncd \!* -zoom 300 &"
alias ximp4 "imp2ncd \!* -zoom 400 &"
alias bill "xloadimage $IMG/gif/billcat -onroot"
alias bucky "xloadimage -zoom 50 $IMG/gif/bball -onroot"

#cd2
