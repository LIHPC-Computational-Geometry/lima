!*****************************************************************************\
!*    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
!*****************************************************************************/
!
!******************************************************************************
! Fichier : io_fortran.F
! Cree le : Fri Jul  7 14:41:21 MET DST 2000
! Auteur  : JM Bellard
!*****************************************************************************/
!
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
! recherche l'unite logique d'un fichier a partir de son nom (de longueur LN)
!      SUBROUTINE findul(FILENAME, LN, IU,IER)
!      IMPLICIT NONE
!      INTEGER LN
!      CHARACTER (LEN = LN) FILENAME
!      INTEGER IU,IER
!      LOGICAL INQUIRE
!      INQUIRE(FILE=FILENAME, IOSTAT=IER, ERR=10, NUMBER=IU)
! 10   CONTINUE
!      END
!
!
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
subroutine findfreeul(iu,ier)
  !     Fichier Recherche I Unite logique libre
  ! 
  !.. Implicit Declarations .. 
  implicit none
  ! 
  !.. Formal Arguments .. 
  integer(kind(1)), intent(out) :: ier,iu
  ! 
  !.. Local Scalars .. 
  logical :: lopen
  ! 
  ! ... Executable Statements ...
  ! 
  iu = 8
  inquire (iu,OPENED = lopen,IOSTAT = ier,ERR = 1000)
  do while (lopen)
    iu = iu + 1
    inquire (iu,OPENED = lopen,IOSTAT = ier,ERR = 1000)
  end do
  1000 return
end subroutine findfreeul
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
! Ouverture du fichier FIC (nom de longueur LN)
subroutine openf(fic,iu,ier)
  ! 
  !.. Implicit Declarations .. 
  implicit none
  ! 
  !.. Formal Arguments .. 
  character(LEN=*), intent(in) :: fic
  integer(kind(1)), intent(in) :: iu
  integer(kind(1)), intent(out) :: ier
  ! 
  ! ... Executable Statements ...
  ! 
  !      CHARACTER STA
  !     ouverture fichier en non formatte
  open (iu,FILE = fic,FORM = "UNFORMATTED",STATUS = "unknown",IOSTAT = ier, &
        ERR = 1000)
  1000 return
end subroutine openf
!
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
! Fermeture du fichier
subroutine closef(iu,ier)
  ! 
  !.. Implicit Declarations .. 
  implicit none
  ! 
  !.. Formal Arguments .. 
  integer(kind(1)), intent(in) :: iu
  integer(kind(1)), intent(out) :: ier
  ! 
  ! ... Executable Statements ...
  ! 
  close (iu,IOSTAT = ier,ERR = 1000)
  1000 return
end subroutine closef
!
!
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
! Ecriture en binaire d'un tableau d'entier T de longueur L 
subroutine writeint(t,l,iu,ier)
  !     L est le nombre d'elements de T a ecrire
  !     IU est l'unite logique, IER l'erreur retournee
  ! 
  !.. Implicit Declarations .. 
  implicit none
  ! 
  !.. Formal Arguments .. 
  integer(kind(1)), intent(in) :: l
  integer(kind(1)), dimension(l), intent(in) :: t
  integer(kind(1)), intent(in) :: iu
  integer(kind(1)), intent(out) :: ier
  ! 
  !.. Local Scalars .. 
  integer(kind(1)) :: i 
  ! 
  ! ... Executable Statements ...
  ! 
  if (l > 0) then
    write (iu,IOSTAT = ier,ERR = 1000) l, (t(i), i = 1,l)
    write (1,"(i10)") l
    write (1,"(5i14)") (t(i), i = 1,l)
  else
    write (iu,IOSTAT = ier,ERR = 1000) l
  end if
  1000 return
end subroutine writeint
!
!CC   78901234567890123456789012345678901234567890123456789012345678901
!
! Lecture en binaire d'un tableau d'entier T de longueur L 
subroutine readint(t,l,iu,ier)
  !     L est le nombre d'elements de T a lire
  !     IU est l'unite logique, IER l'erreur retournee
  ! 
  !.. Implicit Declarations .. 
  implicit none
  ! 
  !.. Formal Arguments .. 
  integer(kind(1)), intent(inout) :: l
  integer(kind(1)), dimension(l), intent(out) :: t
  integer(kind(1)), intent(in) :: iu
  integer(kind(1)), intent(out) :: ier
  ! 
  !.. Local Scalars .. 
  integer(kind(1)) :: i
  ! 
  ! ... Executable Statements ...
  ! 
  if (l > 0) then
    read (iu,IOSTAT = ier,ERR = 1000) l, (t(i), i = 1,l) !         READ(1,'(i10)')l
                                                         !         READ(1,'(5i14)')(t(i),i=1,l)
  else
    read (iu,IOSTAT = ier,ERR = 1000) l
  end if
  1000 return
end subroutine readint
!
!
!
!
