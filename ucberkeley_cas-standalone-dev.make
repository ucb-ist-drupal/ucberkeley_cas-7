api = 2
core = 7.x

; phpCAS library
libraries[phpcas][download][type] = "get"
libraries[phpcas][download][url] = "https://github.com/Jasig/phpCAS/archive/1.3.5.tar.gz"

; CAS
projects[cas][type] = module
projects[cas][version] = 1.5
projects[cas][patch][1394666] = "https://drupal.org/files/cas-library-detection-1394666-15.patch"

; CAS Attributes
projects[cas_attributes][type] = module
projects[cas_attributes][version] = 1.0-rc3

; LDAP
projects[ldap][type] = module
projects[ldap][version] = 2.2
; If https://github.com/CellarDoorMedia/Lockr-Patches/pull/2 is accepted, update the URI to point to Cellar Door's file.
projects[ldap][patch][patchforkey] = "https://raw.githubusercontent.com/bwood/Lockr-Patches/ad678dbca82a8d4df49a4db91dfdb6737e69c245/drupal7/ldap/ldap-7.x-2.2-key-integration.patch"

; UC Berkeley CAS Feature
; specify type=module to prevent "No release history was found for the requested project (ucberkeley_cas)."
projects[ucberkeley_cas][type] = "module"
projects[ucberkeley_cas][download][type] = "git"
projects[ucberkeley_cas][download][url] = "git@github.com:bwood/ucberkeley_cas-7.git"
projects[ucberkeley_cas][download][branch] = "openucb-1575"
