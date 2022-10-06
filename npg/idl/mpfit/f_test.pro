FUNCTION F_TEST,  X, Y, P
	;val = P[0]*(X-P[1])^2 + P[2]*(Y-P[3])^2 + P[4]*(X-P[1]) + P[5]*(Y-P[3]) + P[6]
	;val = (X-P[0])^2 + (Y-P[1])^2 + P[2]
	;val = (X-P[0])^2 + (Y-P[1])^2 + P[2]*X + P[3]*Y + P[4]
	;val = P[0]*(X-P[1])^2 + P[2]*(Y-P[3])^2 + P[4]*X + P[5]*Y + P[6]
	val = P[0]*(X-P[1])^2 + P[2]*(Y-P[3])^2 + P[4]
	return, val
END

