From newsreader.wustl.edu!newspump.wustl.edu!fas-news.harvard.edu!das-news2.harvard.edu!cam-news-feed3.bbnplanet.com!cam-news-hub1.bbnplanet.com!cpk-news-hub1.bbnplanet.com!news.bbnplanet.com!news-peer.sprintlink.net!news.sprintlink.net!Sprint!uunet!in4.uu.net!128.130.2.5!news.kom.tuwien.ac.at!news Wed Sep  3 11:30:03 1997
Path: newsreader.wustl.edu!newspump.wustl.edu!fas-news.harvard.edu!das-news2.harvard.edu!cam-news-feed3.bbnplanet.com!cam-news-hub1.bbnplanet.com!cpk-news-hub1.bbnplanet.com!news.bbnplanet.com!news-peer.sprintlink.net!news.sprintlink.net!Sprint!uunet!in4.uu.net!128.130.2.5!news.kom.tuwien.ac.at!news
From: Christian Oehreneder <co@ipf.tuwien.ac.at>
Newsgroups: comp.lang.idl-pvwave
Subject: Re: forcing equal aspect ratio for PLOT
Date: Mon, 25 Aug 1997 13:10:37 +0200
Organization: Vienna University of Technology, Austria
Lines: 95
Message-ID: <3401682D.4D4@ipf.tuwien.ac.at>
References: <33E9E51D.124B@csc.noaa.gov>
NNTP-Posting-Host: fbgeo3.tuwien.ac.at
Mime-Version: 1.0
Content-Type: text/plain; charset=us-ascii
Content-Transfer-Encoding: 7bit
X-Mailer: Mozilla 3.01Gold (X11; I; HP-UX A.09.05 9000/720)
To: Michael Hearne <mhearne@csc.noaa.gov>
Xref: newsreader.wustl.edu comp.lang.idl-pvwave:6201935

Michael Hearne wrote:
> 
> I am having some trouble forcing plot to maintain an equal aspect ratio.
> For example, if you create a series of points that define a square:
> square(0,*) = [0,0,1,1,0]
> square(1,*) = [0,1,1,0,0]
> plot, square(0,*),square(1,*)
> 
> you will get a square, with 90 degree vertices.
> 
> Now rotate this matrix (I believe this is the correct method):
> theta = -!pi/4  ; 45 degrees
> rot(0,*) = [cos(theta), -sin(theta)]
> rot(1,*) = [sin(theta),cos(theta)]
> newsquare = square##rot  ; matrix multiplication
> plot, newsquare(0,*),newsquare(1,*)
> 
> I get a rotated non_square that does _not_ have 90 degree vertices
> because, I think, the x axis and y axis are not equal in length.  I
> measured them on my screen with a ruler.  Is there some way to force the
> aspect ratio for PLOT to be 1?  Setting xstyle and ystyle equal to 1
> does not seem to work.
> 
> Help!
> -Mike Hearne

Maybe the following procedure helps. It is a wrapper routine for plot
and allows the specification of how many cm correspond to one data unit.

No guarantee!

Greetings
Christian



pro plot_eq, x, y, _extra = e, cm_data= cd, xstyle = xst, ystyle = yst,
device = dev
; Author    : Christian Oehreneder
; created on 25.8.97
; Purpose   : wrapper routine for plot for explicitly determining
;             the scale of the plot
; Inputs    : same as plot, keyword device disabled
; additional: cm_data ... cm per data unit
   
   if n_elements(xst) eq 0 then xst = 0       ; set defaults
   if n_elements(yst) eq 0 then yst = 0

   xst_ = xst + (1 - (xst /4) mod 2)*4        ; Set 4th Bist of x/ystyle
to
   yst_ = yst + (1 - (yst /4) mod 2)*4        ; supress axis

   plot, x, y, xstyle = xst_, ystyle = xst_ $ ; determine the axis range
according 
       , /nodata, _extra = e                  ; to plot default
behaviour
                                              ; nothing is plotted !!!

   lx = !x.crange(1)-!x.crange(0)             ; size of plot-window in
data units
   ly = !y.crange(1)-!y.crange(0)

   xfak = cd*float(!d.x_px_cm) / !d.x_size    ; 1/data
   yfak = cd*float(!d.y_px_cm) / !d.y_size    ; 1/data

   lx = lx*xfak                               ; size of plot-window in
normal units
   ly = ly*yfak

   mid = [0.5,0.5]                            ; center of the
plot-position
                                              ; could be defined more
elaborate

   pos = fltarr(4)                            ; set plot-position 
   pos(0) = mid(0) - lx*0.5        
   pos(1) = mid(1) - ly*0.5
   pos(2) = mid(0) + lx*0.5
   pos(3) = mid(1) + ly*0.5

   plot, x, y, _extra = e, position = pos, xstyle = xst, ystyle = xst
end


**************************************************************************
Christian Oehreneder

Institute for  Photogrammetry and Remote Sensing
Technical University Vienna 

TU Wien, Institut 122        Tel.: +43 1 58801-3730 
Gusshausstrasse 27-29        FAX:  +43 1 5056268
A-1040 Wien                  Email: co@ipf.tuwien.ac.at
AUSTRIA                            ((coehrene@fbgeo1.tuwien.ac.at))
**************************************************************************

