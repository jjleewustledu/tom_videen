/*
 * Copyright (c) 2003, 2006 Matteo Frigo
 * Copyright (c) 2003, 2006 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sat Jul  1 13:55:20 EDT 2006 */

#include "codelet-dft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_notw -fma -reorder-insns -schedule-for-pipeline -compact -variables 4 -pipeline-latency 4 -n 14 -name n1_14 -include n.h */

/*
 * This function contains 148 FP additions, 84 FP multiplications,
 * (or, 64 additions, 0 multiplications, 84 fused multiply/add),
 * 80 stack variables, and 56 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.9 2006-02-12 23:34:12 athena Exp $
 * $Id: fft.ml,v 1.4 2006-01-05 03:04:27 stevenj Exp $
 * $Id: gen_notw.ml,v 1.30 2006-02-12 23:34:12 athena Exp $
 */

#include "n.h"

static void n1_14(const R *ri, const R *ii, R *ro, R *io, stride is, stride os, INT v, INT ivs, INT ovs)
{
     DK(KP974927912, +0.974927912181823607018131682993931217232785801);
     DK(KP801937735, +0.801937735804838252472204639014890102331838324);
     DK(KP900968867, +0.900968867902419126236102319507445051165919162);
     DK(KP554958132, +0.554958132087371191422194871006410481067288862);
     DK(KP692021471, +0.692021471630095869627814897002069140197260599);
     DK(KP356895867, +0.356895867892209443894399510021300583399127187);
     INT i;
     for (i = v; i > 0; i = i - 1, ri = ri + ivs, ii = ii + ivs, ro = ro + ovs, io = io + ovs, MAKE_VOLATILE_STRIDE(is), MAKE_VOLATILE_STRIDE(os)) {
	  E Tp, T1L, T24, T1W, T1X, T28, T2a, T1Y, T29, T2b;
	  {
	       E T3, T1x, T1b, To, T1i, T1M, Ts, Ta, T1k, Tv, Th, T1j, T1K, Ty, TZ;
	       E T14, Tz, T1Z, T27, T2c, T1d, TI, T23, T1G, T1D, TW, T1e, T22, T1A, TP;
	       E T1c, T1n, T1s, T1f, T1P;
	       {
		    E T1, T2, T19, T1a;
		    T1 = ri[0];
		    T2 = ri[WS(is, 7)];
		    T19 = ii[0];
		    T1a = ii[WS(is, 7)];
		    {
			 E Tq, T6, Tr, T9, Te, Tx, Tn, Tw, Tk, Tf, Tb, Tc;
			 {
			      E Tl, Tm, Ti, Tj;
			      {
				   E T4, T5, T7, T8;
				   T4 = ri[WS(is, 2)];
				   Tp = T1 + T2;
				   T3 = T1 - T2;
				   T1x = T19 + T1a;
				   T1b = T19 - T1a;
				   T5 = ri[WS(is, 9)];
				   T7 = ri[WS(is, 12)];
				   T8 = ri[WS(is, 5)];
				   Tl = ri[WS(is, 8)];
				   Tq = T4 + T5;
				   T6 = T4 - T5;
				   Tr = T7 + T8;
				   T9 = T7 - T8;
				   Tm = ri[WS(is, 1)];
			      }
			      Ti = ri[WS(is, 6)];
			      Tj = ri[WS(is, 13)];
			      Te = ri[WS(is, 10)];
			      Tx = Tl + Tm;
			      Tn = Tl - Tm;
			      Tw = Ti + Tj;
			      Tk = Ti - Tj;
			      Tf = ri[WS(is, 3)];
			      Tb = ri[WS(is, 4)];
			      Tc = ri[WS(is, 11)];
			 }
			 {
			      E Tu, Tg, Tt, Td;
			      To = Tk + Tn;
			      T1i = Tn - Tk;
			      Tu = Te + Tf;
			      Tg = Te - Tf;
			      Tt = Tb + Tc;
			      Td = Tb - Tc;
			      T1M = Tr - Tq;
			      Ts = Tq + Tr;
			      Ta = T6 + T9;
			      T1k = T9 - T6;
			      T1L = Tt - Tu;
			      Tv = Tt + Tu;
			      Th = Td + Tg;
			      T1j = Tg - Td;
			      T1K = Tw - Tx;
			      Ty = Tw + Tx;
			      TZ = FNMS(KP356895867, Ta, To);
			      T14 = FNMS(KP356895867, To, Th);
			      Tz = FNMS(KP356895867, Th, Ta);
			      T1Z = FNMS(KP356895867, Ts, Ty);
			 }
		    }
		    {
			 E T1B, TE, T1C, TH, T1F, TV, TJ, T1E, TS, T1z, TO, TK, T1y, TL;
			 {
			      E TF, TG, TT, TU, TC, TD;
			      TC = ii[WS(is, 4)];
			      TD = ii[WS(is, 11)];
			      T27 = FNMS(KP356895867, Tv, Ts);
			      T2c = FNMS(KP356895867, Ty, Tv);
			      TF = ii[WS(is, 10)];
			      T1B = TC + TD;
			      TE = TC - TD;
			      TG = ii[WS(is, 3)];
			      TT = ii[WS(is, 8)];
			      TU = ii[WS(is, 1)];
			      {
				   E TQ, TR, TM, TN;
				   TQ = ii[WS(is, 6)];
				   T1C = TF + TG;
				   TH = TF - TG;
				   T1F = TT + TU;
				   TV = TT - TU;
				   TR = ii[WS(is, 13)];
				   TM = ii[WS(is, 12)];
				   TN = ii[WS(is, 5)];
				   TJ = ii[WS(is, 2)];
				   T1E = TQ + TR;
				   TS = TQ - TR;
				   T1z = TM + TN;
				   TO = TM - TN;
				   TK = ii[WS(is, 9)];
			      }
			 }
			 T1d = TE + TH;
			 TI = TE - TH;
			 T23 = T1F - T1E;
			 T1G = T1E + T1F;
			 T1D = T1B + T1C;
			 T24 = T1C - T1B;
			 T1y = TJ + TK;
			 TL = TJ - TK;
			 TW = TS - TV;
			 T1e = TS + TV;
			 T22 = T1y - T1z;
			 T1A = T1y + T1z;
			 TP = TL - TO;
			 T1c = TL + TO;
			 T1n = FNMS(KP356895867, T1c, T1e);
			 T1s = FNMS(KP356895867, T1d, T1c);
			 T1f = FNMS(KP356895867, T1e, T1d);
			 T1P = FNMS(KP356895867, T1A, T1G);
		    }
	       }
	       {
		    E T1U, T1H, T11, T12, T1o, T1q;
		    ro[WS(os, 7)] = T3 + Ta + Th + To;
		    io[WS(os, 7)] = T1b + T1c + T1d + T1e;
		    T1U = FNMS(KP356895867, T1D, T1A);
		    T1H = FNMS(KP356895867, T1G, T1D);
		    ro[0] = Tp + Ts + Tv + Ty;
		    io[0] = T1x + T1A + T1D + T1G;
		    {
			 E TB, TY, T1u, T1w, T10;
			 {
			      E TA, TX, T1t, T1v;
			      TA = FNMS(KP692021471, Tz, To);
			      TX = FMA(KP554958132, TW, TP);
			      T1t = FNMS(KP692021471, T1s, T1e);
			      T1v = FMA(KP554958132, T1i, T1k);
			      TB = FNMS(KP900968867, TA, T3);
			      TY = FMA(KP801937735, TX, TI);
			      T1u = FNMS(KP900968867, T1t, T1b);
			      T1w = FMA(KP801937735, T1v, T1j);
			 }
			 T10 = FNMS(KP692021471, TZ, Th);
			 ro[WS(os, 1)] = FMA(KP974927912, TY, TB);
			 ro[WS(os, 13)] = FNMS(KP974927912, TY, TB);
			 io[WS(os, 13)] = FNMS(KP974927912, T1w, T1u);
			 io[WS(os, 1)] = FMA(KP974927912, T1w, T1u);
			 T11 = FNMS(KP900968867, T10, T3);
			 T12 = FMA(KP554958132, TI, TW);
			 T1o = FNMS(KP692021471, T1n, T1d);
			 T1q = FMA(KP554958132, T1j, T1i);
		    }
		    {
			 E T1J, T1N, T2d, T2f;
			 {
			      E T16, T17, T1g, T1l;
			      {
				   E T13, T1p, T1r, T15;
				   T15 = FNMS(KP692021471, T14, Ta);
				   T13 = FNMS(KP801937735, T12, TP);
				   T1p = FNMS(KP900968867, T1o, T1b);
				   T1r = FNMS(KP801937735, T1q, T1k);
				   T16 = FNMS(KP900968867, T15, T3);
				   ro[WS(os, 9)] = FMA(KP974927912, T13, T11);
				   ro[WS(os, 5)] = FNMS(KP974927912, T13, T11);
				   io[WS(os, 9)] = FMA(KP974927912, T1r, T1p);
				   io[WS(os, 5)] = FNMS(KP974927912, T1r, T1p);
				   T17 = FNMS(KP554958132, TP, TI);
			      }
			      T1g = FNMS(KP692021471, T1f, T1c);
			      T1l = FNMS(KP554958132, T1k, T1j);
			      {
				   E T18, T1h, T1m, T1I;
				   T1I = FNMS(KP692021471, T1H, T1A);
				   T18 = FNMS(KP801937735, T17, TW);
				   T1h = FNMS(KP900968867, T1g, T1b);
				   T1m = FNMS(KP801937735, T1l, T1i);
				   T1J = FNMS(KP900968867, T1I, T1x);
				   ro[WS(os, 3)] = FMA(KP974927912, T18, T16);
				   ro[WS(os, 11)] = FNMS(KP974927912, T18, T16);
				   io[WS(os, 11)] = FNMS(KP974927912, T1m, T1h);
				   io[WS(os, 3)] = FMA(KP974927912, T1m, T1h);
				   T1N = FMA(KP554958132, T1M, T1L);
			      }
			      T2d = FNMS(KP692021471, T2c, Ts);
			      T2f = FMA(KP554958132, T22, T24);
			 }
			 {
			      E T1R, T1S, T20, T25;
			      {
				   E T1O, T2e, T2g, T1Q;
				   T1Q = FNMS(KP692021471, T1P, T1D);
				   T1O = FNMS(KP801937735, T1N, T1K);
				   T2e = FNMS(KP900968867, T2d, Tp);
				   T2g = FNMS(KP801937735, T2f, T23);
				   T1R = FNMS(KP900968867, T1Q, T1x);
				   io[WS(os, 10)] = FNMS(KP974927912, T1O, T1J);
				   io[WS(os, 4)] = FMA(KP974927912, T1O, T1J);
				   ro[WS(os, 4)] = FMA(KP974927912, T2g, T2e);
				   ro[WS(os, 10)] = FNMS(KP974927912, T2g, T2e);
				   T1S = FMA(KP554958132, T1L, T1K);
			      }
			      T20 = FNMS(KP692021471, T1Z, Tv);
			      T25 = FMA(KP554958132, T24, T23);
			      {
				   E T1T, T21, T26, T1V;
				   T1V = FNMS(KP692021471, T1U, T1G);
				   T1T = FMA(KP801937735, T1S, T1M);
				   T21 = FNMS(KP900968867, T20, Tp);
				   T26 = FMA(KP801937735, T25, T22);
				   T1W = FNMS(KP900968867, T1V, T1x);
				   io[WS(os, 12)] = FNMS(KP974927912, T1T, T1R);
				   io[WS(os, 2)] = FMA(KP974927912, T1T, T1R);
				   ro[WS(os, 2)] = FMA(KP974927912, T26, T21);
				   ro[WS(os, 12)] = FNMS(KP974927912, T26, T21);
				   T1X = FNMS(KP554958132, T1K, T1M);
			      }
			      T28 = FNMS(KP692021471, T27, Ty);
			      T2a = FNMS(KP554958132, T23, T22);
			 }
		    }
	       }
	  }
	  T1Y = FNMS(KP801937735, T1X, T1L);
	  T29 = FNMS(KP900968867, T28, Tp);
	  T2b = FNMS(KP801937735, T2a, T24);
	  io[WS(os, 8)] = FNMS(KP974927912, T1Y, T1W);
	  io[WS(os, 6)] = FMA(KP974927912, T1Y, T1W);
	  ro[WS(os, 6)] = FMA(KP974927912, T2b, T29);
	  ro[WS(os, 8)] = FNMS(KP974927912, T2b, T29);
     }
}

static const kdft_desc desc = { 14, "n1_14", {64, 0, 84, 0}, &GENUS, 0, 0, 0, 0 };
void X(codelet_n1_14) (planner *p) {
     X(kdft_register) (p, n1_14, &desc);
}

#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_notw -compact -variables 4 -pipeline-latency 4 -n 14 -name n1_14 -include n.h */

/*
 * This function contains 148 FP additions, 72 FP multiplications,
 * (or, 100 additions, 24 multiplications, 48 fused multiply/add),
 * 43 stack variables, and 56 memory accesses
 */
/*
 * Generator Id's : 
 * $Id: algsimp.ml,v 1.9 2006-02-12 23:34:12 athena Exp $
 * $Id: fft.ml,v 1.4 2006-01-05 03:04:27 stevenj Exp $
 * $Id: gen_notw.ml,v 1.30 2006-02-12 23:34:12 athena Exp $
 */

#include "n.h"

static void n1_14(const R *ri, const R *ii, R *ro, R *io, stride is, stride os, INT v, INT ivs, INT ovs)
{
     DK(KP222520933, +0.222520933956314404288902564496794759466355569);
     DK(KP900968867, +0.900968867902419126236102319507445051165919162);
     DK(KP623489801, +0.623489801858733530525004884004239810632274731);
     DK(KP433883739, +0.433883739117558120475768332848358754609990728);
     DK(KP781831482, +0.781831482468029808708444526674057750232334519);
     DK(KP974927912, +0.974927912181823607018131682993931217232785801);
     INT i;
     for (i = v; i > 0; i = i - 1, ri = ri + ivs, ii = ii + ivs, ro = ro + ovs, io = io + ovs, MAKE_VOLATILE_STRIDE(is), MAKE_VOLATILE_STRIDE(os)) {
	  E T3, Tp, T16, T1f, Ta, T1q, Ts, T10, TG, T1z, T19, T1i, Th, T1s, Tv;
	  E T12, TU, T1B, T17, T1o, To, T1r, Ty, T11, TN, T1A, T18, T1l;
	  {
	       E T1, T2, T14, T15;
	       T1 = ri[0];
	       T2 = ri[WS(is, 7)];
	       T3 = T1 - T2;
	       Tp = T1 + T2;
	       T14 = ii[0];
	       T15 = ii[WS(is, 7)];
	       T16 = T14 - T15;
	       T1f = T14 + T15;
	  }
	  {
	       E T6, Tq, T9, Tr;
	       {
		    E T4, T5, T7, T8;
		    T4 = ri[WS(is, 2)];
		    T5 = ri[WS(is, 9)];
		    T6 = T4 - T5;
		    Tq = T4 + T5;
		    T7 = ri[WS(is, 12)];
		    T8 = ri[WS(is, 5)];
		    T9 = T7 - T8;
		    Tr = T7 + T8;
	       }
	       Ta = T6 + T9;
	       T1q = Tr - Tq;
	       Ts = Tq + Tr;
	       T10 = T9 - T6;
	  }
	  {
	       E TC, T1g, TF, T1h;
	       {
		    E TA, TB, TD, TE;
		    TA = ii[WS(is, 2)];
		    TB = ii[WS(is, 9)];
		    TC = TA - TB;
		    T1g = TA + TB;
		    TD = ii[WS(is, 12)];
		    TE = ii[WS(is, 5)];
		    TF = TD - TE;
		    T1h = TD + TE;
	       }
	       TG = TC - TF;
	       T1z = T1g - T1h;
	       T19 = TC + TF;
	       T1i = T1g + T1h;
	  }
	  {
	       E Td, Tt, Tg, Tu;
	       {
		    E Tb, Tc, Te, Tf;
		    Tb = ri[WS(is, 4)];
		    Tc = ri[WS(is, 11)];
		    Td = Tb - Tc;
		    Tt = Tb + Tc;
		    Te = ri[WS(is, 10)];
		    Tf = ri[WS(is, 3)];
		    Tg = Te - Tf;
		    Tu = Te + Tf;
	       }
	       Th = Td + Tg;
	       T1s = Tt - Tu;
	       Tv = Tt + Tu;
	       T12 = Tg - Td;
	  }
	  {
	       E TQ, T1m, TT, T1n;
	       {
		    E TO, TP, TR, TS;
		    TO = ii[WS(is, 4)];
		    TP = ii[WS(is, 11)];
		    TQ = TO - TP;
		    T1m = TO + TP;
		    TR = ii[WS(is, 10)];
		    TS = ii[WS(is, 3)];
		    TT = TR - TS;
		    T1n = TR + TS;
	       }
	       TU = TQ - TT;
	       T1B = T1n - T1m;
	       T17 = TQ + TT;
	       T1o = T1m + T1n;
	  }
	  {
	       E Tk, Tw, Tn, Tx;
	       {
		    E Ti, Tj, Tl, Tm;
		    Ti = ri[WS(is, 6)];
		    Tj = ri[WS(is, 13)];
		    Tk = Ti - Tj;
		    Tw = Ti + Tj;
		    Tl = ri[WS(is, 8)];
		    Tm = ri[WS(is, 1)];
		    Tn = Tl - Tm;
		    Tx = Tl + Tm;
	       }
	       To = Tk + Tn;
	       T1r = Tw - Tx;
	       Ty = Tw + Tx;
	       T11 = Tn - Tk;
	  }
	  {
	       E TJ, T1j, TM, T1k;
	       {
		    E TH, TI, TK, TL;
		    TH = ii[WS(is, 6)];
		    TI = ii[WS(is, 13)];
		    TJ = TH - TI;
		    T1j = TH + TI;
		    TK = ii[WS(is, 8)];
		    TL = ii[WS(is, 1)];
		    TM = TK - TL;
		    T1k = TK + TL;
	       }
	       TN = TJ - TM;
	       T1A = T1k - T1j;
	       T18 = TJ + TM;
	       T1l = T1j + T1k;
	  }
	  ro[WS(os, 7)] = T3 + Ta + Th + To;
	  io[WS(os, 7)] = T16 + T19 + T17 + T18;
	  ro[0] = Tp + Ts + Tv + Ty;
	  io[0] = T1f + T1i + T1o + T1l;
	  {
	       E TV, Tz, T1e, T1d;
	       TV = FNMS(KP781831482, TN, KP974927912 * TG) - (KP433883739 * TU);
	       Tz = FMA(KP623489801, To, T3) + FNMA(KP900968867, Th, KP222520933 * Ta);
	       ro[WS(os, 5)] = Tz - TV;
	       ro[WS(os, 9)] = Tz + TV;
	       T1e = FNMS(KP781831482, T11, KP974927912 * T10) - (KP433883739 * T12);
	       T1d = FMA(KP623489801, T18, T16) + FNMA(KP900968867, T17, KP222520933 * T19);
	       io[WS(os, 5)] = T1d - T1e;
	       io[WS(os, 9)] = T1e + T1d;
	  }
	  {
	       E TX, TW, T1b, T1c;
	       TX = FMA(KP781831482, TG, KP974927912 * TU) + (KP433883739 * TN);
	       TW = FMA(KP623489801, Ta, T3) + FNMA(KP900968867, To, KP222520933 * Th);
	       ro[WS(os, 13)] = TW - TX;
	       ro[WS(os, 1)] = TW + TX;
	       T1b = FMA(KP781831482, T10, KP974927912 * T12) + (KP433883739 * T11);
	       T1c = FMA(KP623489801, T19, T16) + FNMA(KP900968867, T18, KP222520933 * T17);
	       io[WS(os, 1)] = T1b + T1c;
	       io[WS(os, 13)] = T1c - T1b;
	  }
	  {
	       E TZ, TY, T13, T1a;
	       TZ = FMA(KP433883739, TG, KP974927912 * TN) - (KP781831482 * TU);
	       TY = FMA(KP623489801, Th, T3) + FNMA(KP222520933, To, KP900968867 * Ta);
	       ro[WS(os, 11)] = TY - TZ;
	       ro[WS(os, 3)] = TY + TZ;
	       T13 = FMA(KP433883739, T10, KP974927912 * T11) - (KP781831482 * T12);
	       T1a = FMA(KP623489801, T17, T16) + FNMA(KP222520933, T18, KP900968867 * T19);
	       io[WS(os, 3)] = T13 + T1a;
	       io[WS(os, 11)] = T1a - T13;
	  }
	  {
	       E T1t, T1p, T1C, T1y;
	       T1t = FNMS(KP433883739, T1r, KP781831482 * T1q) - (KP974927912 * T1s);
	       T1p = FMA(KP623489801, T1i, T1f) + FNMA(KP900968867, T1l, KP222520933 * T1o);
	       io[WS(os, 6)] = T1p - T1t;
	       io[WS(os, 8)] = T1t + T1p;
	       T1C = FNMS(KP433883739, T1A, KP781831482 * T1z) - (KP974927912 * T1B);
	       T1y = FMA(KP623489801, Ts, Tp) + FNMA(KP900968867, Ty, KP222520933 * Tv);
	       ro[WS(os, 6)] = T1y - T1C;
	       ro[WS(os, 8)] = T1y + T1C;
	  }
	  {
	       E T1v, T1u, T1E, T1D;
	       T1v = FMA(KP433883739, T1q, KP781831482 * T1s) - (KP974927912 * T1r);
	       T1u = FMA(KP623489801, T1o, T1f) + FNMA(KP222520933, T1l, KP900968867 * T1i);
	       io[WS(os, 4)] = T1u - T1v;
	       io[WS(os, 10)] = T1v + T1u;
	       T1E = FMA(KP433883739, T1z, KP781831482 * T1B) - (KP974927912 * T1A);
	       T1D = FMA(KP623489801, Tv, Tp) + FNMA(KP222520933, Ty, KP900968867 * Ts);
	       ro[WS(os, 4)] = T1D - T1E;
	       ro[WS(os, 10)] = T1D + T1E;
	  }
	  {
	       E T1w, T1x, T1G, T1F;
	       T1w = FMA(KP974927912, T1q, KP433883739 * T1s) + (KP781831482 * T1r);
	       T1x = FMA(KP623489801, T1l, T1f) + FNMA(KP900968867, T1o, KP222520933 * T1i);
	       io[WS(os, 2)] = T1w + T1x;
	       io[WS(os, 12)] = T1x - T1w;
	       T1G = FMA(KP974927912, T1z, KP433883739 * T1B) + (KP781831482 * T1A);
	       T1F = FMA(KP623489801, Ty, Tp) + FNMA(KP900968867, Tv, KP222520933 * Ts);
	       ro[WS(os, 12)] = T1F - T1G;
	       ro[WS(os, 2)] = T1F + T1G;
	  }
     }
}

static const kdft_desc desc = { 14, "n1_14", {100, 24, 48, 0}, &GENUS, 0, 0, 0, 0 };
void X(codelet_n1_14) (planner *p) {
     X(kdft_register) (p, n1_14, &desc);
}

#endif				/* HAVE_FMA */
