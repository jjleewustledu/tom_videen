curvature: A combined linear and non-linear registration with an curvature smoother.
A displacement field is calculated iterative over a fix point iteration.
The upcoming linear system in every iteration is solved by a FFT.
This solution is used to update the displacment field by a line-search algorithm.
The registration is controled by the parameter file "curvature.prm".


curvatureTM: A combined linear and non-linear registration with an curvature smoother.
A displacement field is calculated iterative over a time-marching algorithm.
The upcoming linear system in every iteration is solved by a FFT.
The registration is controled by the parameter file "curvatureTM.prm".


elastic: A non-linear registration with an elastic smoother.
A displacement field is calculated iterative over a fix point iteration.
The upcoming linear system in every iteration is solved by a FFT.
This solution is used to update the displacment field by a line-search algorithm.
The registration is controled by the parameter file "elastic.prm".


diffusiveFFT: A non-linear registration with a diffusive smoother.
A displacement field is calculated iterative over a time-marching algorithm.
The upcoming linear system in every iteration is solved by a FFT.
The registration is controled by the parameter file "diffusiveFFT.prm".



diffusiveAOS: A non-linear registration with a diffusive smoother.
A displacement field is calculated iterative over a time-marching algorithm.
The upcoming linear system in every iteration is solved by an AOS schema.
The registration is controled by the parameter file "diffusiveAOS.prm".


