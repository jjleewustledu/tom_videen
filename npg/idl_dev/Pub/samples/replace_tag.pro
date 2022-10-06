From newsreader.wustl.edu!newspump.wustl.edu!rice!bcm.tmc.edu!news.msfc.nasa.gov!europa.clark.net!205.252.116.205!howland.erols.net!news-peer.gsl.net!news.gsl.net!gip.net!jupiter.NIC.DTAG.DE!RRZ.Uni-Koeln.DE!usenet Wed Sep  3 11:22:45 1997
Path: newsreader.wustl.edu!newspump.wustl.edu!rice!bcm.tmc.edu!news.msfc.nasa.gov!europa.clark.net!205.252.116.205!howland.erols.net!news-peer.gsl.net!news.gsl.net!gip.net!jupiter.NIC.DTAG.DE!RRZ.Uni-Koeln.DE!usenet
From: Alex Schuster <alex@rosa.mpin-koeln.mpg.de>
Newsgroups: comp.lang.idl-pvwave
Subject: Re: delete a tagname from a structure
Date: Thu, 21 Aug 1997 13:08:54 +0200
Organization: Regional Computing Center, University of Cologne
Lines: 75
Message-ID: <33FC21C6.489D@rosa.mpin-koeln.mpg.de>
NNTP-Posting-Host: 194.94.16.21
Mime-Version: 1.0
Content-Type: multipart/mixed; boundary="------------1BA853742701"
X-Mailer: Mozilla 3.0 (X11; I; SunOS 5.5.1 sun4u)
CC: r.bauer@fz-juelich.de
Xref: newsreader.wustl.edu comp.lang.idl-pvwave:6201913

This is a multi-part message in MIME format.

--------------1BA853742701
Content-Type: text/plain; charset=us-ascii
Content-Transfer-Encoding: 7bit

Some days ago, in an article I don't have here, R. Bauer asked for such
a routine. Now I needed something like this for myself and wrote this
small routine I've attached here. It replaces a tag from a structure by
another, and also allows its deletion. Seems to work, although I don't
guarantee for anything...


        Alex
-- 
  Alex Schuster     Wonko@weird.cologne.de          PGP Key available
                    alex@pet.mpin-koeln.mpg.de

--------------1BA853742701
Content-Type: text/plain; charset=us-ascii; name="replace_tag.pro"
Content-Transfer-Encoding: 7bit
Content-Disposition: inline; filename="replace_tag.pro"

;+
; NAME:
;       REPLACE_TAG
;
; PURPOSE:
;       Replaces a tag in a structure by another
;
; CALLING:
;       replace_tag, struct, old_tag, new_tag, value
;
; INPUTS:
;	struct		- the structure to be changed
;	old_tag		- name of the tag to be changed
;	new_tag		- name of the new tag
;			  If set to '', old_tag is deleted from the structure
;	value		- value of the new tag
;
; RESTRICTIONS:
;	Does not work with named structures
;
; MODIFICATION HISTORY:
;       Alex Schuster, MPIfnF, 8/97 - Written
;-

pro replace_tag, struct, old_tag, new_tag, value

	tags = tag_names( struct )

	pos = (where( tags eq strupcase( old_tag ) ))(0)
	if ( pos eq -1L ) then begin
		print, 'Error: Tag ', old_tag, ' not in struct.' 
		return
	endif

	if ( ( pos eq 0 ) and ( new_tag ne '' ) ) then begin
		new_struct = create_struct( new_tag, value )
	endif else begin
		new_struct = create_struct( tags(0), struct.(0) )
		for i = 1, pos-1 do $
			new_struct = create_struct( new_struct, tags(i), struct.(i) )
		if ( new_tag ne '' ) then $
			new_struct = create_struct( new_struct, new_tag, value )
	endelse
	for i = pos+1, n_elements( tags )-1 do $
		new_struct = create_struct( new_struct, tags(i), struct.(i) )

	struct = new_struct

end

--------------1BA853742701--


