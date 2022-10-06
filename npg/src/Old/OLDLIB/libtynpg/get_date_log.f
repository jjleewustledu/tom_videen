C$Id: get_date_log.f,v 1.1 1995/12/01 23:19:52 ty7777 Exp $
C$Log: get_date_log.f,v $
c Revision 1.1  1995/12/01  23:19:52  ty7777
c Initial revision
c
C
      subroutine get_date_log(string)
      character*256 string
      character*256 rcsheader /'$Header: /home/petsun4/ty7777/src/libnpg/RCS/get_date_log.f,v 1.1 1995/12/01 23:19:52 ty7777 Exp $'/

      call fdate(string)
      call getlog(string(26:))
      l=lnblnk(string)+1
      string(l:)='\0'
      return
      end
