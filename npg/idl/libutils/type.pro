FUNCTION Type, variable, NAME=name
   
   types = ['UNDEFINED', 'BYTE', 'INTEGER', 'LONG', 'FLOAT',  $
            'DOUBLE', 'COMPLEX', 'STRING', 'STRUCTURE', $
            'DCOMPLEX', 'POINTER', 'OBJECT']
   s = Size(variable)
   IF keyword_set(NAME) THEN RETURN, types(s(s(0) + 1)) $
   ELSE return, s(s(0) + 1)
   
END
