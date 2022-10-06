PRO header__define

   HEAD = {Header,	$
           fp:		0L,			$ ; file pointer (for Interfile and PETT VI)
           mh6:		{M6header},			$ ; Matrix 6 mainheader
           sh6:		{Subheader6_img},	$ ; Matrix 6 image subheader
           ss6:		{Subheader6_scn},	$ ; Matrix 6 scan subheader
           sa6:		{Subheader6_atn},	$ ; Matrix 6 attencor subheader
           mh7:		{M7header},			$ ; Matrix 7 mainheader
           sh7:		{Subheader7_img},	$ ; Matrix 7 image subheader
           ss7:		{Subheader7_scn3d},	$ ; Matrix 7 scan subheader
           sa7:		{Subheader7_atn},	$ ; Matrix 7 attencor subheader
           p6:		{PETT6header}		$ ; PETT VI header
;;;      ifh:		{IFheader},			$ ; Interfile header
   }

END 
