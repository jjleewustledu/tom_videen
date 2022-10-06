pro globals
;------------------------------------------
;Define some system variables for different
;machine architectures
;-------------------------------------------
cd,current = program_dir
;spawn,'pwd',program_dir
Case !version.os of
	'Win32': begin
				separator = '\'
				little_endian = 1
				big_endian = 0
			  end
	'sunos': begin
				separator = '/'
				little_endian = 0
				big_endian = 1
			 end
	else: begin
			separator = '/'
			little_endian = 1
			big_endian = 0
		end
endcase
defsysv, '!separator', separator
defsysv, '!program_dir', program_dir(0) + !separator
defsysv, '!little_endian' ,little_endian
defsysv, '!big_endian' ,big_endian

end

