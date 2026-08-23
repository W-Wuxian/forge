(define-module (fwht-local)
  #:use-module (guix packages)
  #:use-module (guix git-download)
  #:use-module (guix build-system gnu)
  #:use-module ((guix licenses) #:prefix license:)
  #:use-module (gnu packages)
  #:use-module (guix gexp))

;; Get the current commit with:
;;   git ls-remote https://github.com/hadipourh/fwht HEAD
(define commit "f6313205f6461f26997d8e1f0f20ea516662f3b3")
(define revision "1")

(define-public fwht
  (package
    (name "fwht")
    (version (git-version "0.0.0" revision commit))
    (source (origin
              (method git-fetch)
              (uri (git-reference
                    (url "https://github.com/hadipourh/fwht")
                    (commit commit)))
              (file-name (git-file-name name version))
              (sha256
               (base32
                ;; Keep the real hash you already validated for this
                ;; commit.
                "0ipxzjhf6kzhgj3yj2cymzn1wr0xjasrpbyd38fld8cflqwnal0p"))))
    (build-system gnu-build-system)
    (arguments
     (list
      #:tests? #f
      #:make-flags
      #~(list (string-append "PREFIX=" #$output)
              "NO_CUDA=1")
      #:phases
      #~(modify-phases %standard-phases
          (delete 'bootstrap)
          (delete 'configure)
          (add-after 'unpack 'use-portable-simd-target
            (lambda _
              (substitute* "Makefile"
                (("-march=native")
                 "-march=x86-64-v3 -mtune=generic")))))))
    (home-page "https://github.com/hadipourh/fwht")
    (synopsis "Fast Walsh-Hadamard Transform library")
    (description
     "This package provides a high-performance C99 library for computing
the Fast Walsh-Hadamard Transform (FWHT), a fundamental tool in
cryptanalysis and Boolean function analysis.  It offers an
OpenMP-accelerated CPU backend with AVX2 SIMD support, static and shared
library builds, and an optional command-line tool.")
    (license license:gpl3)))

fwht
