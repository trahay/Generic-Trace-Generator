!!!   
!!!   @file traceGeneratorBasic1_f.f90
!!!   
      module traceGeneratorBasic1_f
      
      implicit none
      
!     ! The kind of trace to generate
      integer :: traceType
      
      integer, parameter :: PAJE = 0
      integer, parameter :: VITE = 1
      integer, parameter :: OTF  = 2
      integer, parameter :: TAU  = 3
      
      interface
         function  getTraceType_i ()
         integer :: getTraceType_i
         end function getTraceType_i   
      
      function  getMode_i ()
      integer :: getMode_i
      end function getMode_i   
      end interface
      
      contains 
      
      subroutine setTraceType_f (type, mode)
      integer, intent (in) :: type
      integer, intent (in) :: mode
      call setTraceType_i (type, mode)
      end subroutine setTraceType_f
      
      function  getTraceType_f ()
      integer :: getTraceType_f
      getTraceType_f = getTraceType_i ()
      end function getTraceType_f

      subroutine initTrace_f (filename, rank, ierr)
      character (len=*), intent (in) :: filename
      integer, intent (out) :: ierr
      integer, intent (in) :: rank
      call initTrace_i (filename, len (trim (filename)), rank, ierr)
      end subroutine initTrace_f
      

      subroutine addContType_f   (alias, contType, name, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: contType
      character (len=*), intent (in) :: name
      integer, intent (out) :: ierr
      call addContType_i (alias   , len (trim (alias))   ,
     &                    contType, len (trim (contType)),
     &                    name    , len (trim (name)), ierr)
      end subroutine addContType_f
      
      subroutine addStateType_f (alias, contType, name, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: contType
      character (len=*), intent (in) :: name
      integer, intent (out) :: ierr
      call addStateType_i (alias   , len (trim (alias))   ,
     &                     contType, len (trim (contType)),
     &                     name    , len (trim (name)), ierr)
      end subroutine addStateType_f
      
      subroutine addEventType_f (alias, contType, name, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: contType
      character (len=*), intent (in) :: name
      integer, intent (out) :: ierr
      call addEventType_i (alias   , len (trim (alias))   ,
     &                     contType, len (trim (contType)),
     &                     name    , len (trim (name)), ierr)
      end subroutine addEventType_f


      subroutine addLinkType_f (alias, name, contType, srcContType,
     & destContType, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: name
      character (len=*), intent (in) :: contType
      character (len=*), intent (in) :: srcContType
      character (len=*), intent (in) :: destContType
      integer, intent (out) :: ierr
      call addLinkType_i (alias       , len (trim (alias))      ,
     &                    name        , len (trim (name))       ,
     &                    contType    , len (trim (contType))   ,
     &                    srcContType , len (trim (srcContType)),
     &                    destContType, len (trim (destContType)), ierr)
      end subroutine addLinkType_f
 
      subroutine addVarType_f (alias, name, contType, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: contType
      character (len=*), intent (in) :: name
      integer, intent (out) :: ierr
      call addVarType_i (alias   , len (trim (alias))   ,
     &                   contType, len (trim (contType)),
     &                   name    , len (trim (name)), ierr)
      end subroutine addVarType_f

      subroutine addEntityValue_f (alias, entType, name , color, ierr)
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: entType
      character (len=*), intent (in) :: name
      character (len=*), intent (in) :: color
      integer, intent (out) :: ierr
      call addEntityValue_i (alias  , len (trim (alias))   ,
     &                       entType, len (trim (entType)),
     &                       name   , len (trim (name))    ,
     &                       color  , len (trim (color)), ierr)
      end subroutine addEntityValue_f

      subroutine addContainer_f (time, alias, type, container, name,
     & file, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: alias
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: container
      character (len=*), intent (in) :: name
      character (len=*), intent (in) :: file
      integer, intent (out) :: ierr
      call addContainer_i (time     ,
     &                     alias    , len (trim (alias))    ,
     &                     type     , len (trim (type))     ,
     &                     container, len (trim (container)),
     &                     name     , len (trim (name))     ,
     &                     file     , len (trim (file)), ierr)
      end subroutine addContainer_f
      
      subroutine destroyContainer_f (time, name, type, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: name
      character (len=*), intent (in) :: type
      integer, intent (out) :: ierr
      call destroyContainer_i (time,
     &                         name, len (trim (name)),
     &                         type, len (trim (type)), ierr)
      end subroutine destroyContainer_f

      subroutine setState_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      character (len=*), intent (in) :: val
      integer, intent (out) :: ierr
      call setState_i (time,
     &                 type, len (trim (type)),
     &                 cont, len (trim (cont)),
     &                 val , len (trim (val)), ierr)
      end subroutine setState_f

      subroutine pushState_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      character (len=*), intent (in) :: val
      integer, intent (out) :: ierr
      call pushState_i (time,
     &                  type, len (trim (type)),
     &                  cont, len (trim (cont)),
     &                  val , len (trim (val)), ierr)
      end subroutine pushState_f

      subroutine popState_f (time, type, cont, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      integer, intent (out) :: ierr
      call popState_i(time,
     &                type, len (trim (type)),
     &                cont, len (trim (cont)),
     &                ierr)
      end subroutine popState_f

      subroutine addEvent_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      character (len=*), intent (in) :: val
      integer, intent (out) :: ierr
      call addEvent_i(time,
     &                type, len (trim (type)),
     &                cont, len (trim (cont)),
     &                val , len (trim (val)), ierr)
      end subroutine addEvent_f

      subroutine startLink_f   (time, type, cont, src, dest, val, key,
     & ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      character (len=*), intent (in) :: src
      character (len=*), intent (in) :: dest
      character (len=*), intent (in) :: val
      character (len=*), intent (in) :: key
      integer, intent (out) :: ierr
      call startLink_i (time,
     &                  type, len (trim (type)),
     &                  cont, len (trim (cont)),
     &                  src , len (trim (src)) ,
     &                  dest, len (trim (dest)),
     &                  val , len (trim (val)) ,
     &                  key , len (trim (key)), ierr)
       end subroutine startLink_f

      subroutine endLink_f   (time, type, cont, src, dest, val, key,
     & ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      character (len=*), intent (in) :: src
      character (len=*), intent (in) :: dest
      character (len=*), intent (in) :: val
      character (len=*), intent (in) :: key
      integer, intent (out) :: ierr
      call endLink_i  (time,
     &                 type, len (trim (type)),
     &                 cont, len (trim (cont)),
     &                 src , len (trim (src)) ,
     &                 dest, len (trim (dest)),
     &                 val , len (trim (val)) ,
     &                 key , len (trim (key)), ierr)
      end subroutine endLink_f

      subroutine setVar_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      real(8), intent (in) :: val
      integer, intent (out) :: ierr
      call setVar_i (time,
     &               type, len (trim (type)),
     &               cont, len (trim (cont)),
     &               val, ierr)
      end subroutine setVar_f

      subroutine addVar_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      real(8), intent (in) :: val
      integer, intent (out) :: ierr
      call addVar_i  (time,
     &                type, len (trim (type)),
     &                cont, len (trim (cont)),
     &                val, ierr)
      end subroutine addVar_f

      subroutine subVar_f (time, type, cont, val, ierr)
      real(8), intent (in) :: time
      character (len=*), intent (in) :: type
      character (len=*), intent (in) :: cont
      real(8), intent (in) :: val
      integer, intent (out) :: ierr
      call subVar_i  (time,
     &                type, len (trim (type)),
     &                cont, len (trim (cont)),
     &                val, ierr)
      end subroutine subVar_f

      subroutine endTrace_f (ierr)
      integer, intent (out) :: ierr
      call endTrace_i (ierr)
      end subroutine endTrace_f

      end module traceGeneratorBasic1_f
      
