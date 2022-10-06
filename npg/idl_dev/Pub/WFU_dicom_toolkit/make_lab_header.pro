function make_lab_header



;revisions
;; add "if var_set(cross) then  cube=1.0 "in convert_imgfile   WCL & JAM 02/07/97
;; add headersize in read_header	WCL & JAM 02/07/97 
;; add fill-header routine so it doesn't keep asking for magnet transform (changed import_header)
;; corrected magnet transform  so that extracts from P file if no file chosen when prompted  JAM 2-14-97 (Happy Valentines Day)

an = make_analyze_header()
id_matrix = fltarr(4,4)
for i=0,3 do id_matrix(i,i)=1.0

typef    = ''
messagef = ''
parentf  = ''
planef   = ''

typef    = string(typef   , format = '(A20)' )
messagef = string(messagef, format = '(A50)' )
parentf  = string(parentf , format = '(A100)')
planef   = string(planef  , format = '(A10)' )
typef    = string(typef   , format = '(A20)' )
messagef = string(messagef, format = '(A50)' )
parentf  = string(parentf , format = '(A100)')
planef   = string(planef  , format = '(A10)' )

header = { $
	vchar			: 'V', $
	version			: 3.00, $
	an			: an, $
	x			: 0, $ ;same as in analyze header
	y			: 0, $ ;same as in analyze header
	z			: 0, $ ;same as in analyze header
	time			: 1, $ ;NOT same as in analyze header - this is the total time for the whole stack.  If this is not a stack or diffmap, then an.t_dim = 1 instead of header.time
	magnet_transform	: id_matrix, $
	magnet_tr_parent	: parentf, $
	landmark		: 0.0, $
	direction		: 0.0, $
	fov			: 0, $
	forward			: 0, $
	startslice		: 0, $
	type			: typef, $
	message			: messagef, $
	parent			: parentf, $
	plane_name		: planef, $
	filter_name		: messagef, $
	plane			: 0, $
	filter_cutoff		: 0.0, $
	ontime			: 0.0, $
	offtime			: 0.0, $
	inc			: 0.0, $
	intro			: 0.0, $
	offset			: 0.0, $
	shift			: 0.0, $
	te			: 0.0, $
	tr			: 0.0, $
	pt_weight		: 0.0, $
	headersize		: 2048, $ ; end of V3.00
	tal_transform		: id_matrix, $

	excess			: bytarr(1235 - 64) $ ; this must be updated when adding to header
}

header.an.t_dim		= 1

return, header
end
