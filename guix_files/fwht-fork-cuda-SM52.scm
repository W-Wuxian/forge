(define-module (fwht-cuda-openmp-simd-local)
  #:use-module (guix packages)
  #:use-module (guix git-download)
  #:use-module (guix build-system gnu)
  #:use-module ((guix licenses) #:prefix license:)
  #:use-module (gnu packages)
  #:use-module (guix gexp)
  #:use-module (gnu packages gcc)
  #:use-module (gnu packages autotools)
  #:use-module (gnu packages commencement)
  #:use-module (gnu packages elf)
  #:use-module (guix-science-nonfree packages cuda))

;;   git ls-remote https://github.com/W-Wuxian/hadi-fwht config
;; puis remplacer la valeur de `commit' ci-dessous par le hash obtenu.
(define commit "f19060e1d64e5a55477999b0d2e65092e01da868")
(define revision "1")

(define-public fwht
  (package
    (name "fwht-cuda-openmp-simd")
    (version (git-version "0.0.0" revision commit))
    (source (origin
              (method git-fetch)
              (uri (git-reference
                    (url "https://github.com/W-Wuxian/hadi-fwht")
                    (commit commit)))
              (file-name (git-file-name name version))
              (sha256
               ;; Placeholder : `guix build` echouera au premier essai
               ;; et affichera le hash SHA256 reel a copier ici, ou
               ;; utiliser `guix hash -rx .` sur un clone local du
               ;; commit vise.
               (base32
                "1kx30zvkh2h8bqsjravbv1whvqnka05wmz4hazfr2p4hqmbm3kdz"))))
    (build-system gnu-build-system)
    (arguments
     (list
      #:tests? #f

      #:configure-flags
      #~(list
         (string-append "--with-cuda-include="
                         #$(this-package-input "cuda-toolkit")
                         "/include")
         (string-append "--with-cuda-lib="
                         #$(this-package-input "cuda-toolkit")
                         "/lib64")
         "--with-cuda-arch=52"
         "--disable-simd"
         "CFLAGS=-march=x86-64-v3 -mtune=generic -O3"
         "CXXFLAGS=-march=x86-64-v3 -mtune=generic -O3")

      #:phases
      #~(modify-phases %standard-phases

          (add-after 'install 'fix-runpath
            (lambda* (#:key outputs #:allow-other-keys)
              (let ((out (assoc-ref outputs "out")))
                (invoke "patchelf" "--set-rpath"
                        (string-append out "/lib:"
                                       #$(this-package-input "cuda-toolkit")
                                       "/lib64:"
                                       #$(this-package-native-input "gcc-toolchain")
                                       "/lib")
                        (string-append out "/bin/fwht_cli"))))))))

    (native-inputs

     (list autoconf automake libtool
           gcc-toolchain patchelf))
    (inputs
     (list cuda-12.4))

    (home-page "https://github.com/W-Wuxian/hadi-fwht")
    (synopsis "Fast Walsh-Hadamard Transform library")
    (description
     "This package provides a high-performance C99 library for computing
the Fast Walsh-Hadamard Transform (FWHT), a fundamental tool in
cryptanalysis and Boolean function analysis. It offers an
OpenMP-accelerated CPU backend with AVX2 SIMD support and CUDA GPU
backend, static and shared library builds, and an optional command-line
tool. Built via GNU Autotools (./configure && make).")
    (license license:gpl3)))

fwht