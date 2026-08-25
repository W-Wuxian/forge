(define-module (fwht-cuda-openmp-simd-local)
  #:use-module (guix packages)
  #:use-module (guix git-download)
  #:use-module (guix build-system gnu)
  #:use-module ((guix licenses) #:prefix license:)
  #:use-module (gnu packages)
  #:use-module (guix gexp)
  #:use-module (gnu packages gcc)
  #:use-module (gnu packages commencement)
  #:use-module (guix-science-nonfree packages cuda))

;; git ls-remote https://github.com/hadipourh/fwht HEAD
(define commit "f6313205f6461f26997d8e1f0f20ea516662f3b3")
(define revision "1")

(define-public fwht
  (package
    (name "fwht-cuda-openmp-simd")
    (version (git-version "0.0.0" revision commit))
    (source (origin
              (method git-fetch)
              (uri (git-reference
                    (url "https://github.com/hadipourh/fwht")
                    (commit commit)))
              (file-name (git-file-name name version))
              (sha256
               (base32
                "0ipxzjhf6kzhgj3yj2cymzn1wr0xjasrpbyd38fld8cflqwnal0p"))))
    (build-system gnu-build-system)
    (arguments
     (list
      #:tests? #f
      #:make-flags
      #~(let* ((cuda (this-package-input "cuda-toolkit")))
          (list
           (string-append "PREFIX=" #$output)
           (string-append "CUDA_PATH=" #$cuda)
           (string-append "CUDA_LDFLAGS=-L" #$cuda "/lib64 -lcudart -lstdc++")
           "LDFLAGS=-fopenmp"))
      #:phases
      #~(modify-phases %standard-phases
          (delete 'bootstrap)
          (delete 'configure)
          (add-after 'unpack 'use-portable-simd-target
            (lambda _
              (substitute* "Makefile"
                (("-march=native")
                 "-march=x86-64-v3 -mtune=generic"))))
          (replace 'build
            (lambda* (#:key make-flags #:allow-other-keys)
              (apply invoke "make"
                     (append make-flags (list "all" "lib" "cli"))))))
          ))
    (native-inputs
     (list gcc-toolchain))
    (inputs
     (list cuda-12.9))
    (home-page "https://github.com/hadipourh/fwht")
    (synopsis "Fast Walsh-Hadamard Transform library")
    (description
     "This package provides a high-performance C99 library for computing
the Fast Walsh-Hadamard Transform (FWHT), a fundamental tool in
cryptanalysis and Boolean function analysis.  It offers an
OpenMP-accelerated CPU backend with AVX2 SIMD support and CUDA GPU backend, static and shared
library builds, and an optional command-line tool.")
    (license license:gpl3)))

fwht
