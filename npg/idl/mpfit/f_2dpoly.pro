FUNCTION F_2DPOLY,  X, Y, P
	poly = P[0]*X^2 + P[1]*X + P[2]*Y^2 + P[3]*Y + P[4]*X*Y + P[5]
	return, poly
END

