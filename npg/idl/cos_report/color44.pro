PRO Color44, r,g,b,nc
; substitute dark blue for purple in next to lowest index of Color42 LUT

   r=intarr(nc)
   g=intarr(nc)
   b=intarr(nc)
   vmax = 255./15.

   r[0]  = 15.*vmax
   r[1]  = 0
   r[2]  = 3.*vmax
   r[3]  = 3.*vmax
   r[4]  = 5.*vmax
   r[5]  = 6.*vmax
   r[6]  = 7.*vmax
   r[7]  = 8.*vmax
   r[8]  = 3.*vmax
   r[9]  = 3.*vmax
   r[10] = 5.*vmax
   r[11] = 4.*vmax
   r[12] = 12.*vmax
   r[13] = 15.*vmax
   r[14] = 14.*vmax
   r[15] = 14.*vmax
   r[16] = 14.*vmax
   r[17] = 13.*vmax
   r[18] = 12.*vmax
   r[19] = 15.*vmax

   g[0]  = 15.*vmax
   g[1]  = 0
   g[2]  = 1.*vmax
   g[3]  = 3.*vmax
   g[4]  = 5.*vmax
   g[5]  = 6.*vmax
   g[6]  = 7.*vmax
   g[7]  = 8.*vmax
   g[8]  = 6.*vmax
   g[9]  = 9.*vmax
   g[10] = 12.*vmax
   g[11] = 14.*vmax
   g[12] = 15.*vmax
   g[13] = 14.*vmax
   g[14] = 13.*vmax
   g[15] = 12.*vmax
   g[16] = 8.*vmax
   g[17] = 6.*vmax
   g[18] = 4.*vmax
   g[19] = 0*vmax

   b[0]  = 15.*vmax
   b[1]  = 3.*vmax
   b[2]  = 5.*vmax
   b[3]  = 7.*vmax
   b[4]  = 8.*vmax
   b[5]  = 11.*vmax
   b[6]  = 12.*vmax
   b[7]  = 14.*vmax
   b[8]  = 3.*vmax
   b[9]  = 3.*vmax
   b[10] = 5.*vmax
   b[11] = 4.*vmax
   b[12] = 2.*vmax
   b[13] = 6.*vmax
   b[14] = 5.*vmax
   b[15] = 4.*vmax
   b[16] = 2.*vmax
   b[17] = 1.*vmax
   b[18] = 0
   b[19] = 0

   return
end

