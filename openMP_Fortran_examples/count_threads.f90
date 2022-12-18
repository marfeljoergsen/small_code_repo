! gfortran -fopenmp count_threads.f90 && ./a.out

program count_threads
  use omp_lib
  implicit none

  integer :: n

  n = 0

  ! gfortran -fopenmp count_threads.f90 && ./a.out
  if (.false.) then !(.true.) then
     ! *** BAD: Another thread could update n between read and write ***
     ! BAD: There were          62 threads

     ! *** BAD: Another thread could update n between read and write ***
     ! BAD: There were          59 threads

     ! *** BAD: Another thread could update n between read and write ***
     ! BAD: There were          61 threads
     ! -----------------------------------

     print *,'*** BAD: Another thread could update n between read and write ***'
     !$omp parallel num_threads(64)
     n = n + 1
     !$omp end parallel
     print *,'BAD: There were', n, 'threads'

  else
     ! *** GOOD: Using atomic operation ***
     !  *** Using atomic operations: The following read/write operation ***
     !      must be executed as one (n=n+1)...
     !  ATOMIC: There were          64 threads
     print *,'*** Using atomic operations: The following read/write operation ***'
     print *,'    must be executed as one (n=n+1)...'
     !$omp parallel num_threads(64)
     !$omp atomic
     n = n + 1
     !$omp end parallel
     print *,'ATOMIC: There were', n, 'threads'
  end if

end program count_threads
