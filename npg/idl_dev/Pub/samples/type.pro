FUNCTION Type, variable
types = ['UNDEFINED', 'BYTE', 'INTEGER', 'LONG', 'FLOAT',  $
         'DOUBLE', 'COMPLEX', 'STRING', 'STRUCTURE', $
         'DCOMPLEX', 'POINTER', 'OBJECT']
s = Size(variable)
RETURN, types(s(s(0) + 1))
END
