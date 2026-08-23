(define-module (fftw-local)
  #:use-module (guix packages)
  #:use-module (guix download)
  #:use-module (guix build-system gnu)
  #:use-module ((guix licenses) #:prefix license:)
  #:use-module (gnu packages gcc)
  #:use-module (gnu packages perl))

(define-public fftw
  (package
    (name "fftw")
    (version "3.3.11")
    (source (origin
             (method url-fetch)
             (uri (string-append "ftp://ftp.fftw.org/pub/fftw/fftw-"
                                 version ".tar.gz"))
             (sha256
              (base32
               "189rybk0p3a52y3gz27kz5a4f8rlk4d4pszp28b16fxkvr6c4c2n"))))
    (build-system gnu-build-system)
    (arguments
     `(#:configure-flags
       '("--enable-shared" "--enable-openmp" "--enable-threads"
         ,@(let ((system (or (%current-target-system) (%current-system))))
             ;; Enable SIMD extensions for codelets.  See details at:
             ;; <http://fftw.org/fftw3_doc/Installation-on-Unix.html>.
             (cond
              ((string-prefix? "x86_64" system)
               '("--enable-sse2" "--enable-avx" "--enable-avx2"
                 "--enable-avx512" "--enable-avx-128-fma"))
              ((string-prefix? "i686" system)
               '("--enable-sse2"))
              ((string-prefix? "aarch64" system)
               ;; Note that fftw supports NEON on 32-bit ARM only when
               ;; compiled for single-precision.
               '("--enable-neon"))
              (else
               '())))
         ;; By default '-mtune=native' is used.  However, that may cause the
         ;; use of ISA extensions (e.g. AVX) that are not necessarily
         ;; available on the user's machine when that package is built on a
         ;; different machine.
         "ax_cv_c_flags__mtune_native=no")))
    (native-inputs (list perl))
    (home-page "https://fftw.org")
    (synopsis "Computing the discrete Fourier transform")
    (description
     "FFTW is a C subroutine library for computing the discrete Fourier
transform (DFT) in one or more dimensions, of arbitrary input size, and of
both real and complex data (as well as of even/odd data---i.e. the discrete
cosine/ sine transforms or DCT/DST).")
    (license license:gpl2+)))

fftw
