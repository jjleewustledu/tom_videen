function path_of, path_name

;--------------------------------------------------------------------------------------------------
;This routine Defines Lab specific paths, device names,  and global paths
;---------------------------------------------------------------------------------------------------


dicomprogs = !program_dir

case path_name of

'dicom'			: return, dicomprogs
'shell_scripts'		: return, dicomprogs
'dicom_executable' : return,dicomprogs + !version.os + !separator
else			: print, 'path_of: error, unknown path_name: "',path_name,'"'

endcase
end





