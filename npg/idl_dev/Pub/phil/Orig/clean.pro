;+
; NAME: CLEAN.PRO
;
; PURPOSE: Delete all undefined variables
;
; CATEGORY: Utility Functions
;
; CALLING SEQUENCE: IDL> clean
; 
; INPUTS: none
;
; OPTIONAL INPUTS: none
;	
; KEYWORD PARAMETERS: none
;
; OUTPUTS: none
;
; OPTIONAL OUTPUTS: none
;
; COMMON BLOCKS: none
;
; SIDE EFFECTS: none
;
; RESTRICTIONS: Only works on variables in the Main program space.
;               Most likely will crash w/ common blocks.  Must be run
;               as $MAIN$.
;
; PROCEDURE: Straigthforward
;
; MODIFICATION HISTORY:
;    v1.0 written by Phil Williams 11/6/1996
;              williams@irc.chmcc.org
;-

;--- Get the Help Data
HelpData = gethelp(/fullstring)

vars = ''
for i = 0,n_elements(HelpData)-1 do begin
;--- Find the undefined ones
    pos = strpos(HelpData(i), 'UNDEFINED')

;--- if undefined add to the list
    if pos ne -1 then begin
        pos = strpos(HelpData(i), ' ')
        vars = vars + strmid(HelpData(i),0,pos) + ', '
    endif
endfor

;--- Chop off the last ,
vars = strmid(vars, 0, strlen(vars) - 2)

;--- Make the DELVAR command
cmd = 'DELVAR, ' + vars

;--- Execute it!
r = execute(cmd)


end
