(define-module (fwht-2.0.1-local)
  #:use-module (guix packages)
  #:use-module (guix git-download)
  #:use-module (guix build-system gnu)
  #:use-module ((guix licenses) #:prefix license:)
  #:use-module (gnu packages)
  #:use-module (guix gexp))

(define tag "v2.0.1")

(define-public fwht
  (package
    (name "fwht")
    (version "2.0.1")
    (source (origin
              (method git-fetch)
              (uri (git-reference
                    (url "https://github.com/hadipourh/fwht")
                    (commit tag)))
              (file-name (git-file-name name version))
              (sha256
               (base32
                "1g11lkdclh96895ms6q01af63fl17x5y4cgh0wlp7rkfp98wp1b2"))))
    (build-system gnu-build-system)
    (arguments
     (list
      #:tests? #t
      #:make-flags
      #~(list "NO_CUDA=1")
      #:phases
      #~(modify-phases %standard-phases
          (delete 'bootstrap)
          (delete 'configure)
          (add-after 'unpack 'use-portable-simd-target
            (lambda _
              (substitute* "Makefile"
                (("-march=native")
                 "-march=x86-64-v3 -mtune=generic"))))
          ;; v2.0.1's Makefile hard-codes "/usr/local" in the install
          ;; recipe instead of honoring a PREFIX variable, so passing
          ;; PREFIX=... on the make command line has no effect.
          ;; Redirect every literal "/usr/local" occurrence to the
          ;; package's actual store output directory instead.
          (add-after 'unpack 'fix-hardcoded-install-prefix
            (lambda* (#:key outputs #:allow-other-keys)
              (let ((out (assoc-ref outputs "out")))
                (substitute* "Makefile"
                  (("/usr/local") out)))))
          (replace 'check
            (lambda* (#:key tests? make-flags #:allow-other-keys)
              (when tests?
                (apply invoke "make" "test" make-flags)))))))
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
