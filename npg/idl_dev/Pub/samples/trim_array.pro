From newsreader.wustl.edu!newspump.wustl.edu!rice!bcm.tmc.edu!news.msfc.nasa.gov!infeed1.internetmci.com!newsfeed.internetmci.com!164.67.42.145!nntp.info.ucla.edu!ihnp4.ucsd.edu!news1.ucsd.edu!not-for-mail Wed Sep  3 11:19:23 1997
Path: newsreader.wustl.edu!newspump.wustl.edu!rice!bcm.tmc.edu!news.msfc.nasa.gov!infeed1.internetmci.com!newsfeed.internetmci.com!164.67.42.145!nntp.info.ucla.edu!ihnp4.ucsd.edu!news1.ucsd.edu!not-for-mail
From: David Foster <foster@bial1.ucsd.edu>
Newsgroups: comp.lang.idl-pvwave
Subject: Re: Efficient comparison of arrays
Date: Tue, 26 Aug 1997 15:28:22 -0700
Organization: Univ. of Calif San Diego
Lines: 130
Message-ID: <34035886.5D92@bial1.ucsd.edu>
References: <33FB033D.62319AC4@commander.eushc.org>
NNTP-Posting-Host: bial1.ucsd.edu
Mime-Version: 1.0
Content-Type: text/plain; charset=us-ascii
Content-Transfer-Encoding: 7bit
X-Mailer: Mozilla 3.01 (X11; I; SunOS 5.5 sun4c)
To: John Votaw <votaw@commander.eushc.org>
Xref: newsreader.wustl.edu comp.lang.idl-pvwave:6201950

John Votaw wrote:
> 
> I very frequently have a task similar to finding common elements in
> arrays as discussed in this thread.  I have a large array A and an array
> B which is known to be a subset of A.  No elements in A or B are
> repeated.  This situation occurs when A is an array of indicies into an
> image volume forming a region of interest and B are the indicies of some
> feature you would like to remove from the region of interest.  The
> problem is to return an array that contains the elements of A that are
> not in B.
> 
> Following the lead of J. D. Smith, I wrote the following routine:
> 
> function eliminate,a,b
>   c =[a,b]
>   cs=c(sort(c))
>   keepers=where(cs ne shift(cs,1) and cs ne shift(cs,-1), count)
>   if count ne 0 then return,cs(keepers)
>   return,-1
> end
> 
> The brute force method:
> 
> function eliminate_bf,a,b
>   mn=min(a)
>   c=[mn-2,a,mn-1]               ;remove possibility of end effects
>   for i=0,n_elements(b)-1 do begin
>     j=(where(b(i) eq c))(0)
>     c=[c(0:j-1),c(j+1:*)]
>   endfor
>   return,c(1:n_elements(c)-2)
> end
> 
> In my applications, a has about 20000 elements and b has between 1 and
> 1000.  If the number of elements in b is less than 35, then the brute
> force method is faster, otherwise eliminate is faster -- very much so.
> When the number of elements in b is 100, it is 3 times faster.
> 
> Does anyone have another algorithm or comments?

I have written the routine TRIM_ARRAY.PRO for this purpose. This
routine assumes that each array has positive unique elements, and would
be useful for your application. We use it for the same purpose, to
adjust regions-of-interest in volumetric datasets. This routine creates
an array with size equal to the largest element of A, so you wouldn't
want to use it in cases where this could get really really big!

Here are some speed comparisons, run on a Sparc 2/Weitek running 
Solaris 2.5, with IDL 5.0:

	ELIMINATE:	0.48 (sec)
	ELIMINATE_BF:	53.98         (brute-force method above)
	TRIM_ARRAY:	0.17

In this case, A was BYTARR(15097) and B was BYTARR(2530); both were
lists of indices into an array of size 65536.

You can download the routine TRIM_ARRAY.PRO via anonymous ftp from:

	ftp://bial8.ucsd.edu    pub/software/idl/share/idl_share.tar.gz

This comes as a package, so if you want the routine individually
email me and I'll send it to you. Oh, what the heck, here it is:

--------------- <-8 --------------------------------------------------
;  TRIM_ARRAY.PRO    2-02-96   DSF
;
; Use this routine to use one array to remove elements from another
; array. Returns an array which is Arg1 array minus those elements
; contained in the Arg2 array (the intersection of Arg1 and Arg2 is
; removed from Arg1). SEE BELOW!
;
; Assumptions:
;
;   This routine is for arrays with POSITIVE UNIQUE ELEMENTS ONLY! No
;   test is made to ensure this, for speed considerations.
;
;   This routine must allocate a LONARR with dimensions equal to
;   the largest element of ARRAY. If this is very high there may
;   be insufficient memory.
;
; One application is for use with sculpting ROI's, where one ROI's
; elements are removed from another ROI.
;
; Returns ARRAY with elements from SUB removed. If ALL elements of ARRAY
; are removed, returns -2. If either input array is undefined returns
-3.
;
;  5-19-94 DSF Fix bug that removed elements from SUB!
;  2-02-96 DSF Improve coding.
 
 
FUNCTION trim_array, array, sub
 
if ( n_elements(array) le 1 or n_elements(sub) eq 0) then begin
    temp = -3
endif else begin
    maxvalue = max(array)
    index = lonarr(maxvalue+1)        ; Create array of index locations
 
    temp = where(sub le maxvalue)     ; Elements in SUB < MAXVALUE only
    if (temp(0) eq -1) then begin
        return, array                 ; We're done if no elements
    endif else begin
        sub_copy = sub(temp)          ; Remove elements of SUB > MAX
        index(array) = 1              ; Flag elements of ARRAY
        index(sub_copy) = 0           ; "Unflag" elements in SUB
        temp = where(index)           ; Get remaining elements
        if (temp(0) eq -1) then $     ; ALL elements removed from ARRAY
            temp = -2
    endelse
endelse
 
return, temp
END
--------------- <-8 --------------------------------------------------


Hope this is useful.

Dave
-- 

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    David S. Foster         Univ. of California, San Diego
     Programmer/Analyst     Brain Image Analysis Laboratory
     foster@bial1.ucsd.edu  Department of Psychiatry
     (619) 622-5892         8950 Via La Jolla Drive, Suite 2240
                            La Jolla, CA  92037
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

